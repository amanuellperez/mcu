// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __AVR_TIMER0_BASIC_H__
#define __AVR_TIMER0_BASIC_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del timer 0 del AVR. Es un timer de 8 bits.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *
 *      06/08/2017 v0.0: Escrito.
 *      18/07/2019 v0.1: Reescrito.
 *      04/01/2020       Creado traductor.
 *      14/02/2021 v0.2: Convertido en traductor puro.
 *      17/11/2022       disable_..._interrupt()
 *		         Y no era un traductor puro, ya que al activar las
 *		         interrupciones también activaba las interrupciones
 *		         del avr, cosa que no viene en la datasheet. Recordar
 *		         que un traductor se limita a traducir la datasheet
 *		         sin añadir/quitar nada 
 *	02/01/2023       Convertido en traductor puro (???)
 *			 Ya es la 4ª vez que lo intento convertir en traductor
 *			 puro. Espero que ya no haya dependencias. He
 *			 eliminado la función on<> moviéndola al
 *			 Timer0_generic. 
 *			 Esto tiene que ser un traductor: se limita a TRADUCIR
 *			 la datasheet sin añadir nada.
 *
 *	08/07/2024	 mode(), prescaler()
 ****************************************************************************/
#include <avr/io.h> // registros: DDRB... PORT...
#include "avr_timern_basic.h"

#include <atd_bit.h>
#include <atd_type_traits.h>

#include "avr_interrupt.h"
#include "mega_pin.h"
#include <avr_constants.h>

#include <array>

