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

#ifndef __MEGA_TIMER2_GENERIC_H__
#define __MEGA_TIMER2_GENERIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Generic views of hwd::Timer2
 *    
 *  HISTORIA
 *    Manuel Perez
 *    13/00/2023 Escrito
 *
 ****************************************************************************/
#include "mega_timer2_hwd.h"
#include "mega_import_avr.h"
#include "mega_interrupt.h" // Disable_interrupts
#include "mega_clock_frequencies.h"	


#include <atd_type_traits.h>
#include <tuple>    // std::tie
		    
namespace mega_{

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
	hwd::Timer2::clock_frequency_no_prescaling();
    
    else if constexpr (period == 8u)
	hwd::Timer2::clock_frequency_divide_by_8();

    else if constexpr (period == 32u)
	hwd::Timer2::clock_frequency_divide_by_32();

    else if constexpr (period == 64u)
	hwd::Timer2::clock_frequency_divide_by_64();

    else if constexpr (period == 128u)
	hwd::Timer2::clock_frequency_divide_by_128();

    else if constexpr (period == 256u)
	hwd::Timer2::clock_frequency_divide_by_256();

    else if constexpr (period == 1024u)
	hwd::Timer2::clock_frequency_divide_by_1024();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect hwd::Timer2 period. Try another one. "
		    "Valid ones: 1, 8, 32, 64, 128, 256 or 1024.");
}

// avr clock at 8MHz
// -----------------
// a 125 ns
//template<>
//inline void hwd::Timer2::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

// TODO: revisar y escribir
//template<uint16_t period>
//inline void set_clock_period_in_us_8MHz() 
//{
//    if constexpr (period == 1u)
//	hwd::Timer2::clock_frequency_divide_by_8();
//
//    else if constexpr (period == 8u)
//	hwd::Timer2::clock_frequency_divide_by_64();
//
//    else if constexpr (period == 32u)
//	hwd::Timer2::clock_frequency_divide_by_256();
// 
//    else if constexpr (period == 128u)
//	hwd::Timer2::clock_frequency_divide_by_1024();
// 
//    else
//	static_assert(atd::always_false_v<int>,
//		    "Incorrect hwd::Timer2 period. Try another one. "
//		    "Valid ones: 1, 8, 32 or 128.");
//}


template<uint16_t period
	, uint32_t clock_frequency_in_Hz>
inline void set_clock_period_in_us()
{
    if constexpr (hwd::clock_cpu() == 1000000UL)
	set_clock_period_in_us_1MHz<period>();

//    else if constexpr (clock_cpu() == 8000000UL)
//	set_clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "set_clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



}// namespace



/***************************************************************************
 *			    Time_counter2
 ***************************************************************************/
class Time_counter2{
public:
// Types
// -----
    using Hwd	       = mega_::hwd::Timer2; // hardware que hay por debajo
    using Timer        = mega_::hwd::Timer2;
    using counter_type = typename Timer::counter_type;
    static constexpr counter_type minus_one = static_cast<counter_type>(-1); 


// Constructor
// -----------
// De momento el interfaz es static. Prohibo su construcción.
    Time_counter2() = delete;

    static void init(counter_type top0 = max_top());

// Interrupts
// ----------
    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER0_OVF que depende del hwd::Timer2 y no es genérico.
    // Genera una interrupción cuando llega al top.
    // Capturarla con ISR_TIMER2_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}
    

// Timer on/off
// ------------
    // Encendemos el Counter en modo síncrono. El usuario será responsable de
    // llamar a la función init y activar las interrupciones si las necesita.
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_Hz = hwd::clock_cpu()>
    struct turn_on_with_clock_period_of{ static void us(); };

    template <uint32_t clock_frequency_in_hz = hwd::clock_cpu()>
    static counter_type turn_on_with_overflow_to_count_1s();

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
inline void Time_counter2::init(counter_type top0)
{ 
    Timer::CTC_mode();
    reset();
    top(top0);
}


// Turn on
// -------
template<uint16_t period_in_us, uint32_t clock_frequency_in_Hz>
inline void 
Time_counter2::
turn_on_with_clock_period_of<period_in_us, clock_frequency_in_Hz>::us()
{timer2_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}



// (RRR) Números mágicos:
//	 Para 1 MHz: 125 * 125 * 64 us = 1.000.000 us = 1 s
template <uint32_t clock_frequency_in_hz>
Time_counter2::counter_type 
Time_counter2::turn_on_with_overflow_to_count_1s()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(125);
	Timer::clock_frequency_divide_by_64();

