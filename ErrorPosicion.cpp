#include "ErrorPosicion.h"

ErrorPosicion::ErrorPosicion(string error): range_error(error) {
}

ErrorPosicion::ErrorPosicion(const ErrorPosicion& orig): range_error(orig) {    
}

ErrorPosicion::~ErrorPosicion() throw() {
}