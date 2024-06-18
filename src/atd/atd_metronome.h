// Copyright (C) 2021 Manuel Perez 
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

#ifndef __ATD_METRONOME_H__
#define __ATD_METRONOME_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Metrónomo. Nos marca el ritmo.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    20/03/2021 v0.0 
 *
 ****************************************************************************/
#include "atd_bcd.h"
#include <array>
#include <cstddef>  // size_t
#include <limits>

namespace atd{
/*!
 *  (MMM) Cuando un usuario pulsa una tecla y la mantiene pulsada para, por 
 *	  ejemplo, poner en hora un reloj al principio queremos que la hora
 *	  cambie lentamente pero según mantiene la tecla pulsada queremos que
 *	  vaya más rápido. Eso lo controlo con esta clase.
 *
 *  Ejemplo:
 *	Entrada: el tick de un reloj (llamamos a tick())
 *	Salida : se generan pulsos (el trigger()) cada cierto tiempo.
 *	Ver ejemplos (el test)
 *
 */
template <typename array_d1, typename array_d0>
class Metronome{
public:
    Metronome() : counter_{d1_max[0], d0_max[0]} , i_{0}
    {}

    void reset();
    void tick();

    bool trigger() const { return counter_.d0() == d0_max[i_];}

    bool end() const { return (i_ == nstates - 1) and (counter_.is_max()); }


// Observer (for debug)
    uint8_t as_uint() const {return counter_.as_uint();}

private:
    atd::Counter_BCD2 counter_;

    uint8_t i_; // indice del state en el que estamos

// Configuración del contador
    static_assert(array_d0::size == array_d1::size);

    constexpr static uint8_t nstates = array_d0::size;
    constexpr static array_d1 d1_max{};
    constexpr static array_d0 d0_max{};

// Helper functions
    void next_state();
};


template <typename D1, typename D0>
void Metronome<D1, D0>::reset() 
{ 
    counter_.reset();
    counter_.max(d1_max[0], d0_max[0]); 

    i_ = 0;
}


template <typename D1, typename D0>
void Metronome<D1,D0>::tick() 
{
    if (counter_.is_max())
	next_state();
    else
	counter_.tick();

}

template <typename D1, typename D0>
void Metronome<D1,D0>::next_state()
{
    if (i_ < nstates - 1){
	++i_;
	counter_.max(d1_max[i_], d0_max[i_]);
	counter_.reset();
    }
}


/*!
 *  \brief  Sucesión de valores constante a trozos (de ahi el step).
 *
 *  (MMM) Cuando un usuario tiene que seleccionar un número de un LCD pulsando
 *	  únicamente un botón para UP, la idea es que al principio suba 
 *	  lentamente y luego más rápidamente. 
 *
 *	  Una forma de implementarlo es ir marcando tiempo con un reloj (o un
 *	  metronomo) e irle sumando primero 1, luego 10, luego 100...
 *
 *	  Esta sucesión se encarga de definir la sucesión 1, ... , 10, ...,
 *	  100. Sería el equivalente a la sucesión:
 *		1,1,1,1,1,1,1,1,1,10,10,10,10,10,100,100,100,...
 *
 *
 *  Definimos la sucesión a trozos dando:
 *	array_x = 0,  9,  14
 *	array_y = 1, 10, 100
 *
 *  Que es equivalente a definir la siguiente sucesión a trozos:
 *        /  1   si 0 <= n < 9
 *        |  
 *  y_n = |  10  si 9 <= n < 14
 *        |
 *        |  100 si 14 <= n 
 *
 *  N = número de ramas de la función a trozos (en el ejemplo anterior 3)
 *  Int = tipo integral usado para los índices (en avr será uint8_t, mientras
 *	  que en un programa de ordenador int)
 *  T = Tipo de y_n (puede ser uint16_t, avr::Frequency, ...)
 */
template <size_t N, typename Int, typename T>
class Sequence_of_steps{
public:
    using value_type = T;

    constexpr Sequence_of_steps(const std::array<Int, N>& x, 
				const std::array<T, N>& y)
	    : array_x_{x}, array_y_{y} { }

    void reset() {i_ = 0; i_array_ = 0;}
    void next() 
    { 
	++i_; 
	if (i_array_ < N - 1){
	    if (array_x_[i_array_ + 1] == i_)
		++i_array_;
	}
    }

    value_type value() const
    { return array_y_[i_array_]; }

private:
    std::array<Int, N> array_x_;
    std::array<T, N> array_y_;

    uint8_t i_ = 0;
    uint8_t i_array_= 0; // posición dentro del array_x
};


// Implementación static:
template <typename Array_x, typename Array_y>
class static_Sequence_of_steps{
public:
    using value_type = typename Array_y::value_type;

    void reset() {i_ = 0; i_array_ = 0;}
    void next() 
    { 
	++i_; 
	if (i_array_ < Array_x::size - 1){
	    if (array_x[i_array_ + 1] == i_)
		++i_array_;
	}
    }

    value_type value() const
    { return array_y[i_array_]; }

private:
    static_assert(Array_x::size < std::numeric_limits<uint8_t>::max());
    static_assert(Array_x::size == Array_y::size);

    constexpr static Array_x array_x{};
    constexpr static Array_y array_y{};

    uint8_t i_ = 0;
    uint8_t i_array_= 0; // posición dentro del array_x
};



}// namespace


#endif


