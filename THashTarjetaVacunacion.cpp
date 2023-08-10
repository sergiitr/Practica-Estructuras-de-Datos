
#include "THashTarjetaVacunacion.h"

int contador1 = 0;

THashTarjetaVacunacion::THashTarjetaVacunacion() {
}

/**
 * @brief Constructor que construye una tabla dado un tamaño predefinido
 * @param eligehash de tipo int
 * @param tamTabla de tipo int
 */
THashTarjetaVacunacion::THashTarjetaVacunacion(int eligehash, int tamTabla): tfis(primosiguiente(long(tamTabla)/0.65)), tlog(0), maxColisiones(0),
                                                                             sumaColisiones(0), eligehash(eligehash), tabla(tfis, Entrada()) {
    primoRel = primomenor(tfis);
}

/**
 * @brief Constructor de copia
 * @param orig de tipo THashTarjetaVacunacion&
 */
THashTarjetaVacunacion::THashTarjetaVacunacion(const THashTarjetaVacunacion& orig): tabla(orig.tabla), tfis(orig.tfis), tlog(orig.tlog), maxColisiones(orig.maxColisiones),
                                                                                    sumaColisiones(orig.sumaColisiones), primoRel(orig.primoRel),eligehash(orig.eligehash) {
}

/**
 * @brief Operador asignacion
 * @param thash de tipo THashTarjetaVacunacion
 * @return this de tipo THashTarjetaVacunacion
 */
THashTarjetaVacunacion& THashTarjetaVacunacion::operator=(const THashTarjetaVacunacion &thash) {
    if (this != &thash) {
        tabla = thash.tabla;
        tfis = thash.tfis;
        tlog = thash.tlog;
        primoRel = thash.primoRel;
        maxColisiones = thash.maxColisiones;
        sumaColisiones = thash.sumaColisiones;
        eligehash = thash.eligehash;
    }
    return *this;
}

/**
 * @brief Funcion que inserta una nueva tarjeta de vacunación en la tabla. No se permiten repetidos por tanto, es necesario buscar antes de insertar
 * @param clave de tipo long
 * @param tar de tipo TarjetaVacunacio
 * @return true si se inserta la tarjeta, false en caso contrario
 */
bool THashTarjetaVacunacion::insertar(unsigned long clave, TarjetaVacunacion &tar) {
    int intento = 0;
    unsigned long insertaAqui = 0;
    insertaAqui = hash2(clave,intento);
    while (tabla[insertaAqui]._estado == Ocupado) {
        if (tabla[insertaAqui]._dato == tar) {
            return false;
        }
        intento++;
        insertaAqui = hash2(clave,intento);
    }
    if (intento > maxColisiones) {
        maxColisiones = intento;
    }
    if (intento > 10) {
        num_veces_mayor_10 ++;
    }
    sumaColisiones = sumaColisiones+intento;
    tabla[insertaAqui] = Entrada(clave, tar);
    tabla[insertaAqui]._estado = Ocupado;
    tlog++;
    return true;
}

/**
 * @brief Funcionque busca un dato a partir de su clave numérica y devuelva el objeto TarjetaVacunacion a través de un puntero
 * @param clave de tipo long
 * @param id de tipo string
 * @param tar de tipo TarjetaVacunacion
 * @return true si se ha encontrado y false en caso contrario
 */
bool THashTarjetaVacunacion::buscar(unsigned long clave, string &id, TarjetaVacunacion *&tar) {
    int intento = 0;
    unsigned long insertaAqui = 0;
    insertaAqui = hash2(clave,intento);
    while (tabla[insertaAqui]._estado == Borrado || tabla[insertaAqui]._estado == Ocupado) {
        if (tabla[insertaAqui]._dato.GetId() == id && tabla[insertaAqui]._estado != Borrado ) { 
            tar = &(tabla[insertaAqui]._dato);
            return true;
        }
        intento++;
        insertaAqui = hash2(clave,intento);
    }
    return false;
}

/**
 * @brief Funcion que borra la tarjeta de la tabla
 * @param clave de tipo long
 * @param id de tipo string
 * @return true si se ha borrado una tarjeta, false en caso contrario
 */
bool THashTarjetaVacunacion::borrar(unsigned long clave, string &id) {
    int intento = 0;
    unsigned long insertaAqui = 0;
    insertaAqui = hash2(clave,intento);
    
    while (tabla[insertaAqui]._estado == Borrado || tabla[insertaAqui]._estado == Ocupado) {
        if (tabla[insertaAqui]._dato.GetId() == id) {
            tabla[insertaAqui] = Entrada();
            tabla[insertaAqui]._estado = Borrado;
            tlog--;
            return true;
        } else {
           intento++;
           insertaAqui = hash2(clave,intento);
        }
    }
    return false;
}

/**
 * @brief Funcion que nos muestra el numero de tarjetas hay
 * @return cont de tipo int
 */
unsigned int THashTarjetaVacunacion::numTarjetas() {
    return tlog;
}

/**
 * @brief Funcion que devuelve el número máximo de colisiones que se han producido en la operación de inserción más costosa realizada sobre la tabla
 * @return aux de tipo int
 */
unsigned int THashTarjetaVacunacion::MaxColisiones() const {
    return maxColisiones;
}

/**
 * @brief Funcion que devuelve el número de veces que se superan 10 colisiones al intentar realizar la operación de inserción sobre la tabla de un dato
 * @return cont de tipo int
 */
