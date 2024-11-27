// Copyright (C) 2022-2024 Manuel Perez 
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

#ifndef __MEGA_HWD_TIMERN_H__
#define __MEGA_HWD_TIMERN_H__
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

#include "mega_import_avr.h"

namespace mega_{

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


// PWM cfg contiene la forma en que hay que configurar el timer para generar
// una determinada señal PWM
template <typename Timern>
class PWM_cfg{
public:
// Types
    using Timer        = Timern;
    using top_type     = typename Timer::counter_type;

// Data
    top_type top;
    uint16_t prescaler; // puede llegar hasta 1024

// Calculo de la frecuencia generada por el modo
    // Calcula la frecuencia que generaria este PWM_cfg configurado 
    // como fast mode
    Frequency frequency_fast_mode(const Frequency& freq_clk) const;

    // Calcula la frecuencia que generaria este PWM_cfg configurado 
    // como phase mode
    Frequency frequency_phase_mode(const Frequency& freq_clk) const;

// Calculo del prescaler a partir del top.
// Lo que hace es rellenar el valor del top con el valor correspondiente al
// prescaler. Dan por supuesto que prescaler tiene el valor de entrada.
// Quizas mejor: 
//  [top, error] = top_fast_mode(freq_clk, freq_gen,  prescaler); (???)
    // Devuelve el error cometido (100 en caso de no poder calcular la cfg)
    // fast mode
    uint8_t prescaler2top_fast_mode( const Frequency::Rep& freq_clk,
				     const Frequency::Rep& freq_gen);
    // phase mode
    uint8_t prescaler2top_phase_mode( const Frequency::Rep& freq_clk,
				      const Frequency::Rep& freq_gen);

// Errores
    // Manejamos números positivos
    template <std::integral Int>
    static uint8_t percentage_error(const Int& x, const Int& y);

};

template <typename C>
auto PWM_cfg<C>::frequency_fast_mode(const Frequency& freq_clk) const
    -> Frequency
{ return freq_clk / (prescaler * (top + 1)); }

template <typename C>
inline 
auto PWM_cfg<C>::frequency_phase_mode(const Frequency& freq_clk) const
    -> Frequency
{ return freq_clk / (2 * prescaler * top); }



// Devuelve el error (100 en caso de no poder calcular la cfg)
// precondition: prescaler tiene un valor válido
//
//  Fast  PWM: freq_gen = freq_clk/(p * (top + 1);
template <typename C>
uint8_t PWM_cfg<C>::prescaler2top_fast_mode( const Frequency::Rep& freq_clk,
				 const Frequency::Rep& freq_gen)
{
    using Rep = Frequency::Rep;

    // Si se trunca se comete en algunas ocasiones mucho error
    auto q = atd::divide_rounding(freq_clk, freq_gen);

    if (prescaler > q){
	top = 0;
	return 100;
    }

    Rep top2 = atd::divide_rounding(q, Rep{prescaler}) - 1;
    if (atd::overflow<top_type>(top2)) // cuidado con los overflow!!!
	return 100;

    top = static_cast<top_type>(top2);

    if (top > 3 and top < Timer::max()){

	Frequency::Rep freq_res = freq_clk / (prescaler * (top + 1));
	return percentage_error(freq_res, freq_gen);
    }

    else
	return 100;

}

// Devuelve el error (100 en caso de no poder calcular la cfg)
// precondition: prescaler tiene un valor válido
//
//  Phase PWM: freq_gen = freq_clk/(2 * p * top);
template <typename C>
uint8_t PWM_cfg<C>::prescaler2top_phase_mode( const Frequency::Rep& freq_clk,
				 const Frequency::Rep& freq_gen)
{
    using Rep = Frequency::Rep;

    // Si se trunca se comete en algunas ocasiones mucho error
    auto q = atd::divide_rounding(freq_clk, freq_gen);

    if (prescaler > q){
	top = 0;
	return 100;
    }

    auto top2 = atd::divide_rounding(q, Rep{2 * prescaler});
    if (atd::overflow<top_type>(top2)) // cuidado con los overflow!!!
	return 100;

    top = static_cast<top_type>(top2);
    
    if (top > 4 and top < Timer::max()){

//	Frequency::Rep freq_res = frequency_phase_mode(freq_clk).value();
	Frequency::Rep freq_res = freq_clk / (2 * prescaler * top);
	return percentage_error(freq_res, freq_gen);
    }

    else
	return 100;

}


template <typename C>
template <std::integral Int>
uint8_t PWM_cfg<C>::percentage_error(const Int& x, const Int& y)
{ 
    if (x >= y)
	return ((x - y) * 100) / y;

    else
	return ((y - x) * 100) / y;
}

}// namespace timer_
 
}// namespace


#endif


