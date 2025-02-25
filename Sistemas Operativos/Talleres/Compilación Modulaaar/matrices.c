// matrices.c
#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"

void iniMatriz(int *ma, int *mb, int N) {
    for (int i = 0; i < N * N; i++) {
        ma[i] = 2 * i;
        mb[i] = 2 - i;
    }
}

void imprimirMatriz(int *matriz, int N, const char *nombre) {
    printf("\n%s:\n", nombre);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf(" %4d", matriz[i * N + j]);
        }
        printf("\n");
    }
    printf("-------------------------\n");
}

void multiMatriz(int *mA, int *mB, int *mC, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mC[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                mC[i * N + j] += mA[i * N + k] * mB[k * N + j];
            }
        }
    }
}

void liberarMemoria(int *mA, int *mB, int *mC) {
    free(mA);
    free(mB);
    free(mC);
}