unsigned int THashTarjetaVacunacion::numMax10() {
    return num_veces_mayor_10;
}

/**
 * @brief Funcion que devuelve el promedio de colisiones por operación de inserción realizada sobre la tabla
 * @return promedio de tipo float
 */
float THashTarjetaVacunacion::PromedioColisiones() {
    float promedio = ((float)sumaColisiones/tlog);
    return promedio;
}

/**
 * @brief Funcion que devuelve el factor de carga de la tabla de dispersión
 * @return factorCarga de tipo float
 */
float THashTarjetaVacunacion::factorCarga() {
    float factorCarga = (float)tlog/tfis;
    return factorCarga;
}

/**
 * @brief Funcion que devuelve el tamaño de la tabla de dispersión
 * @return tabla.size() de tipo int
 */
unsigned int THashTarjetaVacunacion::tamTabla() {
    return tabla.size();
}

/**
 * @brief Función que redispersa la tabla a un nuevo tamaño tam
 * @param tam de tipo unsigned
 */
void THashTarjetaVacunacion::redispersar(unsigned tam) {
    int copiaAqui = 0, intento = 0;
    vector<Entrada> tabla2(tam, Entrada());
    tfis = tam;             primoRel = primomenor(tam);
    sumaColisiones = 0;     maxColisiones = 0;
    for (int i=0; i<tabla.size(); i++) {
        bool copiado = false;
        intento = 0;
        if (tabla[i]._estado == Ocupado) {
            while (!copiado) {
                copiaAqui = hash2(tabla[i]._clave, intento);
                if (tabla2[copiaAqui]._estado == Libre || tabla2[copiaAqui]._estado == Borrado) { 
                    tabla2[copiaAqui]._dato = tabla[i]._dato;
                    tabla2[copiaAqui]._dato.GetId() = tabla[i]._dato.GetId();
                    tabla2[copiaAqui]._clave = tabla[i]._clave;
                    tabla2[copiaAqui]._estado = Ocupado;
                    copiado = true;
                } else {
                    intento++;
                }
            }
            if (intento > maxColisiones) {
                maxColisiones = intento;
            }
            sumaColisiones += intento;
        }
    }
    tabla = tabla2;
}

/**
 * @brief Funcion que cambia de string a unsigned long
 * @param str de tipo string
 * @return hash de tipo unsigned long
 */
unsigned long THashTarjetaVacunacion::djb2(string str) {
    unsigned long hash = 5381;
    for (int i=0; i<str.length(); ++i) {
        hash = ((hash << 5)+hash)+str[i];
    }
    return hash;
}

/**
 * @brief Destructor
 */
THashTarjetaVacunacion::~THashTarjetaVacunacion() {
}

/**
 * @brief Funcion privada de dispersión cuadrática que nos devuelve la posicion donde intentamos posicionarnos en la tabla hash
 * @param clave(long) es el valor del iterador, es decir, el numero de colisiones para posicionar un dato intento(int) es el valor del dato a posicionar
 * @return colocateAqui(unsigned long) valor del la posición final a ubicarse
 */
unsigned long THashTarjetaVacunacion::hash1(unsigned long clave, int intento) {
    unsigned long colocateAqui;
    colocateAqui = (clave+(intento*intento))%tfis;
    return colocateAqui;
}

/**
 * @brief Funcion privada de dispersión doble que nos devuelve la posicion donde intentamos posicionarrnos en la tabla hash
 * @param clave(long) es el valor del iterador, es decir, el numero de colisiones para posicionar un dato intento(int) es el valor del dato a posicionar
 * @return colocateAqui(unsigned long) valor del la posición final a ubicarse
 */
unsigned long THashTarjetaVacunacion::hash2(unsigned long clave, int intento) {
    unsigned long colocateAqui;
    colocateAqui = ((clave%tfis) + (intento*(1+(clave%primoRel))))%tfis;
    return colocateAqui;
}

/**
 * @brief Funcion privada de dispersión doble que nos devuelve la posicion donde intentamos posicionarnos en la tabla hash
 * @param clave(long) es el valor del iterador, es decir, el numero de colisiones para posicionar un dato intento(int) es el valor del dato a posicionar
 * @return colocateAqui(unsigned long) valor del la posición final a ubicarse
 */
unsigned long THashTarjetaVacunacion::hash3(unsigned long clave, int intento) {
    unsigned long colocateAqui;
    colocateAqui = ((clave%tfis) + (intento*(primoRel-(clave%primoRel))))%tfis;
    return colocateAqui;
}

/**
 * @brief Funcion que comprueba si un número es primo o no
 * @param n es el número a comprobar si es primo o no
 * @return Si n es primo devuelve true, si no, false
 */
bool THashTarjetaVacunacion::esprimo(long n) {
    for (int i=2; i<=n/2; i++) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Funcion que nos devuelve el número primo menor al tamaño físico de la tabla hash
 * @param n de tipo long
 * @return El numero primo menor que el tamaño fisico
 */
long THashTarjetaVacunacion::primomenor(long n) {
    long num = n-1;
    while (!esprimo(num)) {
        num--;
    }
    return num;
}

/**
 * @brief Funcion que nos devuelve cual es el siguiente numero primo dado un parametro
 * @param n de tipo long
 * @return num de tipo long
 */
int THashTarjetaVacunacion::primosiguiente(int n) {
    while (!esprimo(n)) {
        n++;
    }
    return n;
}