#include <iostream>
#include <string>
using namespace std;

#ifndef UTM_H
#define UTM_H

class UTM {
    public:
        UTM() = default;
        UTM(float latitud, float longtitud);
        UTM(const UTM& orig);
        void SetLongtitud(float _longtitud);
        float GetLongtitud() const;
        void SetLatitud(float _latitud);
        float GetLatitud() const;
        virtual ~UTM();
    private:
        float _latitud;
        float _longtitud;
};

#endif /* UTM_H */