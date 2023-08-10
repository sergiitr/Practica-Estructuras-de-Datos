#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <climits>
using namespace std;

#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

template<class T>
class ListaEnlazada {
    template<class X>
    class Nodo {
        public:
            X _dato;
            Nodo *_sig;
            Nodo(const X &aDato, Nodo *aSig = 0): _dato(aDato), _sig(aSig) {
            }
    };
    public:
        Nodo<T> *_cabecera, *_cola;
        unsigned _tama;
    public:
        template<class U>
        class Iterador {
            public:
                Nodo<U> *_nodo;
                Iterador(Nodo<U> *aNodo = 0): _nodo(aNodo) {
                }
                bool fin() {
                    return _nodo == 0;
                }
                void siguiente() {
                    _nodo = _nodo->_sig;
                }
                Iterador<U>& operator=(const Iterador<U>& orig) {
                    _nodo = orig._nodo;
                    return *this;
                }
                U &dato() {
                    return _nodo->_dato;
                }
                virtual ~Iterador() {
                };
        };
        ListaEnlazada(): _cabecera(0), _cola(0), _tama(0) {
        }
        ListaEnlazada(const ListaEnlazada<T>& orig);
        ListaEnlazada &operator=(const ListaEnlazada<T> &orig);
        Iterador<T> iterador() const;
        void insertaInicio(const T& dato);
        void insertaFin(const T& dato);
        void inserta(Iterador<T> &i,const T &dato);
        void borraInicio();
        void borraFinal();
        void borra(Iterador<T> &i);
        int tam();
        bool buscar(const T &dato,Iterador<T> &it);
        virtual ~ListaEnlazada();
        T& inicio () {
            if (_cabecera == nullptr) {
                throw string("Error");
            }
            return _cabecera->_dato;
        }
        T& Final () {
            if (_cola == nullptr) {
                throw string("Error");
            }
            return _cola->_dato;
        }
};

/**
 * @brief Situa el iterador en la cabecera
 * @return Iterador<T> (_cabecera)
 */
template<class T>
ListaEnlazada<T>::Iterador<T> ListaEnlazada<T>::iterador() const {
    return Iterador<T> (_cabecera);
}

/**
 * @brief Devuelve el tamaño de la lista enlazada
 * @return cont
 */
template<class T>
int ListaEnlazada<T>::tam() {
    int cont = 0;
    Nodo<T> *aux;
    if (_cabecera == nullptr && _cola == nullptr) {
        cont = 0;   
    }
    if (_cabecera != _cola) {
        aux = _cabecera;
        cont = 2;
        while (aux->_sig != _cola) {
           cont++;
           aux = aux->_sig;
        } 
    }
    return cont;
}

/**
 * @brief Constructor de copia
 * @param Objeto Listaenlazada de tipo T: orig 
 */
template<class T>
ListaEnlazada<T>::ListaEnlazada(const ListaEnlazada<T>& orig) {
    Nodo<T> *iterador = orig._cabecera;
    _cabecera = 0;
    _cola = 0;
    while (iterador != nullptr) {
        Nodo<T> *nuevo;
        nuevo = new Nodo<T>(iterador->_dato, 0);
        if (_cola != 0) {
            _cola->_sig = nuevo;
        }
        if (_cabecera == 0) {
            _cabecera = nuevo;
        }
        iterador = iterador->_sig;
        _cola = nuevo;
    }
}

/**
 * @brief Funcion que busca un dato de tipo T mediante un iterador
 * @paramin dato de tipo T, objeto de la clase itereador dr tipo t
 * @paramout referencia al iterador donde he encontrado ese dato
 * @return valor booleano
 */
template<class T>
bool ListaEnlazada<T>::buscar(const T& dato, Iterador<T>& it) {
    Nodo<T>* ubico = _cabecera;
    if (ubico != _cola) {
        while (ubico != nullptr) {
            if (ubico->_dato == dato) {
                Iterador<T>itera(ubico);
                it = itera;
                return true;
            }
            ubico = ubico->_sig;
        }
    }
    return false;
}

/**
 * @brief Operador asignacion
 * @param Objeto ListaEnlazada de tipo T orig
 * @return Referencia al mismo objeto
 */
