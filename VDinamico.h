#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <climits>
#include "ErrorPosicion.h"
using namespace std;

#ifndef VDINAMICO_H
#define VDINAMICO_H

template<class T>
class VDinamico {
    public:
        VDinamico();
        VDinamico(unsigned int tamlog);
        VDinamico(const VDinamico<T>& orig);
        VDinamico(const VDinamico<T>& orig, unsigned int inicio, unsigned int num);
        VDinamico& operator=(const VDinamico<T> &orig);
        T &operator[](unsigned int pos);
        void insertar(const T& dato, unsigned int pos=UINT_MAX);
        T borrar(unsigned int pos=UINT_MAX);
        void aumenta();
        void ordenar();
        void ordenarRev();
        unsigned int BusquedaBin(T& dato);
        unsigned int tamlog();
        virtual ~VDinamico();
    private:
        int _tL;    // Tamaño lógico
        int _tF;    // Tamaño físico
        T* v;
};


/**
 * @brief Constructor por defecto de un objeto VDinamico
 * @brief Se inicializa el tamaño físico a 1 y el lógico a 0.
 */
template<class T>
VDinamico<T>::VDinamico(): _tL(0), _tF(1) {
    v = new T[_tF];
};

/**
 * @brief Constructor de copia de un objeto VDinamico
 * @param Objeto de tipo VDinamico
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico& orig): _tL(orig._tL), _tF(orig._tF) {
    v = new T[_tF];
    for(unsigned int i=0; i<_tL; i++) {
        v[i] = orig.v[i];
    }
};

/**
 * @brief Constructor dando un tamaño lógico inicial iniciando el tamaño físico a la potencia de 2 inmediatamente superior a tamlog
 * @param tamaño logico
 */
template<class T>
VDinamico<T>::VDinamico(unsigned int tamlog) {
    while(_tF <= tamlog) {
        _tF *= 2;
    }
    v = new T[_tF];
};

/**
 * @brief Constructor de copia parcial, el cual genera un vector de tamaño fisico potencia de 2
 * @param orig
 * @param inicio
 * @param num
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico<T>& orig, unsigned int inicio, unsigned int num) {
    if (( inicio < orig._tL) && (inicio+num <= orig._tL && num > 0)) {
        _tL = num;
        while (_tF <= _tL) {
            _tF *= 2;
        }
        v = new T[_tF];
        for (int i=0; i<num; i++) {
            v[i] = orig.v[inicio+i];
        }
    } else {
        throw ErrorPosicion("[VDinamico<T>::vDinamico()]: Los valores estan fuera del rango logico del elemento a copiar");
    }
};

/**
 * @brief Operador asignacion
 * @param orig de tipo VDinamico
 */
template<class T>
VDinamico<T> &VDinamico<T>::operator=(const VDinamico<T>& orig) {
    if (this != &orig) {
        delete[] v;
        _tF = orig._tF;
        
        v = new T[_tL = orig._tL];
        for(unsigned int i=0; i<_tL; i++) {
            v[i] = orig.v[i];
        }
    }
    return *this;
};

/**
 * @brief Operador[] para acceder a un dato para lectura/escritura.
 * @param pos
 */
template <class T>
T& VDinamico<T>::operator[](unsigned int pos) {
    if (pos < _tF) {
        return v[pos];
    }
};

/**
 * @brief Función que aumenta eficientemente la capacidad de un vector en función de si necesita mas capacidad
 */
template<class T>
void VDinamico<T>::aumenta() {
    T *v_aux;
    v_aux = new T[_tF = _tF*2];
    for (int i=0; i<_tL; i++) {
        v_aux[i] = v[i];
    }
    delete[] v;
    v = v_aux;
};

/**
 * @brief Funcion de insertar elementos al final del vector o en una posición determinada en función del parámetro que se le pase o no
 * @param dato de tipo T el cual debe almacenarse en el vector v, posicion en la cual quiero insertar datos 
 *        (si no introduzco nada por dfecto se hara al final del vector) 
 * @return elemento borrado por si se quiere utilizar
 */
