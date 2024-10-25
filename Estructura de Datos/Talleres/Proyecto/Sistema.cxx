#include <deque>
#include <iostream>
#include <string_view>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "Sistema.h"

using namespace std;

/**
 * Constructor de la clase Sistema.
 */
Sistema::Sistema() : objetos() {}

/**
 * Devuelve la lista de objetos en el sistema.
 *
 * @return deque<Objeto> lista de objetos
 */
deque<Objeto> &Sistema::getObjetos() { return objetos; }

/**
 * Establece la lista de objetos en el sistema.
 *
 * @param o lista de objetos
 */
void Sistema::setObjetos(deque<Objeto> o) { this->objetos = o; }

/**
 * Busca un objeto por nombre en el sistema.
 *
 * @param nombre nombre del objeto a buscar
 * @return int índice del objeto si existe, -1 si no existe
 */
int Sistema::buscarObjeto(string nombre)
{
    int i;
    for (i = 0; i < objetos.size(); ++i)
    {
        if (objetos[i].getNombre() == nombre)
        {
            return i;
        }
    }
    return -1;
}

/**
 * Verifica si un nombre es válido (no vacío y sin espacios).
 *
 * @param nombre nombre a verificar
 * @return bool true si el nombre es válido, false si no lo es
 */
bool Sistema::nombreValido(string_view nombre)
{
    if (nombre.empty())
        return false;

    for (auto it = nombre.begin(); it != nombre.end(); ++it)
    {
        if (*it == ' ')
            return false;
    }
    return true;
}

/**
 * Verifica si un número es natural (no vacío y solo dígitos).
 *
 * @param numero número a verificar
 * @return bool true si el número es natural, false si no lo es
 */
bool Sistema::numeroNaturalValido(string_view numero)
{
    if (numero.empty())
        return false;

    for (auto it = numero.begin(); it != numero.end(); it++)
    {
        if (!isdigit(*it))
            return false;
    }

    return true;
}

/**
 * Verifica si un número es entero (no vacío y solo dígitos, puede ser negativo).
 *
 * @param numero número a verificar
 * @return bool true si el número es entero, false si no lo es
 */
bool Sistema::numeroEnteroValido(string_view numero)
{
    if (numero.empty())
        return false;

    if ((numero[0] == '-') && (numero.length() > 1))
        numero = numero.substr(1);

    for (auto it = numero.begin(); it != numero.end(); it++)
    {
        if (!isdigit(*it))
            return false;
    }

    return true;
}

/**
 * Verifica si un archivo contiene un objeto 3D válido.
 *
 * @param ruta ruta del archivo a verificar
 * @return bool true si el archivo contiene un objeto 3D válido, false si no lo es
 */
bool Sistema::validarArchivo(string ruta)
{
    ifstream archivo(ruta, ios::ate);

    if (archivo.tellg() == 0)
        return false;

    archivo.seekg(0);

    string linea, aux;

    if (!getline(archivo, linea) || !nombreValido(linea))
        return false;

    if (!getline(archivo, linea) || !numeroNaturalValido(linea))
        return false;

    int numero = stoi(linea);

    for (int i = 0; i < numero; i++)
    {
        if (!getline(archivo, linea))
            return false;

        istringstream iss(linea);

        for (int j = 0; j < 3; ++j)
        {
            if (!(iss >> aux) || !numeroEnteroValido(aux))
                return false;
        }

        if (!iss.eof())
            return false;
    }

    archivo.clear();
    archivo.seekg(0, ios::end);
    int fileSize = archivo.tellg();

    for (int pos = fileSize - 1; pos >= 0; --pos)
    {
        archivo.seekg(pos);
        if (archivo.peek() == '\n')
        {
            archivo.seekg(pos + 1);
            break;
        }
    }

    getline(archivo, linea);

    if (linea != "-1")
        return false;

    archivo.close();
    return true;
}

/**
 * Crea aristas para una cara.
 *
 * @param cara cara a la que se le crean aristas
 */
