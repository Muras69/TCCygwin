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

/*
 * Usuwa znak '\n' dodany przez fgets().
 */
static void remove_newline(char *text)
{
    text[strcspn(text, "\n")] = '\0';
}

/*
 * Pobiera tekst od użytkownika.
 */
static int get_input(
    const char *message,
    char *buffer,
    size_t size
)
{
    printf("%s", message);

    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }

    remove_newline(buffer);

    return 0;
}

/*
 * Pobiera numer opcji za pomocą strtol().
 */
static int get_option(void)
{
    char input[BUFFER_SIZE];
    char *end;
    long value;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    remove_newline(input);

    errno = 0;
    value = strtol(input, &end, 10);

    /*
     * Sprawdzamy:
     *
     * - czy wystąpił błąd konwersji,
     * - czy nie podano żadnej liczby,
     * - czy po liczbie zostały nieprawidłowe znaki.
     */
    if (errno != 0 || end == input || *end != '\0') {
        return -1;
    }

    if (value < 0 || value > 10) {
        return -1;
    }

    return (int)value;
}

/*
 * Wyświetla bieżący katalog.
 */
static void show_current_directory(void)
{
    char path[BUFFER_SIZE];

    if (getcwd(path, sizeof(path)) == NULL) {
        perror("getcwd");
        return;
    }

    printf("\nBieżący katalog:\n%s\n", path);
}

/*
 * Wyświetla zawartość bieżącego katalogu.
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
 * Tworzy plik i zapisuje do niego tekst.
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
            "Podaj tekst: ",
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
 * Odczytuje zawartość pliku.
 */
static void read_file(void)
{
    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int fd;

    if (get_input(
            "\nPodaj nazwę pliku: ",
            filename,
            sizeof(filename)
        ) == -1) {
        return;
    }

    fd = open(filename, O_RDONLY);

    if (fd == -1) {
        perror("open");
        return;
    }

    printf("\nZawartość pliku \"%s\":\n", filename);
    printf("----------------------------\n");

    for (;;) {
        ssize_t bytes_read;

        bytes_read = read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("read");
            close(fd);
            return;
        }

        if (bytes_read == 0) {
            break;
        }

        buffer[bytes_read] = '\0';

        printf("%s", buffer);
    }

    printf("\n");

    if (close(fd) == -1) {
        perror("close");
    }
}

/*
 * Usuwa plik.
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

    printf("Usunięto plik: %s\n", filename);
}

/*
 * Tworzy katalog.
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
 * Usuwa pusty katalog.
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
 * Przechodzi do wybranego katalogu.
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

    show_current_directory();
}

/*
 * Przechodzi do katalogu nadrzędnego.
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
 * Zmienia nazwę pliku lub katalogu.
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
 * Kopiuje plik.
 *
 * Schemat:
 *
 * open()  -> read() -> write() -> close()
 */
static void copy_file(void)
{
    char source[BUFFER_SIZE];
    char destination[BUFFER_SIZE];

    int source_fd;
    int destination_fd;

    char buffer[2048];

    if (get_input(
            "\nPlik źródłowy: ",
            source,
            sizeof(source)
        ) == -1) {
        return;
    }

    if (get_input(
            "Plik docelowy: ",
            destination,
            sizeof(destination)
        ) == -1) {
        return;
    }

    source_fd = open(source, O_RDONLY);

    if (source_fd == -1) {
        perror("open źródłowy");
        return;
    }

    destination_fd = open(
        destination,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination_fd == -1) {
        perror("open docelowy");
        close(source_fd);
        return;
    }

    for (;;) {
        ssize_t bytes_read;

        bytes_read = read(
            source_fd,
            buffer,
            sizeof(buffer)
        );

        if (bytes_read == -1) {

            if (errno == EINTR) {
                continue;
            }

            perror("read");
            close(source_fd);
            close(destination_fd);
            return;
        }

        if (bytes_read == 0) {
            break;
        }

        {
            ssize_t total_written = 0;

            while (total_written < bytes_read) {
                ssize_t bytes_written;

                bytes_written = write(
                    destination_fd,
                    buffer + total_written,
                    (size_t)(bytes_read - total_written)
                );

                if (bytes_written == -1) {

                    if (errno == EINTR) {
                        continue;
                    }

                    perror("write");
                    close(source_fd);
                    close(destination_fd);
                    return;
                }

                total_written += bytes_written;
            }
        }
    }

    if (close(source_fd) == -1) {
        perror("close");
    }

    if (close(destination_fd) == -1) {
        perror("close");
    }

    printf(
        "Skopiowano \"%s\" do \"%s\".\n",
        source,
        destination
    );
}

/*
 * Czyści ekran terminala.
 */
static void clear_screen(void)
{
    printf("\033[2J");
    printf("\033[H");
}

/*
 * Wyświetla menu.
 */
static void print_menu(void)
{
    printf("\n");
    printf("==============================\n");
    printf("       MINI FILE MANAGER\n");
    printf("==============================\n");
    printf("1.  Wyświetl pliki\n");
    printf("2.  Utwórz plik\n");
    printf("3.  Odczytaj plik\n");
    printf("4.  Usuń plik\n");
    printf("5.  Utwórz katalog\n");
    printf("6.  Usuń pusty katalog\n");
    printf("7.  Wejdź do katalogu\n");
    printf("8.  Wróć do katalogu nadrzędnego\n");
    printf("9.  Pokaż bieżący katalog\n");
    printf("10. Zmień nazwę\n");
    printf("11. Kopiuj plik\n");
    printf("12. Wyczyść ekran\n");
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
    int running = 1;

    clear_screen();

    printf("Mini menedżer plików POSIX\n");

    show_current_directory();

    while (running) {
        int option;

        print_menu();

        printf("\nWybierz opcję: ");

        option = get_option();

        if (option == -1) {
            printf("Nieprawidłowa opcja.\n");
            continue;
        }

        switch (option) {

        case 1:
            list_directory();
            break;

        case 2:
            create_file();
            break;

        case 3:
            read_file();
            break;

        case 4:
            delete_file();
            break;

        case 5:
            create_directory();
            break;

        case 6:
            delete_directory();
            break;

        case 7:
            change_directory();
            break;

        case 8:
            parent_directory();
            break;

        case 9:
            show_current_directory();
            break;

        case 10:
            rename_entry();
            break;

        case 11:
            copy_file();
            break;

        case 12:
            clear_screen();
            break;

        case 0:
            running = 0;
            break;

        default:
            printf("Nieprawidłowa opcja.\n");
            break;
        }
    }

    printf("\nKoniec programu.\n");

    return EXIT_SUCCESS;
}
