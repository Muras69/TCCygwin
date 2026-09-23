#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * ============================================================
 * KONFIGURACJA
 * ============================================================
 */

#define TEST_DIRECTORY "posix_test"
#define TEXT_FILE      "wiadomosc.txt"
#define TEMP_FILE      "tymczasowy.txt"
#define RENAMED_FILE   "zmieniony.txt"

#define BUFFER_SIZE 256

/*
 * Kolory terminala.
 */
#define COLOR_RESET  "\033[0m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE   "\033[34m"
#define COLOR_RED    "\033[31m"
#define COLOR_CYAN   "\033[36m"

/*
 * ============================================================
 * FUNKCJE POMOCNICZE
 * ============================================================
 */

static void print_header(const char *title)
{
    printf(
        "\n"
        COLOR_CYAN
        "============================================================\n"
        " %s\n"
        "============================================================\n"
        COLOR_RESET,
        title
    );
}

static void print_success(const char *message)
{
    printf(
        COLOR_GREEN "[OK] %s" COLOR_RESET "\n",
        message
    );
}

static void print_info(const char *message)
{
    printf(
        COLOR_BLUE "[INFO] %s" COLOR_RESET "\n",
        message
    );
}

static void print_error(const char *operation)
{
    fprintf(
        stderr,
        COLOR_RED "[BŁĄD] %s: %s" COLOR_RESET "\n",
        operation,
        strerror(errno)
    );
}

/*
 * ============================================================
 * OKREŚLANIE TYPU PLIKU
 * ============================================================
 *
 * Nie korzystamy z:
 *
 *     DT_DIR
 *     DT_REG
 *     DT_LNK
 *
 * Zamiast tego używamy lstat(), które pobiera informacje
 * o rzeczywistym wpisie katalogu.
 */

static const char *get_file_type(const char *path)
{
    struct stat file_info;

    if (lstat(path, &file_info) == -1) {
        return "[BŁĄD STAT]";
    }

    if (S_ISDIR(file_info.st_mode)) {
        return "[KATALOG]";
    }

    if (S_ISREG(file_info.st_mode)) {
        return "[PLIK]";
    }

    if (S_ISLNK(file_info.st_mode)) {
        return "[LINK]";
    }

    if (S_ISCHR(file_info.st_mode)) {
        return "[URZĄDZENIE CHR]";
    }

    if (S_ISBLK(file_info.st_mode)) {
        return "[URZĄDZENIE BLK]";
    }

    if (S_ISFIFO(file_info.st_mode)) {
        return "[FIFO]";
    }

    if (S_ISSOCK(file_info.st_mode)) {
        return "[SOCKET]";
    }

    return "[INNY]";
}

/*
 * ============================================================
 * WYŚWIETLANIE ZAWARTOŚCI KATALOGU
 * ============================================================
 */

static int list_directory(const char *path)
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(path);

    if (directory == NULL) {
        print_error("opendir");
        return -1;
    }

    printf(
        COLOR_YELLOW
        "\nZawartość katalogu: %s\n"
        COLOR_RESET,
        path
    );

    while ((entry = readdir(directory)) != NULL) {

        char full_path[1024];

        /*
         * Pomijamy:
         *
         * .
         * ..
         */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        /*
         * Tworzymy pełną ścieżkę do wpisu.
         *
         * Dzięki temu lstat() będzie wiedział, który obiekt
         * chcemy sprawdzić.
         */
        int result = snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            path,
            entry->d_name
        );

        if (result < 0 ||
            (size_t)result >= sizeof(full_path)) {

            fprintf(
                stderr,
                COLOR_RED
                "[BŁĄD] Ścieżka jest zbyt długa: %s\n"
                COLOR_RESET,
                entry->d_name
            );

            continue;
        }

        /*
         * Pobieramy typ obiektu za pomocą lstat().
         */
        const char *file_type = get_file_type(full_path);

        printf(
            "  %-30s %s\n",
            entry->d_name,
            file_type
        );
    }

    if (closedir(directory) == -1) {
        print_error("closedir");
        return -1;
    }

    return 0;
}

/*
 * ============================================================
 * INFORMACJE O PLIKU
 * ============================================================
 */