void Sistema::crearAristas(Cara &cara)
{
    for (auto it = cara.getIndicesVertice().begin(); it != cara.getIndicesVertice().end(); it++)
    {
        if (it + 1 != cara.getIndicesVertice().end())
            cara.getAristas().push_back(Arista(*it, *(it + 1)));
        else
            cara.getAristas().push_back(Arista(*it, cara.getIndicesVertice().front()));
    }
}

/**
 * Carga un objeto desde un archivo.
 *
 * @param ruta ruta del archivo que contiene el objeto
 */
void Sistema::cargarObjeto(string ruta)
{
    ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        cout << "\nEl archivo " << ruta << " no existe o es ilegible.\n\n";
        return;
    }

    if (!validarArchivo(ruta))
    {
        cout << "\nEl archivo " << ruta << " no contiene un objeto 3D valido.\n\n";
        archivo.close();
        return;
    }

    string linea;
    getline(archivo, linea);

    if (buscarObjeto(linea) != -1)
    {
        cout << "\nEl objeto " << linea << " ya ha sido cargado en memoria.\n\n";
        archivo.close();
        return;
    }

    Objeto objeto(linea);

    getline(archivo, linea);

    int cantidad = stoi(linea);
    float x, y, z;

    for (int i = 0; i < cantidad; i++)
    {
        getline(archivo, linea);
        istringstream iss(linea);
        iss >> x >> y >> z;
        objeto.getVertices().push_back(Vertice(x, y, z));
    }

    while ((getline(archivo, linea)) && (linea != "-1"))
    {
        istringstream iss(linea);
        string aux;
        iss >> aux;
        int numero = stoi(aux);
        Cara cara;

        for (int i = 0; i < numero; i++)
        {
            iss >> aux;
            int indice = stoi(aux);
            cara.getIndicesVertice().push_back(indice);
        }

        crearAristas(cara);
        objeto.getCaras().push_back(cara);
    }

    objetos.push_back(objeto);
    archivo.close();
    cout << "\nEl objeto " << objeto.getNombre() << " ha sido cargado exitosamente desde el archivo " << ruta << "." << endl
         << endl;
}

/**
 * Muestra la lista de objetos en memoria.
 */
void Sistema::listado()
{
    if (objetos.empty())
    {
        cout << "\nNingun objeto ha sido cargado en memoria.\n\n";
        return;
    }

    int i = 1;

    cout << "\nHay " << objetos.size() << " objeto(s) en memoria:\n\n";

    for (auto it = objetos.begin(); it != objetos.end(); it++, i++)
    {
        cout << i << ". " << it->getNombre() << " contiene " << it->getVertices().size() << " veritces, ";
        cout << it->calcularAristasUnicas() << " aristas y " << it->getCaras().size() << " caras.\n";
    }

    cout << endl;
}

/**
 * Agrega vértices a una cara.
 *
 * @param cara cara a la que se le agregan vértices
 * @param i1 índice del primer vértice
 * @param i2 índice del segundo vértice
 * @param i3 índice del tercer vértice
 * @param i4 índice del cuarto vértice
 */
void Sistema::agregarVerticesCara(Cara &cara, int i1, int i2, int i3, int i4)
{
    cara.getIndicesVertice().push_back(i1);
    cara.getIndicesVertice().push_back(i2);
    cara.getIndicesVertice().push_back(i3);
    cara.getIndicesVertice().push_back(i4);
}

/**
 * Crea una caja envolvente para un objeto.
 *
 * @param nombre nombre del objeto para el que se crea la caja envolvente
 */
