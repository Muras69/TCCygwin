#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Dane przekazywane do funkcji wątku.
 */
typedef struct {
    int id;
    const char *message;
} ThreadData;

/*
 * Funkcja wykonywana przez pierwszy wątek.
 */
static void *worker_function1(void *arg)
{
    const ThreadData *data = arg;

    if (data == NULL) {
        fprintf(stderr, "[Wątek 1] Otrzymano nieprawidłowe dane.\n");
        return (void *)(intptr_t)EXIT_FAILURE;
    }

    printf(
        "[Wątek 1] Uruchomiony. ID: %d, wiadomość: \"%s\"\n",
        data->id,
        data->message
    );

    /*
     * Symulacja wykonywania pracy.
     */
    sleep(1);

    printf("[Wątek 1] Kończę pracę.\n");

    /*
     * Zwrócenie wartości do pthread_join().
     *
     * intptr_t jest typem całkowitym wystarczającym do
     * bezpiecznego przechowania wartości wskaźnikowej.
     */
    return (void *)(intptr_t)42;
}

/*
 * Funkcja wykonywana przez drugi wątek.
 */
static void *worker_function2(void *arg)
{
    (void)arg;

    printf("[Wątek 2] Uruchomiony. Pracuję równolegle...\n");

    /*
     * Symulacja dłuższej pracy.
     */
    sleep(2);

    printf("[Wątek 2] Kończę pracę.\n");

    return NULL;
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    ThreadData thread1_data = {
        .id = 101,
        .message = "Witaj z wątku!"
    };

    void *thread1_result;
    int error_code;

    printf("[Główny] Tworzę wątki...\n");

    /*
     * =========================================================
     * TWORZENIE WĄTKU 1
     * =========================================================
     */
    error_code = pthread_create(
        &thread1,
        NULL,
        worker_function1,
        &thread1_data
    );

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_create (wątek 1): %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    /*
     * =========================================================
     * TWORZENIE WĄTKU 2
     * =========================================================
     */
    error_code = pthread_create(
        &thread2,
        NULL,
        worker_function2,
        NULL
    );

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_create (wątek 2): %s\n",
            strerror(error_code)
        );

        /*
         * Wątek 1 został już utworzony, więc musimy na niego
         * zaczekać, aby nie pozostawić uruchomionego wątku.
         */
        pthread_join(thread1, NULL);

        return EXIT_FAILURE;
    }

    printf(
        "[Główny] Oba wątki zostały utworzone. "
        "Czekam na ich zakończenie...\n"
    );

    /*
     * =========================================================
     * OCZEKIWANIE NA WĄTEK 1
     * =========================================================
     */
    error_code = pthread_join(thread1, &thread1_result);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_join (wątek 1): %s\n",
            strerror(error_code)
        );

        /*
         * Wątek 2 nadal może działać, dlatego czekamy również
         * na jego zakończenie przed opuszczeniem programu.
         */
        pthread_join(thread2, NULL);

        return EXIT_FAILURE;
    }

    printf(
        "[Główny] Wątek 1 zakończony. Zwrócił wartość: %ld\n",
        (long)(intptr_t)thread1_result
    );

    /*
     * =========================================================
     * OCZEKIWANIE NA WĄTEK 2
     * =========================================================
     */
    error_code = pthread_join(thread2, NULL);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_join (wątek 2): %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    printf("[Główny] Oba wątki zakończyły pracę.\n");
    printf("[Główny] Koniec programu.\n");

    return EXIT_SUCCESS;
}
