#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

/*
 * Wyświetlenie zawartości bieżącego katalogu.
 */
static void list_directory(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    printf("\nZawartość bieżącego katalogu:\n");
    printf("-----------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("closedir");
    }
}

/*
 * Utworzenie pliku i zapisanie do niego tekstu.
 */
static void create_file(void)
{
    char filename[BUFFER_SIZE];
    char text[BUFFER_SIZE];
    int fd;

    printf("\nPodaj nazwę pliku: ");

    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return;
    }

    /*
     * Usunięcie znaku '\n' dodanego przez fgets().
     */
    filename[strcspn(filename, "\n")] = '\0';

    if (filename[0] == '\0') {
        printf("Nazwa pliku nie może być pusta.\n");
        return;
    }

    printf("Podaj tekst do zapisania: ");

    if (fgets(text, sizeof(text), stdin) == NULL) {
        return;
    }

    fd = open(
        filename,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (fd == -1) {
        perror("open");
        return;
    }

    if (write(fd, text, strlen(text)) == -1) {
        perror("write");
        close(fd);
        return;
    }

    if (close(fd) == -1) {
        perror("close");
        return;
    }

    printf("Plik \"%s\" został utworzony.\n", filename);
}

/*
 * Usunięcie pliku.
 */
static void delete_file(void)
{
    char filename[BUFFER_SIZE];

    printf("\nPodaj nazwę pliku do usunięcia: ");

    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return;
    }

    filename[strcspn(filename, "\n")] = '\0';

    if (filename[0] == '\0') {
        printf("Nazwa pliku nie może być pusta.\n");
        return;
    }

    if (unlink(filename) == -1) {
        perror("unlink");
        return;
    }

    printf("Plik \"%s\" został usunięty.\n", filename);
}

/*
 * Wyświetlenie menu.
 */
static void print_menu(void)
{
    printf("\n");
    printf("========================\n");
    printf("        MENU POSIX\n");
    printf("========================\n");
    printf("1. Wyświetl katalog\n");
    printf("2. Utwórz plik\n");
    printf("3. Usuń plik\n");
    printf("4. Wyczyść ekran\n");
    printf("5. Zakończ\n");
    printf("========================\n");
    printf("Wybierz opcję: ");
}

/*
 * Główna funkcja programu.
 */
int main(void)
{
    char input[BUFFER_SIZE];
    int running = 1;

    while (running) {

        print_menu();

        /*
         * Pobieramy wybór użytkownika.
         */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        switch (input[0]) {

        case '1':
            list_directory();
            break;

        case '2':
            create_file();
            break;

        case '3':
            delete_file();
            break;

        case '4':
            /*
             * Sekwencje ANSI:
             *
             * \033[2J - wyczyść ekran
             * \033[H  - ustaw kursor na początku
             */
            printf("\033[2J");
            printf("\033[H");
            break;

        case '5':
            printf("\nKoniec programu.\n");
            running = 0;
            break;

        default:
            printf("\nNieprawidłowa opcja.\n");
            break;
        }
    }

    return EXIT_SUCCESS;
}
