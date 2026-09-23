#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child_pid;
    int status;

    /* Utworzenie procesu potomnego. */
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        /* Proces potomny. */
        printf("[Potomek] Jestem procesem potomnym.\n");
        printf("[Potomek] Mój PID: %ld\n", (long)getpid());
        printf("[Potomek] PID mojego rodzica: %ld\n", (long)getppid());

        return EXIT_SUCCESS;
    }

    /* Proces rodzicielski. */
    printf("[Rodzic] Jestem procesem rodzicielskim.\n");
    printf("[Rodzic] Mój PID: %ld\n", (long)getpid());
    printf("[Rodzic] PID mojego potomka: %ld\n", (long)child_pid);

    /*
     * Oczekiwanie na zakończenie konkretnego procesu potomnego.
     * waitpid() jest bardziej precyzyjne niż wait(), ponieważ
     * jednoznacznie wskazuje, na który proces oczekujemy.
     */
    if (waitpid(child_pid, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf(
            "[Rodzic] Proces potomny zakończył działanie "
            "z kodem: %d\n",
            WEXITSTATUS(status)
        );
    } else if (WIFSIGNALED(status)) {
        printf(
            "[Rodzic] Proces potomny został zakończony "
            "przez sygnał: %d\n",
            WTERMSIG(status)
        );
    }

    printf("[Rodzic] Kończę działanie.\n");

    return EXIT_SUCCESS;
}
