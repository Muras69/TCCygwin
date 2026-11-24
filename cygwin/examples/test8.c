#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
    DIR *dir;
    struct dirent *entry;
    int fd;

    const char *directory_name = "test_posix";
    const char *file_name = "test_posix/tekst.txt";
    const char *empty_directory = "pusty_katalog";
    const char *text = "To jest przykładowy plik tekstowy.\n";

    /*
     * Wyświetlenie zawartości bieżącego katalogu.
     */
    printf("Zawartość bieżącego katalogu:\n");

    dir = opendir(".");

    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    /*
     * Utworzenie katalogu.
     */
    printf("\nTworzenie katalogu: %s\n", directory_name);

    if (mkdir(directory_name, 0755) == -1) {
        perror("mkdir");
        return EXIT_FAILURE;
    }

    /*
     * Utworzenie pliku w katalogu.
     */
    printf("Tworzenie pliku: %s\n", file_name);

    fd = open(
        file_name,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /*
     * Zapisanie tekstu do pliku.
     */
    if (write(fd, text, strlen(text)) == -1) {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    /*
     * Utworzenie pustego katalogu.
     */
    printf("Tworzenie pustego katalogu: %s\n", empty_directory);

    if (mkdir(empty_directory, 0755) == -1) {
        perror("mkdir");
        return EXIT_FAILURE;
    }

    /*
     * Usunięcie pustego katalogu.
     */
    printf("Usuwanie pustego katalogu: %s\n", empty_directory);

    if (rmdir(empty_directory) == -1) {
        perror("rmdir");
        return EXIT_FAILURE;
    }

    /*
     * Ponowne wyświetlenie zawartości bieżącego katalogu.
     */
    printf("\nZawartość bieżącego katalogu po wykonaniu operacji:\n");

    dir = opendir(".");

    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
