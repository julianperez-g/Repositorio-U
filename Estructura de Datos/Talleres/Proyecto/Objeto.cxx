#include "Objeto.h"

using namespace std;

#include "Objeto.h"
#include <string>
#include <set>

/**
 * @brief Constructor por defecto de la clase Objeto.
 *
 * Inicializa el objeto con un nombre vacío.
 */
Objeto::Objeto() : nombre("") {}

/**
 * @brief Constructor de la clase Objeto con un nombre específico.
 *
 * @param nombre El nombre del objeto.
 */
Objeto::Objeto(string nombre) : nombre(nombre) {}

/**
 * @brief Obtiene el nombre del objeto.
 *
 * @return El nombre del objeto.
 */
string Objeto::getNombre() { return nombre; }

/**
 * @brief Obtiene la lista de vértices del objeto.
 *
 * @return La lista de vértices del objeto.
 */
deque<Vertice> &Objeto::getVertices() { return vertices; }

/**
 * @brief Obtiene la lista de caras del objeto.
 *
 * @return La lista de caras del objeto.
 */
deque<Cara> &Objeto::getCaras() { return caras; }

/**
 * @brief Establece el nombre del objeto.
 *
 * @param n El nuevo nombre del objeto.
 */
void Objeto::setNombre(string n) { this->nombre = n; }

/**
 * @brief Establece la lista de vértices del objeto.
 *
 * @param vertices La nueva lista de vértices del objeto.
 */
void Objeto::setVertices(deque<Vertice> &vertices) { this->vertices = vertices; }

/**
 * @brief Establece la lista de caras del objeto.
 *
 * @param caras La nueva lista de caras del objeto.
 */
void Objeto::setCaras(std::deque<Cara> &caras) { this->caras = caras; }

/**
 * @brief Obtiene el vértice con mayor valor en cada coordenada (x, y, z).
 *
 * @return El vértice con mayor valor en cada coordenada.
 */
Vertice Objeto::puntoMayor()
{
    float mayorx = vertices.front().getX();
    float mayory = vertices.front().getY();
    float mayorz = vertices.front().getZ();

    for (auto it = vertices.begin() + 1; it != vertices.end(); it++)
    {
        if (it->getX() > mayorx)
        {
            mayorx = it->getX();
        }
        if (it->getY() > mayory)
        {
            mayory = it->getY();
        }
        if (it->getZ() > mayorz)
        {
            mayorz = it->getZ();
        }
    }

    return Vertice(mayorx, mayory, mayorz);
}

/**
 * @brief Obtiene el vértice con menor valor en cada coordenada (x, y, z).
 *
 * @return El vértice con menor valor en cada coordenada.
 */
Vertice Objeto::puntoMenor()
{
    float menorx = vertices.front().getX();
    float menory = vertices.front().getY();
    float menorz = vertices.front().getZ();

    for (auto it = vertices.begin() + 1; it != vertices.end(); it++)
    {
        if (it->getX() < menorx)
        {
            menorx = it->getX();
        }
        if (it->getY() < menory)
        {
            menory = it->getY();
        }
        if (it->getZ() < menorz)
        {
            menorz = it->getZ();
        }
    }

    return Vertice(menorx, menory, menorz);
}

/**
 * @brief Calcula el número de aristas únicas en el objeto.
 *
 * @return El número de aristas únicas en el objeto.
 */
int Objeto::calcularAristasUnicas()
{
    set<pair<int, int>> aristasUnicas;

    for (auto itCaras = caras.begin(); itCaras != caras.end(); itCaras++)
    {
        for (auto itAristas = itCaras->getAristas().begin(); itAristas != itCaras->getAristas().end(); itAristas++)
        {
            int menor = min(itAristas->getIndice1(), itAristas->getIndice2());
            int mayor = max(itAristas->getIndice1(), itAristas->getIndice2());

            aristasUnicas.insert(make_pair(menor, mayor));
        }
    }

    return aristasUnicas.size();
}

int Objeto::verticeCercano(Vertice &v)
{
    if (vertices.empty()) 
    {
        return -1; // Indicar que no hay vértices en el objeto
    }

    int indice = 0;
    int i = 1;
    float distanciaActual;

    float menorDistancia = vertices.front().distancia(v);

    for (auto it = vertices.begin() + 1; it != vertices.end(); it++, i++)
    {
        distanciaActual = it->distancia(v);
        if (distanciaActual < menorDistancia)
        {
            menorDistancia = distanciaActual;
            indice = i;
        }
    }

    return indice;
}
