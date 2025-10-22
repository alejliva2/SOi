#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Manejador de la señal SIGUSR1
void manejador(int sig) {
    printf("Hijo (PID=%d): Señal SIGUSR1 recibida, salgo de pause().\n", getpid());
}

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        exit(1);
    }

    if (pid == 0) {
        // Código del hijo
        printf("Hijo (PID=%d): esperando señal con pause()...\n", getpid());

        // Instalar manejador para SIGUSR1
        signal(SIGUSR1, manejador);

        // Bloquear hasta recibir una señal
        pause();

        printf("Hijo (PID=%d): he continuado después de pause().\n", getpid());
        exit(0);
    } else {
        // Código del padre
        printf("Padre (PID=%d): creado hijo con PID=%d\n", getpid(), pid);

        // Esperar 5 segundos
        sleep(5);

        printf("Padre (PID=%d): enviando SIGUSR1 al hijo (PID=%d)\n", getpid(), pid);
        kill(pid, SIGUSR1);

        // Esperar a que el hijo termine
        wait(NULL);

        printf("Padre (PID=%d): hijo terminado, fin del programa.\n", getpid());
    }

    return 0;
}
