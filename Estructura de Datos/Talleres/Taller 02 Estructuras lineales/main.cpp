/********************************************************
Pontificia Universidad Javeriana

Grupo: 
-David rodriguez
-Sebastian Rodriguez 
-Daniel Galvis
-Julian Perez

Fecha: 19/08/2024
Materia: Estructuras de datos
Tema: Taller 2 Estructuras lineales
********************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>

#include "Clases.h"


/**
 * Lee un archivo de texto y devuelve un vector de listas de palabras.
 * Cada lista de palabras representa una línea del archivo.
 *
 * @param nombreArchivo El nombre del archivo a leer.
 * @return Un vector de listas de palabras.
 */
std::vector<std::list<Palabra>> leerArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::vector<std::list<Palabra>> vectorLineas;
    std::string palabra;
    int numLineas = 0;

    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        exit(1);
    }

    archivo >> numLineas;
    archivo.ignore('\n');

    vectorLineas.resize(numLineas);

    int lineaActual = 3;
    while ((lineaActual <= numLineas) && (std::getline(archivo, palabra))){
        std::istringstream ss(palabra);
        while (ss >> palabra) {
            Palabra p(palabra, lineaActual);
            vectorLineas[lineaActual - 1].push_back(p);
        }
        ++lineaActual;
    }

    archivo.close();
    return vectorLineas;
}

/**
 * Invierte una cadena de texto.
 *
 * @param cadena La cadena a invertir.
 * @return La cadena invertida.
 */
std::string invertirCadena(const std::string& cadena) {
    std::string invertida = cadena;
    std::reverse(invertida.begin(), invertida.end());
    return invertida;
}

int main(int argc, char* argv[]) {

    /**
     * Se le pasa el nombre del archivo como un argumento desde la terminal al momento de la ejecucion.
     */
    if (argc < 2)
    {
        std::cout<<"No se dio un archivo a leer\n\n";
        exit(1);
    } 
   
    std::string nombreArchivo = argv[1];

    std::ifstream lectura(nombreArchivo);

    std::vector<std::list<Palabra>> vectorLineas = leerArchivo(nombreArchivo);

    ArchivoTexto archivoTexto;

    archivoTexto.FijarListaLineas(vectorLineas);

    std::string subcadena;
    std::getline(lectura, subcadena);
    std::getline(lectura, subcadena);
    lectura.close();

    /**
     * Busca las palabras que empiezan con la subcadena ingresada.
     */
    std::list<Palabra> palabrasInicio = archivoTexto.BuscarPrincipio(subcadena);
    std::cout << "\nHay " << palabrasInicio.size() << " palabras que empiecen con: " << subcadena << std::endl;

    for (auto it = palabrasInicio.begin(); it != palabrasInicio.end(); ++it) {
        std::cout << "linea " << it->ObtenerNumLinea() << ": " << it->ObtenerPalabra() << std::endl;
    }

    /**
     * Busca las palabras que contienen la subcadena ingresada.
     */
    std::list<Palabra> palabrasContiene = archivoTexto.BuscarContiene(subcadena);
    std::cout << "\nHay " << palabrasContiene.size() << " palabras que contienen: " << subcadena << std::endl;
    for (auto it = palabrasContiene.begin(); it != palabrasContiene.end(); ++it) {
        std::cout << "linea " << it->ObtenerNumLinea() << ": " << it->ObtenerPalabra() << std::endl;
    }

    /**
     * Busca las palabras que contienen la subcadena invertida.
     */
    std::string subcadenaInvertida = invertirCadena(subcadena);
    std::list<Palabra> palabrasContieneInvertida = archivoTexto.BuscarContiene(subcadenaInvertida);
    std::cout << "\nHay " << palabrasContieneInvertida.size() << " palabras que contienen: " << subcadenaInvertida << std::endl;
    for (auto it = palabrasContieneInvertida.begin(); it != palabrasContieneInvertida.end(); ++it) {
        std::cout << "linea " << it->ObtenerNumLinea() << ": " << it->ObtenerPalabra() << std::endl;
    }

    return 0;
}
