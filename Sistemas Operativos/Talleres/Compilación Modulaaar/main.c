/****************************
Nombre: Julian Perez
Fecha: 25-02-25
Materia: Sistemas Operativos
Tema: Compilación Modular
*//////////////////////////////

// main.c
#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"
#include "vectores.h"

int main() {
    int opcion;
    
    do {
        printf("\nSeleccione una opción:\n");
        printf("1. Operaciones con Matrices\n");
        printf("2. Operaciones con Vectores\n");
        printf("3. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        
        if (opcion == 1) {
            int N;
            printf("Ingrese tamaño de la matriz: ");
            scanf("%d", &N);

            int *mA = (int *)malloc(N * N * sizeof(int));
            int *mB = (int *)malloc(N * N * sizeof(int));
            int *mC = (int *)calloc(N * N, sizeof(int));

            if (!mA || !mB || !mC) {
                printf("Error en la asignación de memoria.\n");
                return 1;
            }

            iniMatriz(mA, mB, N);
            imprimirMatriz(mA, N, "Matriz A");
            imprimirMatriz(mB, N, "Matriz B");
            multiMatriz(mA, mB, mC, N);
            imprimirMatriz(mC, N, "Matriz C (Resultado)");

            liberarMemoria(mA, mB, mC);
        } 
        else if (opcion == 2) {
            vectorDinamico v;
            vectorInicio(&v);

            addVector(&v, "Hola");
            addVector(&v, "Mundo");

            for (int i = 0; i < totalVector(&v); i++) {
                printf("%s ", (char *)getVector(&v, i));
            }
            printf("\n");

            freeVector(&v);
        }
    } while (opcion != 3);
    
    return 0;
}
