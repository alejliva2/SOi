#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

volatile sig_atomic_t fin = 0;     // bandera para terminar
volatile sig_atomic_t n = 0;       // contador de términos
volatile double ultimo = 0.0;      // último valor calculado

// Manejador de SIGALRM: muestra el último valor y reprograma la alarma
void manejador_alarm(int sig) {
    printf("n=%d, sqrt(n)=%.6f\n", n, ultimo);
    alarm(1); // reprograma la alarma para dentro de 1 segundo
}

// Manejador de SIGUSR1: activa la bandera de fin
void manejador_usr1(int sig) {
    printf("Señal SIGUSR1 recibida. Terminando programa...\n");
    fin = 1;
}

int main(void) {
    // Instalar manejadores
    signal(SIGALRM, manejador_alarm);
    signal(SIGUSR1, manejador_usr1);

    // Primera alarma en 1 segundo
    alarm(1);

    // Bucle infinito de cálculo
    while (!fin) {
        n++;
        ultimo = pow(n, 0.5); // calcula n^(1/2) = sqrt(n)
        // pequeño retardo para no saturar CPU
        usleep(1000);
    }

    printf("Programa finalizado correctamente.\n");
    return 0;
}
