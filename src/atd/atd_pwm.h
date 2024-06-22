// Copyright (C) 2024 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#ifndef __ATD_PWM_H__
#define __ATD_PWM_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar señales PWM
 *
 * HISTORIA
 *    Manuel Perez
 *    05/06/2024 PWM_signal: versión mínima
 *    22/06/2024 SW_signal (a fin de cuentas es una PWM de 50% de duty cycle)
 *			    SW_signal = square wave signal
 *
 ****************************************************************************/
#include "atd_percentage.h"

namespace atd{
/***************************************************************************
 *			    PWM_signal
 ***************************************************************************/
template <typename Frequency_t, typename Time_t>
class PWM_signal{
public:
// typs
    using Frequency = Frequency_t;
    using Time      = Time_t;

// constructors
    PWM_signal(const Frequency& f, const Percentage& duty_cycle)
	    : frequency_{f}, duty_cycle_{duty_cycle} { }

// caracteristicas
    Frequency frequency() const { return frequency_;}
    void frequency(const Frequency& f) { frequency_ = f;}

    Time period() const { return inverse(frequency_);}
    void period(const Time& T) { frequency_ = inverse(T);}

    void duty_cycle(const Percentage& d) {duty_cycle_ = d;}
    Percentage duty_cycle() const { return duty_cycle_;}


private:
    Frequency frequency_;
    Percentage duty_cycle_; 
};


/***************************************************************************
 *			    SW_signal
 ***************************************************************************/
// (RRR) ¿es necesaria esta clase?
//  (1) Muestra intención:
//	Comparar dos prototipos de funciones:
//		generate(Frequency);
//		generate(SW_signal);
//	Aunque es verdad que las dos dicen lo mismo la segunda la entiendo
//	mejor. Muestra claramente que lo que quiero generar es una onda
//	cuadrada.
//
//  (2) Las ondas cuadradas se pueden definir con la frecuencia o con el
//      periodo, que se encarga SW_signal de gestionar el cambio.
//      Usando SW_signal todas las funciones tendrán un único prototipo:
//		generate(SW_signal);
//
//	pero si no se usa SW_signal habrá que definir dos funciones:
//		generate(Frequency);
//		generate(Time);
//	o que el cliente sea responsable de pasar de periodo a frecuencia,
//	complicando el código.
//
//  Por estos dos motivos la defino.
//
//	    
template <typename Frequency_t, typename Time_t>
class SW_signal{
public:
// typs
    using Frequency = Frequency_t;
    using Time      = Time_t;

// constructors
    SW_signal(const Frequency& f)
	    : frequency_{f}{ }

// caracteristicas
    Frequency frequency() const { return frequency_;}
    void frequency(const Frequency& f) { frequency_ = f;}

    Time period() const { return inverse(frequency_);}
    void period(const Time& T) { frequency_ = inverse(T);}

    // Le voy a suministrar esta función para que se puedan usar
    // PWM_signal y SW_signal casi de forma intercambiable
    Percentage duty_cycle() const { return Percentage{50};}

private:
    Frequency frequency_;
};

}// namespace

#endif


