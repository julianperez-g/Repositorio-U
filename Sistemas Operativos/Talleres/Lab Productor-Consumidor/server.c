/**************************************************************
 * UNIVERSIDAD JAVERIANA
 * Facultad de Ingenieria - Ingenieri­a de Sistemas
 * Autor: Julian Perez
 * Fecha: 8/4/2025
 * Archivo: servidor.c
 * Descripcion: Servidor que recibe cadenas por un FIFO,
 *              invierte el contenido de la cadena y la reenvia al cliente.
 *              Termina su ejecucion cuando recibe "end".
 **************************************************************/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit, EXIT_FAILURE
#include <string.h>     // strlen, strcmp, strcpy
#include <fcntl.h>      // open, O_RDWR
#include <unistd.h>     // read, write, close, sleep
#include <sys/stat.h>   // mkfifo

#define FIFO_FILE "/tmp/fifo_twoway"  // Ruta del FIFO

// Prototipo de funcion que invierte una cadena
void reverse_string(char *str);

int main() {
    int fd;
    char readbuf[80];
    char end[10] = "end";
    int to_end;
    int read_bytes;

    // Crear el FIFO si no existe
    if (mkfifo(FIFO_FILE, 0640) == -1) {
        perror("Error al crear el FIFO (puede que ya exista)");
    }

    // Abrir el FIFO para lectura/escritura
    fd = open(FIFO_FILE, O_RDWR);
    if (fd == -1) {
        perror("Error al abrir el FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Leer cadena enviada por el cliente
        read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
        if (read_bytes <= 0) continue;

        readbuf[read_bytes] = '\0';  // Asegurar terminaciÃ³n de string
        printf("Servidor> Recibido: \"%s\" (longitud: %d)\n", readbuf, (int)strlen(readbuf));

        // Verificar si se debe terminar
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }

        // Invertir cadena y reenviar
        reverse_string(readbuf);
        printf("Servidor> Enviando cadena invertida: \"%s\"\n", readbuf);
        write(fd, readbuf, strlen(readbuf));

        // Pausa para evitar leer su propia escritura
        sleep(2);
    }

    return 0;
}

// Funcion que invierte los caracteres de una cadena
void reverse_string(char *str) {
    int first = 0;
    int last = strlen(str) - 1;
    char temp;

    while (first < last) {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
}
