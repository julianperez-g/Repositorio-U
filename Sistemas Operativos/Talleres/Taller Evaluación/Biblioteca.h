#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <pthread.h>
#include <sys/time.h>

// Tamaño del bloque de memoria para las matrices
#define DATA_SIZE (1024 * 1024 * 64 * 3)

// Estructura para los parámetros de cada hilo
struct parametros {
    int nH;   // Número de hilos
    int idH;  // Identificador del hilo
    int N;    // Tamaño de la matriz
};

// Declaración de variables globales
extern pthread_mutex_t MM_mutex;
extern double *mA, *mB, *mC;
extern struct timeval start, stop;

// Declaración de funciones
void llenar_matriz(int SZ);
void print_matrix(int sz, double *matriz);
void inicial_tiempo();
void final_tiempo();
void *mult_thread(void *variables);

#endif // BIBLIOTECA_H
