#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include "VDinamico.h"
using namespace std;

#ifndef AVL_H
#define AVL_H

template<typename T>
class AVL;

template<typename T>
class Nodo {
    private:
        Nodo<T> *izq, *der;
        T dato;
        char bal; //-1,0,1 para arbol
        friend class AVL<T>;
    public:
        Nodo(T &ele): izq(0), der(0), bal(0), dato(ele) {
        }
        Nodo(Nodo<T> &orig): izq(0), der(0), bal(orig.bal), dato(orig.dato) {
        }
};

template<typename T>
class AVL {
    public:
        /**
         * @brief Constructor por defecto
         */
        AVL() {
            raiz = 0;
        };
        AVL(const AVL<T>& orig);
        bool inserta(T &dato);
        unsigned int altura();
        unsigned int numElementos();
        /**
         * @brief Funcion reccorrer inorden
         * @return v de tipo VDinamico
         */
        VDinamico<T*> recorreInorden() {
            VDinamico<T*> v;
            inorden(raiz, v);
            return v;
        }
        T* busquedaRec(T* &dato);
        T* busquedaIte(T* &dato);
        /**
         * @brief Destructor
         */
        virtual ~AVL() {
            DestructorArboles(raiz);
        };
    private:
        Nodo<T> *raiz;
        Nodo<T> *buscaClave(T &ele, Nodo<T> *p);
        int insertaPrivado(Nodo<T>* &c, T& dato);
        void copiaAVL(Nodo<T>* &nodonuevo,Nodo<T>* nodoacopiar);
        void rotDecha(Nodo<T>* &p);
        void rotIzqda(Nodo<T>* &p);
        void buscaRec(Nodo<T>* &padre,T* &dato, T* &valor);
        void inorden(Nodo<T>* p, VDinamico<T*>& v);
        unsigned int altura2(Nodo<T>* p);
        void busqueda_Rec(T* &dato, Nodo<T>* &aux);
        AVL &operator=(const AVL<T> &orig);
        unsigned int numElementosRecursivo(Nodo<T> *p);
        void DestructorArboles(Nodo<T>* &r);
};

/**
 * @brief Constructor de copia
 * @param orig de tipo AVL
 */
template<typename T>
AVL<T>::AVL(const AVL<T>& orig) {
    copiaAVL(raiz,orig.raiz);
}

/**
 * @brief Funcion publica que llama al constructor copia privado
 * @paramin Puntero a Objeto tipo Nodo<T> que va a modificarse (nodonuevo: raiz del arbol nuevo), puntero a objeto tipo Nodo<T> (nodoacopiar: raiz del arbol a copiar)
 * @paramout Nuevo valor del puntero del Nodo que apunta a null (al terminar recursividad) ya que el arbol ya esta copiado
 */
template<typename T>
void AVL<T>::copiaAVL(Nodo<T>*& nodonuevo, Nodo<T>* nodoacopiar) {
    if (nodoacopiar != nullptr) {
        nodonuevo = new Nodo<T>(nodoacopiar->dato);
        nodonuevo->bal = nodoacopiar->bal;
        copiaAVL(nodonuevo->izq,nodoacopiar->izq);
        copiaAVL(nodonuevo->der,nodoacopiar->der);
    } else {
        nodonuevo = nullptr;
    }
}

/**
 * @brief Operador de asignacion que destruye el arbol existente y copia la información del que se le quiere asignar 
 * @paramin Objeto tipo AVL<T> (orig: arbol)
 * @return referencia al propio objeto this
 */
template<typename T>
AVL<T>& AVL<T>::operator =(const AVL<T>& orig) {
    if (this != &orig) {
        DestructorArboles(raiz);
        copiaAVL(raiz, orig.raiz);
    }
    return *this;
}

/**
 * @brief Funcion privada que devuelve la altura de un arbol +1
 * @paramin Puntero a objeto tipo Nodo<T> (p: puntero al nodo raiz)
 * @return retorna 0 en caso de no tener ese nodo actual algo a la izquierda o derecha por lo que sube hacia los nodos padres para recorrer otras rutas
 */
