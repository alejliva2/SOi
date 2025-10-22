#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// solo se manda este mensaje si hacemos kill [PID] desde otra terminal
void manejador(int sig) {
    printf("Recibida señal %d. Sigo vivo.\n", sig);
}

int main(void) {
    printf("PID del proceso: %d\n", getpid());
    printf("Esperando entrada por stdin (scanf)...\n");
    printf("Prueba a matarme con: kill -KILL <PID>\n");

    struct sigaction sa;
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);

    int x;
    if (scanf("%d", &x) == 1) {
        printf("Leído: %d\n", x);
    } else {
        printf("scanf terminó sin leer un entero.\n");
    }

    printf("Fin normal del programa.\n");
    return 0;
}
