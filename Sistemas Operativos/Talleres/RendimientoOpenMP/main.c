/***********************************************************
Nombre: Julián Pérez
Materia: Sistemas Operativos
Tema: OpenMP
Descripcion: Uso de directivas basicas basadas en OPenMP para hacer concurrencias y aprovechas el paralelimos con hilos de ejecuciion
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Crea una matriz NxN usando calloc
double **crearMatriz(int N) {
    double **matriz = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriz[i] = (double *)calloc(N, sizeof(double));
    }
    return matriz;
}

// Llena una matriz NxN con valores aleatorios entre 0 y 9
void llenarMatrizAleatoria(double **matriz, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = rand() % 10;
}

// Imprime una matriz (solo útil para tamaños pequeños)
void imprimirMatriz(double **matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%5.1f ", matriz[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Libera la memoria de una matriz NxN
void liberarMatriz(double **matriz, int N) {
    for (int i = 0; i < N; i++)
        free(matriz[i]);
    free(matriz);
}

// Multiplica A y B y guarda el resultado en C, usando OpenMP
void multiplicarMatrices(double **A, double **B, double **C, int N, int numHilos) {
    omp_set_num_threads(numHilos);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}

int main(int argc, char *argv[]) {
    // Verificación de argumentos
    if (argc != 3) {
        printf("Uso: %s <tamMatriz> <numHilos>\n", argv[0]);
        return 1;
    }

    // Guardar argumentos
    int N = atoi(argv[1]);
    int numHilos = atoi(argv[2]);

    if (N <= 0 || numHilos <= 0) {
        printf("Error: El tamaño de matriz y número de hilos deben ser mayores a cero.\n");
        return 1;
    }

    srand(time(NULL));  // Inicializar generador de números aleatorios

    // Crear y llenar matrices
    double **A = crearMatriz(N);
    double **B = crearMatriz(N);
    double **C = crearMatriz(N);

    llenarMatrizAleatoria(A, N);
    llenarMatrizAleatoria(B, N);

    // Imprimir matrices si el tamaño es pequeño (opcional)
    if (N <= 10) {
        printf("Matriz A:\n");
        imprimirMatriz(A, N);
        printf("Matriz B:\n");
        imprimirMatriz(B, N);
    }

    // Medir el tiempo de ejecución
    double inicio = omp_get_wtime();
    multiplicarMatrices(A, B, C, N, numHilos);
    double fin = omp_get_wtime();
    double tiempo = fin - inicio;

    // Imprimir matriz resultado (opcional)
    if (N <= 10) {
        printf("Resultado (C = A x B):\n");
        imprimirMatriz(C, N);
    }

    // Mostrar tiempo de ejecución
    printf("Tiempo de ejecución para N=%d con %d hilos: %.6f segundos\n", N, numHilos, tiempo);

    // Liberar memoria
    liberarMatriz(A, N);
    liberarMatriz(B, N);
    liberarMatriz(C, N);

    return 0;
}
/*Conclusiones:
Paralelización efectiva con OpenMP: El uso de OpenMP en la función multiplicarMatrices permite distribuir la carga de trabajo entre múltiples hilos, lo cual puede reducir significativamente el tiempo de ejecución en equipos con múltiples núcleos.
Escalabilidad según tamaño de la matriz y número de hilos: Para matrices pequeñas, el beneficio de paralelizar puede ser mínimo o incluso negativo debido a la sobrecarga del manejo de hilos. Sin embargo, al aumentar el tamaño de la matriz (N), se observa una mejor escalabilidad y una mayor eficiencia de ejecución en paralelo.
Asignación de memoria dinámica adecuada: El programa usa calloc para crear matrices dinámicamente, lo que asegura que los valores iniciales sean cero y se administre adecuadamente la memoria.
Medición precisa del tiempo de ejecución: Se emplea omp_get_wtime() para medir el tiempo de ejecución, lo que es adecuado para evaluar el rendimiento en aplicaciones paralelas.*/

/*Recomendaciones: 
Evitar impresión en ejecuciones grandes: Para valores altos de N, es preferible deshabilitar completamente la impresión de matrices para evitar sobrecarga en consola y reducir el impacto en el rendimiento.
Agregar verificación de errores al asignar memoria: Aunque poco común, calloc podría fallar si no hay suficiente memoria. Se recomienda verificar que la asignación fue exitosa antes de continuar.
Optimizar orden de bucles para mejorar localidad de datos: Cambiar el orden de los bucles internos puede mejorar la eficiencia de la caché (por ejemplo, usar el orden i-k-j o técnicas como loop tiling para matrices muy grandes).
Pruebas de rendimiento sistemáticas: Automatizar pruebas para diferentes tamaños de matriz y número de hilos, y registrar los resultados, permitiría evaluar mejor el comportamiento del algoritmo en distintas condiciones.
Considerar versiones más eficientes del algoritmo: Para aplicaciones críticas en rendimiento, se podría implementar algoritmos de multiplicación más sofisticados como Strassen o utilizar bibliotecas optimizadas como BLAS.
Agregar opción para guardar resultados: Para experimentos de rendimiento o validación, sería útil guardar los resultados (tiempos, matrices, etc.) en un archivo de texto o CSV.*/