namespace avr_{

// La creo como clase para luego poderla pasar como parámetro a templates
// (por ejemplo, a Contador). Si la defino como namespace no puedo hacerlo.
class Timer0{

public:
// CARACTERÍSTICAS DEL TIMER
    using counter_type = cfg::timer0::counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom() {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr counter_type OCA_pin() {return cfg::timer0::OCA_pin; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr counter_type OCB_pin() {return cfg::timer0::OCB_pin;}


// CONFIGURACIÓN DEL RELOJ
    // (RRR) ¿por qué definir explícitamente aquí los prescaler_factor?
    //       Los podía definir en cfg::timer0, pero la implementación de esta
    //       clase conoce los prescalers en las funciones
    //       clock_frequency_no_prescaling, ... O se generalizan esas
    //       funciones metiendolas en cfg::timer0 o no mejor hacerlo todo
    //       concreto.
    //
    // Array con los posibles prescaler factors del timer.
    static constexpr std::array<uint16_t, 5> 
				prescaler_factor = {1, 8, 64, 256, 1024};

    enum class Frequency_divisor{
		    no_clock_prescaling,    
		    off = no_clock_prescaling,
		    no_prescaling,
		    divide_by_8,
		    divide_by_64,
		    divide_by_256,
		    divide_by_1024};

    // Obtenemos el divisor de frecuencia que se aplica al reloj del micro.
    static Frequency_divisor frequency_divisor();


    // Selección del reloj y de su velocidad (según tabla 19-10)
    // Establecemos el divisor de frecuencia a aplicar al reloj del micro.
    static void clock_frequency_no_prescaling();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_256();
    static void clock_frequency_divide_by_1024();


    // Devuelve el prescaler correspondiente
    static uint16_t prescaler();

    /// Seleccionamos la frecuencia a la que funciona el timer usando el
    /// divisor de frecuencias (prescaler_factor)
    /// Enciende el timer (si prescaler_factor != 0) o lo apaga (si es == 0)
    static void prescaler(uint16_t prescaler_factor);

    /// Frecuencia a la que funciona internamente el timer.
    /// Se cumple que clock_frequency() = 1 / clock_period();
    template <uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
    static Frequency clock_frequency();


    /// Paramos el timer.
    static void off();

    static bool is_on();
    static bool is_off();


// SELECCIÓN DE RELOJ EXTERNO
    /// External clock source on T0 pin. Clock on falling edge.
    static void external_clock_falling_edge();

    /// External clock source on T0 pin. Clock on rising edge.
    static void external_clock_rising_edge();


// ACCESO A REGISTROS
    static counter_type counter() {return TCNT0;}
    static void counter(counter_type x) { TCNT0 = x;}

    static counter_type output_compare_register_A() {return OCR0A;}
    static void output_compare_register_A(counter_type x){OCR0A = x;}

    static counter_type output_compare_register_B() {return OCR0B;}
    static void output_compare_register_B(counter_type x) {OCR0B = x;}

// WAVEFORM GENERATION MODES (table 19-9)
    enum class Mode{
	normal, CTC, 
	fast_PWM_top_0xFF, fast_PWM_top_OCRA,
	PWM_phase_correct_top_0xFF, PWM_phase_correct_top_OCRA,
	reserved};

    static Mode mode();

// Normal mode
    static void normal_mode();

// CTC mode
    static void CTC_mode();

// Fast PWM modes
    static void fast_PWM_mode_top_OCRA();
    static void fast_PWM_mode_top_0xFF();

// PWM phase correct modes
    static void PWM_phase_correct_mode_top_OCRA();
    static void PWM_phase_correct_mode_top_0xFF();


// pins operation 
    // comunes a todas las tablas (19-3 y siguientes)
    static void pin_A_disconnected();
    static void pin_B_disconnected();

    static bool is_pin_A_disconnected();
    static bool is_pin_A_connected();

    static bool is_pin_B_disconnected();
    static bool is_pin_B_connected();

    // table 19-3 and 19-6
    static void CTC_pin_A_toggle_on_compare_match();
    static void CTC_pin_A_clear_on_compare_match();
    static void CTC_pin_A_set_on_compare_match();

    static void CTC_pin_B_toggle_on_compare_match();
    static void CTC_pin_B_clear_on_compare_match();
    static void CTC_pin_B_set_on_compare_match();

    // tables 19-4,5, 7 and 8.
    static void PWM_pin_A_non_inverting_mode();
    static void PWM_pin_A_inverting_mode();
    static void PWM_pin_B_non_inverting_mode();
    static void PWM_pin_B_inverting_mode();

    // Solo para los modos mode_PWM_phase_correct_top_OCRA y
    // mode_fast_PWM_top_OCRA
    static void PWM_pin_A_toggle_on_compare_match();

    // La datasheet indica que no hay toggle mode en el pin B.
//    static void PWM_pin_B_toggle_on_compare_match();


// INTERRUPCIONES
//  Recordar que hay que llamar a avr::enable_interrupts(); para que
//  se puedan usar las interrupciones.
    /// Se captura con ISR_TIMER0_OVF
    static void enable_overflow_interrupt();
    static void disable_overflow_interrupt();

    /// Se captura con ISR_TIMER0_COMPA
    static void enable_output_compare_A_match_interrupt();
    static void disable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER0_COMPB
    static void enable_output_compare_B_match_interrupt();
    static void disable_output_compare_B_match_interrupt();

    static void clear_pending_interrupts();

// TODO
    // Faltan FOC0A/FOC0B
    
}; // Timer0




// preescaler
// ----------
inline bool Timer0::is_on()
{return !is_off();}

inline bool Timer0::is_off()
{ return prescaler() == 0; }

inline void Timer0::off()
{ // 000
    atd::write_bits<CS02, CS01, CS00>::to<0,0,0>::in(TCCR0B);
}


inline void Timer0::clock_frequency_no_prescaling() 
{   // 001
    atd::write_bits<CS02, CS01, CS00>::to<0,0,1>::in(TCCR0B);
}

/// Frecuencia del reloj preescalado =  clk_io/8
inline void Timer0::clock_frequency_divide_by_8()
{   // 010
    atd::write_bits<CS02, CS01, CS00>::to<0,1,0>::in(TCCR0B);
}

/// Frecuencia del reloj preescalado =  clk_io/64
inline void Timer0::clock_frequency_divide_by_64()
{   //011
    atd::write_bits<CS02, CS01, CS00>::to<0,1,1>::in(TCCR0B);
}

/// Frecuencia del reloj preescalado =  clk_io/256
inline void Timer0::clock_frequency_divide_by_256()
{   // 100
    atd::write_bits<CS02, CS01, CS00>::to<1,0,0>::in(TCCR0B);
}

/// Frecuencia del reloj preescalado =  clk_io/1024
inline void Timer0::clock_frequency_divide_by_1024()
{   // 101
    atd::write_bits<CS02, CS01, CS00>::to<1,0,1>::in(TCCR0B);
}


inline void Timer0::external_clock_falling_edge()
{// 110
    atd::write_bits<CS02, CS01, CS00>::to<1,1,0>::in(TCCR0B);
}

inline void Timer0::external_clock_rising_edge()
{// 111
    atd::write_bits<CS02, CS01, CS00>::to<1,1,1>::in(TCCR0B);
}


// Modos de funcionamiento
// -----------------------
inline void Timer0::normal_mode()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,0>::in(TCCR0A);
}


inline void Timer0::CTC_mode()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,0>::in(TCCR0A);
}

inline void Timer0::fast_PWM_mode_top_0xFF()
{// 011
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,1>::in(TCCR0A);
}

inline void Timer0::fast_PWM_mode_top_OCRA()
{// 111
    atd::write_bits<WGM02>::to<1>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,1>::in(TCCR0A);
}

inline void Timer0::PWM_phase_correct_mode_top_0xFF()
{// 001
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,1>::in(TCCR0A);
}

inline void Timer0::PWM_phase_correct_mode_top_OCRA()
{// 101
    atd::write_bits<WGM02>::to<1>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,1>::in(TCCR0A);
}


// pins
// ----
inline bool Timer0::is_pin_A_connected()
{return !is_pin_A_disconnected(); }

inline bool Timer0::is_pin_A_disconnected()
{
    return atd::read_bits<COM0A1, COM0A0>::of(TCCR0A) ==
	atd::zero<uint8_t>::with_bits<COM0A1, COM0A0>::to<0,0>();
}

inline bool Timer0::is_pin_B_connected()
{return !is_pin_B_disconnected(); }


inline bool Timer0::is_pin_B_disconnected()
{
    return atd::read_bits<COM1B1, COM1B0>::of(TCCR0A) ==
	atd::zero<uint8_t>::with_bits<COM1B1, COM1B0>::to<0,0>();
}


inline void Timer0::pin_A_disconnected()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<0,0>::in(TCCR0A); 
    // (???) ¿En qué estado dejar el puerto pin_B()?
}

