#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Gestor de señales en senalesPadre.c.
 *
 * @param numero_de_senhal Número de señal recibida.
 */
void gestionPadres(int numero_de_senhal){ 
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
        //exit(); // salir inmediatamente desde el handler
        break;
    default:
        printf("Señal %d recibida. Soy %d\n", numero_de_senhal, getpid());
        break;
    }
}


int main(void) {
    pid_t padre, hijo;

    // instalamos el mismo gestor para SIGTERM que para SI
    if (signal(SIGUSR1, gestionPadres) == SIG_ERR) printf("Error al crear gestor 1\n");
    if (signal(SIGUSR2, gestionPadres) == SIG_ERR) printf("Error al crear gestor 2\n");
    if (signal(SIGTERM, gestionPadres) == SIG_ERR) printf("Error al crear gestor TERM\n");

    padre = getpid();

    if ((hijo = fork()) == 0) {
        kill(padre, SIGUSR1); // hijo envía SIGUSR1 al padre

        // esperamos señales del padre indefinidamente
        for (;;) {
            pause(); // espera hasta recibir una señal
        }
    } else if (hijo > 0) {
        // usamos pause() para sincronizarnos con el SIGUSR1 del hijo:
        pause(); // Padre recibe SIGUSR1 
        kill(hijo, SIGUSR2); // padre envía SIGUSR2 al hijo 

        // esperamos un poco y luego termina al hijo con SIGTERM
        sleep(5); 
        kill(hijo, SIGTERM); // padre envía SIGTERM al hijo, es decir lo termina

        int status;
        waitpid(hijo, &status, 0); // esperamos a que el hijo termine

        if (WIFSIGNALED(status)) { // sigue si el hijo terminó por señal
            printf("Padre (%d): hijo (%d) terminó por señal %d\n", getpid(), hijo, WTERMSIG(status));
        } 
        else if (WIFEXITED(status)) { // sigue si el hijo terminó normalmente
            printf("Padre (%d): hijo (%d) terminó con código %d\n", getpid(), hijo, WEXITSTATUS(status));
        }

        return 0;
    } 
    
    else {
        perror("fork");
        return 1;
    }
}

