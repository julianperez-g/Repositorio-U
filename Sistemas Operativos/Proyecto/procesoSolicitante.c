/**
 * @file cliente_biblioteca.c
 * @brief Cliente para enviar solicitudes al sistema de gestión de biblioteca
 * @author Julián Camilo Pérez Gómez, Diego Andrés Martínez Cuervo, Jose David Ontiveros Gutiérrez, Daniel Galviz 
 * @date 26/05/2025
 * 
 * Este programa permite enviar solicitudes de préstamo (P), devolución (D),
 * renovación (R) o terminación (Q) al sistema de biblioteca a través de un pipe.
 * Puede leer solicitudes desde un archivo o interactivamente desde la consola.
 * 
 * Licencia: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
 * @brief Función principal del cliente
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @return int Código de salida (EXIT_SUCCESS o EXIT_FAILURE)
 * 
 * Uso:
 *   cliente_biblioteca [-i archivo_entrada] -p pipe_nombre
 * 
 * Opciones:
 *   -i archivo  Lee solicitudes desde archivo en lugar de modo interactivo
 *   -p pipe     Nombre del pipe para enviar solicitudes (obligatorio)
 */
int main(int argc, char* argv[]) {
    char* archivoEntrada = NULL;    ///< Nombre del archivo de entrada (opcional)
    char* pipeNombre = NULL;        ///< Nombre del pipe para comunicación (obligatorio)

    // --------------------- PARSEO DE ARGUMENTOS ---------------------
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            archivoEntrada = argv[++i];  // Siguiente argumento es el nombre de archivo
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipeNombre = argv[++i];      // Siguiente argumento es el nombre del pipe
        } else {
            fprintf(stderr, "Uso: %s [-i archivo] -p pipeReceptor\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    // Validar parámetro obligatorio
    if (pipeNombre == NULL) {
        fprintf(stderr, "Error: El parámetro -p es obligatorio.\n");
        return EXIT_FAILURE;
    }

    // --------------------- CONFIGURACIÓN DEL PIPE ---------------------
    
    // Abrir pipe para escritura (el pipe debe existir previamente)
    FILE* pipe = fopen(pipeNombre, "w");
    if (!pipe) {
        perror("No se pudo abrir el pipe para escritura");
        return EXIT_FAILURE;
    }

    // --------------------- MODO ARCHIVO O INTERACTIVO ---------------------
    
    if (archivoEntrada != NULL) {
        // Modo archivo: leer solicitudes desde archivo
        FILE* entrada = fopen(archivoEntrada, "r");
        if (!entrada) {
            perror("No se pudo abrir el archivo de entrada");
            fclose(pipe);
            return EXIT_FAILURE;
        }

        // Leer y enviar línea por línea
        char linea[256];
        while (fgets(linea, sizeof(linea), entrada)) {
            fputs(linea, pipe);
            fflush(pipe);  // Asegurar que se envía inmediatamente
        }

        fclose(entrada);
    } else {
        // Modo interactivo: leer solicitudes desde consola
        char operacion;             ///< Tipo de operación (P/D/R/Q)
        char nombreLibro[100];      ///< Nombre del libro para la operación
        int isbn;                  ///< ISBN del libro

        printf("Modo interactivo. Ingrese solicitudes (Q para salir):\n");
        
        while (1) {
            // Solicitar datos de la operación
            printf("\nOperación (P=Préstamo, D=Devolución, R=Renovación, Q=Salir): ");
            scanf(" %c", &operacion);
            
            // Verificar si es comando de salida
            if (operacion == 'Q') {
                fprintf(pipe, "Q, salir, 0\n");  // Enviar comando de terminación
                fflush(pipe);
                break;
            }

            // Solicitar detalles del libro
            printf("Nombre del libro: ");
            scanf(" %[^\n]", nombreLibro);  // Leer hasta fin de línea
            
            printf("ISBN: ");
            scanf("%d", &isbn);

            // Formatear y enviar la solicitud
            fprintf(pipe, "%c, %s, %d\n", operacion, nombreLibro, isbn);
            fflush(pipe);  // Asegurar envío inmediato
            
            printf("Solicitud enviada: %c, %s, %d\n", operacion, nombreLibro, isbn);
        }
    }

    // --------------------- LIMPIEZA Y SALIDA ---------------------
    
    fclose(pipe);
    return EXIT_SUCCESS;
}
