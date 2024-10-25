#include "Arista.h"

/**
 * Constructor por defecto de la clase Arista.
 * Inicializa los índices en -1.
 */
Arista::Arista() : indice1(-1), indice2(-1) {}

/**
 * Constructor de la clase Arista con parámetros.
 * @param i1 El primer índice de la arista.
 * @param i2 El segundo índice de la arista.
 * 
 * Ejemplo de uso:
 * Arista arista(1, 2); // Crea una arista con índices 1 y 2.
 */
Arista::Arista(int i1, int i2) : indice1(i1), indice2(i2) {}

/**
 * Obtiene el primer índice de la arista.
 * @return El primer índice de la arista.
 * 
 * Ejemplo de uso:
 * Arista arista(1, 2);
 * int indice1 = arista.getIndice1(); // indice1 será 1.
 */
int Arista::getIndice1() { return indice1; }

/**
 * Obtiene el segundo índice de la arista.
 * @return El segundo índice de la arista.
 * 
 * Ejemplo de uso:
 * Arista arista(1, 2);
 * int indice2 = arista.getIndice2(); // indice2 será 2.
 */
int Arista::getIndice2() { return indice2; }

/**
 * Establece el primer índice de la arista.
 * @param i El nuevo valor del primer índice.
 * 
 * Ejemplo de uso:
 * Arista arista(1, 2);
 * arista.setIndice1(3); // El primer índice de la arista será 3.
 */
void Arista::setIndice1(int i) { indice1 = i; }

/**
 * Establece el segundo índice de la arista.
 * @param i El nuevo valor del segundo índice.
 * 
 * Ejemplo de uso:
 * Arista arista(1, 2);
 * arista.setIndice2(4); // El segundo índice de la arista será 4.
 */
void Arista::setIndice2(int i) { indice2 = i; }
