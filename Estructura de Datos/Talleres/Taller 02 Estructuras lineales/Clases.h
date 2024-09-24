#ifndef CLASES_H
#define CLASES_H

#include <iostream>
#include <vector>
#include <list>
#include <string>

/**
 * Clase que representa una palabra en un archivo de texto.
 */
class Palabra {
private:
    std::string palabra; /**< La palabra en sí */
    unsigned int n_linea; /**< El número de línea en el que se encuentra la palabra */

public:
    /**
     * Constructor por defecto. Inicializa la palabra y el número de línea en valores vacíos.
     */
    Palabra() : palabra(""), n_linea(0) {}

    /**
     * Constructor que inicializa la palabra y el número de línea con los valores proporcionados.
     * @param n_palabra La palabra a inicializar
     * @param n_num El número de línea en el que se encuentra la palabra
     */
    Palabra(const std::string& n_palabra, unsigned int n_num)
        : palabra(n_palabra), n_linea(n_num) {}

    /**
     * Establece la palabra.
     * @param n_palabra La nueva palabra
     */
    void FijarPalabra(const std::string& n_palabra) {
        palabra = n_palabra;
    }

    /**
     * Establece el número de línea.
     * @param n_num El nuevo número de línea
     */
    void FijarNumLinea(unsigned int n_num) {
        n_linea = n_num;
    }

    /**
     * Devuelve la palabra.
     * @return La palabra
     */
    std::string ObtenerPalabra() const {
        return palabra;
    }

    /**
     * Devuelve el número de línea.
     * @return El número de línea
     */
    unsigned int ObtenerNumLinea() const {
        return n_linea;
    }
};


/**
 * Clase que representa un archivo de texto.
 */
class ArchivoTexto {
private:
    std::vector<std::list<Palabra>> lineasTexto; /**< Las líneas de texto del archivo */

public:
    /**
     * Constructor por defecto. Inicializa el archivo de texto vacío.
     */
    ArchivoTexto() {}

    /**
     * Establece la lista de líneas de texto.
     * @param n_lista La nueva lista de líneas de texto
     */
    void FijarListaLineas(const std::vector<std::list<Palabra>>& n_lista) {
        lineasTexto = n_lista;
    }

    /**
     * Devuelve la lista de líneas de texto.
     * @return La lista de líneas de texto
     */
    std::vector<std::list<Palabra>> ObtenerListaLineas() const {
        return lineasTexto;
    }

    /**
     * Devuelve el número de líneas de texto.
     * @return El número de líneas de texto
     */
    size_t ObtenerNumLineas() const {
        return lineasTexto.size();
    }

    /**
     * Agrega una lista de palabras a la lista de líneas de texto.
     * @param n_lista La lista de palabras a agregar
     */
    void AgregarListaPals(const std::list<Palabra>& n_lista) {
        lineasTexto.push_back(n_lista);
    }

    /**
     * Busca palabras que comienzan con una subcadena.
     * @param subcadena La subcadena a buscar
     * @return La lista de palabras que comienzan con la subcadena
     */
    std::list<Palabra> BuscarPrincipio(const std::string& subcadena) const {
        std::list<Palabra> resultados;

        for (std::vector<std::list<Palabra>>::const_iterator linea_it = lineasTexto.begin(); linea_it != lineasTexto.end(); ++linea_it) {
            for (std::list<Palabra>::const_iterator palabra_it = linea_it->begin(); palabra_it != linea_it->end(); ++palabra_it) {
                if (palabra_it->ObtenerPalabra().find(subcadena) == 0) {
                    resultados.push_back(*palabra_it);
                }
            }
        }

        return resultados;
    }

    /** 
     * Busca palabras que contienen una subcadena.
     * @param subcadena La subcadena a buscar
     * @return La lista de palabras que contienen la subcadena
     */
    std::list<Palabra> BuscarContiene(const std::string& subcadena) const {
        std::list<Palabra> resultados;

        for (std::vector<std::list<Palabra>>::const_iterator linea_it = lineasTexto.begin(); linea_it != lineasTexto.end(); ++linea_it) {
            for (std::list<Palabra>::const_iterator palabra_it = linea_it->begin(); palabra_it != linea_it->end(); ++palabra_it) {
                if (palabra_it->ObtenerPalabra().find(subcadena) != std::string::npos) {
                    resultados.push_back(*palabra_it);
                }
            }
        }

        return resultados;
    }
};

#endif // CLASES_H
