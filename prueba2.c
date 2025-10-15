// señales_padre_hijo.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static void gestion(int numero_de_senhal); // Declaración del gestor

int main(void) {
    pid_t padre, hijo;

    // Instala el mismo gestor para SIGUSR1 y SIGUSR2, y también para SIGTERM
    if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");
    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");
    if (signal(SIGTERM, gestion) == SIG_ERR) printf("Error al crear gestor TERM\n");

    padre = getpid();

    if ((hijo = fork()) == 0) {
        // Trabajo del hijo
        // Envia señal al padre para avisar que está listo
        kill(padre, SIGUSR1);

        // Espera señales del padre indefinidamente
        // Usamos pause() para dormir hasta recibir una señal
        for (;;) {
            pause();
        }
    } else if (hijo > 0) {
        // Trabajo del padre

        // Opcional: espera un momento para asegurar que el hijo arrancó y envió SIGUSR1
        // (También podrías usar pause() y que la señal del hijo te despierte)
        // Aquí usamos pause() para sincronizarnos con el SIGUSR1 del hijo:
        pause(); // Padre recibe SIGUSR1 y ejecuta 'gestion'

        // Tras recibir SIGUSR1 del hijo, el padre le envía SIGUSR2
        kill(hijo, SIGUSR2);

        // Espera un poco y luego termina al hijo con SIGTERM
        sleep(10);
        kill(hijo, SIGTERM);

        // Espera a que el hijo termine
        int status;
        waitpid(hijo, &status, 0);

        if (WIFSIGNALED(status)) {
            printf("Padre (%d): hijo (%d) terminó por señal %d\n",
                   getpid(), hijo, WTERMSIG(status));
        } else if (WIFEXITED(status)) {
            printf("Padre (%d): hijo (%d) terminó con código %d\n",
                   getpid(), hijo, WEXITSTATUS(status));
        }

        return 0;
    } 
    
    else {
        perror("fork");
        return 1;
    }
}

static void gestion(int numero_de_senhal) {
    switch (numero_de_senhal) {
    case SIGUSR1:
        // Señal enviada por el hijo al padre al inicio
        printf("Señal tipo 1 (SIGUSR1) recibida. Soy %d\n", getpid());
        break;
    case SIGUSR2:
        // Señal enviada por el padre al hijo
        printf("Señal tipo 2 (SIGUSR2) recibida. Soy %d\n", getpid());
        break;
    case SIGTERM:
        // Terminación ordenada: el proceso que recibe SIGTERM se despide y sale
        printf("Señal de terminación (SIGTERM) recibida. Soy %d. Saliendo...\n", getpid());
        // Importante: finalizar el proceso que recibió SIGTERM
        _exit(0); // salir inmediatamente desde el handler
        break;
    default:
        printf("Señal %d recibida. Soy %d\n", numero_de_senhal, getpid());
        break;
    }
}
