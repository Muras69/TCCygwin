#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Prosty program pokazujący manipulację terminalem
 * za pomocą sekwencji ANSI.
 */

int main(void)
{
    /*
     * Czyszczenie ekranu i ustawienie kursora
     * w lewym górnym rogu.
     */
    printf("\033[2J");
    printf("\033[H");

    printf("=================================\n");
    printf("       PROSTY TERMINAL POSIX     \n");
    printf("=================================\n\n");

    printf("Program rozpoczal dzialanie.\n");

    sleep(1);

    /*
     * Przesunięcie kursora o kilka wierszy.
     */
    printf("\033[5;10H");
    printf("Witaj w terminalu!");

    sleep(2);

    /*
     * Powrót kursora na początek.
     */
    printf("\033[1;1H");

    printf("\n\n");
    printf("Nacisnij ENTER, aby kontynuowac...");

    getchar();

    /*
     * Ponowne wyczyszczenie ekranu.
     */
    printf("\033[2J");
    printf("\033[H");

    printf("Ekran zostal wyczyszczony.\n\n");

    printf("Koniec programu.\n");

    return EXIT_SUCCESS;
}
