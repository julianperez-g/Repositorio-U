/**************************************************************
 * UNIVERSIDAD JAVERIANA
 * Facultad de IngenierÃ­a - Ingenieria de Sistemas
 * Autor: Julian Perez
 * Fecha: 8/4/2025
 * Archivo: cliente.c
 * Descripcion: Cliente que envia cadenas a traves de un FIFO 
 *              hacia un servidor. El cliente
 *              finaliza su ejecucion al escribir "end".
 **************************************************************/

#include <stdio.h>      // printf, fgets, stdin
#include <stdlib.h>     // exit, EXIT_FAILURE
#include <string.h>     // strcpy, strlen, strcmp
#include <fcntl.h>      // open, O_RDWR, O_CREAT
#include <unistd.h>     // write, close
#include <sys/stat.h>   // permisos para mkfifo

#define FIFO_FILE "/tmp/fifo_twoway"  // Ruta del FIFO

int main() {
    int fd;
    char readbuf[80];
    char end_str[10] = "end";
    int stringlen;
    int end_process;

    printf("FIFO_CLIENT: Escribe mensajes. Para salir, escribe \"end\"\n");

    // Abrir FIFO para lectura/escritura
    fd = open(FIFO_FILE, O_CREAT | O_RDWR, 0640);
    if (fd == -1) {
        perror("Error al abrir el FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Cliente> ");
        // Leer cadena desde stdin
        fgets(readbuf, sizeof(readbuf), stdin);

        // Eliminar el salto de lÃ­nea al final (opcional)
        readbuf[strcspn(readbuf, "\n")] = '\0';

        // Verificar si es "end" para terminar el proceso
        end_process = strcmp(readbuf, end_str);
        if (end_process == 0) {
            write(fd, readbuf, strlen(readbuf));
            break;
        }

        // Enviar cadena al servidor
        write(fd, readbuf, strlen(readbuf));

        // Leer respuesta del servidor (cadena invertida)
        int read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
        if (read_bytes > 0) {
            readbuf[read_bytes] = '\0';  // Asegurar terminaciÃ³n de string
            printf("Servidor> %s\n", readbuf);
        }
    }

    // Cerrar FIFO
    close(fd);
    return 0;
}
