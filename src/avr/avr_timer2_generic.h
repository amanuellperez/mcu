// Copyright (C) 2023 Manuel Perez 
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

#ifndef __AVR_TIMER2_GENERIC_H__
#define __AVR_TIMER2_GENERIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Generic views of Timer2
 *    
 *  HISTORIA
 *    Manuel Perez
 *    13/00/2023 Escrito
 *
 ****************************************************************************/
#include "avr_timer2_basic.h"
#include "avr_cfg.h"	// MCU_CLOCK_FREQUENCY_IN_HZ

#include <atd_type_traits.h>
#include <tuple>    // std::tie
		    
namespace avr_{

namespace timer2_{

// DUDA: aunque queda muy bien que devuelvan Time y Frequency esto es un
// traductor y es de bajo nivel. Time y Frequency generan un montón de
// dependencias que no deben de tener los traductores. ¿Cambiarlo a uint32_t y
// que todo sea en microsegundos y herzios?

// avr clock at 1MHz
// -----------------
template<uint16_t period>
inline void set_clock_period_in_us_1MHz() 
{
    if constexpr (period == 1u)
	Timer2::clock_frequency_no_preescaling();
    
    else if constexpr (period == 8u)
	Timer2::clock_frequency_divide_by_8();

    else if constexpr (period == 32u)
	Timer2::clock_frequency_divide_by_32();

    else if constexpr (period == 64u)
	Timer2::clock_frequency_divide_by_64();

    else if constexpr (period == 128u)
	Timer2::clock_frequency_divide_by_128();

    else if constexpr (period == 256u)
	Timer2::clock_frequency_divide_by_256();

    else if constexpr (period == 1024u)
	Timer2::clock_frequency_divide_by_1024();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer2 period. Try another one. "
		    "Valid ones: 1, 8, 32, 64, 128, 256 or 1024.");
}

// avr clock at 8MHz
// -----------------
// a 125 ns
//template<>
//inline void Timer2::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

// TODO: revisar y escribir
//template<uint16_t period>
//inline void set_clock_period_in_us_8MHz() 
//{
//    if constexpr (period == 1u)
//	Timer2::clock_frequency_divide_by_8();
//
//    else if constexpr (period == 8u)
//	Timer2::clock_frequency_divide_by_64();
//
//    else if constexpr (period == 32u)
//	Timer2::clock_frequency_divide_by_256();
// 
//    else if constexpr (period == 128u)
//	Timer2::clock_frequency_divide_by_1024();
// 
//    else
//	static_assert(atd::always_false_v<int>,
//		    "Incorrect Timer2 period. Try another one. "
//		    "Valid ones: 1, 8, 32 or 128.");
//}


template<uint16_t period
	, uint32_t clock_frequency_in_Hz>
inline void set_clock_period_in_us()
{
    if constexpr (clock_frequency_in_Hz == 1000000UL)
	set_clock_period_in_us_1MHz<period>();

//    else if constexpr (clock_frequency_in_Hz == 8000000UL)
//	set_clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "set_clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



}// namespace



/***************************************************************************
 *			    Time_counter2_g
 ***************************************************************************/
class Time_counter2_g{
public:
// Types
// -----
    using Timer        = avr_::Timer2;
    using counter_type = typename Timer::counter_type;

// Constructor
// -----------
// De momento el interfaz es static. Prohibo su construcción.
    Time_counter2_g() = delete;

    static void init(counter_type top0 = max_top());

// Interrupts
// ----------
    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER0_OVF que depende del Timer2 y no es genérico.
    // Genera una interrupción cuando llega al top.
    // Capturarla con ISR_TIMER2_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}
    

// Timer on/off
// ------------
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_Hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    struct turn_on_with_clock_period_of{
	static void us()
	{timer2_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}
    };

    /// Apagamos el generador de señales.
    static void turn_off() { Timer::off(); }


    /// Devuelve el valor del contador en ticks.
    static counter_type value() 
    { return Timer::counter(); }
    
    /// Hace que el counter = 0.
    static void reset() { Timer::counter(0); }

    /// Define el top del counter.
    static void top(counter_type top)
    {Timer::output_compare_register_A(top);}

    /// Valor del top
    static counter_type top()
    { return Timer::output_compare_register_A(); }

    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }
};


// TODO: a .cpp?
inline void Time_counter2_g::init(counter_type top0)
{ 
    Timer::CTC_mode();
    reset();
    top(top0);
}


}// namespace 



#endif



