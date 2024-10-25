#ifndef OBJETO_H
#define OBJETO_H

#include <string>
#include <deque>
#include "Vertice.h"
#include "Arista.h"
#include "Cara.h"

using namespace std;

/**
 * @class Objeto
 * @brief Representa un objeto 3D con vértices y caras.
 */
class Objeto
{
private:
    string nombre; ///< Nombre del objeto.
    deque<Vertice> vertices; ///< Lista de vértices del objeto.
    deque<Cara> caras; ///< Lista de caras del objeto.

public:
    /**
     * @brief Constructor por defecto.
     */
    Objeto();

    /**
     * @brief Constructor con nombre del objeto.
     * @param nombre Nombre del objeto.
     */
    Objeto(string nombre);

    /**
     * @brief Obtiene el nombre del objeto.
     * @return Nombre del objeto.
     */
    string getNombre();

    /**
     * @brief Obtiene la lista de vértices del objeto.
     * @return Lista de vértices del objeto.
     */
    deque<Vertice>& getVertices();

    /**
     * @brief Obtiene la lista de caras del objeto.
     * @return Lista de caras del objeto.
     */
    deque<Cara>& getCaras();

    /**
     * @brief Establece el nombre del objeto.
     * @param n Nuevo nombre del objeto.
     */
    void setNombre(string n);

    /**
     * @brief Establece la lista de vértices del objeto.
     * @param vertices Nueva lista de vértices del objeto.
     */
    void setVertices(deque<Vertice> &vertices);

    /**
     * @brief Establece la lista de caras del objeto.
     * @param caras Nueva lista de caras del objeto.
     */
    void setCaras(deque<Cara> &caras);

    /**
     * @brief Obtiene el vértice con mayor valor en el objeto.
     * @return Vértice con mayor valor en el objeto.
     */
    Vertice puntoMayor();

    /**
     * @brief Obtiene el vértice con menor valor en el objeto.
     * @return Vértice con menor valor en el objeto.
     */
    Vertice puntoMenor();

    /**
     * @brief Calcula el número de aristas únicas en el objeto.
     * @return Número de aristas únicas en el objeto.
     */
    int calcularAristasUnicas();

    int verticeCercano(Vertice &v);
};

#endif // OBJETO_H
