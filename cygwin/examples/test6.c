#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * ============================================================
 * KONFIGURACJA
 * ============================================================
 */

#define BUFFER_SIZE       5
#define PRODUCER_COUNT    2
#define CONSUMER_COUNT    2
#define ITEMS_PER_PRODUCER 5

/*
 * ============================================================
 * STRUKTURA DANYCH
 * ============================================================
 *
 * Prosty bufor cykliczny FIFO.
 */
typedef struct {
    int values[BUFFER_SIZE];

    size_t head;       /* Pozycja następnego elementu do pobrania. */
    size_t tail;       /* Pozycja następnego miejsca do zapisania. */
    size_t count;      /* Liczba elementów znajdujących się w buforze. */

    size_t produced;   /* Łączna liczba wyprodukowanych elementów. */
    size_t consumed;   /* Łączna liczba skonsumowanych elementów. */

    int producers_finished;

    pthread_mutex_t mutex;

    /*
     * Bufor nie jest pusty.
     *
     * Konsument czeka tutaj, kiedy nie ma danych.
     */
    pthread_cond_t not_empty;

    /*
     * Bufor nie jest pełny.
     *
     * Producent czeka tutaj, kiedy bufor jest pełny.
     */
    pthread_cond_t not_full;
} SharedBuffer;

/*
 * Dane przekazywane producentowi.
 */
typedef struct {
    SharedBuffer *buffer;
    int producer_id;
} ProducerData;

/*
 * Dane przekazywane konsumentowi.
 */
typedef struct {
    SharedBuffer *buffer;
    int consumer_id;
} ConsumerData;

/*
 * ============================================================
 * FUNKCJE POMOCNICZE
 * ============================================================
 */

/*
 * Zwraca aktualny czas + podaną liczbę sekund.
 *
 * Funkcja jest wykorzystywana przez pthread_cond_timedwait().
 */
static int get_timeout(struct timespec *timeout, int seconds)
{
    if (clock_gettime(CLOCK_REALTIME, timeout) == -1) {
        return -1;
    }

    timeout->tv_sec += seconds;

    return 0;
}

/*
 * Dodanie elementu do bufora.
 *
 * Funkcja zakłada, że mutex jest już zablokowany.
 */
static void buffer_push(SharedBuffer *buffer, int value)
{
    buffer->values[buffer->tail] = value;

    buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
    buffer->count++;
    buffer->produced++;
}

/*
 * Pobranie elementu z bufora.
 *
 * Funkcja zakłada, że mutex jest już zablokowany.
 */
static int buffer_pop(SharedBuffer *buffer)
{
    int value = buffer->values[buffer->head];

    buffer->head = (buffer->head + 1) % BUFFER_SIZE;
    buffer->count--;
    buffer->consumed++;

    return value;
}

/*
 * ============================================================
 * PRODUCENT
 * ============================================================
 */

static void *producer_thread(void *arg)
{
    ProducerData *data = arg;
    SharedBuffer *buffer = data->buffer;

    printf(
        "[Producent %d] Uruchomiony.\n",
        data->producer_id
    );

    for (int i = 0; i < ITEMS_PER_PRODUCER; ++i) {

        /*
         * Symulacja przygotowywania danych.
         */
        sleep(1);

        /*
         * Przykładowa wartość produkowana przez ten wątek.
         */
        int value = data->producer_id * 100 + i;

        /*
         * Uzyskanie dostępu do wspólnego bufora.
         */
        int error_code = pthread_mutex_lock(&buffer->mutex);

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Producent %d] pthread_mutex_lock: %s\n",
                data->producer_id,
                strerror(error_code)
            );

            return NULL;
        }

        /*
         * Jeżeli bufor jest pełny, producent musi poczekać.
         *
         * Używamy while, a nie if.
         *
         * pthread_cond_wait() może zostać obudzone bez tego,
         * że warunek faktycznie będzie spełniony.
         */
        while (buffer->count == BUFFER_SIZE) {

            printf(
                "[Producent %d] Bufor pełny. Czekam...\n",
                data->producer_id
            );

            error_code = pthread_cond_wait(
                &buffer->not_full,
                &buffer->mutex
            );

            if (error_code != 0) {
                fprintf(
                    stderr,
                    "[Producent %d] pthread_cond_wait: %s\n",
                    data->producer_id,
                    strerror(error_code)
                );

                pthread_mutex_unlock(&buffer->mutex);
                return NULL;
            }
        }

        /*
         * Dodanie danych do bufora.
         */
        buffer_push(buffer, value);

        printf(
            "[Producent %d] Wyprodukowano wartość %d "
            "(bufor: %zu/%d).\n",
            data->producer_id,
            value,
            buffer->count,
            BUFFER_SIZE
        );

        /*
         * Bufor nie jest już pusty.
         *
         * Budzimy jednego oczekującego konsumenta.
         */
        error_code = pthread_cond_signal(&buffer->not_empty);

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Producent %d] pthread_cond_signal: %s\n",
                data->producer_id,
                strerror(error_code)
            );
        }

        pthread_mutex_unlock(&buffer->mutex);
    }

    printf(
        "[Producent %d] Zakończyłem produkcję.\n",
        data->producer_id
    );

    return NULL;
}

