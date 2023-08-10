/**
 * Practica 6 Estructuras de Datos
 * Grupo Jueves 8:30 a 10:30
 * Integrantes del grupo: Carlos Martos López y Sergio Trillo Rodríguez
 */

#include <fstream>
#include <sstream>
#include "GestionVacunas.h"
#include "CentroVacunacion.h"
#include "TarjetaVacunacion.h"
using namespace std;

int main() {
    TarjetaVacunacion *tarjetaVacunacion;
    Usuario *usuario = nullptr;

    try {
        int usuariosConPasaporteCovid = 0;
        clock_t t_ini = clock();
        GestionVacunas *gestionVacunas = new GestionVacunas("usuarios.txt", "dosis.txt", "centros.txt");
        cout << "Tiempo del constructor por defecto del GestionVacunas: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;

        vector<string> vecAuxiliar = gestionVacunas->listadoNSS();
        int vacunados = 0;
        string auxiliar;
        
        t_ini = clock();
        for (int i=0; i<vecAuxiliar.size(); i++) {
            usuario = gestionVacunas->buscarUsuario(vecAuxiliar[i]);
            auxiliar = vecAuxiliar[i][vecAuxiliar[i].size()-1];
            int edad = usuario->Edad();
            if (usuario != nullptr) {
                if (edad >= 12) {
                    if(auxiliar == "0" || auxiliar == "1" || auxiliar == "2" || auxiliar == "3" || auxiliar == "4" || auxiliar == "6" || auxiliar == "8" || auxiliar == "9") {
                        string idtar = usuario->GetNss() + usuario->GetNombre();
                        tarjetaVacunacion=gestionVacunas->buscarTarjeta(idtar);
                        if (tarjetaVacunacion != nullptr) {
                            gestionVacunas->vacunarUsuario(tarjetaVacunacion);
                            vacunados++;
                        } 
                    }
                }
            }
        }
        cout << "Se han vacunado " << vacunados << " personas mayores de 12 años cuyo nss acaba distinto de 5 o 7" << endl;
        cout << "Tiempo de la primera vacunacion es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        vacunados = 0;
        
        t_ini = clock();
        for (int i=0; i<vecAuxiliar.size(); i++) {
            usuario = gestionVacunas->buscarUsuario(vecAuxiliar[i]);
            int edad = usuario->Edad();
            if (edad >= 25) {
                string idtar = usuario->GetNss() + usuario->GetNombre();
                tarjetaVacunacion = gestionVacunas->buscarTarjeta(idtar);
                if (tarjetaVacunacion!= nullptr) {
                    gestionVacunas->vacunarUsuario(tarjetaVacunacion);
                    vacunados++;
                } 
            }
        }
        cout << "Se han vacunado " << vacunados << " personas mayores de 25 años" << endl;  
        cout << "Tiempo de la segunda vacunacion es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        bool logico = false;
        
        t_ini = clock();
        for (int i=0; i<vecAuxiliar.size(); i++) {
            usuario = gestionVacunas->buscarUsuario(vecAuxiliar[i]);
            string idtar = usuario->GetNss() + usuario->GetNombre();
            tarjetaVacunacion = gestionVacunas->buscarTarjeta(idtar);
            if (tarjetaVacunacion != nullptr) {
                if (tarjetaVacunacion->pasaporteCovidCode(logico) != "" /*&& tarjetaVacunacion->GetDosis().size()>0*/) {
                    usuariosConPasaporteCovid++;
                }   
            }
        }
        cout << "Tienen el pasaporte covid " << usuariosConPasaporteCovid << " personas" << endl;
        cout << "Tiempo para saber cuantos tienen el pasaporte covid es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        vacunados = 0;
        TarjetaVacunacion *taux;
       
        vector<string> idsTarjetas;
        logico = false;
        
        t_ini = clock();
        idsTarjetas = gestionVacunas->llamadaVacunacionMenores(0.35);
        vector<string> vectorAuxiliar;
        
        for (int i=0; i<idsTarjetas.size(); i++) {
            logico = false;
            taux = gestionVacunas->buscarTarjeta(idsTarjetas[i]);
            if (taux->GetUsuario()->Edad() >= 5 && taux->GetUsuario()->Edad() < 12) {
                for (int j=0; j<idsTarjetas[i].size(); j++) {
                    if (taux != nullptr) {
                        if (idsTarjetas[i][j] == '5') {
                            logico = true; 
                            break;
                        }
                    }
                }
            }
            if (logico) {
                vectorAuxiliar.push_back(idsTarjetas[i]);
                vacunados++;
            }  
        }
        gestionVacunas->vacunarConjuntoTarjetas(vectorAuxiliar);
        cout << "Los avisados entre 5 y 11 años son: " << idsTarjetas.size() << endl;
        cout << "El numero de vacunados entre 5 y 11 años que tienen en su nss un 5 son: " << vacunados << endl;
        cout << "Tiempo para avisar y vacunar es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        int porcentaje = 0;   // Contador auxiliar para el porcentaje
        
        t_ini = clock();
        idsTarjetas = gestionVacunas->llamadaVacunacionMayoresDe(60,0.5);
        cout << "Los avisados mayores de 60 años son: " << idsTarjetas.size() << endl;
        cout << "Tiempo para avisar a los mayores de 60 años es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        vector<string> aux2;
        int i=0;
        int limiteVacunacion = idsTarjetas.size()*0.9;
        
        t_ini = clock();
        while (porcentaje < limiteVacunacion) {
            aux2.push_back(idsTarjetas[i]);
            porcentaje++;
            i++;   
        }
        gestionVacunas->vacunarConjuntoTarjetas(aux2);
        cout << "De las personas llamadas a vacunacion el 90% vacunadas han sido " << porcentaje << " personas." << endl;
        cout << "Tiempo para vacunar a los mayores de 60 años es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        Usuario *user;
        string auxnss;
        TarjetaVacunacion *tar;
        int contPautas = 0;
        
        t_ini = clock();
        for (i=0; i<vecAuxiliar.size(); i++) {
            user = gestionVacunas->buscarUsuario(vecAuxiliar[i]);
            if (user != nullptr) {
                auxnss = user->GetNss()+user->GetNombre();
                tar = gestionVacunas->buscarTarjeta(auxnss);
                if (tar->PautaCompletaRedefinida()) {
                    contPautas++;
                }
            }
        }
        cout << "El numero de personas con la pauta completa con las condiciones relegidas son: " << contPautas << endl;
        cout << "Tiempo para ver el numero de las personas con la pauta completa es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        vector<string> avisados = gestionVacunas->avisoColectivo(18);
        cout << "Los NSS de los usuarios llamados debido al contagio de la fiesta son: " << endl;
        
        for (i=0; i<avisados.size(); i++) {
            cout << "   - " << avisados[i] << endl;
        }
        cout << "Cont avisados: " << avisados.size() << endl;
        cout << "Tiempo para ver el nss  de las personas avisadas es: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;       
    } catch (exception& e) {
        cout << e.what() << endl;
    }
}