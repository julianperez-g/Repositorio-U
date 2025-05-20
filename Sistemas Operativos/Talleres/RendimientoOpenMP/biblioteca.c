/***********************************************************
Nombre: Julián Pérez
Materia: Sistemas Operativos
Tema: OpenMP
Descripcion: Uso de directivas basicas basadas en OPenMP para hacer concurrencias y aprovechas el paralelimos con hilos de ejecuciion
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "biblioteca.h"

// Multiplicación clásica de matrices NxN con paralelismo OpenMP
void multiplicarMatriz(double **A, double **B, double **C, int N, int numHilos) {
    omp_set_num_threads(numHilos);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Reserva memoria para una matriz NxN
double **crearMatriz(int N) {
    double **matriz = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++)
        matriz[i] = (double *)malloc(N * sizeof(double));
    return matriz;
}

// Libera memoria
void liberarMatriz(double **matriz, int N) {
    for (int i = 0; i < N; i++)
        free(matriz[i]);
    free(matriz);
}

// Llena la matriz con valores aleatorios entre 0 y 10
void llenarMatriz(double **matriz, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = rand() % 10;
}
