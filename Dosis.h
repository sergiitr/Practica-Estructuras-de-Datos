#include "fecha.h"
#include "VDinamico.h"
using namespace std;

#ifndef DOSIS_H
#define DOSIS_H

enum Fabricante {
    Pfizer, Moderna, AstraZeneca, Johnson, Nulo
};

enum Estado {
    administrada, enAlmacen
};

class Dosis {
    public:
        Dosis() = default;
        Dosis (int id, int idLote, Fabricante fab, Fecha* fabricacion);
        Dosis(int id);
        Dosis(const Dosis& orig);
        virtual ~Dosis();
        bool operator==(const Dosis &orig);
        bool operator<(const Dosis &_d);
        bool operator>(const Dosis &orig);
        Fecha* GetFechaCaducidad() const;
        void SetFechaFabricacion(Fecha* _fechaFabricacion);
        Fecha* GetFechaFabricacion() const;
        void SetIdLote(int _idLote);
        int GetIdLote() const;
        void SetId(int _id);
        int GetId() const;
        void SetFabricante(Fabricante _fabricante);
        Fabricante GetFabricante() const;
        void SetEstado(Estado _estado);
        Estado GetEstado() const;
    private:
        int _id;
        int _idLote;
        Fabricante _fabricante;
        Fecha *_fechaFabricacion;
        Fecha *_fechaCaducidad;
        Estado _estado = enAlmacen;
};

#endif /* DOSIS_H */