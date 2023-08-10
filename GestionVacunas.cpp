#include "GestionVacunas.h"
#include <sstream>
#include <cmath>
#include <stdexcept>

/**
 * @brief Constructor por defecto
 */
GestionVacunas::GestionVacunas() {
}

/**
 * @brief Constructor en el que inicializaremos los valores de "usuarios2.txt" y "dosis2.txt"
 * @param usuarios de tipo string
 * @param dosis de tipo string
 */
GestionVacunas::GestionVacunas(string usuarios, string dosis, string centros) {
    string palabra;
    int corte = 0;
    string nombre;
    string apellido;
    string nss;
    int id;
    int __dia = 0;
    int __mes = 0;
    int __anio = 0;
    float latitud = 0;
    float longitud = 0;
    _tarjetas = THashTarjetaVacunacion(1, 10000);   
    ifstream __centro(centros);
    CentroVacunacion *_centros;
    if (__centro.good()) {
        while (getline(__centro, palabra)) {
            corte = palabra.find(';');
            id = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            corte = palabra.find(';');
            latitud = stof(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            longitud = stof(palabra);
            UTM* c = new UTM(latitud,longitud);
            _centros = new CentroVacunacion(id,*c);
            _centros->SetGestionVacunas(this);
            _vCentrosVacunacion.push_back(*_centros);
            _centros = nullptr;
        }
    } else {
        throw invalid_argument("Fichero no abierto correctamente");
    }
    __centro.close();
    float xmin = numeric_limits<float>::max();
    float xmax = numeric_limits<float>::min();
    float ymin = numeric_limits<float>::max();
    float ymax = numeric_limits<float>::min();
    ifstream _usuarios(usuarios);
    Usuario usuario;
    corte = 0;
    latitud = 0;
    longitud = 0;
    if (_usuarios.good()) {
        while (getline(_usuarios, palabra)) {
            UTM *utm = new UTM();
            corte = palabra.find(';');
            nombre = palabra.substr(0, corte);
            palabra.erase(0, corte+1);

            corte = palabra.find(';');
            apellido = palabra.substr(0, corte);
            palabra.erase(0, corte+1);

            corte = palabra.find(';');
            nss = palabra.substr(0, corte);
            palabra.erase(0, corte+1);

            corte = palabra.find('/');
            __dia = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            corte = palabra.find('/');
            __mes = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            corte = palabra.find(';');
            __anio = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            corte = palabra.find(';');
            latitud = stof(palabra.substr(0, corte));
            palabra.erase(0, corte+1);

            longitud = stof(palabra);

            Fecha *f2 = new Fecha(__dia, __mes, __anio);
            
            utm->SetLatitud(latitud);
            utm->SetLongtitud(longitud);

            usuario = Usuario(nombre,apellido,nss,f2,utm);
            _stlUsuarios.insert(pair<string,Usuario>(usuario.GetNss(),usuario));
            TarjetaVacunacion tarjeta (&(*_stlUsuarios.find(nss)).second, false, 0);
            _tarjetas.insertar(_tarjetas.djb2(tarjeta.GetId()),tarjeta);
            
            if (latitud < xmin) {
                xmin = latitud;
            }
            if (longitud < ymin) {
                ymin = longitud;
            }
            if (latitud > xmax) {
                xmax = latitud;
            }
            if (longitud > ymax) {
                ymax = longitud;
            }
        }
        _posTarjetas = MallaRegular<TarjetaVacunacion*>(xmin,ymin,xmax,ymax,25);
        TarjetaVacunacion *tarjetaAux;
        
        map<string,Usuario>::iterator it = _stlUsuarios.begin();
        while (it != _stlUsuarios.end()) {
            tarjetaAux = this->buscarTarjeta(it->second.GetNss()+it->second.GetNombre());
            _posTarjetas.insertar(tarjetaAux->GetX(),tarjetaAux->GetY(),tarjetaAux);
            it++;
        }
    } else {
        throw invalid_argument("Fichero no abierto correctamente");
    }
    _usuarios.close();
  
    _dosis = ifstream(dosis);
    suministrarNDosisAlCentro(&_vCentrosVacunacion[0], 8000);
    suministrarNDosisAlCentro(&_vCentrosVacunacion[1], 8200);
    suministrarNDosisAlCentro(&_vCentrosVacunacion[2], 8500);
    suministrarNDosisAlCentro(&_vCentrosVacunacion[3], 5000);
    suministrarNDosisAlCentro(&_vCentrosVacunacion[4], 50);   
}

/**
 * @brief Constructor de copia
 * @param orig de tipo GestionVacunas&
 */
GestionVacunas::GestionVacunas(const GestionVacunas& orig) {
}

/**
 * @brief Funcion que borra una tarjeta
 * @param idtar de tipo string
 * @return true si se borra la tarjeta, false en caso contrario
 */
bool GestionVacunas::borrar(string idtar) {
   return _tarjetas.borrar(_tarjetas.djb2(idtar), idtar);
}

/**
 * @brief Funcion que llama a _tarjetas.redispersar
 */
void GestionVacunas::redispersar() {
    _tarjetas.redispersar(1879);
}

/**
 * @brief Buscamos un usuario determinado por nss
 * @param nss de tipo string
 * @return Si se encuentra el usuario, se devuelve dicho usuario, en caso contrario salta una excepcion
 */
Usuario* GestionVacunas::buscarUsuario(string nss) {
    Usuario *aux = nullptr;
    if (&_stlUsuarios.find(nss)->second != nullptr) {
        aux = &_stlUsuarios.find(nss)->second;
        return aux;
    } else {
        throw invalid_argument("El nss pasado no esta asociado a ningún usuario");
    }
}

/**
 * @brief Mostramos el listado nss de los usuarios
 * @return aux de tipo vector<string>
 */
vector<string> GestionVacunas::listadoNSS() {
    vector<string> aux;
    map<string,Usuario>::iterator it;
    for (it=_stlUsuarios.begin(); it!=_stlUsuarios.end(); it++) {
        aux.push_back(it->second.GetNss()); 
    }
    return aux;    
}

/**
 * @brief Vemos la cantidad de personas que tienen la pauta completa de la vacuna
 * @return dosisCompletadas de tipo int
 */
int GestionVacunas::pautaCompleta() {
    map<string,TarjetaVacunacion>::iterator it;
    int dosisCompletadas = 0;
    int nDosis;
    for (it=_stlTarjetas.begin(); it!=_stlTarjetas.end(); it++) {
        nDosis = it->second.GetDosis().size();
        if (it->second.GetUsuario()->Edad() >= 75 && nDosis == 3) {
            dosisCompletadas++;
        } else {
            if (nDosis == 2 && it->second.GetUsuario()->Edad() >= 12) {
                dosisCompletadas++;
            }
        }      
    }
    return dosisCompletadas;
}

/**
 * @brief Funcion que devuelve las personas con la dosis completada
 * @return dosisCompletadas de tipo int
 */
int GestionVacunas::pautaCompletaRedefinida() {
    map<string,TarjetaVacunacion>::iterator it;
    int dosisCompletadas = 0;
    int nDosis;
    for (it=_stlTarjetas.begin(); it!=_stlTarjetas.end(); it++) {
        nDosis = it->second.GetDosis().size();
        if (it->second.GetUsuario()->Edad() >= 5 && it->second.GetUsuario()->Edad() < 12 && nDosis == 1) {
            dosisCompletadas++;
        } else {
            if (nDosis == 2 && it->second.GetUsuario()->Edad() >= 5) {
                dosisCompletadas++;
            }
        }
    }
    return dosisCompletadas;
}

/**
 * @brief Muestra una lista de las personas con la pauta de vacunacion no recomendada
 * @return tarjetasNR de tipo vector<TarjetaVacunacion*>
 */
vector<TarjetaVacunacion*> GestionVacunas::listadoVacunacionNR() {
    bool logico = false;
    map<string,TarjetaVacunacion>::iterator it;
    vector<TarjetaVacunacion*> tarjetasNR;
    for (it=_stlTarjetas.begin(); it!=_stlTarjetas.end(); it++) {
        for (int j = 0; j < it->second.GetDosis().size(); j++) {
            if (it->second.GetDosisRecomendable() != it->second.GetDosis()[j]->GetFabricante()) {
                logico = true;
            }
            if (logico) {
                tarjetasNR.push_back(&it->second);
                logico = false;
            }
        }
    }
    return tarjetasNR;
}

/**
 * @brief Funcion que vacuna a un usuario
 * @param t de tipo TarjetaVacunacion
 * @return centro de tipo CentroVacunacion*
 */
CentroVacunacion* GestionVacunas::vacunarUsuario(TarjetaVacunacion* t) {
    string id = t->GetUsuario()->GetNss() + t->GetUsuario()->GetNombre();
    _tarjetas.buscar(_tarjetas.djb2(id),id, t);
    float distanciaMinima = numeric_limits<float>::max();
    int pos = 0;
    for (int i=0; i<_vCentrosVacunacion.size(); i++) {
        if (CalcularDistancia(&_vCentrosVacunacion[i], t) <= distanciaMinima) {
            distanciaMinima = CalcularDistancia(&_vCentrosVacunacion[i], t);
            pos = i;
        }
    }
    CentroVacunacion *centro = &_vCentrosVacunacion[pos];
    centro->anadirTarjetaLista(t);
    centro->administrarDosis(t, t->GetDosisRecomendable());
    t->SetIdCentroCercano(centro->GetId());
    return centro;
}

/**
 * @brief Cantidad de vacunas que hay en el centro de vacunacion segun fabricante
 * @param TypeFabricante de tipo Fabricante
 * @return totalVacunas de tipo int
 */
int GestionVacunas::numTotalVacunasTipo(Fabricante TypeFabricante) {
    int totalVacunas = 0;
    for (int i=0; i<_vCentrosVacunacion.size(); i++) {
        totalVacunas += _vCentrosVacunacion[i].numVacunasTipo(TypeFabricante);
    }
    return totalVacunas;
}

/**
 * @brief Calculamos la distancia entre un centro de vacunacion y un usuario
 * @param centro de tipo CentroVacunacion
 * @param u de tipo Usuario
 * @return distancia de tipo float
 */
float GestionVacunas::CalcularDistancia(CentroVacunacion *centro, TarjetaVacunacion *t) {
    float distancia = 0;
    distancia = sqrt((pow(t->GetDomicilio()->GetLatitud()-centro->GetDireccion().GetLatitud(),2)) + pow(t->GetDomicilio()->GetLongtitud()-centro->GetDireccion().GetLongtitud(),2));
    return distancia;
}

/**
 * @brief Suministramos una cantidad de vacunas al centro
 * @param centro de tipo CentroVacunacion*
 * @param nDosis de tipo int
 */
void GestionVacunas::suministrarNDosisAlCentro(CentroVacunacion* centro, int nDosis) {
    Dosis *d1;
    vector<Dosis> vDosis;
    int _id;
    int _idLote;
    int _fab;
    int _dia;
    int _mes;
    int _anio;
    stringstream ss;
    string id;
    string idLote;
    string fab;
    string dia;
    string mes;
    string anio;
    string linea;
    nDosis = nDosis+_cont;
    if (_dosis.good()) {
        while (_cont<nDosis && getline(_dosis, linea)) {
            Fabricante aux;
            ss.str(linea);
            getline(ss, id, ';');
            getline(ss, idLote, ';');
            getline(ss, fab, ';');
            getline(ss, dia, '/');
            getline(ss, mes, '/');
            getline(ss, anio, '\n');
            _id = stoi(id);
            _idLote = stoi(idLote);
            _fab = stoi(fab);
            if(_fab == 0) {
                aux = Pfizer;       // d1->SetFabricante(Pfizer);
            } else if(_fab == 1) {
                aux = Moderna;      // d1->SetFabricante(Moderna);
            } else if(_fab == 2) {
                aux = AstraZeneca;  //d1->SetFabricante(AstraZeneca);
            } else if(_fab == 3) {
                aux = Johnson;      //d1->SetFabricante(Johnson);
            } else {
                throw invalid_argument("Fabricante no valido");
            }
            _dia = stoi(dia);
            _mes = stoi(mes);
            _anio = stoi(anio);
            Fecha *f1 = new Fecha(_dia, _mes, _anio);
            d1 = new Dosis(_id, _idLote, aux, f1);
            vDosis.push_back(*d1);
            ss.clear();
            _cont++;
        }
    } else {
        throw invalid_argument("Fichero no abierto correctamente");
    }
    centro->anadirNDosisAlmacen(vDosis);
}

/**
 * @brief Funcion que muestra el tamaño de la tabla y el factor de carga del THashTarjetaVacunacion
 */
void GestionVacunas::DatosHash() {
    cout << "El tamaño de la tabla es: " << _tarjetas.tamTabla() << endl;
    cout << "El factor de carga es: " << _tarjetas.factorCarga() << endl;
}

/**
 * @brief Buscamos el centro de vacunacion mas cercano a partir de una tarjeta de vacunacion
 * @param t de tipo TarjetaVacunacion
 * @return idCentro de tipo int
 */
int GestionVacunas::IDcentroMasCercano(TarjetaVacunacion *t) {
    float distanciaMinima = numeric_limits<float>::max();
    int pos = 0;
    for(int i=0; i<_vCentrosVacunacion.size(); i++) {
        if(CalcularDistancia(&_vCentrosVacunacion[i], t) <= distanciaMinima) {
            distanciaMinima = CalcularDistancia(&_vCentrosVacunacion[i], t);
            pos = i;
        }
    }
    int idCentro = _vCentrosVacunacion[pos].GetId();
    return idCentro;
}

/**
 * @brief Funcion que busca una tarjeta en el THash segun su id
 * @param id de tipo string
 * @return si se encuentra, devolvemos la tarjeta, en caso contrario devolvemos nullptr
 */
TarjetaVacunacion* GestionVacunas::buscarTarjeta(string id) {
    TarjetaVacunacion* tarjeta = nullptr;
    if (_tarjetas.buscar(_tarjetas.djb2(id), id, tarjeta)) {
        return tarjeta;
    } else {
        return nullptr;
    }
}

/**
 * @brief Funcion que inserta una tarjeta en el THash
 * @param tar de tipo TarjetaVacunacion
 * @return estado de tipo bool (true si se ha insertado, false si no se ha insertado)
 */
bool GestionVacunas::insertar(TarjetaVacunacion &tar) {
    bool estado = _tarjetas.insertar(_tarjetas.djb2(tar.GetId()),tar);
    return estado;
}

/**
 * @brief Funcion que nos dice el numero de tarjetas hay
 * @return num_tarjetas de tipo int
 */
int GestionVacunas::ntarjetas() {
    int num_tarjetas = _tarjetas.numTarjetas();
    return num_tarjetas;
}

/**
 * @brief Funcion que nos muestra el maximo de colisiones en el THash
 * @return  _tarjetas.MaxColisiones() de tipo unsigned;
 */
unsigned int GestionVacunas::MaxColisiones() const{
    return _tarjetas.MaxColisiones();
}

/**
 * @brief Funcion que nos muestra la cantidad de veces que ha habido mas de 10 colisiones en el THash
 * @return _tarjetas.numMax10() de tipo unsigned;
 */
unsigned int GestionVacunas::numMax10() {
    return _tarjetas.numMax10();
}

/**
 * @brief Funcion que devuelve el promedio de colisiones en el THash
 * @return _tarjetas.PromedioColisiones() de tipo float;
 */
float GestionVacunas::PromedioColisiones() {
    return _tarjetas.PromedioColisiones();
}

/**
 * @biref añadimos las tarjetas al Thash
 * @param id de tipo string
 */
void GestionVacunas::addTarjetas(string id) {
    TarjetaVacunacion* tarjeta;
    _tarjetas.insertar(_tarjetas.djb2(id),*tarjeta);
    tarjeta = this->buscarTarjeta(id);
    _posTarjetas.insertar(tarjeta->GetDomicilio()->GetLatitud(),tarjeta->GetDomicilio()->GetLongtitud(), tarjeta);
}

/**
 * @brief Función que borram las tarjetas del THash 
 * @param id de tipo string
 */
void GestionVacunas::borrarTarjetas(string id) {
    TarjetaVacunacion *tarjeta;
    tarjeta = this->buscarTarjeta(id);
    _tarjetas.borrar(_tarjetas.djb2(id),id);
    _posTarjetas.borrar(tarjeta->GetDomicilio()->GetLatitud(),tarjeta->GetDomicilio()->GetLongtitud(),tarjeta);
}

/**
 * @brief Funcion que llama a a los menores entre 5 y11
 * @param radio de tipo float
 * @return ids de tipo vector<string>
 */
vector<string> GestionVacunas::llamadaVacunacionMenores(float radio) {
    vector<string> ids;
    vector<TarjetaVacunacion**> tarjetas;
    float  xcentro = 0, ycentro = 0;
    string aux;
    for (int i=0; i<_vCentrosVacunacion.size(); i++) {
        xcentro = _vCentrosVacunacion[i].GetDireccion().GetLatitud();
        ycentro = _vCentrosVacunacion[i].GetDireccion().GetLongtitud();
        tarjetas = _posTarjetas.buscarRadio(xcentro,ycentro,radio);
        for (int j=0; j<tarjetas.size(); j++) {
            if ((**tarjetas[j]).GetUsuario()->Edad() >= 5 && (**tarjetas[j]).GetUsuario()->Edad() <= 12) {
                stringstream ss;
                ss << (**tarjetas[j]).GetId();
                string str = ss.str();
                ids.push_back(str);
                if ((**tarjetas[j]).getAviso() != "") { // Esto quiere decir que es una tarjeta no llamada por ningun centro anteriormente
                    string aux = "";
                    stringstream ss;
                    ss << (**tarjetas[i]).GetIdCentroCercano();
                    string str = ss.str();
                    aux = "<<" + str + ">>: Acuda al centro más cercano para vacunarse";
                    (**tarjetas[j]).SetAviso(aux);
                    if (_vCentrosVacunacion[i].GetId() == (**tarjetas[j]).GetIdCentroCercano()) {
                        _vCentrosVacunacion[i].anadirTarjetaLista((*tarjetas[j]));
                    }
                }
            }
        }
    }
    return ids;
}

/**
 * @brief Funcion que llama a los mayores de una cierta edad que nos pasan como parametro
 * @param edad de tipo int
 * @param radio de tipo float
 * @return ids de tipo vector<string>
 */
vector<string> GestionVacunas::llamadaVacunacionMayoresDe(int edad, float radio) {
    vector<string> ids;
    vector<TarjetaVacunacion**> tarjetas;
    float  xcentro = 0, ycentro = 0;
    string aux;
    for (int i=0; i<_vCentrosVacunacion.size(); i++) {
        xcentro = _vCentrosVacunacion[i].GetDireccion().GetLatitud();
        ycentro = _vCentrosVacunacion[i].GetDireccion().GetLongtitud();
        tarjetas = _posTarjetas.buscarRadio(xcentro,ycentro,radio);
        for(int j=0; j<tarjetas.size(); j++){
            if ((**tarjetas[j]).GetUsuario()->Edad() >= edad) {
                stringstream ss;
                ss << (**tarjetas[j]).GetId();
                string str=ss.str();
                ids.push_back(str);
                if ((**tarjetas[j]).getAviso() != "") { // Esto quiere decir que es una tarjeta no llamada por ningun centro anteriormente
                    string aux = "";
                    stringstream ss;
                    ss << (**tarjetas[i]).GetIdCentroCercano();
                    string str = ss.str();
                    aux = "<<" + str + ">>:Acuda al centro más cercano para vacunarse";
                    (**tarjetas[j]).SetAviso(aux);
                    if (_vCentrosVacunacion[i].GetId() == (**tarjetas[j]).GetIdCentroCercano()) {
                        _vCentrosVacunacion[i].anadirTarjetaLista((*tarjetas[j]));
                    }
                }
            }
        }
    } 
    return ids;
}

/**
 * @briefFuncion que avisa a un colectivo segun la edad
 * @param edad de tipo int
 * @return aux de tipo vector<string>
 */
vector<string> GestionVacunas::avisoColectivo(int edad) {
    vector<TarjetaVacunacion**> tarjetas;
    float xcentro = 0, ycentro = 0;
    vector<string> aux;
    for (int i=0; i<_vCentrosVacunacion.size(); i++) {
        xcentro = _vCentrosVacunacion[i].GetDireccion().GetLatitud();
        ycentro = _vCentrosVacunacion[i].GetDireccion().GetLongtitud();
        map<string,Usuario>::iterator it = _stlUsuarios.begin();
        while (it != _stlUsuarios.end()) {
            if ((*it).second.Edad() == edad) {
                tarjetas = _posTarjetas.buscarRadio((*it).second.GetDomicilio()->GetLatitud(), (*it).second.GetDomicilio()->GetLongtitud(), 0.075);
            }
            it++;
        }
    }
    for (int i=0; i<tarjetas.size(); i++) {
        if ((**tarjetas[i]).getAviso() != "") { // Esto quiere decir que es una tarjeta no llamada por ningun centro anteriormente
            string aux2 = "";
            stringstream ss;
            ss << (**tarjetas[i]).GetIdCentroCercano();
            string str = ss.str();
            aux2 = "<<" + str + ">>:Acuda al centro más cercano para vacunarse";
            (**tarjetas[i]).SetAviso(aux2);
            if (_vCentrosVacunacion[i].GetId() == (**tarjetas[i]).GetIdCentroCercano()) {
                _vCentrosVacunacion[i].anadirTarjetaLista((*tarjetas[i]));
            }
        }
        aux.push_back((*tarjetas[i])->GetUsuario()->GetNss());    
    }
    return aux;
}

/**
 * @brief vacunamos un conjunto de usuarios a través de sus ids
 * @param idTarjetas de tipo vector<string>
 */
void GestionVacunas::vacunarConjuntoTarjetas(vector<string> idTarjetas) {
    int cont = 0;
    TarjetaVacunacion *t;
    for (int i=0; i<idTarjetas.size(); i++) {
        string id = idTarjetas[i];
        t = this->buscarTarjeta(id);
        if (t != nullptr) {
            if (t->getAviso() != "") {
                this->vacunarUsuario(t);
                cont++;
                t->SetAviso(""); 
            }   
        }
    }   
}

/**
 * @brief Destructor
 */
GestionVacunas::~GestionVacunas() {
}