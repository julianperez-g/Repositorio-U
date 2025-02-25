// matrices.h
#ifndef MATRICES_H
#define MATRICES_H

void iniMatriz(int *ma, int *mb, int N);
void imprimirMatriz(int *matriz, int N, const char *nombre);
void multiMatriz(int *mA, int *mB, int *mC, int N);
void liberarMemoria(int *mA, int *mB, int *mC);

#endif
