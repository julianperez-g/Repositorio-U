/*  
 * Autor: Julian Perez  
 * Programa: OpenMP.c  
 * Fecha: 13 Mayo 2025  
 * Tema: Programación Paralela - Introducción a OpenMP  
 *  
 * Resumen:  
 * Este programa utiliza la API de OpenMP para crear múltiples hilos  
 * de ejecución. Cada hilo imprime un mensaje que identifica su número.  
 */  
#include <omp.h>     // Cabecera necesaria para invocar la API de OpenMP
#include <stdio.h>   // Para las funciones de entrada/salida (printf)
#include <stdlib.h>  // Para funciones estándar

int main(int argc, char *argv[]) {
    // Imprimimos la cantidad de hilos disponibles en el sistema
    printf("OpenMP ejecutando con %d hilos\n", omp_get_max_threads());

    // Directiva de OpenMP para crear hilos en paralelo
    #pragma omp parallel
    {
        // Cada hilo imprime su identificador único
        printf("Hello World desde el thread %d\n", omp_get_thread_num());
    }

    return 0;
}
