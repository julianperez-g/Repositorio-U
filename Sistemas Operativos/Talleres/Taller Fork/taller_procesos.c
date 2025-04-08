#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * Lee un arreglo de enteros desde un archivo.
 */
void leer_arreglo(const char* archivo, int* arreglo, int n) {
    FILE* f = fopen(archivo, "r");
    if (!f) {
        perror("Error abriendo archivo");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d", &arreglo[i]);
    }
    fclose(f);
}

/**
 * Calcula la suma de los elementos de un arreglo.
 */
int suma_arreglo(int* arreglo, int n) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += arreglo[i];
    }
    return suma;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s N1 archivo00 N2 archivo01\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Convierte los argumentos de entrada a enteros.
    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[3]);

    // Reserva memoria dinámica para ambos arreglos.
    int* A = malloc(sizeof(int) * N1);
    int* B = malloc(sizeof(int) * N2);

    leer_arreglo(argv[2], A, N1);
    leer_arreglo(argv[4], B, N2);

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Primer hijo
        pid_t pid_grand = fork();
        if (pid_grand == 0) {
            // Nieto: calcula la suma de A y la escribe en el pipe.
            int sumaA = suma_arreglo(A, N1);
            write(pipefd[1], &sumaA, sizeof(int));
            exit(0);
        } else {
            wait(NULL); // Espera al nieto
            // Segundo hijo dentro del primer hijo
            pid_t pid2 = fork();
            if (pid2 == 0) {
                // Segundo hijo: calcula la suma de B y la escribe en el pipe.
                int sumaB = suma_arreglo(B, N2);
                write(pipefd[1], &sumaB, sizeof(int));
                exit(0);
            } else {
                wait(NULL); // Espera al segundo hijo
                // El primer hijo ahora calcula la suma total y la escribe en el pipe.
                int sumaTotal = suma_arreglo(A, N1) + suma_arreglo(B, N2);
                write(pipefd[1], &sumaTotal, sizeof(int));
                exit(0);
            }
        }
    } else {
        wait(NULL);           // Espera a que termine el primer hijo
        close(pipefd[1]);     // Cierra el extremo de escritura del pipe

        // Variables para recibir los resultados desde el pipe.
        int sumaA, sumaB, sumaTotal;
        read(pipefd[0], &sumaA, sizeof(int));
        read(pipefd[0], &sumaB, sizeof(int));
        read(pipefd[0], &sumaTotal, sizeof(int));

        // Imprime los resultados.
        printf("Suma del archivo 1: %d\n", sumaA);
        printf("Suma del archivo 2: %d\n", sumaB);
        printf("Suma total: %d\n", sumaTotal);
    }

    // Libera la memoria asignada dinámicamente.
    free(A);
    free(B);
    return 0;
}
