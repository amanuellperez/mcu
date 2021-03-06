// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_TIMER1_BASIC_H__
#define __AVR_TIMER1_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del timer 1 del AVR. 
 *   
 *  - HISTORIA:
 *   A.Manuel L.Perez
 *     05/08/2017 Escrita v0.0
 *     18/07/2019 v0.1 Reescrito.
 *     03/01/2020 Que de un mensaje legible el compilador 
 *		  si el periodo o la frecuencia no es correcta.
 *     01/02/2021 frequency_divisor, clock_period_in_us
 *     06/02/2021 v0.2. Reestructurado: lo escribo como traductor puro.
 *                Al principio había creado distintas clases Timer1_CTC_mode,
 *                Timer1_normal_mode... para separar los diferentes modos de
 *                funcionamiento del timer. Sin embargo, si se quiere hacer un
 *                generador de señales, este va a usar funciones de CTC_mode y
 *                de PWM_mode con lo que no puedo usar 2 clases diferentes.
 *                Además, al dejarlo como traductor es más sencillo de
 *                implementar: basta leer la datasheet e ir traduciéndola a
 *                código.
 *                
 *	
 *  DUDA: meter todas las funciones dentro de un .cpp (???)
 *
 ****************************************************************************/
#include <limits>

#include <atd_bit.h>
#include <atd_type_traits.h>

#include "avr_interrupt.h"
#include "avr_cfg.h"
#include "avr_pin.h"