void Sistema::envolvente(string nombre)
{
    int indice = buscarObjeto(nombre);

    if (indice == -1)
    {
        cout << "\nEl objeto " << nombre << " no ha sido cargado en memoria.\n\n";
        return;
    }

    Objeto objeto = objetos[indice];
    string nombreEnvolvente = "env_";
    nombreEnvolvente += objeto.getNombre();

    if(buscarObjeto(nombreEnvolvente) != -1)
    {
        cout << "\nEL envolvente del objeto " << nombre << " ya existe.\n\n";
        return;
    }

    Objeto envolvente(nombreEnvolvente);

    Vertice puntoMax = objeto.puntoMayor();
    Vertice puntoMin = objeto.puntoMenor();

    envolvente.getVertices().push_back(puntoMin);                                                   // Posicion 0
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMin.getY(), puntoMin.getZ())); // Posicion 1
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMax.getY(), puntoMin.getZ())); // Posicion 2
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMax.getY(), puntoMin.getZ())); // Posicion 3
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMin.getY(), puntoMax.getZ())); // Posicion 4
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMin.getY(), puntoMax.getZ())); // Posicion 5
    envolvente.getVertices().push_back(puntoMax);                                                   // Posicion 6
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMax.getY(), puntoMax.getZ())); // Posicion 7

    Cara frontal, trasera, izquierda, derecha, superior, inferior;

    agregarVerticesCara(frontal, 0, 1, 2, 3);
    agregarVerticesCara(trasera, 4, 5, 6, 7);
    agregarVerticesCara(izquierda, 0, 4, 7, 3);
    agregarVerticesCara(derecha, 1, 5, 6, 2);
    agregarVerticesCara(superior, 3, 2, 6, 7);
    agregarVerticesCara(inferior, 1, 5, 6, 2);

    crearAristas(frontal);
    crearAristas(trasera);
    crearAristas(izquierda);
    crearAristas(derecha);
    crearAristas(superior);
    crearAristas(inferior);

    envolvente.getCaras().push_back(frontal);
    envolvente.getCaras().push_back(trasera);
    envolvente.getCaras().push_back(izquierda);
    envolvente.getCaras().push_back(derecha);
    envolvente.getCaras().push_back(superior);
    envolvente.getCaras().push_back(inferior);

    objetos.push_back(envolvente);
    cout << "\nLa caja envolvente del objeto " << objeto.getNombre() << " se ha generado con el nombre " << nombreEnvolvente << " y se ha agregado a los objetos en memoria.\n\n";
}

/**
 * Devuelve el punto máximo general de todos los objetos en memoria.
 *
 * @return Vertice punto máximo general
 */
Vertice Sistema::puntoMayorGeneral()
{
    Vertice puntoMax = objetos.front().puntoMayor();

    for (auto it = objetos.begin() + 1; it != objetos.end(); it++)
    {
        if (it->puntoMayor().getX() > puntoMax.getX())
        {
            puntoMax.setX(it->puntoMayor().getX());
        }
        if (it->puntoMayor().getY() > puntoMax.getY())
        {
            puntoMax.setY(it->puntoMayor().getY());
        }
        if (it->puntoMayor().getZ() > puntoMax.getZ())
        {
            puntoMax.setZ(it->puntoMayor().getZ());
        }
    }

    return puntoMax;
}

/**
 * Devuelve el punto mínimo general de todos los objetos en memoria.
 *
 * @return Vertice punto mínimo general
 */
Vertice Sistema::puntoMenorGeneral()
{
    Vertice puntoMin = objetos.front().puntoMenor();

    for (auto it = objetos.begin() + 1; it != objetos.end(); it++)
    {
        if (it->puntoMenor().getX() < puntoMin.getX())
        {
            puntoMin.setX(it->puntoMenor().getX());
        }
        if (it->puntoMenor().getY() < puntoMin.getY())
        {
            puntoMin.setY(it->puntoMenor().getY());
        }
        if (it->puntoMenor().getZ() < puntoMin.getZ())
        {
            puntoMin.setZ(it->puntoMenor().getZ());
        }
    }

    return puntoMin;
}

/**
 * Crea una caja envolvente global para todos los objetos en memoria.
 */