template<typename T>
unsigned int AVL<T>::altura2(Nodo<T>* p) {
    if (p == nullptr) {
        return 0;
    }
    int altizq = altura2(p->izq);
    int altder = altura2(p->der);
    if (altizq > altder) {
        return altizq+1;
    } else {
        return altder+1;
    }
}

/**
 * @brief Funcion publica que devuelve la altura de un arbol llamando a un metodo publico recursivo
 * @return retorna 0 en caso de tener una raiz inservible(No hay arbol) o la altura de éste si el arbol tiene algun elemento
 */
template<typename T>
unsigned int AVL<T>::altura() {
    if (raiz == nullptr) {
        return 0;
    } else {
        return altura2(raiz)-1;
    }
}

/**
 * @brief Funcion privada que ejecuta una rotacion de una rama del arbol hacia la derecha
 * @paramin referencia a puntero de objeto tipo Nodo<T> (p: puntero del nodo del que se quiere hacer esa rotacion)
 */
template<typename T>
void AVL<T>::rotDecha(Nodo<T>*& p) {
    Nodo<T> *q=p, *l;
    p = l = q->izq;
    q->izq = l->der;
    l->der = q;
    q->bal--;
    if (l->bal > 0) {
        q->bal -= l->bal;
    }
    l->bal--;
    if (q->bal < 0) {
        l->bal -= -q->bal;
    }
}

/**
 * @brief Funcion privada que ejecuta una rotacion de una rama del arbol hacia la izquierda
 * @paramin referencia a puntero de objeto tipo Nodo<T> (p: puntero del nodo del que se quiere hacer esa rotacion)
 */
template<typename T>
void AVL<T>::rotIzqda(Nodo<T>*& p) {
    Nodo<T> *q=p, *r;
    p = r = q->der;
    q->der = r->izq;
    r->izq = q;
    q->bal++;
    if (r->bal < 0) {
        q->bal += -r->bal;
    }
    r->bal++;
    if (q->bal > 0) {
        r->bal += q->bal;
    }
}

/**
 * @brief Funcion privada que devuelve el dato que se desea encontrar
 * @paramin Objeto tipo T a encontrar (ele), Puntero a Objeto Nodo<T> (p) que es el puntero al nodo raiz a partir de donde se quiere buscar el dato
 * @return retorna 0 si ese nodo no tiene nada a sus extremos (debe de volver a su padre el puntero), si tiene algo a los extremos y no es el dato entra en recursividad y si lo encuentra devuelve el dato
 */
template<typename T>
Nodo<T> *AVL<T>::buscaClave(T& ele, Nodo<T>* p) {
    if (p == nullptr) {
        return 0;
    } else if (ele < p->dato) {
        return buscaClave(ele,p->izq);
    } else if (p->dato < ele) {
        return buscaClave(ele,p->der);
    } else {
        return p;
    }
}

/**
 * @brief Funcion privada recursiva que inserta un dato 
 * @paramin referencia a puntero de tipo Nodo<T> (c) que es el puntero del nodo raiz a recorrer que se va a ir actualizando, Objeto tipo T (dato) que es el dato a insertar
 * @return retorna deltaH (int)
 */
template<typename T>
int AVL<T>::insertaPrivado(Nodo<T>* &c, T& dato) {
    Nodo<T> *p = c;
    int deltaH = 0;
    if (!p) {
        p = new Nodo<T>(dato);
        c = p; 
        deltaH = 1;
    } else if (p->dato < dato) {
        if (insertaPrivado(p->der, dato)) {
            p->bal--;
            if (p->bal == -1) {
                deltaH = 1;
            } else if (p->bal == -2) {
                if (p->der->bal == 1) rotDecha(p->der);
                    rotIzqda(c);
            } 
        } 
    } else if (dato < p->dato) {
        if (insertaPrivado(p->izq, dato)) {
            p->bal++;
            if (p->bal == 1) {
                deltaH = 1;
            } else if (p->bal == 2) {
                if (p->izq->bal == -1) {
                    rotIzqda(p->izq);
                }
                rotDecha(c);
            } 
        } 
    }
    return deltaH;
}