	enable_top_interrupt();
	return 125;
    }

    else
        static_assert(atd::always_false_v<int>,
                      "turn_on_with_overflow_to_count_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");


    return minus_one; // Nunca debería de llegar aquí
}


/***************************************************************************
 *			    Time_counter2_32kHz_g
 ***************************************************************************/
// Conectamos el hwd::Timer2 a un cristal externo de 32kHz.
// El hwd::Timer2 tiene que funcionar en modo asíncrono.
// De acuerdo a la datasheet:
// When writing to one of the registers TCNT2, OCR2x, or TCCR2x, the value 
// is transferred to a temporary register, and latched after two positive 
// edges on TOSC1. The user should not write a new value before the contents 
// of the temporary register have been transferred to its destination.  
// Each of the five mentioned registers has its individual temporary register,
// which means that e.g. writing to TCNT2 does not disturb an OCR2x write 
// in progress. The Asynchronous Status Register (ASSR) indicates that a 
// transfer to the destination register has taken place.
//
// TODO: de momento hago la implementación mínima para que funcione con
// Clock_s. Ir ampliando la clase a medida que se necesite (teniendo en cuenta
// el párrafo anterior)
//
template <uint16_t max_timeout_ms0 = 3000>
class Time_counter2_32kHz_g{
public:
    using Hwd	       = mega_::hwd::Timer2; // hardware que hay por debajo

// Asserts
// -------
    static_assert(hwd::clock_cpu() >= 4 * 32768,
		"The CPU main clock frequency must be more "
		"than four times the oscillator frequency");

// Conections
// ----------
    // Pines donde hay que conectar el cristal
    static constexpr uint8_t crystal_32_kHz_pin1 = hwd::cfg::timer2::TOSC1_pin;
    static constexpr uint8_t crystal_32_kHz_pin2 = hwd::cfg::timer2::TOSC2_pin;

// Types
// -----
    using Timer        = mega_::hwd::Timer2;
    using counter_type = typename Timer::counter_type;
    static constexpr counter_type minus_one = static_cast<counter_type>(-1); 

// Cfg
// ---
    // Las funciones que requieran de un timeout usarán este valor por defecto
    static constexpr uint16_t max_timeout_ms = max_timeout_ms0;

// Constructor
// -----------
// De momento el interfaz es static. Prohibo su construcción.
    Time_counter2_32kHz_g() = delete;


// Interrupts
// ----------
    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER0_OVF que depende del hwd::Timer2 y no es genérico.
    // Genera una interrupción cuando llega al top.
    // Capturarla con ISR_TIMER2_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}
    

// Timer on/off
// ------------
    static counter_type 
	turn_on_with_overflow_to_count_1s(uint16_t timeout_ms = max_timeout_ms);

    /// Apagamos el generador de señales.
    static void turn_off() { Timer::off(); }


//    /// Devuelve el valor del contador en ticks.
//    static counter_type value() 
//    { return Timer::counter(); }
//    
//    /// Hace que el counter = 0.
//    CUIDADO: escribe en TCNT2A, la escritura tarda 2 ciclos de reloj!!!
//    static void reset() { Timer::counter(0); }
//
//    /// Define el top del counter.
//    CUIDADO: escribe en OCR2A, la escritura tarda 2 ciclos de reloj!!!
//    static void top(counter_type top)
//    {Timer::output_compare_register_A(top);}
//
//    /// Valor del top
//    static counter_type top()
//    { return Timer::output_compare_register_A(); }

    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }

};




// Turn on
// -------
//template<uint16_t period_in_us, uint32_t clock_frequency_in_Hz>
//inline void 
//Time_counter2::
//turn_on_with_clock_period_of<period_in_us, clock_frequency_in_Hz>::us()
//{timer2_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}
//


// La forma de inicializar el hwd::Timer2 en modo asíncrono viene en el punto 22.9
// de la datasheet "Asynchronous Operation of Timer/Counter2"
template <uint16_t mto>
Time_counter2_32kHz_g<mto>::counter_type 
Time_counter2_32kHz_g<mto>::
		turn_on_with_overflow_to_count_1s(uint16_t timeout_ms)
{
    Disable_interrupts lock;

    Timer::disable_interrupts();
    Timer::enable_asynchronous_mode();
    Timer::CTC_mode();
    Timer::output_compare_register_A(128);
    Timer::clock_frequency_divide_by_256();  // arranca el timer
					      

    while (!Timer::are_registers_ready()){
	wait_ms(1);
	--timeout_ms;
	if (timeout_ms == 0)
	    return minus_one;
    }

    Timer::clear_pending_interrupts();
    Timer::enable_output_compare_A_match_interrupt(); 

    return 1; // genera un tick cada segundo
}



}// namespace 



#endif