void Sistema::envolventeGlobal()
{
    if (objetos.empty())
    {
        cout << "\nNingun objeto ha sido cargado en memoria.\n\n";
        return;
    }

    if(buscarObjeto("env_global") != -1)
    {
        descargar("env_global");
        envolventeGlobal();
        return;
    }

    Objeto envolvente("env_global");

    Vertice puntoMax = puntoMayorGeneral();
    Vertice puntoMin = puntoMenorGeneral();

    envolvente.getVertices().push_back(puntoMin);                                                   // Posicion 0
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMin.getY(), puntoMin.getZ())); // Posicion 1
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMax.getY(), puntoMin.getZ())); // Posicion 2
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMax.getY(), puntoMin.getZ())); // Posicion 3
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMin.getY(), puntoMax.getZ())); // Posicion 4
    envolvente.getVertices().push_back(Vertice(puntoMax.getX(), puntoMin.getY(), puntoMax.getZ())); // Posicion 5
    envolvente.getVertices().push_back(puntoMax);                                                   // Posicion 6
    envolvente.getVertices().push_back(Vertice(puntoMin.getX(), puntoMax.getY(), puntoMax.getZ())); // Posicion 7

    Cara frontal, trasera, izquierda, derecha, superior, inferior;

    agregarVerticesCara(frontal, 0, 1, 2, 3);
    agregarVerticesCara(trasera, 4, 5, 6, 7);
    agregarVerticesCara(izquierda, 0, 4, 7, 3);
    agregarVerticesCara(derecha, 1, 5, 6, 2);
    agregarVerticesCara(superior, 3, 2, 6, 7);
    agregarVerticesCara(inferior, 1, 5, 6, 2);

    crearAristas(frontal);
    crearAristas(trasera);
    crearAristas(izquierda);
    crearAristas(derecha);
    crearAristas(superior);
    crearAristas(inferior);

    envolvente.getCaras().push_back(frontal);
    envolvente.getCaras().push_back(trasera);
    envolvente.getCaras().push_back(izquierda);
    envolvente.getCaras().push_back(derecha);
    envolvente.getCaras().push_back(superior);
    envolvente.getCaras().push_back(inferior);

    objetos.push_back(envolvente);

    cout << "\nLa caja envolvente de los objetos en memoria se ha generado con el nombre env_global y se ha agregado a los objetos en memoria.\n\n";
}

/**
 * Elimina un objeto de la memoria.
 *
 * @param nombre nombre del objeto a eliminar
 */
void Sistema::descargar(string nombre)
{
    int indice = buscarObjeto(nombre);

    if (indice == -1)
    {
        cout << "\nEl objeto " << nombre << " no ha sido cargado en memoria.\n\n";
        return;
    }

    auto it = objetos.begin() + indice;
    objetos.erase(it);

    cout << "\nEl objeto " << nombre << " ha sido eliminado de la memoria de trabajo.\n\n";
}

/**
 * Guarda un objeto en un archivo.
 *
 * @param nombre nombre del objeto a guardar
 * @param ruta ruta del archivo donde se guardará el objeto
 */
void Sistema::guardar(string nombre, string ruta)
{
    int indice = buscarObjeto(nombre);

    if (indice == -1)
    {
        cout << "\nEl objeto " << nombre << " no ha sido cargado en memoria.\n\n";
        return;
    }

    ofstream salida(ruta);

    salida << objetos[indice].getNombre() << endl;
    salida << objetos[indice].getVertices().size() << endl;

    for (auto it = objetos[indice].getVertices().begin(); it != objetos[indice].getVertices().end(); it++)
    {
        salida << it->getX() << " " << it->getY() << " " << it->getZ() << endl;
    }

    for (auto it = objetos[indice].getCaras().begin(); it != objetos[indice].getCaras().end(); it++)
    {
        salida << it->getIndicesVertice().size() << " ";

        for (auto itV = it->getIndicesVertice().begin(); itV != it->getIndicesVertice().end(); itV++)
        {
            if (itV + 1 != it->getIndicesVertice().end())
                salida << *itV << " ";

            else
                salida << *itV << endl;
        }
    }

    salida << "-1";

    salida.close();

    cout << "\nLa información del objeto " << nombre << " ha sido guardada exitosamente en el archivo " << ruta << endl
         << endl;
}

