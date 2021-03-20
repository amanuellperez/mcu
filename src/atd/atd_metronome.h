// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *    20/03/2021 v0.0 
 *
 ****************************************************************************/
#include "atd_bcd.h"
#include <array>

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
}// namespace


#endif