/**
 * @brief Funcion llamada recursiva
 * @param dato
 * @return valor de tipo T*
 */
template<typename T>
T* AVL<T>::busquedaRec(T* &dato) {
    int *v = nullptr;
    buscaRec(raiz,dato,v);
    return v;
}

/**
 * @brief Funcion privada de busqueda recursiva
 * @param padre de tipo Nodo
 * @param dato de tipo T
 * @param v de tipo T
 */
template<typename T>
void AVL<T>::buscaRec(Nodo<T>* &p, T* &dato, T* &v) {
    if (p != nullptr) {
        if (p->dato == *dato) {
            *v = *dato;
        }
        if (p->izq != nullptr) {
            buscaRec(p->izq,dato, v);
        }
        if (p->der != nullptr) {
            buscaRec(p->der,dato, v);
        }
    }
}

/**
 * @brief Funcion privada recorre inorden
 * @param p de tipo Nodo
 * @param v de tipo VDinamico
 */
template <typename T>
void AVL<T>::inorden(Nodo<T>* p, VDinamico<T*>& v) {
    if (p) {
        inorden(p->izq, v);
        v.insertar(&p->dato);
        inorden(p->der, v);
    }
}

/**
 * @brief Funcion iterativa 
 * @param dato de tipo T*
 * @return dato si se encuntra el dato, y nullptr si no se encuentra el dato en el arbol
 */
template<typename T>
T* AVL<T>::busquedaIte(T* &dato) {
    Nodo<T> *aux = raiz;
    while (aux != nullptr) {
        if (*dato > aux->dato) {
            aux = aux->der;
        } else if (*dato < aux->dato) {
            aux = aux->izq;
        } else {
            return &aux->dato;
        }
    }
    return nullptr;
}

/**
 * @brief Funcion publica que llama a su insertaPrivado que es recursiva
 * @paramin Objeto tipo T a insertar (dato)
 * @return retorna true si el dato se ha insertado, false si no se ha isnertado(ya se encontraba anteriormente en el arbol)
 */
template<typename T>
bool AVL<T>::inserta(T& dato) {
    return insertaPrivado(raiz, dato);
}

/**
 * @brief Funcion privada que recorre el arbol obteniendo el numero de sus elementos 
 * @paramin Puntero a Objeto Nodo<T> (p) que es el puntero al nodo raiz
 * @return retorna 0 cuando ha llegado a la ultima hoja de una rama buscando nuevos caminos en el nodo padre, retorna finalmente el numero de elementos del arbol
 */
template<typename T>
unsigned int AVL<T>::numElementosRecursivo(Nodo<T>* p) {
    if (p != nullptr) {
        return numElementosRecursivo(p->izq) + numElementosRecursivo(p->der) + 1;
    } else {
        return 0;
    }
}

/**
 * @brief Funcion publica que llama al metodo privado para obtener el numero de elementos del arbol
 * @return retorna el numero de elementos del arbol
 */
template<typename T>
unsigned int AVL<T>::numElementos() {
    return numElementosRecursivo(raiz);
}

/**
 * @brief Funcion privada que destruye un arbol destruyendo sus hojas desde abajo hacia arriba 
 * @paramin Puntero a Objeto Nodo<T> (r) que es el puntero del nodo raiz a borrar
 */
template<typename T>
void AVL<T>::DestructorArboles(Nodo<T>* &r) {
    if (r != nullptr) {
        DestructorArboles(r->izq);
        DestructorArboles(r->der);
        delete r;
        r = nullptr;
    }
}

#endif /* AVL_H */