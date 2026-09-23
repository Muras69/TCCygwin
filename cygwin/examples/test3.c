#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int pipe_fd[2];
    pid_t child_pid;

    const char *message = "Wiadomosc przeslana przez potok POSIX!";
    char buffer[128];

    /*
     * Utworzenie potoku.
     *
     * pipe_fd[0] - koniec potoku do odczytu,
     * pipe_fd[1] - koniec potoku do zapisu.
     *
     * Potok musi zostać utworzony przed fork(),
     * aby deskryptory zostały odziedziczone przez
     * proces potomny.
     */
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    /*
     * Utworzenie procesu potomnego.
     */
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");

        /*
         * Jeżeli fork() się nie powiedzie, potok został
         * już utworzony, więc należy zamknąć oba deskryptory.
         */
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        /*
         * =====================================================
         * PROCES POTOMNY
         * =====================================================
         *
         * Potomek będzie wyłącznie odczytywał dane z potoku.
         * Nie potrzebuje więc końca do zapisu.
         */

        ssize_t bytes_read;

        if (close(pipe_fd[1]) == -1) {
            perror("[Potomek] close");
            _exit(EXIT_FAILURE);
        }

        printf(
            "[Potomek] PID: %ld. Czekam na dane z potoku...\n",
            (long)getpid()
        );

        /*
         * Odczyt z potoku.
         *
         * read() może zwrócić:
         *   > 0 - liczbę odczytanych bajtów,
         *     0 - koniec danych (EOF),
         *    -1 - błąd.
         */
        do {
            bytes_read = read(
                pipe_fd[0],
                buffer,
                sizeof(buffer) - 1
            );
        } while (bytes_read == -1 && errno == EINTR);

        if (bytes_read == -1) {
            perror("[Potomek] read");
            close(pipe_fd[0]);
            _exit(EXIT_FAILURE);
        }

        if (bytes_read == 0) {
            printf("[Potomek] Potok został zamknięty przez rodzica.\n");
        } else {
            /*
             * read() nie dodaje automatycznie '\0'.
             * Dodajemy terminator, aby można było bezpiecznie
             * użyć bufora jako łańcucha znaków C.
             */
            buffer[bytes_read] = '\0';

            printf(
                "[Potomek] Odebrano %zd bajtów.\n",
                bytes_read
            );

            printf(
                "[Potomek] Odebrany komunikat: \"%s\"\n",
                buffer
            );
        }

        /*
         * Zamknięcie końca potoku używanego do odczytu.
         */
        if (close(pipe_fd[0]) == -1) {
            perror("[Potomek] close");
            _exit(EXIT_FAILURE);
        }

        /*
         * _exit() jest właściwe w procesie potomnym po fork(),
         * jeżeli nie wykonujemy exec().
         */
        _exit(EXIT_SUCCESS);
    }

    /*
     * =========================================================
     * PROCES RODZICIELSKI
     * =========================================================
     *
     * Rodzic będzie wyłącznie zapisywał dane do potoku.
     */

    printf(
        "[Rodzic] PID: %ld. PID potomka: %ld\n",
        (long)getpid(),
        (long)child_pid
    );

    /*
     * Rodzic nie odczytuje danych, więc zamyka koniec
     * potoku przeznaczony do odczytu.
     */
    if (close(pipe_fd[0]) == -1) {
        perror("[Rodzic] close");
        return EXIT_FAILURE;
    }

    printf("[Rodzic] Wysyłam wiadomość do potomka...\n");

    /*
     * write() może zapisać mniej bajtów niż zażądano,
     * dlatego zapisujemy wiadomość w pętli.
     */
    const size_t message_length = strlen(message);
    size_t total_written = 0;

    while (total_written < message_length) {
        ssize_t bytes_written = write(
            pipe_fd[1],
            message + total_written,
            message_length - total_written
        );

        if (bytes_written == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("[Rodzic] write");
            close(pipe_fd[1]);
            return EXIT_FAILURE;
        }

        if (bytes_written == 0) {
            fprintf(
                stderr,
                "[Rodzic] write: zapisano 0 bajtów.\n"
            );
            close(pipe_fd[1]);
            return EXIT_FAILURE;
        }

        total_written += (size_t)bytes_written;
    }

    printf(
        "[Rodzic] Wysłano %zu bajtów.\n",
        total_written
    );

    /*
     * Zamknięcie końca do zapisu jest bardzo ważne.
     *
     * Dzięki temu potomek może otrzymać EOF, gdy wszystkie dane
     * zostaną odczytane.
     */
    if (close(pipe_fd[1]) == -1) {
        perror("[Rodzic] close");
        return EXIT_FAILURE;
    }

    /*
     * Oczekiwanie na zakończenie konkretnego procesu potomnego.
     */
    int status;

    if (waitpid(child_pid, &status, 0) == -1) {
        perror("[Rodzic] waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf(
            "[Rodzic] Potomek zakończył działanie z kodem: %d\n",
            WEXITSTATUS(status)
        );
    } else if (WIFSIGNALED(status)) {
        printf(
            "[Rodzic] Potomek został zakończony przez sygnał: %d\n",
            WTERMSIG(status)
        );
    }

    printf("[Rodzic] Kończę działanie.\n");

    return EXIT_SUCCESS;
}
