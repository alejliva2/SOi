#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/**
 * Manejador genérico para señales enviadas con kill desde otra terminal.
 *
 * @param sig Número de señal recibida.
 */
void manejadorMensajeKill(int sig) { 
    // solo se manda este mensaje si hacemos kill [PID] desde otra terminal
    printf("Recibida señal %d. Sigo vivo.\n", sig);
}


int main(void) {
    printf("PID del proceso: %d\n", getpid()); // mandamos el PID para poder hacer kill desde otra terminal
    printf("Esperando mensaje por entrada (scanf)...\n");
    printf("Prueba a matarme con: kill -KILL <PID>\n");

    // Instalar el manejador para SIGTERM y SIGINT
    struct sigaction sa; // Estructura para especificar la acción de la señal
    sa.sa_handler = manejadorMensajeKill; // asignamos la función que hay que hacer al recibir la señal
    sigemptyset(&sa.sa_mask); // No bloquear otras señales durante la ejecución del manejador
    sa.sa_flags = 0;

    sigaction(SIGTERM, &sa, NULL);  // captura SIGTERM -> esta señal se envía con kill y lo que hace es terminar el programa
    // no usamos SIGKILL porque entonces terminaria el programa sin llamar al manejador, es decir, termina automaticamente el programa
    sigaction(SIGINT,  &sa, NULL);  // captura SIGINT -> esta señal se envía con Ctrl+C y lo que hace es terminar el programa

    int x;
    if (scanf("%d", &x) == 1) {
        printf("Leído: %d\n", x);
    } else { 
        printf("scanf terminó sin leer un entero.\n");
    }

    printf("Fin normal del programa.\n");
    return 0;
}