namespace avr{
/*!
 *  \brief  Funciones genéricas del Timer1.
 *
 */

// ¿Qué diferencia Timer0 del Timer1 y qué cosas hay que configurar?
// 1. Tamaño de los registros:
//		Timer 1
//	TCNT	16 bits
//	OCRA	16 bits
//	OCRB	16 bits
//	ICR	16 bits
//
//	TCCRA	 8 bits
//	TCCRB	 8 bits
//  
//	Observar que puesto que TCNT se va a comparar con OCRA, OCRB e ICR es
//	obligatorio que todos estos registros sean del mismo tamaño que
//	llamaremos counter_type.
//
// 2. Posiciones de los pines dentro del avr:
//	pin de entrada: T0, T1 y ICP.	
//	    Observar que puesto que TCNT se va a comparar con OCRA, OCRB e ICR
//	    es obligatorio que todos estos registros sean del mismo tamaño que
//	    llamaremos counter_type.
//
//	pines de salida: A y B.
//
class Timer1{
public:
// CARACTERÍSTICAS DEL TIMER
    using counter_type = TIMER1::counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom()
    {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr uint8_t OCA_pin() {return TIMER1::OCA_pin; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr uint8_t OCB_pin() {return TIMER1::OCB_pin;}


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

    // Establecemos el divisor de frecuencia a aplicar al reloj del micro.
    static void clock_frequency_no_preescaling();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_256();
    static void clock_frequency_divide_by_1024();

    /// Definimos el periodo del reloj que usa el timer.
    /// clock_frequency_in_hz = es la frecuencia del reloj del AVR.
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void set_clock_period_in_us();

    // Si la frecuencia del micro es 8MHz puede funcionar a 125 ns
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void set_clock_period_in_ns();

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
    // DUDA: a la hora de usarlo 'on' es confuso. Es mejor llamar directamente
    // a set_clock_period_in_us. Además encaja mejor si operamos a 8MHz y
    // queremos llamar a set_clock_period_in_ns. ¿Eliminar esta función?
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {set_clock_period_in_us<period, clock_frequency_in_hz>();}

    /// Paramos el timer.
    static void off();


// SELECCIÓN DE RELOJ EXTERNO
    /// External clock source on T1 pin. Clock on falling edge.
    static void external_clock_falling_edge();

    /// External clock source on T1 pin. Clock on rising edge.
    static void external_clock_rising_edge();


// ACCESO A REGISTROS
    static counter_type counter();		/// Lectura del counter.
    static void counter(counter_type x);	/// Escritura del counter.

    static counter_type output_compare_register_A();      /// read OCRA
    static void output_compare_register_A(counter_type x);/// write OCRA

    static counter_type output_compare_register_B();      /// read OCRB
    static void output_compare_register_B(counter_type x);/// write OCRB

    static counter_type input_capture_register();	 /// Lectura del ICR.
    static void input_capture_register(counter_type x);/// Escritura del ICR.

// WAVEFORM GENERATION MODES (table 20-6)
// normal mode
    static void mode_normal();

// CTC modes
    static void mode_CTC_top_OCR1A();
    static void mode_CTC_top_ICR1();

// Fast PWM modes
    /// El contador va desde 0 hasta 0x00FF. (top = 0x00FF)
    static void mode_fast_PWM_top_0x00FF();

    /// El contador va desde 0 hasta 0x01FF. (top = 0x01FF)
    static void mode_fast_PWM_top_0x01FF();

    /// El contador va desde 0 hasta 0x03FF. (top = 0x03FF)
    static void mode_fast_PWM_top_0x03FF();

    static void mode_fast_PWM_top_ICR1();
    static void mode_fast_PWM_top_OCR1A();

// PWM phase correct modes
    static void mode_PWM_phase_correct_top_0x00FF();
    static void mode_PWM_phase_correct_top_0x01FF();
    static void mode_PWM_phase_correct_top_0x03FF();
    static void mode_PWM_phase_correct_top_ICR1();
    static void mode_PWM_phase_correct_top_OCR1A();

// PWM phase and frequency correct modes
    static void mode_PWM_phase_and_frequency_correct_top_ICR1();
    static void mode_PWM_phase_and_frequency_correct_top_OCR1A();


// pins operation 
    // comunes a todas las tablas (20-3,4 y 5)
    static void pin_A_disconnected();
    static void pin_B_disconnected();

    // comunes a PWM
    static void PWM_pin_A_non_inverting_mode();
    static void PWM_pin_A_inverting_mode();
    static void PWM_pin_B_non_inverting_mode();
    static void PWM_pin_B_inverting_mode();

    // table 20-3
    static void CTC_pin_A_toggle_on_compare_match();
    static void CTC_pin_A_clear_on_compare_match();
    static void CTC_pin_A_set_on_compare_match();

    static void CTC_pin_B_toggle_on_compare_match();
    static void CTC_pin_B_clear_on_compare_match();
    static void CTC_pin_B_set_on_compare_match();

    // table 20-4
    // Only for mode_fast_PWM_top_ICR1() and mode_fast_PWM_top_OCR1A():
    // Toggle OC1A on compare match, OC1B disconnected.
    static void fast_PWM_pin_A_toggle_on_compare_match();

    // static void fast_PWM_pin_B_toggle_on_compare_match(); <--- ???

    // Table 20-5, Phase correct and phase and frequency correct PWM
    // Only for: PWM, phase and frequency correct top OCR1A, and PWM, phase
    // correct top OCR1A.
    // Toggle OC1A on compare match, OC1B disconnected.
    static void phase_PWM_pin_A_toggle_on_compare_match();
    // static void phase_PWM_pin_B_toggle_on_compare_match();<--- ???


// INTERRUPCIONES
    /// Cuando se produce un overflow generamos la interrupción
    /// correspondiente. Se captura con ISR_TIMER1_OVF
    static void enable_overflow_interrupt();

    /// Se captura con ISR_TIMER1_CAPT
    static void enable_input_capture_interrupt();

    /// Se captura con ISR_TIMER1_COMPA
    static void enable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER1_COMPB
    static void enable_output_compare_B_match_interrupt();

private:
    template<uint16_t period>
    static void set_clock_period_in_us_1MHz();

    template<uint16_t period>
    static void set_clock_period_in_us_8MHz();

    static Time clock_period_in_us_1MHz();
    static Frequency clock_frequency_in_Hz_1MHz();

}; // Timer1


inline Timer1::Frequency_divisor Timer1::frequency_divisor()
{ 
    switch(atd::read_bits<CS12, CS11, CS10>::of(TCCR1B)){
	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,0,1>():
	    return Frequency_divisor::no_preescaling;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,1,0>():
	    return Frequency_divisor::divide_by_8;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,1,1>():
	    return Frequency_divisor::divide_by_64;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<1,0,0>():
	    return Frequency_divisor::divide_by_256;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<1,0,1>():
	    return Frequency_divisor::divide_by_1024;
    }

    return Frequency_divisor::undefined;
}


inline void Timer1::off()
{ // 000
    atd::write_bits<CS12, CS11, CS10>::to<0,0,0>::in(TCCR1B);
}

inline void Timer1::clock_frequency_no_preescaling()
{ // 001
    atd::write_bits<CS12, CS11, CS10>::to<0,0,1>::in(TCCR1B);
}
    
inline void Timer1::clock_frequency_divide_by_8()
{// 010
    atd::write_bits<CS12, CS11, CS10>::to<0,1,0>::in(TCCR1B);
}

inline void Timer1::clock_frequency_divide_by_64()
{// 011
    atd::write_bits<CS12, CS11, CS10>::to<0,1,1>::in(TCCR1B);
}

inline void Timer1::clock_frequency_divide_by_256()
{// 100
    atd::write_bits<CS12, CS11, CS10>::to<1,0,0>::in(TCCR1B);
}

inline void Timer1::clock_frequency_divide_by_1024()
{// 101
    atd::write_bits<CS12, CS11, CS10>::to<1,0,1>::in(TCCR1B);
}

inline void Timer1::external_clock_falling_edge()
{// 110
    atd::write_bits<CS12, CS11, CS10>::to<1,1,0>::in(TCCR1B);
}

inline void Timer1::external_clock_rising_edge()
{// 111
    atd::write_bits<CS12, CS11, CS10>::to<1,1,1>::in(TCCR1B);
}



// avr clock at 1MHz
// -----------------
template<uint16_t period>
inline void Timer1::set_clock_period_in_us_1MHz() 
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
		    "Incorrect Timer1 period. Try another one.");
}

// TODO: a .cpp???
inline Time Timer1::clock_period_in_us_1MHz()
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

inline Frequency Timer1::clock_frequency_in_Hz_1MHz()
{
    using namespace literals;
    using Rep = Frequency::Rep;
    switch(frequency_divisor()){
	case Frequency_divisor::no_preescaling	: return 1_MHz;
	case Frequency_divisor::divide_by_8	: return 125_kHz;
	case Frequency_divisor::divide_by_64	: return Frequency{15625ul, 0};
	case Frequency_divisor::divide_by_256	: return Frequency{Rep{3906ul,25ul}, 0};
	case Frequency_divisor::divide_by_1024	: return Frequency{Rep{976ul,56ul}, 0};
	case Frequency_divisor::undefined	: return 0_Hz;
    }

    return 0_Hz;
}


// avr clock at 8MHz
// -----------------
// a 125 ns
template<>
inline void Timer1::set_clock_period_in_ns<125u, 8000000UL>() 
{clock_frequency_no_preescaling();}

template<uint16_t period>
inline void Timer1::set_clock_period_in_us_8MHz() 
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
		    "Incorrect Timer1 period. Try another one.");
}



