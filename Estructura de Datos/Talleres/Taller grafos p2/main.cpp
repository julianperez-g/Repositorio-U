/****************************
Nombre: Julian Perez
Participó: Juan Sebastian Rodriguez
Fecha: 16-10-24
Materia: Estructura de datos
Tema: Grafos
*//////////////////////////////
#include <iostream>
#include "Grafo.h"

using namespace std;

int main() {
    Grafo grafo;

    // Agregar vértices
    grafo.agregarVertice(5);
    grafo.agregarVertice(9);
    grafo.agregarVertice(4);
    grafo.agregarVertice(3);


    // Agregar arista entre vértices
    grafo.agregarArista(5,9);
    grafo.agregarArista(4,3);

    // Imprimir el grafo actual
    grafo.imprimirGrafo();

    return 0;
}
