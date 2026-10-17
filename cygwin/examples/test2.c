#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    const char *filename = "test_posix.txt";
    const char *text = "Witaj z poziomu POSIX API w C99!\n";
    char buffer[128];

    int fd;
    ssize_t bytes_written;
    ssize_t bytes_read;
    size_t text_length;

    /*
     * Otworzenie pliku do zapisu.
     *
     * O_CREAT  - utworzenie pliku, jeśli nie istnieje,
     * O_WRONLY - otwarcie wyłącznie do zapisu,
     * O_TRUNC  - wyzerowanie zawartości istniejącego pliku.
     *
     * 0644:
     * właściciel: odczyt + zapis,
     * grupa:      odczyt,
     * pozostali:  odczyt.
     */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    text_length = strlen(text);

    /*
     * write() może zapisać mniej bajtów niż zażądano.
     * Dlatego zapis wykonujemy w pętli aż do zapisania
     * całego bufora.
     */
    size_t total_written = 0;

    while (total_written < text_length) {
        bytes_written = write(
            fd,
            text + total_written,
            text_length - total_written
        );

        if (bytes_written == -1) {
            /*
             * EINTR oznacza przerwanie wywołania przez sygnał.
             * W takiej sytuacji ponawiamy operację.
             */
            if (errno == EINTR) {
                continue;
            }

            perror("write");
            close(fd);
            return EXIT_FAILURE;
        }

        /*
         * write() zwracające 0 przy niezerowej liczbie żądanych
         * bajtów nie pozwala na dalszy postęp.
         */
        if (bytes_written == 0) {
            fprintf(stderr, "write: zapisano 0 bajtów\n");
            close(fd);
            return EXIT_FAILURE;
        }

        total_written += (size_t)bytes_written;
    }

    /*
     * Zamknięcie deskryptora po zakończeniu zapisu.
     */
    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    printf(
        "Pomyślnie zapisano %zu bajtów do pliku \"%s\".\n",
        total_written,
        filename
    );

    /*
     * Ponowne otworzenie pliku, tym razem wyłącznie do odczytu.
     */
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /*
     * Odczytujemy dane maksymalnie do rozmiaru bufora - 1,
     * pozostawiając miejsce na znak '\0'.
     */
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1) {
        if (errno == EINTR) {
            /*
             * W prostym przykładzie ponowienie odczytu
             * można zrealizować przez ponowne wywołanie read().
             */
            bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        }

        if (bytes_read == -1) {
            perror("read");
            close(fd);
            return EXIT_FAILURE;
        }
    }

    /*
     * read() nie dodaje automatycznie znaku '\0'.
     * Robimy to ręcznie, aby buffer był poprawnym
     * łańcuchem znaków C.
     */
    buffer[bytes_read] = '\0';

    /*
     * Zamknięcie deskryptora po zakończeniu odczytu.
     */
    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    printf("Odczytano %zd bajtów.\n", bytes_read);
    printf("Zawartość pliku:\n%s", buffer);

    return EXIT_SUCCESS;
}
