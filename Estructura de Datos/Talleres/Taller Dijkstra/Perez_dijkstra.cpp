/********************************************************
Pontificia Universidad Javeriana

Grupo: 
-David rodriguez
-Sebastian Rodriguez 
-Julian Perez

Fecha: 30/10/2024
Materia: Estructuras de datos
Tema: Taller 2 Estructuras lineales
********************************************************/

#include <iostream>
#include <climits>
using namespace std;

/**
 * @brief Encuentra el vértice con la distancia mínima entre los vértices no procesados.
 * 
 * @param distance Array de distancias desde el vértice fuente a cada vértice.
 * @param Tset Array booleano que indica si un vértice ha sido procesado.
 * @return int Índice del vértice con la distancia mínima.
 */
int miniDist(int distance[], bool Tset[]) {
    int minimum=INT_MAX,ind;

    for(int k=0;k<7;k++)
    {
        if(Tset[k]==false && distance[k]<=minimum)
        {
            minimum=distance[k];
            ind=k;
        }
    }
    return ind;
}

/**
 * @brief Implementa el algoritmo de Dijkstra para encontrar las distancias más cortas desde un vértice fuente.
 * 
 * @param graph Matriz de adyacencia que representa el grafo.
 * @param src Vértice fuente desde donde se calculan las distancias.
 */
void DijkstraAlgo(int graph[7][7],int src) {
    int distance[7];  // Almacena las distancias más cortas desde src a cada vértice
    bool Tset[7];     // Indica si un vértice ha sido procesado

    // Inicialización: todas las distancias se establecen como infinito y los vértices como no procesados
    for(int k = 0; k<7; k++){
        distance[k] = INT_MAX;
        Tset[k] = false;
    }

    distance[src] = 0;  // La distancia del vértice fuente a sí mismo es 0

    // Encuentra las distancias más cortas para todos los vértices
    for(int k = 0; k<7; k++) {
        int m=miniDist(distance,Tset);  // Encuentra el vértice con la distancia mínima
        Tset[m]=true;  // Marca el vértice como procesado
        
        // Actualiza las distancias de los vértices adyacentes al vértice procesado
        for(int k = 0; k<7; k++){
            /**
             * Actualiza distance[k] si:
             * 1. k no ha sido procesado
             * 2. Hay una arista de m a k
             * 3. La distancia total a través de m es menor que la distancia actual de k
             */
            if(!Tset[k] && graph[m][k] && distance[m]!=INT_MAX && distance[m]+graph[m][k]<distance[k])
                distance[k]=distance[m]+graph[m][k];
        }
    }

    // Imprime los resultados
    cout<<"Vertice \t\t Distancia desde la fuente al Vertice" << endl;
    for(int k = 0; k<7; k++){
        char str=65+k;  // Convierte el índice a letra (A, B, C, ...)
        cout<<str<<"\t\t\t"<<distance[k]<<endl;
    }
}

int main() {
    // Define la matriz de adyacencia del grafo
    int graph[7][7]={
        {0, 2, 4, 7, 17, 22, 19},
        {2, 0, 0, 5, 15, 20, 17},
        {4, 0, 0, 8, 18, 23, 20},
        {7, 5, 8, 0, 10, 15, 12},
        {17, 15, 18, 10, 0, 6, 2},
        {22, 20, 23, 25, 6, 0, 6},
        {19, 17, 20, 12, 2, 6, 0}};
    
    // Ejecuta el algoritmo de Dijkstra con el vértice 0 como fuente
    DijkstraAlgo(graph,0);
    return 0;
}
