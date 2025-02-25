// vectores.c
#include <stdio.h>
#include <stdlib.h>
#include "vectores.h"

void vectorInicio(vectorDinamico *V) {
    V->capacidad = CAPACIDAD_INICIAL;
    V->totalElementos = 0;
    V->elementos = malloc(sizeof(void *) * V->capacidad);
}

int totalVector(vectorDinamico *V) {
    return V->totalElementos;
}

static void resizeVector(vectorDinamico *V, int capacidad) {
    void **elementos = realloc(V->elementos, sizeof(void *) * capacidad);
    if (elementos) {
        V->elementos = elementos;
        V->capacidad = capacidad;
    }
}

void addVector(vectorDinamico *V, void *elemento) {
    if (V->capacidad == V->totalElementos) {
        resizeVector(V, V->capacidad * 2);
    }
    V->elementos[V->totalElementos++] = elemento;
}

void *getVector(vectorDinamico *V, int indice) {
    if (indice >= 0 && indice < V->totalElementos) {
        return V->elementos[indice];
    }
    return NULL;
}

void setVector(vectorDinamico *V, int indice, void *elemento) {
    if (indice >= 0 && indice < V->totalElementos) {
        V->elementos[indice] = elemento;
    }
}

void borrarVector(vectorDinamico *V, int indice) {
    if (indice < 0 || indice >= V->totalElementos) return;

    for (int i = indice; i < V->totalElementos - 1; i++) {
        V->elementos[i] = V->elementos[i + 1];
    }
    V->totalElementos--;

    if (V->totalElementos > 0 && V->totalElementos == V->capacidad / 4) {
        resizeVector(V, V->capacidad / 2);
    }
}

void freeVector(vectorDinamico *V) {
    free(V->elementos);
}
