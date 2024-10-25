#include "Cara.h"

using namespace std;

/**
 * Constructor de la clase Cara.
 * Inicializa los atributos indicesVertice y aristas como deques vacíos.
 */
Cara::Cara() : indicesVertice(), aristas() {}

/**
 * Obtiene los índices de los vértices de la cara.
 * 
 * @return Un deque con los índices de los vértices de la cara.
 * 
 * Ejemplo:
 * Cara cara;
 * deque<int> indices = cara.getIndicesVertice();
 */
deque<int>& Cara::getIndicesVertice() { return indicesVertice; }

/**
 * Obtiene las aristas de la cara.
 * 
 * @return Un deque con las aristas de la cara.
 * 
 * Ejemplo:
 * Cara cara;
 * deque<Arista> aristas = cara.getAristas();
 */
deque<Arista>& Cara::getAristas() { return aristas; }

/**
 * Establece los índices de los vértices de la cara.
 * 
 * @param vertices Un deque con los índices de los vértices de la cara.
 * 
 * Ejemplo:
 * deque<int> vertices = {0, 1, 2};
 * Cara cara;
 * cara.setIndicesVertice(vertices);
 */
void Cara::setIndicesVertice(deque<int> &vertices)
{
    this->indicesVertice = vertices;
}

/**
 * Establece las aristas de la cara.
 * 
 * @param aristas Un deque con las aristas de la cara.
 * 
 * Ejemplo:
 * deque<Arista> aristas = {{0, 1}, {1, 2}, {2, 0}};
 * Cara cara;
 * cara.setAristas(aristas);
 */
void Cara::setAristas(deque<Arista> &aristas)
{
    this->aristas = aristas;
}
