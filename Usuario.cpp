#include "Usuario.h"
#include "GestionVacunas.h"
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

/**
 * @brief Constructor parametrizado
 * @param nombre
 * @param apellidos
 * @param nss
 * @param fecNaz
 */
Usuario::Usuario(string nombre, string apellidos, string nss, Fecha *fecNaz, UTM *utm): _nombre(nombre), _apellidos(apellidos), _nss(nss),
                                                                                        _fecNaz(fecNaz),_domicilio(utm) {
}

/**
 * @brief Constructor de copia
 * @param orig de tipo Usuario&
 */
Usuario::Usuario(const Usuario& orig): _nombre(orig._nombre), _apellidos(orig._apellidos), _nss(orig._nss),
                                       _fecNaz(orig._fecNaz), _domicilio(orig._domicilio) {
}

/**
 * @brief Operador comparacion, donde comparamos si el nss es igual o no
 * @param u1 de tipo Usuario
 * @return Verdadero si ambos son iguales, falso si no son iguales
 */
bool Usuario::operator==(const Usuario& u1) {
    if (this->_nss == u1._nss) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Operador menor donde vemos si un nss es menor que otro o no
 * @param u1 de tipo Usuario
 * @return true si es menor, false en caso contrario
 */
bool Usuario::operator<(const Usuario& u1) const {
    if (this->_nss < u1._nss) {
        return true;
    }
    return false;
}

/**
 * @brief Operador mayor donde vemos si un nss es mayor que otro o no
 * @param u1 de tipo Usuario
 * @return true si es mayor, false en caso contrario
 */
bool Usuario::operator>(const Usuario& u1) const {
    if (this->_nss > u1._nss) {
        return true;
    }
    return false;
}

void Usuario::SetNombre(string _nombre) {
    this->_nombre = _nombre;
}

string Usuario::GetNombre() const {
    return _nombre;
}

void Usuario::SetApellidos(string _apellidos) {
    this->_apellidos = _apellidos;
}

string Usuario::GetApellidos() const {
    return _apellidos;
}

void Usuario::SetNss(string _nss) {
    this->_nss = _nss;
}

string Usuario::GetNss() const {
    return _nss;
}

void Usuario::SetFecNaz(Fecha* _fecNaz) {
    this->_fecNaz = _fecNaz;
}

Fecha* Usuario::GetFecNaz() const {
    return _fecNaz;
}

void Usuario::SetDomicilio(UTM *_domicilio) {
    this->_domicilio = _domicilio;
}

UTM* Usuario::GetDomicilio() const {
    return _domicilio;
}

/**
 * @brief Halla la edad del usuario usando su fecha de nacimiento y la fecha actual
 * @return Retorna la edad actual del usuario
 */
int Usuario::Edad() {
    Fecha* fechaActual = new Fecha();
    int edad = 0;
    int mes = 0;
    int anio = 0;
    int dia = 0;
    if (fechaActual->verAnio() < this->_fecNaz->verAnio()) {
        throw invalid_argument("Fecha de nacimiento no valida");
    } else {
        edad = fechaActual->verAnio() - this->_fecNaz->verAnio();
        if (fechaActual->verMes() < this->_fecNaz->verMes()) {
            edad -= 1;
        } else if (fechaActual->verMes() == this->_fecNaz->verMes()) {
            if (fechaActual->verDia() < this->_fecNaz->verDia()) {
                edad -= 1;
            }
        }
    }
    return edad;
}

/**
 * @brief Destructor
 */
Usuario::~Usuario() {
}