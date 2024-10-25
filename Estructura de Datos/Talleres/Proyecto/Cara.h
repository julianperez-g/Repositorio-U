#ifndef CARA_H
#define CARA_H

#include <deque>
#include "Vertice.h"
#include "Arista.h"

using namespace std;

/**
 * Clase que representa una cara en un grafo.
 */
class Cara
{
private:
    /**
     * Conjunto de índices de vértices que forman parte de esta cara.
     */
    deque<int> indicesVertice;

    /**
     * Conjunto de aristas que forman parte de esta cara.
     */
    deque<Arista> aristas;

public:
    /**
     * Constructor por defecto de la clase Cara.
     */
    Cara();

    /**
     * Obtiene el conjunto de índices de vértices que forman parte de esta cara.
     * 
     * @return Conjunto de índices de vértices.
     * 
     * Ejemplo:
     * Cara cara;
     * deque<int> indices = cara.getIndicesVertice();
     */
    deque<int>& getIndicesVertice();

    /**
     * Obtiene el conjunto de aristas que forman parte de esta cara.
     * 
     * @return Conjunto de aristas.
     * 
     * Ejemplo:
     * Cara cara;
     * deque<Arista> aristas = cara.getAristas();
     */
    deque<Arista>& getAristas();

    /**
     * Establece el conjunto de índices de vértices que forman parte de esta cara.
     * 
     * @param vertices Conjunto de índices de vértices.
     * 
     * Ejemplo:
     * deque<int> vertices = {1, 2, 3};
     * Cara cara;
     * cara.setIndicesVertice(vertices);
     */
    void setIndicesVertice(deque<int> &vertices);

    /**
     * Establece el conjunto de aristas que forman parte de esta cara.
     * 
     * @param aristas Conjunto de aristas.
     * 
     * Ejemplo:
     * deque<Arista> aristas = {Arista(1, 2), Arista(2, 3), Arista(3, 1)};
     * Cara cara;
     * cara.setAristas(aristas);
     */
    void setAristas(deque<Arista> &aristas);
};

#endif // CARA_H
