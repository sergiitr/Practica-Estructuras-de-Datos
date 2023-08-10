#include <iostream>
#include <string>
#include "Usuario.h"
#include "Dosis.h"
#include "picosha2.h"
//#include "CentroVacunacion.h"
using namespace std;

#ifndef TARJETAVACUNACION_H
#define TARJETAVACUNACION_H

class GestionVacunas;

class TarjetaVacunacion {
    public:
        TarjetaVacunacion() = default;
        TarjetaVacunacion(Usuario *usuario, bool pautaCompleta, int idCentroCercano);
        TarjetaVacunacion(const TarjetaVacunacion& orig);
        bool operator==(const TarjetaVacunacion& tarjeta) const;
        void SetIdCentroCercano(int _idCentroCercano);
        int GetIdCentroCercano() const;
        void SetPautaCompleta(bool _pautaCompleta);
        bool IsPautaCompleta();
        void SetId(string _id);
        string GetId() const;
        Usuario* GetUsuario() const;
        Fabricante GetDosisRecomendable();
        vector<Dosis*> GetDosis();
        void anadirDosis(Dosis* _dosis);
        int dosisPorAdministrar();
        UTM* GetDomicilio();
        string pasaporteCovidCode(bool &valido);
        float GetX();
        float GetY();
        void SetAviso(string aviso);
        string getAviso();
        bool PautaCompletaRedefinida();
        virtual ~TarjetaVacunacion();
    private:
        string _id;
        bool _pautaCompleta;
        int _idCentroCercano;
        vector<Dosis*> _dosis;
        Usuario* _usuario;
        string _aviso;     
};

#endif /* TARJETAVACUNACION_H */