#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

#ifndef ERRORPOSICION_H
#define ERRORPOSICION_H

/**
 * @brief Excepción para indicar intentos de extracción sobre una posicion no valida
 */
class ErrorPosicion: public range_error {
    public:
        ErrorPosicion(string error);
        ErrorPosicion(const ErrorPosicion& orig);
        virtual ~ErrorPosicion() throw();
    private:
};

#endif /* ERRORPOSICION_H */