#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define PATH_SIZE   2048

/*
 * ============================================================
 * FUNKCJE POMOCNICZE
 * ============================================================
 */

/*
 * Usuwa znak '\n' z końca tekstu wczytanego przez fgets().
 */
static void remove_newline(char *text)
{
    text[strcspn(text, "\n")] = '\0';
}

/*
 * Pobiera tekst od użytkownika.
 */
static int get_input(const char *message, char *buffer, size_t size)
{
    printf("%s", message);

    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }

    remove_newline(buffer);

    return 0;
}

/*
 * ============================================================
 * BIEŻĄCY KATALOG
 * ============================================================
 */

static void show_current_directory(void)
{
    char path[PATH_SIZE];

    if (getcwd(path, sizeof(path)) == NULL) {
        perror("getcwd");
        return;
    }

    printf("\nBieżący katalog:\n%s\n", path);
}

/*
 * ============================================================
 * WYŚWIETLANIE KATALOGU
 * ============================================================
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

    printf("\nZawartość katalogu:\n");
    printf("-------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("closedir");
    }
}

/*
 * ============================================================
 * TWORZENIE PLIKU
 * ============================================================
 */

static void create_file(void)
{
    char filename[BUFFER_SIZE];
    char text[BUFFER_SIZE];
    int fd;

    if (get_input(
            "\nPodaj nazwę pliku: ",
            filename,
            sizeof(filename)
        ) == -1) {
        return;
    }

    if (filename[0] == '\0') {
        printf("Nazwa pliku nie może być pusta.\n");
        return;
    }

    if (get_input(
            "Podaj tekst do zapisania: ",
            text,
            sizeof(text)
        ) == -1) {
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

    printf("Utworzono plik: %s\n", filename);
}

/*
 * ============================================================
 * USUWANIE PLIKU
 * ============================================================
 */

static void delete_file(void)
{
    char filename[BUFFER_SIZE];

    if (get_input(
            "\nPodaj nazwę pliku: ",
            filename,
            sizeof(filename)
        ) == -1) {
        return;
    }

    if (unlink(filename) == -1) {
        perror("unlink");
        return;
    }

    printf("Usunięto: %s\n", filename);
}

/*
 * ============================================================
 * TWORZENIE KATALOGU
 * ============================================================
 */

static void create_directory(void)
{
    char directory[BUFFER_SIZE];

    if (get_input(
            "\nPodaj nazwę katalogu: ",
            directory,
            sizeof(directory)
        ) == -1) {
        return;
    }

    if (mkdir(directory, 0755) == -1) {
        perror("mkdir");
        return;
    }

    printf("Utworzono katalog: %s\n", directory);
}

/*
 * ============================================================
 * USUWANIE PUSTEGO KATALOGU
 * ============================================================
 */

static void delete_directory(void)
{
    char directory[BUFFER_SIZE];

    if (get_input(
            "\nPodaj nazwę pustego katalogu: ",
            directory,
            sizeof(directory)
        ) == -1) {
        return;
    }

    if (rmdir(directory) == -1) {
        perror("rmdir");
        return;
    }

    printf("Usunięto katalog: %s\n", directory);
}

/*
 * ============================================================
 * WEJŚCIE DO KATALOGU
 * ============================================================
 */

static void change_directory(void)
{
    char directory[BUFFER_SIZE];

    if (get_input(
            "\nPodaj katalog: ",
            directory,
            sizeof(directory)
        ) == -1) {
        return;
    }

    if (chdir(directory) == -1) {
        perror("chdir");
        return;
    }

    printf("Przeniesiono do katalogu: %s\n", directory);

    show_current_directory();
}

/*
 * ============================================================
 * KATALOG NADRZĘDNY
 * ============================================================
 */

static void parent_directory(void)
{
    if (chdir("..") == -1) {
        perror("chdir");
        return;
    }

    show_current_directory();
}

/*
 * ============================================================
 * ZMIANA NAZWY
 * ============================================================
 */

static void rename_entry(void)
{
    char old_name[BUFFER_SIZE];
    char new_name[BUFFER_SIZE];

    if (get_input(
            "\nObecna nazwa: ",
            old_name,
            sizeof(old_name)
        ) == -1) {
        return;
    }

    if (get_input(
            "Nowa nazwa: ",
            new_name,
            sizeof(new_name)
        ) == -1) {
        return;
    }

    if (rename(old_name, new_name) == -1) {
        perror("rename");
        return;
    }

    printf(
        "Zmieniono nazwę \"%s\" na \"%s\".\n",
        old_name,
        new_name
    );
}

/*
 * ============================================================
 * CZYSZCZENIE EKRANU
 * ============================================================
 */

static void clear_screen(void)
{
    printf("\033[2J");
    printf("\033[H");
}

/*
 * ============================================================
 * MENU
 * ============================================================
 */

static void print_menu(void)
{
    printf("\n");
    printf("==============================\n");
    printf("       MINI FILE MANAGER\n");
    printf("==============================\n");
    printf("1.  Wyświetl pliki\n");
    printf("2.  Utwórz plik\n");
    printf("3.  Usuń plik\n");
    printf("4.  Utwórz katalog\n");
    printf("5.  Usuń pusty katalog\n");
    printf("6.  Wejdź do katalogu\n");
    printf("7.  Wróć do katalogu nadrzędnego\n");
    printf("8.  Pokaż bieżący katalog\n");
    printf("9.  Zmień nazwę\n");
    printf("10. Wyczyść ekran\n");
    printf("0.  Zakończ\n");
    printf("==============================\n");
}

/*
 * ============================================================
 * MAIN
 * ============================================================
 */

int main(void)
{
    char input[BUFFER_SIZE];
    int running = 1;

    clear_screen();

    printf("Mini menedżer plików POSIX\n");

    show_current_directory();

    while (running) {

        print_menu();

        if (get_input(
                "\nWybierz opcję: ",
                input,
                sizeof(input)
            ) == -1) {
            break;
        }

        /*
         * Wystarczy sprawdzić pierwszą cyfrę.
         * Dzięki temu menu pozostaje proste.
         */
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
            create_directory();
            break;

        case '5':
            delete_directory();
            break;

        case '6':
            change_directory();
            break;

        case '7':
            parent_directory();
            break;

        case '8':
            show_current_directory();
            break;

        case '9':
            rename_entry();
            break;

        case '0':
            printf("\nKoniec programu.\n");
            running = 0;
            break;

        default:
            /*
             * Obsługa opcji 10.
             */
            if (strcmp(input, "10") == 0) {
                clear_screen();
            } else {
                printf("\nNieprawidłowa opcja.\n");
            }

            break;
        }
    }

    return EXIT_SUCCESS;
}
