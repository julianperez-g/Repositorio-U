
/****************************************
Autor: Julian Camilo Perez
Grupo: Daniel Galviz, David Rodriguez, Sebastian Rodriguez
Fecha: 21 de febrero 2024
Materia: Estructura de Datos
Tema: implementación BST 
****************************************/

#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo *left, *right;
    Nodo(int d) : dato(d), left(NULL), right(NULL) {}
};    

Nodo* nuevoNodo(int dato) {
    return new Nodo(dato);
}

/******* Preorder *********/
void preOrdenT(Nodo *raiz) {
    if (raiz == NULL)
        return;
    cout << raiz->dato << "->"; 
    preOrdenT(raiz->left);
    preOrdenT(raiz->right);
}

/******* Postorder *********/
void postOrdenT(Nodo *raiz) {
    if (raiz == NULL)
        return;
    postOrdenT(raiz->left);
    postOrdenT(raiz->right);
    cout << raiz->dato << "->"; 
}

/******* Inorder *********/
void inOrdenT(Nodo *raiz) {
    if (raiz == NULL)
        return;
    inOrdenT(raiz->left);
    cout << raiz->dato << "->"; 
    inOrdenT(raiz->right);
}

/****** Insertar nuevo nodo, dado un valor (BST) ********/
Nodo* insertarBST(Nodo *nodo, int newDato) {
    if (nodo == NULL)
        return nuevoNodo(newDato);
    if (newDato < nodo->dato)
        nodo->left = insertarBST(nodo->left, newDato);
    else
        nodo->right = insertarBST(nodo->right, newDato);

    return nodo;
}

/****** función que retorna el nodo con menor valor en un BST ********/
Nodo* minValorNodo(Nodo *nodo) {
    Nodo *actual = nodo;
    /*Se busca iterativamente el elemento menor a la izquierda*/
    while (actual && actual->left != NULL)
        actual = actual->left;
    return actual;
}

/****** función que elimina un nodo con valor en un BST ********/
Nodo* borrarNodo(Nodo *nodo, int dato) {
    /*=Arbol vacio=*/
    if (nodo == NULL)
        return nodo;
    /*=si el valor es menor a la raiz, ir por la izquierda=*/
    if (dato < nodo->dato)
        nodo->left = borrarNodo(nodo->left, dato);
    /*=si el valor es mayor a la raiz, ir por la derecha=*/
    else if (dato > nodo->dato)
        nodo->right = borrarNodo(nodo->right, dato);
    /*=el valor esta en la misma raiz=*/
    else {
        /*=Nodo con solo un hijo o sin hijos=*/    
        if (nodo->left == NULL) {
            Nodo *temporal = nodo->right;
            delete nodo;  // Cambiado a delete
            return temporal;
        } else if (nodo->right == NULL) {
            Nodo *temporal = nodo->left;
            delete nodo;  // Cambiado a delete
            return temporal;
        }
        /*=Nodo con ambos hijos, tomar el sucesor y borrar el nodo=*/
        Nodo *temp = minValorNodo(nodo->right);
        /*=copiar el contenido InOrder sucesor a el nodo=*/
        nodo->dato = temp->dato;
        /*=borrar el InOrder sucesor=*/
        nodo->right = borrarNodo(nodo->right, temp->dato);
    }
    return nodo;
}

int main() {
    Nodo *arbol1 = NULL; 

    // Insertar los nuevos valores
    int valores[] = {55, 35, 75, 25, 45, 65, 85, 20, 30, 40, 50, 60, 70, 80, 90};
    for (int i = 0; i < sizeof(valores) / sizeof(valores[0]); ++i) {
        arbol1 = insertarBST(arbol1, valores[i]);
    }

    // Recorridos en el árbol
    cout << "\n -PreOrden ";  
    preOrdenT(arbol1);
    cout << "\n -PostOrden "; 
    postOrdenT(arbol1);
    cout << "\n -InOrden ";
    inOrdenT(arbol1);

    // Eliminar un nodo
    cout << "\n\nSe elimina el nodo (75):" << endl;
    arbol1 = borrarNodo(arbol1, 75);

    // Recorrido en orden después de la eliminación
    cout << "\n -InOrden después de eliminar 75: ";
    inOrdenT(arbol1);
    cout << endl;

    return 0;    
}