/*
 * ============================================================
 * KONSUMENT
 * ============================================================
 */

static void *consumer_thread(void *arg)
{
    ConsumerData *data = arg;
    SharedBuffer *buffer = data->buffer;

    printf(
        "[Konsument %d] Uruchomiony.\n",
        data->consumer_id
    );

    for (;;) {

        int error_code = pthread_mutex_lock(&buffer->mutex);

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Konsument %d] pthread_mutex_lock: %s\n",
                data->consumer_id,
                strerror(error_code)
            );

            return NULL;
        }

        /*
         * Czekamy, jeżeli bufor jest pusty.
         *
         * Dodatkowo stosujemy timeout.
         */
        while (buffer->count == 0) {

            /*
             * Jeżeli wszyscy producenci zakończyli pracę,
             * a bufor jest pusty, konsument nie ma już czego
             * konsumować.
             */
            if (buffer->producers_finished) {

                printf(
                    "[Konsument %d] Brak danych i wszyscy "
                    "producenci zakończyli pracę.\n",
                    data->consumer_id
                );

                pthread_mutex_unlock(&buffer->mutex);

                return NULL;
            }

            struct timespec timeout;

            if (get_timeout(&timeout, 3) == -1) {
                perror(
                    "[Konsument] clock_gettime"
                );

                pthread_mutex_unlock(&buffer->mutex);

                return NULL;
            }

            printf(
                "[Konsument %d] Bufor pusty. "
                "Czekam maksymalnie 3 sekundy...\n",
                data->consumer_id
            );

            error_code = pthread_cond_timedwait(
                &buffer->not_empty,
                &buffer->mutex,
                &timeout
            );

            if (error_code == ETIMEDOUT) {

                /*
                 * Timeout nie oznacza błędu.
                 *
                 * Po prostu sprawdzamy ponownie stan bufora.
                 */
                printf(
                    "[Konsument %d] Timeout oczekiwania.\n",
                    data->consumer_id
                );

                continue;
            }

            if (error_code != 0) {
                fprintf(
                    stderr,
                    "[Konsument %d] pthread_cond_timedwait: %s\n",
                    data->consumer_id,
                    strerror(error_code)
                );

                pthread_mutex_unlock(&buffer->mutex);

                return NULL;
            }
        }

        /*
         * Pobranie elementu z bufora.
         */
        int value = buffer_pop(buffer);

        printf(
            "[Konsument %d] Pobrano wartość %d "
            "(bufor: %zu/%d).\n",
            data->consumer_id,
            value,
            buffer->count,
            BUFFER_SIZE
        );

        /*
         * Zwolniło się miejsce w buforze.
         *
         * Budzimy producenta, który może czekać na wolne miejsce.
         */
        error_code = pthread_cond_signal(&buffer->not_full);

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Konsument %d] pthread_cond_signal: %s\n",
                data->consumer_id,
                strerror(error_code)
            );
        }

        pthread_mutex_unlock(&buffer->mutex);

        /*
         * Symulacja przetwarzania pobranego elementu.
         */
        sleep(2);

        printf(
            "[Konsument %d] Przetworzono wartość %d.\n",
            data->consumer_id,
            value
        );
    }
}

/*
 * ============================================================
 * MAIN
 * ============================================================
 */