static int show_file_info(const char *path)
{
    struct stat file_info;

    if (stat(path, &file_info) == -1) {
        print_error("stat");
        return -1;
    }

    printf(
        "\n"
        COLOR_YELLOW
        "Informacje o: %s\n"
        COLOR_RESET,
        path
    );

    printf(
        "  Rozmiar:        %lld bajtów\n",
        (long long)file_info.st_size
    );

    printf(
        "  Uprawnienia:    %04o\n",
        file_info.st_mode & 0777
    );

    printf(
        "  Inode:          %llu\n",
        (unsigned long long)file_info.st_ino
    );

    printf(
        "  Liczba linków:  %lu\n",
        (unsigned long)file_info.st_nlink
    );

    if (S_ISREG(file_info.st_mode)) {
        printf("  Typ:             zwykły plik\n");
    } else if (S_ISDIR(file_info.st_mode)) {
        printf("  Typ:             katalog\n");
    } else if (S_ISLNK(file_info.st_mode)) {
        printf("  Typ:             dowiązanie symboliczne\n");
    } else {
        printf("  Typ:             inny\n");
    }

    return 0;
}

/*
 * ============================================================
 * ZAPIS DO PLIKU
 * ============================================================
 */

static int write_text_file(
    const char *path,
    const char *text
)
{
    int fd;
    const size_t text_length = strlen(text);
    size_t total_written = 0;

    fd = open(
        path,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (fd == -1) {
        print_error("open");
        return -1;
    }

    while (total_written < text_length) {

        ssize_t bytes_written = write(
            fd,
            text + total_written,
            text_length - total_written
        );

        if (bytes_written == -1) {

            if (errno == EINTR) {
                continue;
            }

            print_error("write");
            close(fd);

            return -1;
        }

        if (bytes_written == 0) {
            fprintf(
                stderr,
                COLOR_RED
                "[BŁĄD] write zwrócił 0 bajtów.\n"
                COLOR_RESET
            );

            close(fd);

            return -1;
        }

        total_written += (size_t)bytes_written;
    }

    if (close(fd) == -1) {
        print_error("close");
        return -1;
    }

    printf(
        COLOR_GREEN
        "[OK] Zapisano %zu bajtów do \"%s\".\n"
        COLOR_RESET,
        total_written,
        path
    );

    return 0;
}

/*
 * ============================================================
 * ODCZYT PLIKU
 * ============================================================
 */

static int read_text_file(const char *path)
{
    int fd;
    char buffer[BUFFER_SIZE];

    fd = open(path, O_RDONLY);

    if (fd == -1) {
        print_error("open");
        return -1;
    }

    printf(
        COLOR_YELLOW
        "\n--- ZAWARTOŚĆ PLIKU ---\n"
        COLOR_RESET
    );

    for (;;) {

        ssize_t bytes_read = read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes_read == -1) {

            if (errno == EINTR) {
                continue;
            }

            print_error("read");
            close(fd);

            return -1;
        }

        if (bytes_read == 0) {
            break;
        }

        buffer[bytes_read] = '\0';

        printf("%s", buffer);
    }

    printf(
        COLOR_YELLOW
        "\n--- KONIEC PLIKU ---\n"
        COLOR_RESET
    );

    if (close(fd) == -1) {
        print_error("close");
        return -1;
    }

    return 0;
}

/*
 * ============================================================
 * SPRAWDZENIE ISTNIENIA PLIKU
 * ============================================================
 */

static int file_exists(const char *path)
{
    struct stat file_info;

    if (stat(path, &file_info) == 0) {
        return 1;
    }

    if (errno == ENOENT) {
        return 0;
    }

    return -1;
}

/*
 * ============================================================
 * MAIN
 * ============================================================
 */

