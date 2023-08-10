#include <list>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

#ifndef MALLAREGULAR_H
#define MALLAREGULAR_H

template<typename T>
class MallaRegular;

template<typename T>
class Casilla {
    list<T> puntos;
    public:
        friend class MallaRegular<T>;
        Casilla(): puntos() {
        }
        /**
         * @brief Funcion que sirve para insertar un elemento a la casill
         * @param dato de tipo T
         */
        void insertar(const T &dato) { 
            puntos.push_back(dato);
        }
        T *buscar(const T &dato);
        bool borrar(const T &dato);
        unsigned get_tam() {
            return puntos.size();
        }
};

/**
 * @brief Funcion que sirve para buscar un elemento en la casilla
 * @param dato de tipo T&
 * @return &(*it) si se encuentra, 0 en caso contrario
 */
template<typename T>
T *Casilla<T>::buscar(const T& dato) {
    typename list<T>::iterator it;
    it = puntos.begin();
    for (it = puntos.begin(); it != puntos.end(); ++it) {
        if (*it == dato)
            return &(*it);
    }
    return 0;
}

/**
 * @brief Funcion que sirve para borrar un dato de la casilla
 * @param dato de tipo T&
 * @return true si se elimina, false en caso contrario
 */
template<typename T>
bool Casilla<T>::borrar(const T& dato) {
    typename list<T>::iterator it;
    for (it = puntos.begin(); it != puntos.end(); ++it) {
        if (*it == dato) {
            puntos.erase(it);
            return true;
        }
    }
    return false;
}

template<typename T>
class MallaRegular {
    public:
        MallaRegular() {
        }
        MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, float aNDiv);
        void insertar(float x, float y, const T &dato);
        T *buscar(float x, float y, const T &dato);
        bool borrar(float x, float y, const T &dato);
        vector<T*> buscarRadio(float xcentro, float ycentro, float radio);
        unsigned maxElementosPorCelda();
        float promedioElementosPorCelda();
        unsigned numElementos();
    private:
        float xMin, yMin, xMax, yMax;       // Tamaño real global
        float tamaCasillaX, tamaCasillaY;   // Tamaño real de cada casilla
        vector<vector<Casilla<T> > > mr;    // Vector 2D de casillas
        Casilla<T> *obtenerCasilla(float x, float y);
        unsigned tlog;
        float _aNDiv;
};

/**
 * @brief Constructor parametrizado
 * @param aXMin de tipo float
 * @param aYMin de tipo float
 * @param aXMax de tipo float
 * @param aYMax de tipo float
 * @param aNDiv de tipo float
 */
template<typename T>
MallaRegular<T>::MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, float aNDiv): xMin(aXMin), yMin(aYMin),  xMax(aXMax), yMax(aYMax) {
    tamaCasillaX = (xMax-xMin)/aNDiv;
    tamaCasillaY = (yMax-yMin)/aNDiv;
    mr.insert(mr.begin(), aNDiv, vector<Casilla<T> > (aNDiv));
}

/**
 * @brief Funcion que obtiene una casilla de la malla
 * @param x de tipo float
 * @param y de tipo float
 * @return &mr[i][j] de tipo Casilla<T>
 */
template<typename T>
Casilla<T> *MallaRegular<T>::obtenerCasilla (float x, float y) {
    int i = (x-xMin)/tamaCasillaX;
    if (i < 0) {
        i = 0;
    }
    if (i >= mr.size()) {
        i = mr.size()-1;  
    }
    int j = (y-yMin)/tamaCasillaY;
    if (j<0) {
        j = 0;
    }
    if (j >= mr.size()) {
        j = mr.size()-1;
        
    }
    return &mr[i][j];
}

/**
 * @brief Funcion que sirve para insertar un dato a la malla
 * @param x de tipo float
 * @param y de tipo float
 * @param dato de tipo T
 */
template<typename T>
void MallaRegular<T>::insertar(float x, float y, const T& dato) {
    Casilla<T> *c = obtenerCasilla(x,y);
    c->insertar(dato);
}

