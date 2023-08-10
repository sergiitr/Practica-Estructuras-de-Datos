#include "Usuario.h"
#include "Dosis.h"
#include "AVL.h"
#include "CentroVacunacion.h"
#include "THashTarjetaVacunacion.h"
#include "MallaRegular.h"
#include "TarjetaVacunacion.h"
#include <map>
#include <fstream>
using namespace std;

#ifndef GESTIONVACUNAS_H
#define GESTIONVACUNAS_H

class GestionVacunas {
    public:
        GestionVacunas();
        GestionVacunas(string usuarios, string dosis, string centros);
        GestionVacunas(const GestionVacunas& orig);
        Usuario* buscarUsuario(string nss);
        vector<string> listadoNSS();
        int pautaCompleta();
        vector<TarjetaVacunacion*> listadoVacunacionNR();
        bool borrar(string idtar);
        bool insertar(TarjetaVacunacion &tar);
        CentroVacunacion* vacunarUsuario(TarjetaVacunacion* t);
        int numTotalVacunasTipo(Fabricante TypeFabricante);
        float CalcularDistancia(CentroVacunacion *centro, TarjetaVacunacion *t);
        void suministrarNDosisAlCentro(CentroVacunacion* centro, int nDosis);
        TarjetaVacunacion* buscarTarjeta(string id);
        void DatosHash();
        void redispersar();
        int IDcentroMasCercano(TarjetaVacunacion *t);
        unsigned int MaxColisiones() const;
        unsigned int numMax10();
        int ntarjetas();
        float PromedioColisiones();
        int pautaCompletaRedefinida();
        void addTarjetas(string id);
        void borrarTarjetas(string id);
        vector<string> llamadaVacunacionMenores(float radio);
        vector<string> llamadaVacunacionMayoresDe(int edad, float radio);
        vector<string>avisoColectivo(int edad);
        void vacunarConjuntoTarjetas(vector<string> idTarjetas);
        virtual ~GestionVacunas();
    private:
        map<string, Usuario> _stlUsuarios;
        map<string, TarjetaVacunacion> _stlTarjetas;
        int dosisCorrectas = 0;
        vector<CentroVacunacion> _vCentrosVacunacion;
        ifstream _dosis;
        int _cont = 0;
        THashTarjetaVacunacion _tarjetas;
        MallaRegular<TarjetaVacunacion*> _posTarjetas;
      
};

#endif /* GESTIONVACUNAS_H */