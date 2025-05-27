/**
 * @file biblioteca.c
 * @brief Sistema de gestión de préstamos y devoluciones de libros para bibliotecas
 * @author Julián Camilo Pérez Gómez, Diego Andrés Martínez Cuervo, Jose David Ontiveros Gutiérrez, Daniel Galviz 
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

// --------------------- CONSTANTES Y CONFIGURACIÓN ---------------------

#define MAX_LIBROS 100          ///< Máximo número de libros en el sistema
#define MAX_EJEMPLARES 10       ///< Máximo ejemplares por libro
#define TAM_BUFFER 10           ///< Tamaño del buffer de solicitudes
#define MAX_EVENTOS 1000        ///< Máximo eventos registrados

// --------------------- VARIABLES GLOBALES ---------------------

int verbose = 0;                ///< Modo verboso (1=activado, 0=desactivado)
FILE* logFile = NULL;           ///< Archivo de log para modo verboso

// --------------------- ESTRUCTURAS DE DATOS ---------------------

/**
 * @struct Ejemplar
 * @brief Representa un ejemplar físico de un libro
 */
typedef struct {
  int numero;                   ///< Número identificador del ejemplar
  char estado;                  ///< 'D'=disponible, 'P'=prestado
  char fecha[20];               ///< Fecha de préstamo/devolución (dd-mm-aaaa)
} Ejemplar;

/**
 * @struct Libro
 * @brief Representa un libro en el catálogo
 */
typedef struct {
  char nombre[100];             ///< Título del libro
  int isbn;                     ///< Código ISBN único
  int cantidad;                 ///< Cantidad total de ejemplares
  Ejemplar ejemplares[MAX_EJEMPLARES]; ///< Array de ejemplares disponibles
} Libro;

/**
 * @struct Solicitud
 * @brief Representa una solicitud de operación
 */
typedef struct {
  char tipo;                    ///< 'D'=devolución, 'R'=renovación
  char nombreLibro[100];        ///< Nombre del libro solicitado
  int isbn;                     ///< ISBN del libro solicitado
} Solicitud;

/**
 * @struct Evento
 * @brief Registra una transacción en el sistema
 */
typedef struct {
  char status[10];              ///< "Prestado", "Devuelto", "Renovado"
  char nombreLibro[100];        ///< Nombre del libro afectado
  int isbn;                     ///< ISBN del libro
  int ejemplar;                 ///< Número de ejemplar
  char fecha[20];               ///< Fecha de la transacción
} Evento;

// --------------------- VARIABLES COMPARTIDAS ---------------------

Evento eventos[MAX_EVENTOS];            ///< Registro histórico de eventos
int total_eventos = 0;                  ///< Contador de eventos registrados
pthread_mutex_t mutex_eventos = PTHREAD_MUTEX_INITIALIZER; ///< Mutex para eventos

Solicitud buffer[TAM_BUFFER];           ///< Buffer circular para solicitudes
int in = 0;                             ///< Índice de inserción en buffer
int out = 0;                            ///< Índice de extracción en buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; ///< Mutex para buffer
sem_t hay_datos;                        ///< Semáforo para datos disponibles
sem_t hay_espacio;                      ///< Semáforo para espacio disponible

int programa_terminado = 0;             ///< Flag para finalización del programa

// --------------------- PROTOTIPOS DE FUNCIONES ---------------------

int cargarBaseDatos(const char* archivo, Libro* libros);
void obtenerFechaActual(char* buffer, size_t tamano);
void procesar_Prestamo(char* nombreLibro, int isbn, Libro* libros, int total_libros);
void* hilo_consumidor(void* arg);
void guardarBaseDatos(const char* archivo, Libro* libros, int total_libros);
void log_event(const char* formato, ...);
void* escuchar_comandos(void* arg);
void registrar_evento(const char* status, const char* nombreLibro, int isbn, int ejemplar, const char* fecha);

// --------------------- FUNCIONES PRINCIPALES ---------------------

/**
 * @brief Función principal del programa
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @return int Código de salida (EXIT_SUCCESS o EXIT_FAILURE)
 * 
 * Esta función maneja:
 * - Parseo de argumentos de línea de comandos
 * - Inicialización del sistema
 * - Creación de hilos
 * - Procesamiento principal
 */
