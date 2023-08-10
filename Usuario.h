#include "fecha.h"
#include "UTM.h"
#include "Dosis.h"
#include <vector> 
#include <list>
#include <map>
#include "ListaEnlazada.h"
using namespace std;

#ifndef USUARIO_H
#define USUARIO_H

class Usuario {
    public:
        Usuario() = default;
        Usuario(string nombre, string apellidos, string nss, Fecha *fecNaz, UTM *utm);
        Usuario(const Usuario& orig);
        bool operator==(const Usuario& u1);
        bool operator<(const Usuario& u1) const;
        bool operator>(const Usuario& u1) const;
        void SetNombre(string _nombre);
        string GetNombre() const;
        void SetApellidos(string _apellidos);
        string GetApellidos() const;
        void SetNss(string _nss);
        string GetNss() const;
        void SetFecNaz(Fecha* _fecNaz);
        Fecha* GetFecNaz() const;
        void SetDomicilio(UTM *_domicilio);
        UTM* GetDomicilio() const;
        int Edad();
        virtual ~Usuario();
    private:
        string _nombre;
        string _apellidos;
        string _nss;
        Fecha *_fecNaz;
        //ListaEnlazada<Dosis*> _vdosis;
        multimap<Fabricante,Dosis> _dosis;  // esto sobra
        vector<Dosis*> _vDosis;             // esto sobra
        UTM *_domicilio;                    // Creamos la clase UTM con parametros latitud y longitud (ambos siendo float)
};

#endif /* USUARIO_H */