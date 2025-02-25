// vectores.h
#ifndef VECTORES_H
#define VECTORES_H

#define CAPACIDAD_INICIAL 4

typedef struct {
    int capacidad;
    int totalElementos;
    void **elementos;
} vectorDinamico;

void vectorInicio(vectorDinamico *V);
int totalVector(vectorDinamico *V);
void addVector(vectorDinamico *V, void *elemento);
void *getVector(vectorDinamico *V, int indice);
void setVector(vectorDinamico *V, int indice, void *elemento);
void borrarVector(vectorDinamico *V, int indice);
void freeVector(vectorDinamico *V);

#endif