int main(void)
{
    char test_path[512];
    char text_file_path[512];
    char temp_file_path[512];
    char renamed_file_path[512];

    snprintf(
        test_path,
        sizeof(test_path),
        "%s",
        TEST_DIRECTORY
    );

    snprintf(
        text_file_path,
        sizeof(text_file_path),
        "%s/%s",
        TEST_DIRECTORY,
        TEXT_FILE
    );

    snprintf(
        temp_file_path,
        sizeof(temp_file_path),
        "%s/%s",
        TEST_DIRECTORY,
        TEMP_FILE
    );

    snprintf(
        renamed_file_path,
        sizeof(renamed_file_path),
        "%s/%s",
        TEST_DIRECTORY,
        RENAMED_FILE
    );

    /*
     * ========================================================
     * 1. AKTUALNY KATALOG
     * ========================================================
     */

    print_header(
        "1. ZAWARTOŚĆ AKTUALNEGO KATALOGU"
    );

    if (list_directory(".") == -1) {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 2. UTWORZENIE KATALOGU
     * ========================================================
     */

    print_header(
        "2. TWORZENIE KATALOGU"
    );

    if (mkdir(TEST_DIRECTORY, 0755) == -1) {

        if (errno == EEXIST) {
            print_info(
                "Katalog już istnieje. Używam istniejącego."
            );
        } else {
            print_error("mkdir");
            return EXIT_FAILURE;
        }

    } else {
        print_success(
            "Utworzono katalog posix_test."
        );
    }

    /*
     * ========================================================
     * 3. UTWORZENIE I ZAPIS PLIKU
     * ========================================================
     */

    print_header(
        "3. TWORZENIE I ZAPIS PLIKU"
    );

    const char *message =
        "Witaj z poziomu POSIX API!\n"
        "Ten plik został utworzony przez program w C.\n"
        "Używamy open(), write() oraz close().\n"
        "To jest test systemowych operacji na plikach.\n";

    if (write_text_file(
            text_file_path,
            message
        ) == -1) {

        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 4. INFORMACJE O PLIKU
     * ========================================================
     */

    print_header(
        "4. INFORMACJE O UTWORZONYM PLIKU"
    );

    if (show_file_info(text_file_path) == -1) {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 5. ODCZYT PLIKU
     * ========================================================
     */

    print_header(
        "5. ODCZYT PLIKU"
    );

    if (read_text_file(text_file_path) == -1) {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 6. UTWORZENIE DRUGIEGO PLIKU
     * ========================================================
     */

    print_header(
        "6. TWORZENIE DRUGIEGO PLIKU"
    );

    if (write_text_file(
            temp_file_path,
            "Jestem plikiem tymczasowym.\n"
            "Za chwilę zmienię nazwę, a następnie zostanę usunięty.\n"
        ) == -1) {

        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 7. ZMIANA NAZWY
     * ========================================================
     */

    print_header(
        "7. ZMIANA NAZWY PLIKU"
    );

    if (rename(
            temp_file_path,
            renamed_file_path
        ) == -1) {

        print_error("rename");
        return EXIT_FAILURE;
    }

    printf(
        COLOR_GREEN
        "[OK] \"%s\" -> \"%s\"\n"
        COLOR_RESET,
        TEMP_FILE,
        RENAMED_FILE
    );

    /*
     * ========================================================
     * 8. LISTOWANIE KATALOGU
     * ========================================================
     */

    print_header(
        "8. ZAWARTOŚĆ KATALOGU POSIX_TEST"
    );

    if (list_directory(TEST_DIRECTORY) == -1) {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 9. USUNIĘCIE DRUGIEGO PLIKU
     * ========================================================
     */

    print_header(
        "9. USUWANIE DRUGIEGO PLIKU"
    );

    if (unlink(renamed_file_path) == -1) {
        print_error("unlink");
        return EXIT_FAILURE;
    }

    printf(
        COLOR_GREEN
        "[OK] Usunięto plik \"%s\".\n"
        COLOR_RESET,
        RENAMED_FILE
    );

    /*
     * ========================================================
     * 10. SPRAWDZENIE USUNIĘCIA
     * ========================================================
     */

    print_header(
        "10. SPRAWDZENIE USUNIĘCIA"
    );

    int exists = file_exists(renamed_file_path);

    if (exists == 0) {

        print_success(
            "Plik rzeczywiście nie istnieje."
        );

    } else if (exists == 1) {

        fprintf(
            stderr,
            COLOR_RED
            "[BŁĄD] Plik nadal istnieje!\n"
            COLOR_RESET
        );

        return EXIT_FAILURE;

    } else {

        print_error("stat");

        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 11. PONOWNE LISTOWANIE
     * ========================================================
     */

    print_header(
        "11. KATALOG PO USUNIĘCIU PLIKU"
    );

    if (list_directory(TEST_DIRECTORY) == -1) {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================
     * 12. USUNIĘCIE GŁÓWNEGO PLIKU
     * ========================================================
     */

    print_header(
        "12. USUNIĘCIE GŁÓWNEGO PLIKU"
    );

    if (unlink(text_file_path) == -1) {
        print_error("unlink");
        return EXIT_FAILURE;
    }

    print_success(
        "Usunięto wiadomosc.txt."
    );

    /*
     * ========================================================
     * 13. USUNIĘCIE KATALOGU
     * ========================================================
     */

    print_header(
        "13. USUNIĘCIE KATALOGU"
    );

    if (rmdir(TEST_DIRECTORY) == -1) {
        print_error("rmdir");
        return EXIT_FAILURE;
    }

    print_success(
        "Usunięto katalog posix_test."
    );

    /*
     * ========================================================
     * KONIEC
     * ========================================================
     */

    print_header(
        "KONIEC TESTU"
    );

    printf(
        COLOR_GREEN
        "Wszystkie operacje POSIX zakończyły się powodzeniem.\n"
        COLOR_RESET
    );

    return EXIT_SUCCESS;
}