int main(int argc, char* argv[]) {
  char* pipeNombre = NULL;      ///< Nombre del pipe para comunicación
  char* archivoDatos = NULL;    ///< Archivo de base de datos de libros
  char* archivoSalida = NULL;   ///< Archivo de salida (opcional)
  
  // Parsear argumentos de línea de comandos
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

  // Validar parámetros obligatorios
  if (pipeNombre == NULL || archivoDatos == NULL) {
    fprintf(stderr, "Error: Los parámetros -p y -f son obligatorios.\n");
    fprintf(stderr,
            "Uso: %s -p pipeReceptor -f filedatos [-v] [-s filesalida]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  // Configurar modo verboso
  if (verbose) {
    logFile = fopen("log.txt", "w");
    if (!logFile) {
      perror("No se pudo abrir log.txt");
      return EXIT_FAILURE;
    }
  }

  // Mostrar configuración
  printf("Pipe receptor: %s\n", pipeNombre);
  printf("Archivo de datos: %s\n", archivoDatos);
  if (archivoSalida != NULL) {
    printf("Archivo de salida: %s\n", archivoSalida);
  }
  if (verbose) {
    printf("Verbose activado.\n");
  }

  // Cargar base de datos de libros
  Libro libros[MAX_LIBROS];
  int total_libros = cargarBaseDatos(archivoDatos, libros);

  if (total_libros < 0) {
    fprintf(stderr, "Error cargando la base de datos.\n");
    return EXIT_FAILURE;
  }

  // Mostrar resumen de libros cargados
  printf("\nLibros cargados:\n");
  for (int i = 0; i < total_libros; i++) {
    printf("%s (ISBN %d): %d ejemplares\n", libros[i].nombre, libros[i].isbn,
           libros[i].cantidad);
    for (int j = 0; j < libros[i].cantidad; j++) {
      printf("  Ejemplar %d - Estado: %c - Fecha: %s\n",
             libros[i].ejemplares[j].numero, libros[i].ejemplares[j].estado,
             libros[i].ejemplares[j].fecha);
    }
  }

  // Inicializar semáforos
  sem_init(&hay_datos, 0, 0);
  sem_init(&hay_espacio, 0, TAM_BUFFER);

  // Crear hilo consumidor
  pthread_t hilo;
  pthread_create(&hilo, NULL, hilo_consumidor, libros);

  // Abrir pipe para lectura
  FILE* pipe = fopen(pipeNombre, "r");
  if (!pipe) {
    perror("Error abriendo el pipe para lectura");
    return EXIT_FAILURE;
  }

  // Crear hilo para comandos de usuario
  pthread_t hilo_comandos;
  pthread_create(&hilo_comandos, NULL, escuchar_comandos, NULL);

  // Procesar solicitudes del pipe
  printf("\nEsperando solicitudes...\n");
  char linea[256];
  while (fgets(linea, sizeof(linea), pipe)) {
    linea[strcspn(linea, "\n")] = '\0';  // Eliminar salto de línea
    
    if (verbose) {
      log_event("Solicitud recibida: %s\n", linea);
    }

    // Parsear operación
    char operacion, nombreLibro[100];
    int isbn;

    if (sscanf(linea, " %c, %[^,], %d", &operacion, nombreLibro, &isbn) == 3) {
      switch (operacion) {
        case 'P': // Préstamo
          procesar_Prestamo(nombreLibro, isbn, libros, total_libros);
          break;
          
        case 'D': // Devolución
        case 'R': // Renovación
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
          break;
          
        case 'Q': // Finalización
          printf("Un PS ha indicado que finalizó\n");
          break;
          
        default:
          printf("Operación no implementada: %c\n", operacion);
      }
    } else {
      printf("Formato inválido en la solicitud\n");
    }
  }

  // Guardar estado final si se especificó archivo de salida
  if (archivoSalida != NULL) {
    guardarBaseDatos(archivoSalida, libros, total_libros);
  }

  // Cerrar archivo de log si está abierto
  if (verbose && logFile) {
    fclose(logFile);
  }

  return EXIT_SUCCESS;
}

// --------------------- FUNCIONES AUXILIARES ---------------------

/**
 * @brief Carga la base de datos de libros desde un archivo
 * @param archivo Nombre del archivo a cargar
 * @param libros Array donde almacenar los libros leídos
 * @return int Número de libros cargados, o -1 en caso de error
 * 
 * Formato esperado del archivo:
 * Título, ISBN, Cantidad
 * NúmeroEjemplar, Estado, Fecha
 * ...
 * (línea vacía entre libros)
 */
int cargarBaseDatos(const char* archivo, Libro* libros) {
  FILE* file = fopen(archivo, "r");
  if (!file) {
    perror("No se pudo abrir el archivo de base de datos");
    return -1;
  }

  char linea[256];
  int total_libros = 0;

  while (fgets(linea, sizeof(linea), file) {
    if (strlen(linea) <= 1) continue;  // Saltar líneas vacías

    // Leer encabezado del libro
    Libro libro;
    if (sscanf(linea, " %[^,], %d, %d", libro.nombre, &libro.isbn,
               &libro.cantidad) != 3) {
      continue;  // No era cabecera de libro
    }

    // Leer ejemplares
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

/**
 * @brief Procesa una solicitud de préstamo de libro
 * @param nombreLibro Nombre del libro solicitado
 * @param isbn ISBN del libro solicitado
 * @param libros Array de libros disponibles
 * @param total_libros Número total de libros en el array
 * 
 * Busca un ejemplar disponible del libro solicitado y lo marca como prestado.
 * Registra el evento y actualiza la fecha de préstamo.
 */
void procesar_Prestamo(char* nombreLibro, int isbn, Libro* libros, int total_libros) {
  for (int i = 0; i < total_libros; i++) {
    if (libros[i].isbn == isbn) {
      // Buscar ejemplar disponible
      for (int j = 0; j < libros[i].cantidad; j++) {
        if (libros[i].ejemplares[j].estado == 'D') {
          // Marcar como prestado
          libros[i].ejemplares[j].estado = 'P';
          char fecha_actual[20];
          obtenerFechaActual(fecha_actual, sizeof(fecha_actual));
          strcpy(libros[i].ejemplares[j].fecha, fecha_actual);
          
          printf("Préstamo realizado: %s (ejemplar %d)\n", libros[i].nombre,
                 libros[i].ejemplares[j].numero);

          if (verbose) {
            log_event("Préstamo realizado: %s (ejemplar %d)\n",
                      libros[i].nombre, libros[i].ejemplares[j].numero);
          }
          
          registrar_evento("Prestado", libros[i].nombre, isbn,
                           libros[i].ejemplares[j].numero, fecha_actual);
          return;
        }
      }
      
      printf("No hay ejemplares disponibles de \"%s\" (ISBN %d)\n",
             libros[i].nombre, isbn);
      return;
    }
  }
  
  printf("No se encontró el libro con ISBN %d\n", isbn);
}

/**
 * @brief Hilo consumidor que procesa solicitudes de devolución/renovación
 * @param arg Puntero al array de libros (Libro*)
 * @return void* NULL
 * 
 * Este hilo:
 * 1. Espera a que haya solicitudes en el buffer
 * 2. Extrae una solicitud (FIFO)
 * 3. Procesa la devolución/renovación
 * 4. Actualiza el estado del ejemplar
 */
void* hilo_consumidor(void* arg) {
  Libro* libros = (Libro*)arg;

  while (1) {
    sem_wait(&hay_datos);       // Esperar datos disponibles
    pthread_mutex_lock(&mutex); // Bloquear acceso al buffer

    Solicitud s = buffer[out];  // Obtener solicitud
    out = (out + 1) % TAM_BUFFER; // Actualizar índice

    pthread_mutex_unlock(&mutex);
    sem_post(&hay_espacio);     // Liberar espacio en buffer

    // Procesar devolución o renovación
    for (int i = 0; i < MAX_LIBROS; i++) {
      if (libros[i].isbn == s.isbn) {
        for (int j = 0; j < libros[i].cantidad; j++) {
          if (libros[i].ejemplares[j].estado == 'P') {
            // Marcar como disponible
            libros[i].ejemplares[j].estado = 'D';
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

/**
 * @brief Guarda el estado actual de los libros en un archivo
 * @param archivo Nombre del archivo de salida
 * @param libros Array de libros a guardar
 * @param total_libros Número total de libros en el array
 * 
 * El formato de salida es compatible con el de entrada (cargarBaseDatos)
 */
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

/**
 * @brief Registra un mensaje en el log (modo verboso)
 * @param formato Cadena de formato (como printf)
 * @param ... Argumentos variables para el formato
 * 
 * Escribe tanto en consola como en el archivo de log (si está activo)
 */
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

/**
 * @brief Obtiene la fecha actual en formato dd-mm-aaaa
 * @param buffer Donde almacenar la fecha
 * @param tamano Tamaño del buffer (mínimo 11 caracteres)
 */
void obtenerFechaActual(char* buffer, size_t tamano) {
  time_t t = time(NULL);          // Obtener el tiempo actual
  struct tm tm = *localtime(&t);  // Convertirlo a tiempo local

  // Formato dd-mm-aaaa
  strftime(buffer, tamano, "%d-%m-%Y", &tm);
}

/**
 * @brief Registra un evento en el historial de transacciones
 * @param status Tipo de evento ("Prestado", "Devuelto", "Renovado")
 * @param nombreLibro Nombre del libro afectado
 * @param isbn ISBN del libro
 * @param ejemplar Número de ejemplar
 * @param fecha Fecha de la transacción
 * 
 * Esta función es thread-safe (protegida por mutex)
 */
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

/**
 * @brief Hilo para escuchar comandos del usuario
 * @param arg No utilizado (NULL)
 * @return void* NULL
 * 
 * Comandos disponibles:
 * - 's': Salir del programa
 * - 'r': Mostrar reporte de transacciones
 */
void* escuchar_comandos(void* arg) {
  char comando[10];

  while (!programa_terminado) {
    if (fgets(comando, sizeof(comando), stdin)) {
      comando[strcspn(comando, "\n")] = '\0';  // quitar salto de línea

      if (strcmp(comando, "s") == 0) {
        printf("Comando de salida recibido. Finalizando...\n");
        programa_terminado = 1;
        exit(0);  // Terminar el programa completo
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
