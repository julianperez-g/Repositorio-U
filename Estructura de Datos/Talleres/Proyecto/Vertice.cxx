#include "Vertice.h"
#include <cmath>

using namespace std;

/**
 * Constructor por defecto de la clase Vertice.
 * Inicializa las coordenadas x, y, z en 0.0.
 */
Vertice::Vertice() : x(0.0), y(0.0), z(0.0) {}

/**
 * Constructor de la clase Vertice que inicializa las coordenadas x, y, z con los valores proporcionados.
 * @param x coordenada x del vértice
 * @param y coordenada y del vértice
 * @param z coordenada z del vértice
 * Ejemplo: Vertice v(1.0, 2.0, 3.0);
 */
Vertice::Vertice(float x, float y, float z) : x(x), y(y), z(z) {}

/**
 * Devuelve la coordenada x del vértice.
 * @return coordenada x del vértice
 * Ejemplo: Vertice v; float x = v.getX(); // x = 0.0
 */
float Vertice::getX() { return x; }

/**
 * Devuelve la coordenada y del vértice.
 * @return coordenada y del vértice
 * Ejemplo: Vertice v; float y = v.getY(); // y = 0.0
 */
float Vertice::getY() { return y; }

/**
 * Devuelve la coordenada z del vértice.
 * @return coordenada z del vértice
 * Ejemplo: Vertice v; float z = v.getZ(); // z = 0.0
 */
float Vertice::getZ() { return z; }

/**
 * Establece la coordenada x del vértice.
 * @param x nueva coordenada x del vértice
 * Ejemplo: Vertice v; v.setX(4.0); // x = 4.0
 */
void Vertice::setX(float x) { this->x = x; }

/**
 * Establece la coordenada y del vértice.
 * @param y nueva coordenada y del vértice
 * Ejemplo: Vertice v; v.setY(5.0); // y = 5.0
 */
void Vertice::setY(float y) { this->y = y; }

/**
 * Establece la coordenada z del vértice.
 * @param z nueva coordenada z del vértice
 * Ejemplo: Vertice v; v.setZ(6.0); // z = 6.0
 */
void Vertice::setZ(float z) { this->z = z; }

float Vertice::distancia(Vertice& otro) {
    return sqrt(pow(this->x - otro.getX(), 2) +
                pow(this->y - otro.getY(), 2) +
                pow(this->z - otro.getZ(), 2));
}
