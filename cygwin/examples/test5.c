#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * Wspólny muteks używany przez wątki.
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Wątek pomocniczy blokuje muteks na określony czas.
 *
 * Dzięki temu możemy przetestować zachowanie:
 *   - pthread_mutex_trylock()
 *   - pthread_mutex_timedlock()
 */
static void *mutex_holder(void *arg)
{
    (void)arg;

    int error_code = pthread_mutex_lock(&mutex);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Blokujący] pthread_mutex_lock: %s\n",
            strerror(error_code)
        );

        return NULL;
    }

    printf("[Blokujący] Zdobyłem muteks.\n");
    printf("[Blokujący] Przetrzymuję muteks przez 5 sekund...\n");

    /*
     * Symulacja pracy wykonywanej podczas posiadania muteksu.
     */
    sleep(5);

    printf("[Blokujący] Zwalniam muteks.\n");

    error_code = pthread_mutex_unlock(&mutex);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Blokujący] pthread_mutex_unlock: %s\n",
            strerror(error_code)
        );
    }

    return NULL;
}

/*
 * Funkcja główna programu.
 */
int main(void)
{
    pthread_t holder_thread;
    int error_code;

    printf("[Główny] Uruchamiam test muteksów.\n");

    /*
     * =========================================================
     * URUCHOMIENIE WĄTKU BLOKUJĄCEGO MUTEX
     * =========================================================
     */
    error_code = pthread_create(
        &holder_thread,
        NULL,
        mutex_holder,
        NULL
    );

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_create: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    /*
     * Dajemy wątkowi pomocniczemu chwilę na zdobycie muteksu.
     *
     * W programie produkcyjnym nie należy synchronizować
     * wątków przez sleep(). Tutaj robimy to celowo,
     * ponieważ jest to prosty przykład testowy.
     */
    sleep(1);

    /*
     * =========================================================
     * 1. pthread_mutex_trylock()
     * =========================================================
     *
     * trylock() nie blokuje wykonania.
     *
     * Jeżeli muteks jest wolny:
     *     -> zdobywa muteks i zwraca 0.
     *
     * Jeżeli muteks jest zajęty:
     *     -> natychmiast zwraca EBUSY.
     */
    printf("\n[Główny] Testuję pthread_mutex_trylock()...\n");

    error_code = pthread_mutex_trylock(&mutex);

    if (error_code == 0) {
        printf(
            "[Główny] trylock(): muteks był wolny. "
            "Udało się go zdobyć.\n"
        );

        /*
         * Jeżeli udało się zdobyć muteks, należy go zwolnić.
         */
        error_code = pthread_mutex_unlock(&mutex);

        if (error_code != 0) {
            fprintf(
                stderr,
                "[Główny] pthread_mutex_unlock: %s\n",
                strerror(error_code)
            );
        }
    } else if (error_code == EBUSY) {
        printf(
            "[Główny] trylock(): muteks jest zajęty. "
            "Nie czekam i wykonuję dalszą pracę.\n"
        );
    } else {
        fprintf(
            stderr,
            "[Główny] pthread_mutex_trylock: %s\n",
            strerror(error_code)
        );
    }

    /*
     * =========================================================
     * 2. pthread_mutex_timedlock()
     * =========================================================
     *
     * timedlock() może czekać na muteks, ale tylko do
     * określonego momentu.
     *
     * Pobieramy aktualny czas CLOCK_REALTIME i dodajemy
     * 2 sekundy.
     */
    printf(
        "\n[Główny] Testuję pthread_mutex_timedlock()...\n"
    );

    struct timespec timeout;

    if (clock_gettime(CLOCK_REALTIME, &timeout) == -1) {
        perror("[Główny] clock_gettime");

        /*
         * Muteks może nadal być używany przez wątek pomocniczy,
         * dlatego czekamy na jego zakończenie.
         */
        pthread_join(holder_thread, NULL);

        return EXIT_FAILURE;
    }

    timeout.tv_sec += 2;

    error_code = pthread_mutex_timedlock(
        &mutex,
        &timeout
    );

    if (error_code == 0) {
        printf(
            "[Główny] timedlock(): udało się zdobyć muteks "
            "przed upływem limitu czasu.\n"
        );

        if (pthread_mutex_unlock(&mutex) != 0) {
            fprintf(
                stderr,
                "[Główny] Błąd podczas zwalniania muteksu.\n"
            );
        }
    } else if (error_code == ETIMEDOUT) {
        printf(
            "[Główny] timedlock(): minęły 2 sekundy, "
            "ale muteks nadal jest zajęty.\n"
        );
    } else {
        fprintf(
            stderr,
            "[Główny] pthread_mutex_timedlock: %s\n",
            strerror(error_code)
        );
    }

    /*
     * =========================================================
     * OCZEKIWANIE NA WĄTEK BLOKUJĄCY
     * =========================================================
     */
    error_code = pthread_join(holder_thread, NULL);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_join: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    printf("\n[Główny] Test zakończony.\n");

    /*
     * Muteks został zainicjalizowany statycznie, więc nie ma
     * konieczności wywoływania pthread_mutex_init().
     *
     * Dla kompletności niszczymy go przed zakończeniem programu.
     */
    error_code = pthread_mutex_destroy(&mutex);

    if (error_code != 0) {
        fprintf(
            stderr,
            "[Główny] pthread_mutex_destroy: %s\n",
            strerror(error_code)
        );

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
