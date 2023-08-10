#include "CentroVacunacion.h"
#include "GestionVacunas.h"

/**
 * @brief Constructor parametrizado
 * @param id de tipo int
 * @param direccion de tipo UTM
 */
CentroVacunacion::CentroVacunacion(int id, UTM direccion) : _id(id), _direccion(direccion) {
}

/**
 * @brief Constructor de copia
 * @param orig
 */
CentroVacunacion::CentroVacunacion(const CentroVacunacion& orig) : _id(orig._id), _direccion(orig._direccion), _gestionVacunas(orig._gestionVacunas) {
}

void CentroVacunacion::SetDireccion(UTM _direccion) {
    this->_direccion = _direccion;
}
UTM CentroVacunacion::GetDireccion() const {
    return _direccion;
}

void CentroVacunacion::SetId(int _id) {
    this->_id = _id;
}
int CentroVacunacion::GetId() const {
    return _id;
}

void CentroVacunacion::SetGestionVacunas(GestionVacunas* _gestionVacunas) {
    this->_gestionVacunas = _gestionVacunas;
}

/**
 * @brief Añadimos un usuario a la lista
 * @param u de tipo Usuario
 */
void CentroVacunacion::anadirUsuarioLista(Usuario *u) {
    _usuarios.push_front(u);
}

/**
 * @brief Administramos una dosis a un usuario
 * @param u de tipo Usuario
 * @param TypeFabricante de tipo Fabricante
 * @return logico de tipo boole
 */
bool CentroVacunacion::administrarDosis(TarjetaVacunacion *t, Fabricante TypeFabricante) {
    bool logico = false;
    list<TarjetaVacunacion*>::iterator it;
    for (it = _tarjetas.begin(); it != _tarjetas.end(); it++) {
        if (*it != t) {
            logico = true;
            break;          // La tarjeta del usuario no se ha encontrado
        }
    }
    if (logico) {
        multimap<Fabricante, Dosis>::iterator it;
        if (_contador == _dosis.size()) {
            this->alarmaFaltaDosis();
        }
        // Comprobar si es del fabricante y no administrada, si no existe del fabricante, comprobar que no esta administrada y administrar cualquiera
        for (it = _dosis.lower_bound(TypeFabricante); it != _dosis.upper_bound(TypeFabricante); it++) {
            if (it->second.GetEstado() == enAlmacen) {
                t->anadirDosis(&it->second); 
                it->second.SetEstado(administrada);
                _contador++;
                _tarjetas.remove(t);    // Esto hay que hacerlo tras vacunar al usuario para que no se pueda volver a vacunar sin permiso 
                return logico;
            }
        }
        for (it = _dosis.begin(); it != _dosis.end(); it++) {
            if (it->second.GetEstado() == enAlmacen) {
                t->anadirDosis(&it->second);  // Esto hay que hacerlo tras vacunar al usuario para que no se pueda volver a vacunar sin permiso 
                it->second.SetEstado(administrada);
                _contador++;
                _tarjetas.remove(t);    // Esto hay que hacerlo tras vacunar al usuario para que no se pueda volver a vacunar sin permiso 
                return logico;
            }
        }
    }
    return false;
}

/**
 * @brief Añadimos dosis al almacen
 * @param packDosis de tipo vector<Dosis>
 */
void CentroVacunacion::anadirNDosisAlmacen(vector<Dosis> &packDosis) {
    for (int i=0; i<packDosis.size(); i++) {
        _dosis.insert(pair<Fabricante, Dosis>(packDosis[i].GetFabricante(), packDosis[i]));
    }
}

/**
 * @brief Cantidad de vacunas de cada tipo
 * @param TypeFabricante de tipo Fabricante
 * @return dosisEnAlmacen de tipo int
 */
int CentroVacunacion::numVacunasTipo(Fabricante TypeFabricante) { 
    int dosisEnAlmacen = 0;
    multimap<Fabricante, Dosis>::iterator it;
    for (it=_dosis.lower_bound(TypeFabricante); it!=_dosis.upper_bound(TypeFabricante); it++) {
        if (it->second.GetEstado() == enAlmacen) {
            dosisEnAlmacen++;
        }
    }
    return dosisEnAlmacen;
}

/**
 * @brief Funcion que añade una tarjeta a un centro de vacunacion
 * @param tarjeta de tipo TarjetaVacunacion
 */
void CentroVacunacion::anadirTarjetaLista(TarjetaVacunacion *tarjeta) {
    _tarjetas.push_back(tarjeta);
}

/**
 * @brief Damos una señal de que faltan dosis
 */
void CentroVacunacion::alarmaFaltaDosis() {
    _gestionVacunas->suministrarNDosisAlCentro(this, 100);
}

/**
 * @brief Destructor
 */
CentroVacunacion::~CentroVacunacion() {
}