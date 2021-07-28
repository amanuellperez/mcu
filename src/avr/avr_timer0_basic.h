// Copyright (C) 2019-2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *
 *      06/08/2017 v0.0: Escrito.
 *      18/07/2019 v0.1: Reescrito.
 *      04/01/2020       Creado traductor.
 *      14/02/2021 v0.2: Convertido en traductor puro.
 *
 ****************************************************************************/

#include <atd_bit.h>
#include <atd_type_traits.h>

#include "avr_interrupt.h"
#include "avr_cfg.h"
#include "avr_pin.h"

namespace avr{

// La creo como clase para luego poderla pasar como parámetro a templates
// (por ejemplo, a Contador). Si la defino como namespace no puedo hacerlo.
class Timer0{

public:
// CARACTERÍSTICAS DEL TIMER
    using counter_type = TIMER0::counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom() {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr counter_type OCA_pin() {return TIMER0::OCA_pin; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr counter_type OCB_pin() {return TIMER0::OCB_pin;}


// CONFIGURACIÓN DEL RELOJ
    enum class Frequency_divisor{
		    undefined,    
		    no_preescaling,
		    divide_by_8,
		    divide_by_64,
		    divide_by_256,
		    divide_by_1024};

    // Obtenemos el divisor de frecuencia que se aplica al reloj del micro.
    static Frequency_divisor frequency_divisor();

    // Selección del reloj y de su velocidad (según tabla 19-10)
    // Establecemos el divisor de frecuencia a aplicar al reloj del micro.
    static void clock_frequency_no_preescaling();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_256();
    static void clock_frequency_divide_by_1024();

    // El timer lo que cuenta es tiempo, cuenta microsegundos.
    // De hecho si el avr funciona a 1MHz, el timer en normal mode solo puede
    // contar tiempos de 1us, 8us, 64us, 256us y 1024us. Es natural hablar de
    // estos tiempos, lo que no es natural es hablar del divisor de frecuencia.
    // Si yo quiero contar tiempo pienso en microsegundos (en tiempo) y no en
    // divisores de frecuencia. 
    //
    // Amen de que al hablar de tiempo en lugar de divisores de frecuencia
    // desvinculamos la implementación del cliente de la frecuencia real del
    // microcontrolador que estemos usando.
    //
    // La función clock_period_in_us traduce la forma de hablar del cliente (en
    // microsegundos) en la forma de hablar del avr (en divisor de frecuencia)
    template<uint16_t period
	    , uint32_t clock_frecuencia_en_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void set_clock_period_in_us();

    template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static Time clock_period();

    /// Frecuencia a la que funciona internamente el timer.
    /// Se cumple que clock_frequency() = 1 / clock_period();
    template <uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static Frequency clock_frequency();


// ENCENDIDO/APAGADO DEL TIMER
    /// Enciende el Timer, usando como reloj el reloj de periodo indicado.
    /// 'periodo' es el periodo en microsegundos al que va a funcionar el timer.
    /// clock_frequency_in_hz = es la frecuencia del reloj del AVR.
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {set_clock_period_in_us<period, clock_frequency_in_hz>();}

    /// Paramos el timer.
    static void off();


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
    static void mode_normal();

    static void mode_CTC();

    static void mode_fast_PWM_top_OCRA();
    static void mode_fast_PWM_top_0xFF();

    static void mode_PWM_phase_correct_top_OCRA();
    static void mode_PWM_phase_correct_top_0xFF();

// pins operation 
    // comunes a todas las tablas (19-3 y siguientes)
    static void pin_A_disconnected();
    static void pin_B_disconnected();

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
    /// Se captura con ISR_TIMER0_OVF
    static void enable_overflow_interrupt();

    /// Se captura con ISR_TIMER0_COMPA
    static void enable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER0_COMPB
    static void enable_output_compare_B_match_interrupt();


private:
    template<uint16_t period>
    static void set_clock_period_in_us_1MHz();

    template<uint16_t period>
    static void set_clock_period_in_us_8MHz();

    static Time clock_period_in_us_1MHz();
    static Frequency clock_frequency_in_Hz_1MHz();

}; // Timer0


inline Timer0::Frequency_divisor Timer0::frequency_divisor()
{ 
    switch(atd::read_bits<CS02, CS01, CS00>::of(TCCR0B)){
	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,0,1>():
	    return Frequency_divisor::no_preescaling;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,1,0>():
	    return Frequency_divisor::divide_by_8;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,1,1>():
	    return Frequency_divisor::divide_by_64;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<1,0,0>():
	    return Frequency_divisor::divide_by_256;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<1,0,1>():
	    return Frequency_divisor::divide_by_1024;
    }

    return Frequency_divisor::undefined;
}


// preescaler
// ----------
inline void Timer0::off()
{ // 000
    atd::write_bits<CS02, CS01, CS00>::to<0,0,0>::in(TCCR0B);
}


inline void Timer0::clock_frequency_no_preescaling() 
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


// avr clock at 1MHz
// -----------------
template<uint16_t period>
inline void Timer0::set_clock_period_in_us_1MHz() 
{
    if constexpr (period == 1u)
	clock_frequency_no_preescaling();
    
    else if constexpr (period == 8u)
	clock_frequency_divide_by_8();

    else if constexpr (period == 64u)
	clock_frequency_divide_by_64();

    else if constexpr (period == 256u)
	clock_frequency_divide_by_256();

    else if constexpr (period == 1024u)
	clock_frequency_divide_by_1024();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer0 period. Try another one.");
}

// avr clock at 8MHz
// -----------------
// a 125 ns
//template<>
//inline void Timer0::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

template<uint16_t period>
inline void Timer0::set_clock_period_in_us_8MHz() 
{
    if constexpr (period == 1u)
	clock_frequency_divide_by_8();

    else if constexpr (period == 8u)
	clock_frequency_divide_by_64();

    else if constexpr (period == 32u)
	clock_frequency_divide_by_256();
 
    else if constexpr (period == 128u)
	clock_frequency_divide_by_1024();
 
    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer0 period. Try another one.");
}


template<uint16_t period
	, uint32_t clock_frequency_in_hz>
inline void Timer0::set_clock_period_in_us()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	set_clock_period_in_us_1MHz<period>();

    else if constexpr (clock_frequency_in_hz == 8000000UL)
	set_clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "set_clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}

// TODO: a .cpp?
inline Time Timer0::clock_period_in_us_1MHz()
{
    using namespace literals;
    switch(frequency_divisor()){
	case Frequency_divisor::no_preescaling	: return 1_us;
	case Frequency_divisor::divide_by_8	: return 8_us;
	case Frequency_divisor::divide_by_64	: return 64_us;
	case Frequency_divisor::divide_by_256	: return 256_us;
	case Frequency_divisor::divide_by_1024	: return 1024_us;
	case Frequency_divisor::undefined	: return 0_us;
    }

    return 0_us;
}


template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
inline Time Timer0::clock_period()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_period_in_us_1MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



inline Frequency Timer0::clock_frequency_in_Hz_1MHz()
{
    using namespace literals;
    using Rep = Frequency::Rep;
    switch(frequency_divisor()){
	case Frequency_divisor::no_preescaling	: return 1_MHz;
	case Frequency_divisor::divide_by_8	: return 125_kHz;
	case Frequency_divisor::divide_by_64	: return Frequency{15625, 0};
	case Frequency_divisor::divide_by_256	: return Frequency{Rep{3906ul,25ul}, 0};
	case Frequency_divisor::divide_by_1024	: return Frequency{Rep{976ul,56ul}, 0};
	case Frequency_divisor::undefined	: return 0_Hz;
    }

    return 0_Hz;
}


template<uint32_t clock_frequency_in_hz>
inline Frequency Timer0::clock_frequency()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_frequency_in_Hz_1MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_frequency: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}

// Modos de funcionamiento
// -----------------------
inline void Timer0::mode_normal()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,0>::in(TCCR0A);
}

inline void Timer0::mode_PWM_phase_correct_top_0xFF()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,1>::in(TCCR0A);
}

inline void Timer0::mode_CTC()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,0>::in(TCCR0A);
}

inline void Timer0::mode_fast_PWM_top_0xFF()
{
    atd::write_bits<WGM02>::to<0>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,1>::in(TCCR0A);
}

inline void Timer0::mode_PWM_phase_correct_top_OCRA()
{
    atd::write_bits<WGM02>::to<1>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<0,1>::in(TCCR0A);
}

inline void Timer0::mode_fast_PWM_top_OCRA()
{
    atd::write_bits<WGM02>::to<1>::in(TCCR0B);
    atd::write_bits<WGM01, WGM00>::to<1,1>::in(TCCR0A);
}


// pins
// ----
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
{
    atd::write_bits<TOIE0>::to<1>::in(TIMSK0);
    avr::enable_all_interrupts();
}


inline void Timer0::enable_output_compare_A_match_interrupt()
{
    atd::write_bits<OCIE0A>::to<1>::in(TIMSK0);
    avr::enable_all_interrupts();
}


inline void Timer0::enable_output_compare_B_match_interrupt()
{
    atd::write_bits<OCIE0B>::to<1>::in(TIMSK0);
    avr::enable_all_interrupts();
}


}// namespace avr


#endif

