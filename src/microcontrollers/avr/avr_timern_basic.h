// Copyright (C) 2022 Manuel Perez 
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

#ifndef __AVR_TIMERN_BASIC_H__
#define __AVR_TIMERN_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Cosas comunes a los atmega::Timers
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    06/12/2022 Escrito
 *
 ****************************************************************************/
#include <utility>  // std::pair
#include <array>
#include <cstdint>

#include "avr_types.h"

namespace avr_{

namespace timer_{ // private. Don't use them 

// CTC mode
// --------
class CTC_mode{
public:

// CUIDADO: estas fórmulas son para el CTC mode.
// Fórmula para calcular la frecuencia dados el divisor d (prescaler_factor) y
// el top M del counter. (pag. 132 datasheet)
// d = prescaler factor
// M = top
template <uint32_t f_clock_in_Hz>
static constexpr 
uint32_t prescaler_top_to_frequency_in_Hz(uint32_t d, uint32_t M)
{ return f_clock_in_Hz / (2 * d * (1 + M)); }


// Función inversa a timer_prescaler_top_to_frequency_in_Hz.
// Calcula el valor del top necesario para generar la frecuencia freq_in_Hz.
// Se podía cometer menos error si se redondeara dx1_M (= d*(1+M)).
// Pero eso llevaría más tiempo y como muestra el test no se comete un error
// de más del 10%.
// Notación: 
//	d = prescaler factor
//	M = top
template <typename Timer, uint32_t f_clock_in_Hz>
static constexpr 
std::pair<uint32_t, uint32_t> 
frequency_in_Hz_to_prescaler_top(Frequency::Rep freq_in_Hz)
{
    // f = f_clock_in_Hz / (2 * d * (1 + M)) ==> d*(1 + M) = f_clock_in_Hz / (2 * f);
    uint32_t dx1_M = f_clock_in_Hz / (2 * freq_in_Hz);

    for (size_t i = 0; i < Timer::prescaler_factor.size(); ++i){
	uint32_t M = dx1_M / Timer::prescaler_factor[i];

	if (M <= Timer::max())
	    return {Timer::prescaler_factor[i], M - 1};
    }

    return {0, 0};

}

// Devuelve el par [prescaler, top] necesario para configurar el Timer en el
// modo CTC para generar la frecuencia freq_gen.
template <typename Timer, uint32_t f_clock_in_Hz>
static constexpr 
std::pair<uint32_t, uint32_t> 
frequency_to_prescaler_top(const Frequency& freq_gen)
{ return frequency_in_Hz_to_prescaler_top<Timer, f_clock_in_Hz>(freq_gen.value()); }

private:
    
};

}// namespace timer_
 
}// namespace


#endif