template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
inline Time Timer1::clock_period()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_period_in_us_1MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


template<uint16_t period
	, uint32_t clock_frequency_in_hz>
inline void Timer1::set_clock_period_in_us()
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



template<uint32_t clock_frequency_in_hz>
inline Frequency Timer1::clock_frequency()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_frequency_in_Hz_1MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_frequency: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



// Devuelve el valor del contador.
// Importante: según la datasheet, pag. 153,  al leer los uint16_t 
// hay que bloquear las interrupciones. Por eso el lock.  
inline Timer1::counter_type Timer1::counter() 
{
    Interrupts_lock l;
    auto res = TCNT1;

    return res;
}

inline void Timer1::counter(Timer1::counter_type x) 
{
    Interrupts_lock l;
    TCNT1 = x;
}


inline Timer1::counter_type Timer1::output_compare_register_A() 
{
    Interrupts_lock l;
    auto res = OCR1A;

    return res;
}

// Escribimos x en el comparador A
inline void Timer1::output_compare_register_A(Timer1::counter_type x)
{
    Interrupts_lock l;
    OCR1A = x;
}


inline Timer1::counter_type Timer1::output_compare_register_B() 
{
    Interrupts_lock l;
    auto res = OCR1B;

    return res;
}

// Escribimos x en el comparador B
inline void Timer1::output_compare_register_B(Timer1::counter_type x)
{
    Interrupts_lock l;
    OCR1B = x;
}


