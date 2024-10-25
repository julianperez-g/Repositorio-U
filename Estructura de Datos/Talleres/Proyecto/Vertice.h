#ifndef VERTICE_H
#define VERTICE_H

using namespace std;

/**
 * Clase que representa un vértice en el espacio 3D.
 */
class Vertice
{
private:
    float x, y, z;

public:
    /**
     * Constructor por defecto. Inicializa el vértice con coordenadas (0, 0, 0).
     */
    Vertice();

    /**
     * Constructor que inicializa el vértice con las coordenadas especificadas.
     * 
     * @param x Coordenada x del vértice.
     * @param y Coordenada y del vértice.
     * @param z Coordenada z del vértice.
     * 
     * Ejemplo: Vertice v(1.0, 2.0, 3.0); // Crea un vértice con coordenadas (1, 2, 3).
     */
    Vertice(float x, float y, float z);

    /**
     * Devuelve la coordenada x del vértice.
     * 
     * @return Coordenada x del vértice.
     * 
     * Ejemplo: Vertice v; float x = v.getX(); // x es 0.0.
     */
    float getX();

    /**
     * Devuelve la coordenada y del vértice.
     * 
     * @return Coordenada y del vértice.
     * 
     * Ejemplo: Vertice v; float y = v.getY(); // y es 0.0.
     */
    float getY();

    /**
     * Devuelve la coordenada z del vértice.
     * 
     * @return Coordenada z del vértice.
     * 
     * Ejemplo: Vertice v; float z = v.getZ(); // z es 0.0.
     */
    float getZ();

    /**
     * Establece la coordenada x del vértice.
     * 
     * @param x Nueva coordenada x del vértice.
     * 
     * Ejemplo: Vertice v; v.setX(1.0); // Establece la coordenada x en 1.0.
     */
    void setX(float x);

    /**
     * Establece la coordenada y del vértice.
     * 
     * @param y Nueva coordenada y del vértice.
     * 
     * Ejemplo: Vertice v; v.setY(2.0); // Establece la coordenada y en 2.0.
     */
    void setY(float y);

    /**
     * Establece la coordenada z del vértice.
     * 
     * @param z Nueva coordenada z del vértice.
     * 
     * Ejemplo: Vertice v; v.setZ(3.0); // Establece la coordenada z en 3.0.
     */
    void setZ(float z);

    float distancia(Vertice& otro);
};

#endif // VERTICE_H