inline void Timer0::pin_B_disconnected()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<0,0>::in(TCCR0A); 
    // (???) ¿En qué estado dejar el puerto pin_B()?
}


inline void Timer0::CTC_pin_A_toggle_on_compare_match()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<0,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer0::CTC_pin_A_clear_on_compare_match()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<1,0>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer0::CTC_pin_A_set_on_compare_match()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<1,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}




inline void Timer0::CTC_pin_B_toggle_on_compare_match()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<0,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer0::CTC_pin_B_clear_on_compare_match()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<1,0>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer0::CTC_pin_B_set_on_compare_match()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<1,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer0::PWM_pin_A_non_inverting_mode()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<1,0>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer0::PWM_pin_A_inverting_mode()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<1,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer0::PWM_pin_B_non_inverting_mode()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<1,0>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer0::PWM_pin_B_inverting_mode()
{ 
    atd::write_bits<COM0B1, COM0B0>::to<1,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer0::PWM_pin_A_toggle_on_compare_match()
{ 
    atd::write_bits<COM0A1, COM0A0>::to<0,1>::in(TCCR0A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}



// Interrupciones
// --------------
inline void Timer0::enable_overflow_interrupt()
{ atd::write_bits<TOIE0>::to<1>::in(TIMSK0); }

inline void Timer0::disable_overflow_interrupt()
{ atd::write_bits<TOIE0>::to<0>::in(TIMSK0); }

inline void Timer0::enable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE0A>::to<1>::in(TIMSK0); }

inline void Timer0::disable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE0A>::to<0>::in(TIMSK0); }

inline void Timer0::enable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE0B>::to<1>::in(TIMSK0); }

inline void Timer0::disable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE0B>::to<0>::in(TIMSK0); }

inline void Timer0::clear_pending_interrupts()
{ atd::write_bits<OCF0B, OCF0A, TOV0>::to<0,0,0>::in(TIFR0); }

}// namespace avr


#endif

