/**
 * @file biblioteca.c
 * @brief Sistema de gestión de préstamos y devoluciones de libros para bibliotecas
 * @author Julián Camilo Pérez Gómez, Diego Andrés Martínez Cuervo, Jose David Ontiveros Gutiérrez, Daniel Galvis
 * @date 26/05/2025
 * 
 * Este programa implementa un sistema concurrente para gestionar préstamos,
 * devoluciones y renovaciones de libros en una biblioteca, utilizando
 * pipes para comunicación y threads para procesamiento concurrente.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <time.h>

int verbose = 0;
FILE* logFile = NULL;

#define MAX_LIBROS 100
#define MAX_EJEMPLARES 10
#define TAM_BUFFER 10

#define MAX_EVENTOS 1000

// --------------------- ESTRUCTURAS ---------------------

typedef struct {
  int numero;
  char estado;  // 'D' = disponible, 'P' = prestado
  char fecha[20];
} Ejemplar;

typedef struct {
  char nombre[100];
  int isbn;
  int cantidad;
  Ejemplar ejemplares[MAX_EJEMPLARES];
} Libro;

typedef struct {
  char tipo;  // 'D' o 'R'
  char nombreLibro[100];
  int isbn;
} Solicitud;

typedef struct {
  char status[10];  // "Prestado", "Devuelto", "Renovado"
  char nombreLibro[100];
  int isbn;
  int ejemplar;
  char fecha[20];
} Evento;

Evento eventos[MAX_EVENTOS];
int total_eventos = 0;
pthread_mutex_t mutex_eventos = PTHREAD_MUTEX_INITIALIZER;
// Declarar buffer, índices, mutex y semáforos

Solicitud buffer[TAM_BUFFER];

int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t hay_datos;
sem_t hay_espacio;

int cargarBaseDatos(const char* archivo, Libro* libros) {
  FILE* file = fopen(archivo, "r");
  if (!file) {
    perror("No se pudo abrir el archivo de base de datos");
    return -1;
  }

  char linea[256];
  int total_libros = 0;

  while (fgets(linea, sizeof(linea), file)) {
    if (strlen(linea) <= 1) continue;  // Saltar líneas vacías

    // Leer encabezado del libro
    Libro libro;
    if (sscanf(linea, " %[^,], %d, %d", libro.nombre, &libro.isbn,
               &libro.cantidad) != 3) {
      continue;  // No era cabecera de libro
    }

    for (int i = 0; i < libro.cantidad; i++) {
      if (fgets(linea, sizeof(linea), file)) {
        int num;
        char estado;
        char fecha[20];
        if (sscanf(linea, " %d, %c, %s", &num, &estado, fecha) == 3) {
          libro.ejemplares[i].numero = num;
          libro.ejemplares[i].estado = estado;
          strcpy(libro.ejemplares[i].fecha, fecha);
        }
      }
    }

    libros[total_libros++] = libro;
  }

  fclose(file);
  return total_libros;
}

void obtenerFechaActual(char* buffer, size_t tamano);
void procesar_Prestamo(char* nombreLibro, int isbn, Libro* libros,
                       int total_libros);
void* hilo_consumidor(void* arg);
void guardarBaseDatos(const char* archivo, Libro* libros, int total_libros);
void log_event(const char* formato, ...);
void* escuchar_comandos(void* arg);
void registrar_evento(const char* status, const char* nombreLibro, int isbn,
                      int ejemplar, const char* fecha);

int main(int argc, char* argv[]) {
  char* pipeNombre = NULL;
  char* archivoDatos = NULL;
  char* archivoSalida = NULL;
  int verbose = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      pipeNombre = argv[++i];
    } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
      archivoDatos = argv[++i];
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      archivoSalida = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0) {
      verbose = 1;
    } else {
      fprintf(stderr,
              "Uso: %s -p pipeReceptor -f filedatos [-v] [-s filesalida]\n",
              argv[0]);
      return EXIT_FAILURE;
    }
  }

  // Validar obligatorios
  if (pipeNombre == NULL || archivoDatos == NULL) {
    fprintf(stderr, "Error: Los parámetros -p y -f son obligatorios.\n");
    fprintf(stderr,
            "Uso: %s -p pipeReceptor -f filedatos [-v] [-s filesalida]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  if (verbose) {
    logFile = fopen("log.txt", "w");
    if (!logFile) {
      perror("No se pudo abrir log.txt");
      return EXIT_FAILURE;
    }
  }

  // Mostrar valores
  printf("Pipe receptor: %s\n", pipeNombre);
  printf("Archivo de datos: %s\n", archivoDatos);
  if (archivoSalida != NULL) {
    printf("Archivo de salida: %s\n", archivoSalida);
  }
  if (verbose) {
    printf("Verbose activado.\n");
  }

  // cargar base de datos
  Libro libros[MAX_LIBROS];

  int total_libros = cargarBaseDatos(archivoDatos, libros);

  if (total_libros < 0) {
    fprintf(stderr, "Error cargando la base de datos.\n");
    return EXIT_FAILURE;
  }

  // mostrar los libros cargados

  printf("\nlibros cargados:\n");
  for (int i = 0; i < total_libros; i++) {
    printf("%s (ISBN %d): %d ejemplares\n", libros[i].nombre, libros[i].isbn,
           libros[i].cantidad);
    for (int j = 0; j < libros[i].cantidad; j++) {
      printf("  Ejemplar %d - Estado: %c - Fecha: %s\n",
             libros[i].ejemplares[j].numero, libros[i].ejemplares[j].estado,
             libros[i].ejemplares[j].fecha);
    }
  }

  pthread_t hilo;
  sem_init(&hay_datos, 0, 0);
  sem_init(&hay_espacio, 0, TAM_BUFFER);

  pthread_create(&hilo, NULL, hilo_consumidor, libros);

  FILE* pipe = fopen(pipeNombre, "r");
  if (!pipe) {
    perror("Error abriendo el pipe para lectura");
    return EXIT_FAILURE;
  }

  char linea[256];

  printf("\nEsperando solicitudes...\n");
  // hilo auxiliar 2
  pthread_t hilo_comandos;
  pthread_create(&hilo_comandos, NULL, escuchar_comandos, NULL);

  while (fgets(linea, sizeof(linea), pipe)) {
    linea[strcspn(linea, "\n")] =
        '\0';  // esto es para eliminar el salto de linea
    if (verbose) {
      log_event("Solicitud recibida: %s\n", linea);
    }

    // procesos segun el tipo de solicitud;

    char operacion, nombreLibro[100];
    int isbn;

    if (sscanf(linea, " %c, %[^,], %d", &operacion, nombreLibro, &isbn) == 3) {
      if (operacion == 'P') {
        procesar_Prestamo(nombreLibro, isbn, libros, total_libros);

      } else if (operacion == 'D' || operacion == 'R') {
        // PRODUCTOR: insertar solicitud en el buffer
        sem_wait(&hay_espacio);
        pthread_mutex_lock(&mutex);

        buffer[in].tipo = operacion;
        strcpy(buffer[in].nombreLibro, nombreLibro);
        buffer[in].isbn = isbn;
        in = (in + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&hay_datos);

        printf("Solicitud de %s enviada al hilo\n",
               operacion == 'D' ? "devolución" : "renovación");
      } else if (operacion == 'Q') {
        printf("Un PS ha indicado que finalizó\n");
      } else {
        printf("Operación no implementada: %c\n", operacion);
      }

    } else {
      printf("Formato inválido en la solicitud\n");
    }
  }

  // guarda en la base de datos
  if (archivoSalida != NULL) {
    guardarBaseDatos(archivoSalida, libros, total_libros);
  }
  // crea un archivo donde se mira todo el proceso
  if (verbose && logFile) {
    fclose(logFile);
  }

  return EXIT_SUCCESS;
}

void procesar_Prestamo(char* nombreLibro, int isbn, Libro* libros,
                       int total_libros) {
  for (int i = 0; i < total_libros; i++) {
    if (libros[i].isbn == isbn) {
      for (int j = 0; j < libros[i].cantidad; j++) {
        if (libros[i].ejemplares[j].estado == 'D') {
          libros[i].ejemplares[j].estado = 'P';
          char fecha_actual[20];
          obtenerFechaActual(fecha_actual, sizeof(fecha_actual));
          strcpy(libros[i].ejemplares[j].fecha, fecha_actual);
          printf("prestamos realizado: %s (ejemplar %d)\n", libros[i].nombre,
                 libros[i].ejemplares[j].numero);

          if (verbose) {
            log_event("prestamos realizado: %s (ejemplar %d)\n",
                      libros[i].nombre, libros[i].ejemplares[j].numero);
          }
          registrar_evento("Prestado", libros[i].nombre, isbn,
                           libros[i].ejemplares[j].numero, fecha_actual);
          return;
        }
      }
      printf("no hay ejemplares disponibles \"%s\" (ISBN %d)\n",
             libros[i].nombre, isbn);
      return;
    }
  }
  printf("no se encontro el libro con ISBN %d\n", isbn);
}

void* hilo_consumidor(void* arg) {
  Libro* libros = (Libro*)arg;

  while (1) {
    sem_wait(&hay_datos);
    pthread_mutex_lock(&mutex);

    Solicitud s = buffer[out];
    out = (out + 1) % TAM_BUFFER;

    pthread_mutex_unlock(&mutex);
    sem_post(&hay_espacio);

    // Procesar devolución o renovación
    for (int i = 0; i < MAX_LIBROS; i++) {
      if (libros[i].isbn == s.isbn) {
        for (int j = 0; j < libros[i].cantidad; j++) {
          if (libros[i].ejemplares[j].estado == 'P') {
            libros[i].ejemplares[j].estado = 'D';  // Marcar como devuelto
            char fecha[20];
            obtenerFechaActual(fecha, sizeof(fecha));
            strcpy(libros[i].ejemplares[j].fecha, fecha);

            printf("%s realizada: %s (Ejemplar %d)\n",
                   s.tipo == 'D' ? "Devolución" : "Renovación",
                   libros[i].nombre, libros[i].ejemplares[j].numero);

            if (verbose) {
              log_event("%s: %s (Ejemplar %d)",
                        s.tipo == 'D' ? "Devolución" : "Renovación",
                        libros[i].nombre, libros[i].ejemplares[j].numero);
            }
            registrar_evento(s.tipo == 'D' ? "Devuelto" : "Renovado",
                             libros[i].nombre, s.isbn,
                             libros[i].ejemplares[j].numero, fecha);
            break;
          }
        }
        break;
      }
    }
  }

  return NULL;
}

void guardarBaseDatos(const char* archivo, Libro* libros, int total_libros) {
  FILE* f = fopen(archivo, "w");
  if (!f) {
    perror("No se pudo crear el archivo de salida");
    return;
  }

  for (int i = 0; i < total_libros; i++) {
    fprintf(f, "%s, %d, %d\n", libros[i].nombre, libros[i].isbn,
            libros[i].cantidad);
    for (int j = 0; j < libros[i].cantidad; j++) {
      fprintf(f, "%d, %c, %s\n", libros[i].ejemplares[j].numero,
              libros[i].ejemplares[j].estado, libros[i].ejemplares[j].fecha);
    }
    fprintf(f, "\n");  // separador entre libros
  }

  fclose(f);
  printf("Estado final guardado en: %s\n", archivo);
}

void log_event(const char* formato, ...) {
  if (!logFile) return;

  va_list args;
  va_start(args, formato);

  // Para imprimir en consola
  va_list args_copy;
  va_copy(args_copy, args);  // Hacer una copia para el segundo uso

  vprintf(formato, args);
  printf("\n");

  vfprintf(logFile, formato, args_copy);
  fprintf(logFile, "\n");

  va_end(args);
  va_end(args_copy);
}

void obtenerFechaActual(char* buffer, size_t tamano) {
  time_t t = time(NULL);          // Obtener el tiempo actual
  struct tm tm = *localtime(&t);  // Convertirlo a tiempo local

  // Formato dd-mm-aaaa
  strftime(buffer, tamano, "%d-%m-%Y", &tm);
}
void registrar_evento(const char* status, const char* nombreLibro, int isbn,
                      int ejemplar, const char* fecha) {
  pthread_mutex_lock(&mutex_eventos);
  if (total_eventos < MAX_EVENTOS) {
    strcpy(eventos[total_eventos].status, status);
    strcpy(eventos[total_eventos].nombreLibro, nombreLibro);
    eventos[total_eventos].isbn = isbn;
    eventos[total_eventos].ejemplar = ejemplar;
    strcpy(eventos[total_eventos].fecha, fecha);
    total_eventos++;
  }
  pthread_mutex_unlock(&mutex_eventos);
}
int programa_terminado = 0;

void* escuchar_comandos(void* arg) {
  char comando[10];

  while (!programa_terminado) {
    if (fgets(comando, sizeof(comando), stdin)) {
      comando[strcspn(comando, "\n")] = '\0';  // quitar salto de línea

      if (strcmp(comando, "s") == 0) {
        printf("Comando de salida recibido. Finalizando...\n");
        programa_terminado = 1;
        exit(0);  // Esto termina el programa completo
      } else if (strcmp(comando, "r") == 0) {
        printf("REPORTE DE TRANSACCIONES:\n");
        printf("Status, Nombre del Libro, ISBN, ejemplar, fecha\n");

        pthread_mutex_lock(&mutex_eventos);
        for (int i = 0; i < total_eventos; i++) {
          printf("%s, %s, %d, %d, %s\n", eventos[i].status,
                 eventos[i].nombreLibro, eventos[i].isbn, eventos[i].ejemplar,
                 eventos[i].fecha);
        }
        pthread_mutex_unlock(&mutex_eventos);
      } else {
        printf("Comando desconocido: %s\n", comando);
      }
    }
  }

  return NULL;
}