template<class T>
void VDinamico<T>::insertar(const T& dato, unsigned int pos) {
    if(pos == UINT_MAX) {
        if (_tL == _tF) {
            aumenta();
        }
        v[_tL] = dato;
    } else {
        if (pos > _tL) {
            throw ErrorPosicion("[VDinamico<T>::vDinamico()]-->Los valores estan fuera del rango logico del elemento a copiar");
        } else if (_tF == _tL) {
            aumenta();
        } else {
            if (_tL == pos && _tF < _tL) {
                v[_tL] = dato;
            } else if (pos < _tL) {
                v[_tL] = v[pos];
                v[pos] = dato;
            }
           
        }
        v[pos] = dato;
    }
    _tL++;
};

/**
 * @brief Funcion de borrado que elimina el ultimo elemento si NO se instroduce ninguna posicion 
 *        y si no borra esa posicion sobreescribiendola y reduciendo el tamaño logico del vector
 * @param posicion en la que se quiere borrar el elemento
 * @return elemento borrado por si se quiere utilizar
 */

template<class T>
T VDinamico<T>::borrar(unsigned int pos) {
    T aux_i;
    T eliminado;
    if (pos == UINT_MAX) {
        if(_tL*3 < _tF) {
            _tF = _tF/2;
            T *v_aux = new T[_tF];
            for (unsigned i=0; i<_tL; i++) {
                v_aux[i] = v[i];
            }
            delete[] v;
            v = v_aux;
        }
        aux_i = v[_tL-1];
    } else if (pos>_tL || pos<0) {
        throw ErrorPosicion("[VDinamico<T>::vDinamico()]: Los valores estan fuera del rango logico del elemento a copiar");
    } else {
        if (_tL*3 < _tF) {
            _tF = _tF/2;
            T *v_aux = new T[_tF];
            for (unsigned i=0; i<_tL; i++) {
                v_aux[i] = v[i];
            }
            delete[] v;
            v = v_aux;
        } else {
            eliminado = v[pos];
            for(int i=0; i<_tL; i++) {
                if(i == pos) {
                    while(i < _tL-1) {
                        v[i] = v[i+1];
                        i++;
                    }
                    break;
                }
            }
        }
    }
    _tL--;
    return aux_i;
}

/**
 * @brief Funcion que ordena un vector T de menor a mayor
 */
template <class T>
void VDinamico<T>::ordenar() {
    sort(v, v+_tL);
};

/**
 * @brief Funcion que ordena al reves un vector T de mayor a menor
 */
template <class T>
void VDinamico<T>::ordenarRev() {
    T aux2;
    sort(v, v+_tL);
    int j = _tL-1;
    for(int i=0; i<_tL/2; i++) {
        aux2 = v[i];
        v[i] = v[j];
        v[j] = aux2;
        j--;
    }
};

/**
 * @brief Funcion que devuelve el atributo de tamaño de la clase VDinamico
 * @return tamaño logico del objeto
 */
template <class T>
unsigned int VDinamico<T>::tamlog() {
    return _tL;
};

/**
 * @brief Funcion que eficientemente encuentra un elemento de tipo T, es necesario que el vector esté ordenado anteriormente
 * @brief Tiene una eficiencia logaritmica
 * @param T& dato: Dato a encontrar
 * @return Posicion en el vector del elemento a buscar o -1 en caso de no encontrarlo
 */
template <class T>
unsigned int VDinamico<T>::BusquedaBin(T& dato) {
    int inf = 0;
    int sup = _tL-1;
    int curIn;
    while (inf <= sup) {
        curIn = (inf+sup)/2;
        if (v[curIn] == dato) {
            return curIn;
        } else if (v[curIn] == inf) {
            return inf;
        } else if (v[curIn] == sup) {
            return sup;
        } else if (v[curIn] < dato) {
            inf = curIn+1;
        } else {
            sup = curIn-1;
        }
    }
    return -1;
};

/**
 * @brief Destructor del VDinamico
 */
template <class T>
VDinamico<T>::~VDinamico() {
    delete []v;
};

#endif /* VDINAMICO_H */