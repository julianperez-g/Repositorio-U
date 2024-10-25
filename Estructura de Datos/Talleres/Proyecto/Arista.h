#ifndef ARISTA_H
#define ARISTA_H

using namespace std;

/**
 * @class Arista
 * @brief Representa una arista en un grafo.
 * 
 * Una arista es una conexión entre dos nodos en un grafo. Esta clase proporciona
 * métodos para obtener y establecer los índices de los nodos conectados.
 */
class Arista
{
private:
    int indice1, indice2; ///< Índices de los nodos conectados.

public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa los índices en 0.
     */
    Arista();

    /**
     * @brief Constructor con índices.
     * 
     * @param i1 Índice del primer nodo.
     * @param i2 Índice del segundo nodo.
     * 
     * Ejemplo:
     * @code
     * Arista a(1, 2); // Crea una arista entre los nodos 1 y 2.
     * @endcode
     */
    Arista(int i1, int i2);

    /**
     * @brief Obtiene el índice del primer nodo.
     * 
     * @return int Índice del primer nodo.
     * 
     * Ejemplo:
     * @code
     * Arista a(1, 2);
     * int i1 = a.getIndice1(); // i1 es 1.
     * @endcode
     */
    int getIndice1();

    /**
     * @brief Obtiene el índice del segundo nodo.
     * 
     * @return int Índice del segundo nodo.
     * 
     * Ejemplo:
     * @code
     * Arista a(1, 2);
     * int i2 = a.getIndice2(); // i2 es 2.
     * @endcode
     */
    int getIndice2();

    /**
     * @brief Establece el índice del primer nodo.
     * 
     * @param i Índice del primer nodo.
     * 
     * Ejemplo:
     * @code
     * Arista a(1, 2);
     * a.setIndice1(3); // El índice del primer nodo es ahora 3.
     * @endcode
     */
    void setIndice1(int i);

    /**
     * @brief Establece el índice del segundo nodo.
     * 
     * @param i Índice del segundo nodo.
     * 
     * Ejemplo:
     * @code
     * Arista a(1, 2);
     * a.setIndice2(4); // El índice del segundo nodo es ahora 4.
     * @endcode
     */
    void setIndice2(int i);
};

#endif // ARISTA_H
