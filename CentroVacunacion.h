#include "UTM.h"
#include "Usuario.h"
#include "TarjetaVacunacion.h"

#ifndef CENTROVACUNACION_H
#define CENTROVACUNACION_H

class GestionVacunas;
class CentroVacunacion {
    public:
        CentroVacunacion() = default;
        CentroVacunacion(int id, UTM direccion);
        CentroVacunacion(const CentroVacunacion& orig);
        void SetDireccion(UTM _direccion);
        UTM GetDireccion() const;
        void SetId(int _id);
        int GetId() const;
        void SetGestionVacunas(GestionVacunas* _gestionVacunas);
        void anadirUsuarioLista(Usuario* u);
        bool administrarDosis(TarjetaVacunacion *t, Fabricante TypeFabricante);
        void anadirNDosisAlmacen(vector<Dosis> &packDosis);
        int numVacunasTipo(Fabricante TypeFabricante);
        void anadirTarjetaLista(TarjetaVacunacion *tarjeta);
        virtual ~CentroVacunacion();
    private:
        int _id;
        UTM _direccion;
        multimap<Fabricante,Dosis> _dosis;
        list<Usuario*> _usuarios;
        void alarmaFaltaDosis();
        GestionVacunas *_gestionVacunas;
        UTM *_localizacion;
        int _contador = 0;
        list<TarjetaVacunacion*> _tarjetas;
};

#endif /* CENTROVACUNACION_H */