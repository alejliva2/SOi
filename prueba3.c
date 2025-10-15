#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int contador = 0;  // cuenta cuántas veces se recibió SIGINT

void gestion(int sig) {
    contador++;

    if (contador <= 3) {
        printf("\nSeñal SIGINT atrapada y desactivada (%d)\n", contador);
    } else {
        char respuesta;
        printf("\nHas pulsado Ctrl+C %d veces. ¿Restaurar comportamiento por defecto? (s/n): ", contador);
        fflush(stdout);

        // leer respuesta del usuario
        respuesta = getchar();
        // limpiar el buffer de entrada por si queda un salto de línea
        while (getchar() != '\n');

        if (respuesta == 's' || respuesta == 'S') {
            printf("Restaurando comportamiento por defecto de SIGINT...\n");
            signal(SIGINT, SIG_DFL);  // restaurar acción por defecto
        } else {
            printf("Señal SIGINT atrapada y desactivada (no restaurada)\n");
        }
    }
}

int main(void) {
    // instalar el manejador
    if (signal(SIGINT, gestion) == SIG_ERR) {
        perror("Error al instalar el manejador de SIGINT");
        exit(1);
    }

    printf("PID: %d. Pulsa Ctrl+C para probar.\n", getpid());

    // bucle infinito
    while (1) {
        pause(); // espera señales, consume menos CPU que un while vacío
    }

    return 0;
}
