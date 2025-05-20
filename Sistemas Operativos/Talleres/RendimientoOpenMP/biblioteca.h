/***********************************************************
Nombre: Julián Pérez
Materia: Sistemas Operativos
Tema: OpenMP
Descripcion: Uso de directivas basicas basadas en OPenMP para hacer concurrencias y aprovechas el paralelimos con hilos de ejecuciion
************************************************************/
#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

void multiplicarMatriz(double **A, double **B, double **C, int N, int numHilos);

double **crearMatriz(int N);
void liberarMatriz(double **matriz, int N);
void llenarMatriz(double **matriz, int N);

#endif