/**
 * @brief Funcion que sirve para borrar un dato de la malla
 * @param x de tipo float
 * @param y de tipo float
 * @param dato de tipo T&
 * @return true si se borra, false si no
 */
template<typename T>
bool MallaRegular<T>::borrar(float x, float y, const T& dato) {
    Casilla<T> *c = obtenerCasilla(x,y);
    return c->borrar(dato);
}

/**
 * @brief Funcion que busca un dato en la malla
 * @param x de tipo float
 * @param y de tipo float
 * @param dato de tipo T&
 * @return si encuentra el dato, lo devolvemos
 */
template<typename T>
T* MallaRegular<T>::buscar(float x, float y, const T& dato) {   // <-- Decir excepcion
    Casilla<T> *c = obtenerCasilla(x,y);
    return c->buscar(dato);
}

/**
 * @brief Funcion que busca los elementos existentes en un radio, dando como inicio un punto con coordenadas (x,y)
 * @param xcentro de tipo float
 * @param ycentro de tipo float
 * @param radio de tipo float
 * @return vecAux de tipo vector<T*>
 */
template<typename T>
vector<T*> MallaRegular<T>::buscarRadio(float xcentro, float ycentro, float radio) {
    int imin, imax, jmin, jmax;
    int tama = mr.size();
    
    imin = ((xcentro-radio)-xMin)/tamaCasillaX;
    imax = ((xcentro+radio)-xMin)/tamaCasillaX;
    jmin = ((ycentro-radio)-yMin)/tamaCasillaY;
    jmax = ((ycentro+radio)-yMin)/tamaCasillaY;
    
    if (imin < 0) {
        imin = 0;
    }
    if (jmin < 0) {
        jmin = 0;
    }
    if (imax >= tama) {
        imax = tama-1;
    }
    if (jmax >= tama) {
        jmax = tama-1;
    }
    vector<T*> resultado;
    for (int i=imin; i<=imax; i++) {
        for (int j=jmin; j<=jmax; j++) {
            for (auto iter=mr[i][j].puntos.begin(); iter!=mr[i][j].puntos.end(); iter++) {
                float x,y,distancia;
                x = (*iter)->GetX();
                y = (*iter)->GetY();
                distancia = sqrt(pow(x-xcentro,2)+pow(y-ycentro,2));
                if (distancia <= radio) {
                    resultado.push_back(&(*iter));
                }
            }
        }
    }
    return resultado;
}

/**
 * @brief Funcion que devuelve el numero de elementos de la malla
 * @return tlog de tipo unsigned
 */
template <class T>
unsigned MallaRegular<T>::numElementos() {
    return tlog;
}

/**
 * @brief Funcion que devuelve el elemento maximo de elementos por celda
 * @return max de tipo unsigned
 */
template <class T>
unsigned MallaRegular<T>::maxElementosPorCelda() {
    unsigned max = 0;
    for (float i=xMin; i<xMax; i=i+tamaCasillaX) {
        for (float j=yMin; j<yMax; j=j+tamaCasillaY) {
            Casilla<T> *c;
            c  = obtenerCasilla(i,j);
            if (c->get_tam() > max) {
                max = c->get_tam();
            }
        }
    }
    return max;
}

/**
 * @brief Funcion en la que hallamos el promedio de elemenetos por celda
 * @return Si el promedio esta entre 15 y 20, lo devolvemos, en caso contrario salta una excepción
 */
template <class T>
float MallaRegular<T>::promedioElementosPorCelda() {
    int casillaOcupada = 0;
    for (int i=0; i<_aNDiv; i++) {
        for (int j=0; j<_aNDiv; j++) {
            if (this->obtenerCasilla(i,j)->puntos.size() > 0) {
                casillaOcupada++; 
            } 
        }
    }
    float promedio = (float)numElementos()/(casillaOcupada);
    if (promedio >= 15 && promedio <= 20) {
        return promedio;
    } else {
        throw invalid_argument("Promedio incorrecto");
    }
}

#endif /* MALLAREGULAR_H */