void Sistema::verticeCercanoObjeto(float px, float py, float pz, string nombre)
{
    int indice = buscarObjeto(nombre);

    if (indice == -1)
    {
        cout << "\nEl objeto " << nombre << " no ha sido cargado en memoria.\n\n";
        return;
    }

    Vertice v(px, py, pz);

    int indiceVertice = objetos[indice].verticeCercano(v);

    float vx = objetos[indice].getVertices()[indiceVertice].getX();
    float vy = objetos[indice].getVertices()[indiceVertice].getY();
    float vz = objetos[indice].getVertices()[indiceVertice].getZ();
    float distancia = objetos[indice].getVertices()[indiceVertice].distancia(v);

    string coordenadasV = "(" + to_string(vx) + ", " + to_string(vy) + ", " + to_string(vz) + ")";
    string coordenadasP = "(" + to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + ")";

    cout << "\nEl vertice " << indiceVertice << " " << coordenadasV << " del objeto " << nombre
         << " es el mas cercano al punto " << coordenadasP << " , a una distancia de " << distancia << ".\n\n";
}

void Sistema::verticeCercanoGeneral(float px, float py, float pz)
{
    if (objetos.empty())
    {
        cout << "\nNo hay objetos cargados en memoria\n\n";
        return;
    }

    Vertice v(px, py, pz);

    Objeto *ptr = &objetos.front();
    int indiceVertice = objetos.front().verticeCercano(v);
    float menorDistancia = objetos.front().getVertices()[indiceVertice].distancia(v);

    for (auto it = objetos.begin() + 1; it != objetos.end(); it++)
    {
        int indiceTemporal = it->verticeCercano(v);
        float distanciaActual = it->getVertices()[indiceTemporal].distancia(v);

        if (distanciaActual < menorDistancia)
        {
            menorDistancia = distanciaActual;
            indiceVertice = indiceTemporal;
            ptr = &(*it);
        }
    }

    float vx = ptr->getVertices()[indiceVertice].getX();
    float vy = ptr->getVertices()[indiceVertice].getY();
    float vz = ptr->getVertices()[indiceVertice].getZ();

    string coordenadasV = "(" + to_string(vx) + ", " + to_string(vy) + ", " + to_string(vz) + ")";
    string coordenadasP = "(" + to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + ")";

    cout << "\nEl vertice " << indiceVertice << " " << coordenadasV << " del objeto " << ptr->getNombre()
         << " es el mas cercano al punto " << coordenadasP << " , a una distancia de " << menorDistancia << ".\n\n";
}

void Sistema::verticeCercanosCaja (string nombre)
{
    int indiceObjeto = buscarObjeto(nombre);

    if (indiceObjeto == -1)
    {
        cout << "\nEl objeto " << nombre << " no ha sido cargado en memoria.\n\n";
        return;
    }

    string nombreEnvolvente = "env_" + nombre;
    
    int indiceEnvolvente = buscarObjeto(nombreEnvolvente);

    if (indiceEnvolvente == -1)
    {
        cout << "\nEl objeto " << nombre << " no tiene caja envolvente por el momento.\n\n";
        return;
    }

    Objeto* objeto = &objetos[indiceObjeto];
    Objeto* envolvente = &objetos[indiceEnvolvente];
    

    cout << "Los vertices del objeto " << nombre << " más cercanos a las esquinas de su caja envolvente son:\n\n"
         << "Esquina\t\t\t\tVertice\t\t\t\tDistancia\n";

    for(int i = 0; i < 8; i++)
    {
        Vertice verticeEnvolvente = envolvente->getVertices()[i];
        int indice = objeto->verticeCercano(verticeEnvolvente);
        Vertice verticeObjeto = objeto->getVertices()[indice];
        float distancia = verticeObjeto.distancia(verticeEnvolvente);

        float vx = verticeEnvolvente.getX();
        float vy = verticeEnvolvente.getY();
        float vz = verticeEnvolvente.getZ();

        float px = verticeObjeto.getX();
        float py = verticeObjeto.getY();
        float pz = verticeObjeto.getZ();

        string coordenadasV = " (" + to_string(vx) + ", " + to_string(vy) + ", " + to_string(vz) + ")";
        string coordenadasP = " (" + to_string(px) + ", " + to_string(py) + ", " + to_string(pz) + ")";

        cout << i << coordenadasV << "\t\t\t\t" << indice << coordenadasP << "\t\t\t\t" << distancia << endl;
    }

}