int main(void)
{
    SharedBuffer buffer = {
        .values = {0},
        .head = 0,
        .tail = 0,
        .count = 0,
        .produced = 0,
        .consumed = 0,
        .producers_finished = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .not_empty = PTHREAD_COND_INITIALIZER,
        .not_full = PTHREAD_COND_INITIALIZER
    };

    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];

    ProducerData producer_data[PRODUCER_COUNT];
    ConsumerData consumer_data[CONSUMER_COUNT];

    int error_code;

    printf("============================================\n");
    printf("       PRODUCER - CONSUMER / POSIX          \n");
    printf("============================================\n\n");

    /*
     * =========================================================
     * URUCHOMIENIE KONSUMENTÓW
     * =========================================================
     */

    for (int i = 0; i < CONSUMER_COUNT; ++i) {

        consumer_data[i].buffer = &buffer;
        consumer_data[i].consumer_id = i + 1;

        error_code = pthread_create(
            &consumers[i],
            NULL,
            consumer_thread,
            &consumer_data[i]
        );

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Główny] pthread_create konsumenta: %s\n",
                strerror(error_code)
            );

            return EXIT_FAILURE;
        }
    }

    /*
     * =========================================================
     * URUCHOMIENIE PRODUCENTÓW
     * =========================================================
     */

    for (int i = 0; i < PRODUCER_COUNT; ++i) {

        producer_data[i].buffer = &buffer;
        producer_data[i].producer_id = i + 1;

        error_code = pthread_create(
            &producers[i],
            NULL,
            producer_thread,
            &producer_data[i]
        );

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Główny] pthread_create producenta: %s\n",
                strerror(error_code)
            );

            /*
             * W przypadku błędu czekamy na już uruchomione
             * wątki producentów.
             */
            for (int j = 0; j < i; ++j) {
                pthread_join(producers[j], NULL);
            }

            return EXIT_FAILURE;
        }
    }

    /*
     * =========================================================
     * OCZEKIWANIE NA PRODUCENTÓW
     * =========================================================
     */

    for (int i = 0; i < PRODUCER_COUNT; ++i) {

        error_code = pthread_join(
            producers[i],
            NULL
        );

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Główny] pthread_join producenta: %s\n",
                strerror(error_code)
            );

            return EXIT_FAILURE;
        }
    }

    /*
     * Wszyscy producenci zakończyli pracę.
     *
     * Informujemy o tym konsumentów.
     */
    error_code = pthread_mutex_lock(&buffer.mutex);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_mutex_lock: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    buffer.producers_finished = 1;

    /*
     * Budzimy wszystkich konsumentów.
     *
     * Nie używamy signal(), ponieważ może być kilku
     * konsumentów czekających jednocześnie.
     */
    error_code = pthread_cond_broadcast(
        &buffer.not_empty
    );

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_cond_broadcast: %s\n",
            strerror(error_code)
        );
    }

    pthread_mutex_unlock(&buffer.mutex);

    /*
     * =========================================================
     * OCZEKIWANIE NA KONSUMENTÓW
     * =========================================================
     */

    for (int i = 0; i < CONSUMER_COUNT; ++i) {

        error_code = pthread_join(
            consumers[i],
            NULL
        );

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Główny] pthread_join konsumenta: %s\n",
                strerror(error_code)
            );

            return EXIT_FAILURE;
        }
    }

    /*
     * =========================================================
     * STATYSTYKI
     * =========================================================
     */

    printf("\n============================================\n");
    printf("                 STATYSTYKI                 \n");
    printf("============================================\n");

    printf(
        "Wyprodukowane elementy: %zu\n",
        buffer.produced
    );

    printf(
        "Skonsumowane elementy:  %zu\n",
        buffer.consumed
    );

    printf(
        "Elementy pozostałe:     %zu\n",
        buffer.count
    );

    /*
     * Sprawdzenie spójności danych.
     */
    if (buffer.produced == buffer.consumed &&
        buffer.count == 0) {

        printf(
            "Status: OK - wszystkie dane zostały "
            "przetworzone.\n"
        );

    } else {

        printf(
            "Status: UWAGA - stan bufora jest niespójny.\n"
        );
    }

    /*
     * =========================================================
     * SPRZĄTANIE
     * =========================================================
     */

    error_code = pthread_mutex_destroy(&buffer.mutex);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_mutex_destroy: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    error_code = pthread_cond_destroy(&buffer.not_empty);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_cond_destroy not_empty: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    error_code = pthread_cond_destroy(&buffer.not_full);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_cond_destroy not_full: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    printf("\n[Główny] Wszystkie zasoby zostały zwolnione.\n");
    printf("[Główny] Koniec programu.\n");

    return EXIT_SUCCESS;
}