template<class T>
ListaEnlazada<T> &ListaEnlazada<T>::operator =(const ListaEnlazada<T>& orig) {
    if (this != &orig) {
        while (_cabecera) {
            Nodo<T> *borrado = _cabecera;
            _cabecera = _cabecera->_sig;
            delete borrado;
            borrado = _cabecera;
        }
        _cola = 0;
        Nodo<T> *iterador = orig._cabecera;
        _cabecera = 0;
        _cola = 0;
        while (iterador != nullptr) {
            Nodo<T> *nuevo;
            nuevo = new Nodo<T>(iterador->_dato, 0);
            if (_cola != 0) {
                _cola->_sig = nuevo;
            }
            if (_cabecera == 0) {
                _cabecera = nuevo;
            }
            iterador = iterador->_sig;
            _cola = nuevo;
        }
    }
    return *this;
}

/**
 * @brief Destructor de la clase ListaEnlazada
 */
template<class T>
ListaEnlazada<T>:: ~ListaEnlazada() {
    if (_cabecera != nullptr) {
        Nodo<T> *borrado = _cabecera;
        while (_cabecera != nullptr) {
            _cabecera = _cabecera->_sig;
            delete borrado;
            borrado = _cabecera;
        }
    }
}

/**
 * @brief Funcion que inserta un dato en su respectivo nodo al inicio de la lista
 * @param Objeto de tipo T: dato
 */
template<class T>
void ListaEnlazada<T>::insertaInicio(const T& dato) {
    Nodo<T> *inicio;
    inicio = new Nodo<T>(dato, _cabecera);
    if (_cola == nullptr) {
        _cola = inicio;
    }
    _cabecera = inicio;
}

/**
 * @brief Funcion que inserta un dato en su respectivo nodo al final de la lista
 * @param Objeto de tipo T: dato
 */
template<class T>
void ListaEnlazada<T>::insertaFin(const T& dato) {
    Nodo<T> *fin;
    fin = new Nodo<T>(dato,0);
    if (_cabecera == nullptr) {
        _cabecera = fin;
    }
    if (_cola != nullptr) {
        _cola->_sig = fin;
    }
    _cola = fin;
}

/**
 * @brief Funcion que inserta un dato en la posicion que indique ese iterador
 * @param Objeto de la clase iterador dr tipo T, dato tipo T
 */
template<class T>
void ListaEnlazada<T>::inserta(Iterador<T>& i, const T& dato) {
    Nodo<T> *auxAnt;
    Nodo<T> *auxSig;
    auxAnt = _cabecera;
    if (i._nodo == nullptr) {
        insertaFin(dato);
    } else {
        if (i._nodo == _cabecera) {
            insertaInicio(dato);
        } else {
            while (auxAnt->_sig != i._nodo) {
                auxAnt = auxAnt->_sig;
            }
            auxSig = new Nodo<T>(dato, i._nodo);
            auxAnt->_sig = auxSig;
        }
    }
}

/**
 * @brief Funcion que borra el primer nodo de la lista ajustando su cabecera debido a esto
 */
template<class T>
void ListaEnlazada<T>::borraInicio() {
    Nodo<T>*borrado;
    borrado = _cabecera;
    _cabecera = _cabecera->_sig;
    delete borrado;
    if (_cabecera == nullptr) {
        _cola = nullptr;
    }
}

/**
 * @brief Funcion que borra el ultimo nodo de la lista ajustando su cola debido a esto
 */
template<class T>
void ListaEnlazada<T>::borraFinal() {
    Nodo<T> *anterior = 0;
    if (_cabecera != _cola) {
        anterior = _cabecera;
        while (anterior->_sig != _cola) {
            anterior = anterior->_sig;
        }
        delete _cola;
        _cola = anterior;
        if (anterior != nullptr) {
            anterior->_sig = nullptr;
        } else {
            _cabecera = 0;
        }
    }
}

/**
 * @brief Funcion que borra el nodo posicionado en el lugar que indique el iterador
 * @param Objeto de la clase iterador de tipo T
 */
template<class T>
void ListaEnlazada<T>::borra(Iterador<T>& i) {
    if (i._nodo == _cola) {
        borraFinal();
        i._nodo = 0;
    } else if (i._nodo == _cabecera) {
        borraInicio();
        i._nodo = _cabecera;
    } else {
        Nodo <T> *auxAnt = _cabecera;
        while (auxAnt->_sig != i._nodo) {
            auxAnt = auxAnt->_sig;
        }
        auxAnt->_sig = i._nodo->_sig;
        delete i._nodo;
        i._nodo = auxAnt->_sig;
    }
}

#endif /* LISTAENLAZADA_H */