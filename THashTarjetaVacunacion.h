#include <iostream>
#include "TarjetaVacunacion.h"
using namespace std;

#ifndef THASHTARJETAVACUNACION_H
#define THASHTARJETAVACUNACION_H

enum estado {
    Libre, Ocupado, Borrado
};

class THashTarjetaVacunacion{
    public:
        THashTarjetaVacunacion();
        THashTarjetaVacunacion(int eligehash, int tamTabla);
        THashTarjetaVacunacion(const THashTarjetaVacunacion& orig);
        THashTarjetaVacunacion& operator=(const THashTarjetaVacunacion &thash);
        bool insertar(unsigned long clave, TarjetaVacunacion &tar);
        bool buscar(unsigned long clave, string &id, TarjetaVacunacion *&tar);
        bool borrar(unsigned long clave, string &id);
        unsigned int numTarjetas();
        unsigned int MaxColisiones() const;
        unsigned int numMax10();
        float PromedioColisiones();
        float factorCarga();
        unsigned int tamTabla();
        void redispersar(unsigned tam);
        unsigned long djb2(string str);
        virtual ~THashTarjetaVacunacion();
    private:        
        class Entrada {
            public:
                unsigned long _clave;
                TarjetaVacunacion _dato;
                estado _estado;
                Entrada(): _estado(Libre), _clave(0), _dato(TarjetaVacunacion()) {
                }
                Entrada(unsigned long clave, TarjetaVacunacion dato): _estado(Libre), _clave(clave), _dato(dato) {
                }
                ~Entrada() {
                };
        };
        int tfis;
        unsigned long primoRel;
        unsigned long sumaColisiones;
        int tlog;
        float promedioColisiones;
        unsigned int maxColisiones;
        unsigned int colisiones;
        unsigned int num_veces_mayor_10 = 0;
        float factorcarga;
        int eligehash;
        vector<Entrada> tabla;
        unsigned long hash1(unsigned long clave, int intento);
        unsigned long hash2(unsigned long clave, int intento);
        unsigned long hash3(unsigned long clave, int intento);
        bool esprimo(long n);
        long primomenor(long n);  
        int primosiguiente(int n);
};

#endif /* THASHTARJETAVACUNACION_H */