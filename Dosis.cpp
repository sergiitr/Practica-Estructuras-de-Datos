#include "Dosis.h"

/**
 * @brief Constructor parametrizado
 * @param id
 * @param idLote
 * @param fab
 * @param fabricacion
 */
Dosis::Dosis(int id, int idLote, Fabricante fab, Fecha* fabricacion): _id(id), _idLote(idLote), _fabricante(fab), _fechaFabricacion(fabricacion) {
    Fecha *fech_cad = new Fecha(*_fechaFabricacion);
    fech_cad->anadirMeses(2);
    _fechaCaducidad = fech_cad;
}

/**
 * @brief Constructor de copia
 * @param orig
 */
Dosis::Dosis(const Dosis& orig): _id(orig._id), _idLote(orig._idLote), _fabricante(orig._fabricante),
                                 _fechaFabricacion(orig._fechaFabricacion), _fechaCaducidad(orig._fechaCaducidad) {
}

/**
 * @brief Destructor
 */
Dosis::~Dosis() {
}

Dosis::Dosis(int id): _id(id) {
}

/**
 * @brief Operador igualdad
 * @param Dosis orig
 * @return devuelve True si this->id es igual al orig.id, falso en otro caso
 */
bool Dosis::operator==(const Dosis &orig) {
    if (this->_id == orig._id) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Operador menor
 * @param Dosis orig
 * @return devuelve True si la fecha del this es menor a la fecha del orig, falso en otro caso
 */
bool Dosis::operator<(const Dosis &orig) {
    return this->_fechaFabricacion->operator <(*orig._fechaFabricacion);
}

/**
 * Operador mayor
 * @param orig
 * @return devuelve True si la fecha del this es mayor a la fecha del orig, falso en otro caso
 */
bool Dosis::operator>(const Dosis &orig) {
    return orig._fechaFabricacion->operator <(*this->_fechaFabricacion);
}

Fecha* Dosis::GetFechaCaducidad() const {
    return _fechaCaducidad;
}

/**
 * @brief Añadimos 2 meses a la fecha de caducidad
 * @param _fechaFabricacion
 */
void Dosis::SetFechaFabricacion(Fecha* _fechaFabricacion) {
    this->_fechaFabricacion = _fechaFabricacion;
    Fecha *fech_cad = new Fecha(*_fechaFabricacion);
    fech_cad->anadirMeses(2);
    _fechaCaducidad = fech_cad;
}

Fecha* Dosis::GetFechaFabricacion() const {
    return _fechaFabricacion;
}

void Dosis::SetIdLote(int _idLote) {
    this->_idLote = _idLote;
}

int Dosis::GetIdLote() const {
    return _idLote;
}

void Dosis::SetId(int _id) {
    this->_id = _id;
}

int Dosis::GetId() const {
    return _id;
}

void Dosis::SetFabricante(Fabricante _fabricante) {
    this->_fabricante = _fabricante;
}

Fabricante Dosis::GetFabricante() const{
    return _fabricante;
}

void Dosis::SetEstado(Estado _estado) {
    this->_estado = _estado;
}

Estado Dosis::GetEstado() const {
    return _estado;
}