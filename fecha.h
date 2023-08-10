#include <iostream>
#include <string>
using namespace std;

#ifndef FECHA_H
#define FECHA_H

/**
 * @brief Excepción que representa fechas incorrectas
 */
class ErrorFechaIncorrecta {
};

/**
 * @brief Clases sencilla para representar fechas y horas
 */
class Fecha {
    /**
     * @brief Fecha y hora
     */
    unsigned dia, mes, anio, hora, min;
    /**
     * @brief Dias por mes
     */
    static const unsigned diasMes[12];

    /**
     * @brief Comprobación de validez de una fecha
     */
    void comprobarFecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora, unsigned aMin) const;
    /**
     * @brief Función auxiliar de conversin desde estructura de tiempo tm de time.h
     */
    void leerTiempo(const tm &t);
    /** 
     * @brief Función auxiliar de conversin a estructura de tiempo tm de time.h
     */
    void escribirTiempo(tm &t);

    public:
	/** 
         * @brief Crea una fecha con la hora actual
         */
	Fecha();
	/** 
         * @brief Crea una fecha concreta. Devuelve una excepción ErrorFechaIncorrecta si la fecha introducida no es correcta 
         */
	Fecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora = 0, unsigned aMin = 0);
	/** 
         * @brief Constructor copia 
         */
	Fecha(const Fecha &f): dia(f.dia), mes(f.mes), anio(f.anio), hora(f.hora), min(f.min) {}

	/** 
         * @brief Asignar un nuevo día 
         */
	void asignarDia(unsigned aDia, unsigned aMes, unsigned aAnio);
	/** 
         * Asignar una nueva hora 
         */
	void asignarHora(unsigned aHora, unsigned aMin);

	/** 
         * @brief Obtener hora
         */
	unsigned verHora() const { return hora; }
	/** 
         * @brief Obtener minutos 
         */
	unsigned verMin() const { return min; }
	/** 
         * @brief Obtener dia 
         */
	unsigned verDia() const { return dia; }
	/** 
         * @brief Obtener mes 
         */
	unsigned verMes() const { return mes; }
	/**
         *  @brief Obtener año 
         */
	unsigned verAnio() const { return anio; }

	/** 
         * @brief Obtener una cadena con el da 
         */
	string cadenaDia() const;
	/**
         *  @brief Obtener una cadena con la hora 
         */
	string cadenaHora() const;
	/** 
         * @brief Obtener una cadena con da y hora 
         */
	string cadena() const {
            return cadenaDia() + " " + cadenaHora(); 
	}
	/** 
         * @brief Indica si las dos fechas tienen el mismo da 
         */
	bool mismoDia(const Fecha &f) const {
            return dia == f.dia && mes == f.mes && anio == f.anio; 
	}
	/**
         *  @brief Comparar fechas 
         */
	bool operator<(const Fecha &f);
	/** 
         * @brief Asignar fechas 
         */
	Fecha &operator=(const Fecha &f);
	/** 
         * @brief Añadir un nmero de minutos indicado 
         */
	void anadirMin(int numMin);
	/** 
         * @brief Añadir un nmero de horas indicado
         */
	void anadirHoras(int numHoras);
	/** 
         * @brief Añadir un nmero de das indicado 
         */
	void anadirDias(int numDias);
	/** 
         * Añadir un nmero de meses indicado 
         */
	void anadirMeses(int numMeses);
	/** 
         * @brief Añadir un nmero de aos indicado
         */
	void anadirAnios(int numAnios);

	~Fecha();
};

/** 
 * @brief Serializacin de fechas: salida 
 */
ostream &operator<<(ostream &os, const Fecha &f);

#endif