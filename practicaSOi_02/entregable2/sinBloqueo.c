#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

void imprimir_hora(const char *mensaje) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char hora[9];
    strftime(hora, 9, "%H:%M:%S", tm_info);
    printf("[%s] %s\n", hora, mensaje);
}

void manejador(int sig) {
    if (sig == SIGUSR1)
        imprimir_hora("P: Recibí SIGUSR1");
    else if (sig == SIGUSR2)
        imprimir_hora("P: Recibí SIGUSR2");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = manejador;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    imprimir_hora("P: Preparado para recibir señales");

    pid_t h1 = fork();
    if (h1 == 0) {
        // Proceso H1
        pid_t n1 = fork();
        if (n1 == 0) {
            // Proceso N1
            imprimir_hora("N1: Enviando SIGUSR2 al abuelo");
            kill(getppid(), SIGUSR2);
            sleep(5);
            imprimir_hora("N1: Terminando con código 21");
            exit(21);
        } else {
            imprimir_hora("H1: Enviando SIGUSR1 al padre");
            kill(getppid(), SIGUSR1);
            wait(NULL); // Espera a N1
            imprimir_hora("H1: Terminando con código 42");
            exit(42);
        }
    } else {
        int status;
        waitpid(h1, &status, 0);
        if (WIFEXITED(status)) {
            int codigo = WEXITSTATUS(status);
            printf("P: H1 terminó con código %d\n", codigo);
        }
    }

    return 0;
}
