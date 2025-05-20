/***********************************************************
Nombre: Julián Pérez
Materia: Sistemas Operativos
Tema: OpenMP
Descripcion: Uso de directivas basicas basadas en OPenMP para hacer concurrencias y aprovechas el paralelimos con hilos de ejecuciion
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "biblioteca.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <tamMatriz> <numHilos>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int numHilos = atoi(argv[2]);

    srand(time(NULL));

    double **A = crearMatriz(N);
    double **B = crearMatriz(N);
    double **C = crearMatriz(N);

    llenarMatriz(A, N);
    llenarMatriz(B, N);

    double start = omp_get_wtime();
    multiplicarMatriz(A, B, C, N, numHilos);
    double end = omp_get_wtime();

    printf("Tiempo ejecución con %d hilos y tamaño %d: %.6f segundos\n", numHilos, N, end - start);

    liberarMatriz(A, N);
    liberarMatriz(B, N);
    liberarMatriz(C, N);

    return 0;
}
