#include "UTM.h"

UTM::UTM(float latitud, float longtitud): _latitud(latitud), _longtitud(longtitud) {
}

UTM::UTM(const UTM& orig): _latitud(orig._latitud), _longtitud(orig._longtitud) {
}

void UTM::SetLongtitud(float _longtitud) {
    this->_longtitud = _longtitud;
}

float UTM::GetLongtitud() const {
    return _longtitud;
}

void UTM::SetLatitud(float _latitud) {
    this->_latitud = _latitud;
}

float UTM::GetLatitud() const {
    return _latitud;
}

UTM::~UTM() {
}