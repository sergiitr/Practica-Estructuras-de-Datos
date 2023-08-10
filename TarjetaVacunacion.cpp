#include "TarjetaVacunacion.h"
#include "CentroVacunacion.h"
#include "GestionVacunas.h"
#include <sstream>

/**
 * @brief Constructor parametrizado
 * @param usuario de tipo Usuario
 * @param pautaCompleta de tipo bool
 * @param idCentroCercano de tipo int
 */
TarjetaVacunacion::TarjetaVacunacion(Usuario *usuario, bool pautaCompleta, int idCentroCercano): _usuario(usuario), _pautaCompleta(pautaCompleta),
                                                                                                 _idCentroCercano(idCentroCercano) {
    _id = (this->_usuario->GetNss()+ this->_usuario->GetNombre());
    this->IsPautaCompleta();
}

/**
 * @brief Constructor de copia
 * @param orig de tipo TarjetaVacunacion&
 */
TarjetaVacunacion::TarjetaVacunacion(const TarjetaVacunacion& orig): _usuario(orig._usuario), _pautaCompleta(orig._pautaCompleta),
                                                                     _idCentroCercano(orig._idCentroCercano), _id(orig._id) {
}

/**
 * @brief Operador==
 * @param tarjeta
 * @return true si el id de la tarjeta y el del this es igual, false en caso contrario
 */
bool TarjetaVacunacion::operator==(const TarjetaVacunacion& tarjeta) const {
    if (tarjeta._id == this->_id) {
        return true;
    } else {
        return false;
    }
}

void TarjetaVacunacion::SetIdCentroCercano(int _idCentroCercano) {
    this->_idCentroCercano = _idCentroCercano;
}

int TarjetaVacunacion::GetIdCentroCercano() const {
    return _idCentroCercano;
}

void TarjetaVacunacion::SetPautaCompleta(bool _pautaCompleta) {
    this->_pautaCompleta = _pautaCompleta;
}

/**
 * @brief Funcion que nos dice las dosis que le quedan por administrar al usuario
 * @return dosisRestantes de tipo int
 */
int TarjetaVacunacion::dosisPorAdministrar() {
    int dosisRestantes = 0;
    if (_usuario->Edad() <= 5) {
        return 0;
    } else if (_usuario->Edad() > 5 && _usuario->Edad() < 60) {
        dosisRestantes = (2 - _dosis.size());
        return dosisRestantes;
    } else {
        dosisRestantes = (3 - _dosis.size());
        return dosisRestantes;
    }
}

/**
 * @brief Funcion que nos dice si se ha puesto la dosis completa o no
 * @return true si se ha puesto la dosis completa, false en caso contrario
 */
bool TarjetaVacunacion::IsPautaCompleta() {
    if (dosisPorAdministrar() == 0 && this->_usuario->Edad() > 5) {
        this->SetPautaCompleta(true);
    } else {
        this->SetPautaCompleta(false);
    }
    return _pautaCompleta;
}

void TarjetaVacunacion::SetId(string _id) {
    this->_id = _id;
}

string TarjetaVacunacion::GetId() const {
    return _id;
}

Usuario* TarjetaVacunacion::GetUsuario() const{
    return _usuario;
}

vector<Dosis*> TarjetaVacunacion::GetDosis() {
    return _dosis;
}

/**
 * @brief Funcion que sirve para añadir una dosis a la tarjeta
 * @param _dosis
 */
void TarjetaVacunacion::anadirDosis(Dosis* _dosis) {
    this->_dosis.push_back(_dosis);
}

/**
 * @brief Funcion que devuelve la dosis recomendable segun la edad del usuario
 * @return la dosis recomendable
 */
Fabricante TarjetaVacunacion::GetDosisRecomendable() {
    if (_usuario->Edad() <= 12) {
       return Nulo; 
    } else if (_usuario->Edad() > 12 && _usuario->Edad() <= 30) {
        return Johnson;
    } else if (_usuario->Edad() > 30 && _usuario->Edad() <= 50) {
        return Moderna;
    } else if (_usuario->Edad() > 50 && _usuario->Edad() <= 65) {
        return AstraZeneca;
    } else {
        return Pfizer;
    }
}

/**
 * @brief Funcion que devuelve el domicilio del usuario
 * @return domicilio de tipo UTM*
 */
UTM* TarjetaVacunacion::GetDomicilio () {
    UTM* domicilio = _usuario->GetDomicilio();
    return domicilio;
}

/**
 * @brief Funcion que hace un pasaporte Covid a un usuario
 * @param valido de tipo boole
 * @return picosha2::hash256_hex_string(cadena) si tiene la pauta completa, una excepción en caso contrario
 */
string TarjetaVacunacion::pasaporteCovidCode(bool &valido) {
    if (this->IsPautaCompleta() == true) {
        stringstream cadena;
        cadena << this->GetId() << to_string(this->GetDosis()[0]->GetFabricante()) << to_string(this->GetDosis().size());
        valido = true;
        return picosha2::hash256_hex_string(cadena.str());
    } else {
        valido = false;
        return "";
    }
}

float TarjetaVacunacion::GetX() {
    return this->_usuario->GetDomicilio()->GetLatitud();
}

float TarjetaVacunacion::GetY() {
    return this->_usuario->GetDomicilio()->GetLongtitud();
}

void TarjetaVacunacion::SetAviso(string aviso) {
    this->_aviso = aviso;
}


string TarjetaVacunacion::getAviso() {
    return this->_aviso;
}

/**
 * @brief Funcion que devuelve la dosis recomendable segun la edad del usuario con la nuevas medidas
 * @return _pautaCompleta de tipo bool
 */
bool TarjetaVacunacion::PautaCompletaRedefinida() {
    if (this->_dosis.size() >= 1 && this->_usuario->Edad() > 5 && this->_usuario->Edad() < 12) {
        this->SetPautaCompleta(true);
        return true;
    } else {
        if(this->_dosis.size() >= 2 && this->_usuario->Edad() > 12 && this->_usuario->Edad() < 60) {
            this->SetPautaCompleta(true);
            return true;
        } else {
            if(this->_dosis.size() == 3 && this->_usuario->Edad() > 60) {
                this->SetPautaCompleta(true);
               return true; 
            } else {
                this->SetPautaCompleta(false);
            }
        }
    }
    return _pautaCompleta;
}

/**
 * @brief Destructor
 */
TarjetaVacunacion::~TarjetaVacunacion() {
}