inline Timer1::counter_type Timer1::input_capture_register() 
{
    Interrupts_lock l;
    auto res = ICR1;

    return res;
}

// Escribimos x en el comparador B
inline void Timer1::input_capture_register(Timer1::counter_type x)
{
    Interrupts_lock l;
    ICR1 = x;
}


inline void Timer1::enable_overflow_interrupt()
{
    atd::write_bits<TOIE1>::to<1>::in(TIMSK1);
    avr::enable_all_interrupts();
}

inline void Timer1::enable_input_capture_interrupt()
{
    atd::write_bits<ICIE1>::to<1>::in(TIMSK1);
    avr::enable_all_interrupts();
}


inline void Timer1::enable_output_compare_A_match_interrupt()
{
    atd::write_bits<OCIE1A>::to<1>::in(TIMSK1);
    avr::enable_all_interrupts();
}


inline void Timer1::enable_output_compare_B_match_interrupt()
{
    atd::write_bits<OCIE1B>::to<1>::in(TIMSK1);
    avr::enable_all_interrupts();
}


// Modos de funcionamiento
// -----------------------
inline void Timer1::mode_normal()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_correct_top_0x00FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);

}

inline void Timer1::mode_PWM_phase_correct_top_0x01FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_correct_top_0x03FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::mode_CTC_top_OCR1A()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::mode_fast_PWM_top_0x00FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);
}

inline void Timer1::mode_fast_PWM_top_0x01FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::mode_fast_PWM_top_0x03FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_and_frequency_correct_top_ICR1()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_and_frequency_correct_top_OCR1A()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_correct_top_ICR1()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::mode_PWM_phase_correct_top_OCR1A()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::mode_CTC_top_ICR1()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::mode_fast_PWM_top_ICR1()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::mode_fast_PWM_top_OCR1A()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}


// pins
// ----
inline void Timer1::pin_A_disconnected()
{
    atd::write_bits<COM1A1, COM1A0>::to<0,0>::in(TCCR1A);
    // (???) ¿En qué estado dejar el puerto pin_A()?
}

inline void Timer1::pin_B_disconnected()
{
    atd::write_bits<COM1B1, COM1B0>::to<0,0>::in(TCCR1A);
    // (???) ¿En qué estado dejar el puerto pin_B()?
}

inline void Timer1::fast_PWM_pin_A_toggle_on_compare_match()
{   // 01
    atd::write_bits<COM1A1, COM1A0>::to<0,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer1::PWM_pin_A_non_inverting_mode()
{   // 10
    atd::write_bits<COM1A1, COM1A0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer1::PWM_pin_A_inverting_mode()
{   // 11
    atd::write_bits<COM1A1, COM1A0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer1::PWM_pin_B_non_inverting_mode()
{   // 10
    atd::write_bits<COM1B1, COM1B0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}

inline void Timer1::PWM_pin_B_inverting_mode()
{   // 11
    atd::write_bits<COM1B1, COM1B0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}



inline void Timer1::CTC_pin_A_toggle_on_compare_match()
{   // 01
    atd::write_bits<COM1A1, COM1A0>::to<0,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer1::CTC_pin_A_clear_on_compare_match()
{   // 10
    atd::write_bits<COM1A1, COM1A0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer1::CTC_pin_A_set_on_compare_match()
{   // 11
    atd::write_bits<COM1A1, COM1A0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer1::CTC_pin_B_toggle_on_compare_match()
{   // 01
    atd::write_bits<COM1B1, COM1B0>::to<0,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}

inline void Timer1::CTC_pin_B_clear_on_compare_match()
{   // 10
    atd::write_bits<COM1B1, COM1B0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer1::CTC_pin_B_set_on_compare_match()
{   // 11
    atd::write_bits<COM1B1, COM1B0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}

}// namespace avr




#endif


