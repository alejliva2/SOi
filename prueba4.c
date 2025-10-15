#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t h1, h2, padre;
    padre = getpid();

    // creamos el primer hijo, H1
    h1 = fork();
    if (h1 < 0) {
        perror("Error creando H1");
        exit(1);
    }

    if (h1 == 0) {
        // Código del hijo H1
        printf("Soy H1 (PID=%d), hijo de %d. Me quedo esperando...\n", getpid(), getppid());
        // H1 se queda esperando indefinidamente en el bucle
        for (;;) {
            pause();
        }
        exit(0);
    }

    // Crear segundo hijo H2
    h2 = fork();
    if (h2 < 0) {
        perror("Error creando H2");
        exit(1);
    }

    if (h2 == 0) {
        // Código del hijo H2
        printf("Soy H2 (PID=%d), hijo de %d.\n", getpid(), getppid());
    
        char resp;
        do {
            printf("Indique 'S' o 's' cuando quiera terminar a H1 (PID=%d)? (s/n): ", h1);
            scanf(" %c", &resp);
        } while (resp != 's' && resp != 'S');
    
        // Cuando el usuario finalmente diga 's'
        if (kill(h1, SIGTERM) == 0) {
            printf("H2: He enviado SIGTERM a H1 (PID=%d).\n", h1);
        } else {
            perror("H2: Error al enviar la señal a H1");
        }
    
        printf("H2 (PID=%d) termina normalmente.\n", getpid());
        exit(0);
    }
    

    // Código del padre P
    printf("Soy el padre P (PID=%d). Esperando a H1 (PID=%d)...\n", padre, h1);

    int status;
    pid_t terminado = waitpid(h1, &status, 0); // espera específicamente a H1

    if (terminado == -1) {
        perror("Error en waitpid");
    } 
    
    else {
        if (WIFSIGNALED(status)) {
            printf("Padre: H1 (PID=%d) terminó por señal %d.\n", terminado, WTERMSIG(status));
        } 
        
        else if (WIFEXITED(status)) {
            printf("Padre: H1 (PID=%d) terminó con código %d.\n", terminado, WEXITSTATUS(status));
        }
    }

    // espera también a H2 para que no quede huérfano
    waitpid(h2, NULL, 0);

    printf("Padre (PID=%d) termina normalmente.\n", padre);
    return 0;
}
