/**
 * @file cliente_biblioteca.c
 * @brief Cliente para enviar solicitudes al sistema de gestión de biblioteca
 * @author Julián Camilo Pérez Gómez, Diego Andrés Martínez Cuervo, Jose David Ontiveros Gutiérrez, Daniel Galvis
 * @date 26/05/2025
 * 
 * Este programa permite enviar solicitudes de préstamo (P), devolución (D),
 * renovación (R) o terminación (Q) al sistema de biblioteca a través de un pipe.
 * Puede leer solicitudes desde un archivo o interactivamente desde la consola.
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    char* archivoEntrada = NULL;
    char* pipeNombre = NULL;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            archivoEntrada = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipeNombre = argv[++i];
        } else {
            fprintf(stderr, "Uso: %s [-i archivo] -p pipeReceptor\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (pipeNombre == NULL) {
        fprintf(stderr, "Error: El parámetro -p es obligatorio.\n");
        return EXIT_FAILURE;
    }

    FILE* pipe = fopen(pipeNombre, "w");
    if (!pipe) {
        perror("No se pudo abrir el pipe para escritura");
        return EXIT_FAILURE;
    }

    if (archivoEntrada != NULL) {
        FILE* entrada = fopen(archivoEntrada, "r");
        if (!entrada) {
            perror("No se pudo abrir el archivo de entrada");
            fclose(pipe);
            return EXIT_FAILURE;
        }

        char linea[256];
        while (fgets(linea, sizeof(linea), entrada)) {
            fputs(linea, pipe);
            fflush(pipe);
        }

        fclose(entrada);
    } else {
        char operacion, nombreLibro[100];
        int isbn;

        while (1) {
            printf("Operación (P/D/R/Q): ");
            scanf(" %c", &operacion);
            if (operacion == 'Q') {
                fprintf(pipe, "Q, salir, 0\n");
                fflush(pipe);
                break;
            }
            printf("Nombre del libro: ");
            scanf(" %[^\n]", nombreLibro);
            printf("ISBN: ");
            scanf("%d", &isbn);

            fprintf(pipe, "%c, %s, %d\n", operacion, nombreLibro, isbn);
            fflush(pipe);
        }
    }

    fclose(pipe);
    return EXIT_SUCCESS;
}
