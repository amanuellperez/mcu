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
 *	
 *  TODO: faltan 3 modos de funcionamiento:
 *	+ El de capturar en el ICR.
 *	+ El phase correct mode.
 *	+ El phase and frequency correct.
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
    using counter_type = TIMER1_counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom()
    {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr uint8_t num_pin_A() {return TIMER1_num_pin_A; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr uint8_t num_pin_B() {return TIMER1_num_pin_B;}


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
    static void clock_speed_no_preescaling();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_256();
    static void clock_frequency_divide_by_1024();

    /// Definimos el periodo del reloj que usa el timer.
    /// clock_frequency_in_hz = es la frecuencia del reloj del AVR.
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void set_clock_period_in_us();

    /// Devuelve el periodo del reloj que usa el timer.
    /// Devuelve 0, si está apagado o si está conectado al pin T1.
    template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static uint16_t clock_period_in_us();

    template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static Microsecond clock_period();


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

    static uint16_t clock_period_in_us_1MHz();

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

inline void Timer1::clock_speed_no_preescaling()
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
	clock_speed_no_preescaling();
    
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
inline uint16_t Timer1::clock_period_in_us_1MHz()
{
    switch(frequency_divisor()){
	case Frequency_divisor::no_preescaling	: return 1u;
	case Frequency_divisor::divide_by_8	: return 8u;
	case Frequency_divisor::divide_by_64	: return 64u;
	case Frequency_divisor::divide_by_256	: return 256u;
	case Frequency_divisor::divide_by_1024	: return 1024u;
	case Frequency_divisor::undefined	: return 0;
    }

    return 0;
}


// avr clock at 8MHz
// -----------------
// a 125 ns
//template<>
//inline void Timer1::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

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


template<uint32_t clock_frequency_in_hz>
inline uint16_t Timer1::clock_period_in_us()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_period_in_us_1MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
inline Microsecond Timer1::clock_period()
{
    return Microsecond{clock_period_in_us<clock_frequency_in_hz>()};
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



/*!
 *  \brief  Timer1 funcionando en normal mode.
 *
 */
class Timer1_normal_mode: public Timer1{
public:
    using counter_type = Timer1::counter_type;

    /// Encendemos el Timer1
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on();

    /// Ponemos el contador a cero.
    static void reset();


private:
    static void set_normal_mode();
    static void pin_AB_disconnected();
};


template<uint16_t period
	, uint32_t clock_frequency_in_hz>
inline void Timer1_normal_mode::on()
{
    Interrupts_lock l;

    set_normal_mode();
    pin_AB_disconnected();
    Timer1::on<period, clock_frequency_in_hz>();
}


// Modo de funcionamiento del timer: normal. Cuenta de 0 a 0xFFFF
// y vuelve a empezar.
inline void Timer1_normal_mode::set_normal_mode()
{
//    Interrupts_lock l; Son de 8 bits, no es necesario bloquearlo

    atd::write_bits<WGM10, WGM11>::to<0,0>::in(TCCR1A);
    atd::write_bits<WGM12, WGM13>::to<0,0>::in(TCCR1B);
}

inline void Timer1_normal_mode::pin_AB_disconnected()
{   // 00
    atd::write_bits<COM1A1, COM1A0, COM1B1, COM1B0>
	       ::to<0,0,0,0>::in(TCCR1A);
}



inline void Timer1_normal_mode::reset()
{
    Interrupts_lock l; 
    TCNT1 = 0; 
}


//

/*!
 *  \brief  CTC mode. 
 *
 *
 *  Una de las utilidades de este modo es generar una onda cuadrada en los
 *  pines A y B. Tal como funciona el Timer1 genera la misma señal en ambos
 *  pines.
 *
 *  Ver el test como ejemplo.
 */
class Timer1_CTC_mode:public Timer1{
public:
    using counter_type = Timer1::counter_type;

    // Configuración del CTC.
    // 1.- ¿Dónde anotamos el valor del TOP? ¿En OCRA o en ICR?
    // Recordar definir el valor de OCR1A/ICR elegido.
    /// Define el valor del TOP como el indicado en OCR1A.
    /// El TCNT va desde 0 hasta el valor escrito en OCR1A.
    static void top_OCRA(const counter_type& top0);

    /// Define el valor del TOP como el indicado en ICR.
    /// El TCNT va desde 0 hasta el valor escrito en ICR.
    static void top_ICR(const counter_type& top0);

    // 2.- ¿Qué generar en los pines A/B cuando sucede un compare match? 
    // ¿Una onda cuadrada (toggle)?  ¿set/clear el pin?
    static void pin_A_disconnected();
    static void pin_A_toggle_on_compare_match();
    static void pin_A_set_on_compare_match();
    static void pin_A_clear_on_compare_match();

    static void pin_B_disconnected();
    static void pin_B_toggle_on_compare_match();
    static void pin_B_set_on_compare_match();
    static void pin_B_clear_on_compare_match();

    // 3.- Recordar encender el Timer (llamar a on).

};



inline void Timer1_CTC_mode::top_OCRA(const Timer1::counter_type& top0)
{
    // 0100
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);

    output_compare_register_A(top0);
}


inline void Timer1_CTC_mode::top_ICR(const Timer1::counter_type& top0)
{
    // 1100
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);

    input_capture_register(top0);
}


// Table 20-3.
inline void Timer1_CTC_mode::pin_A_disconnected()
{
    atd::write_bits<COM1A1, COM1A0>::to<0,0>::in(TCCR1A);
    // (???) ¿En qué estado dejar el puerto pin_A()?
}



inline void Timer1_CTC_mode::pin_A_toggle_on_compare_match()
{   // 01
    atd::write_bits<COM1A1, COM1A0>::to<0,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}

inline void Timer1_CTC_mode::pin_A_clear_on_compare_match()
{   // 10
    atd::write_bits<COM1A1, COM1A0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}


inline void Timer1_CTC_mode::pin_A_set_on_compare_match()
{   // 11
    atd::write_bits<COM1A1, COM1A0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}


inline void Timer1_CTC_mode::pin_B_disconnected()
{
    atd::write_bits<COM1B1, COM1B0>::to<0,0>::in(TCCR1A);
    // (???) ¿En qué estado dejar el puerto pin_B()?
}

inline void Timer1_CTC_mode::pin_B_toggle_on_compare_match()
{   // 01
    atd::write_bits<COM1B1, COM1B0>::to<0,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}

inline void Timer1_CTC_mode::pin_B_clear_on_compare_match()
{   // 10
    atd::write_bits<COM1B1, COM1B0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}


inline void Timer1_CTC_mode::pin_B_set_on_compare_match()
{   // 11
    atd::write_bits<COM1B1, COM1B0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}



/*!
 *  \brief  Hacemos que el Timer 1 funcione en el modo fast PWM.
 *
 *
 */
class Timer1_fast_PWM:public Timer1{
public:
    using counter_type = Timer1::counter_type;

    // Configuración del Fast PWM

    // 1.- ¿Qué valor usamos como TOP?

    /// El contador va desde 0 hasta 0x00FF. (top = 0x00FF)
    static void top_0x00FF();

    /// El contador va desde 0 hasta 0x01FF. (top = 0x01FF)
    static void top_0x01FF();

    /// El contador va desde 0 hasta 0x03FF. (top = 0x03FF)
    static void top_0x03FF();

    /// El contador va desde 0 hasta el valor escrito en el comparador A.
    /// OJO: no tengo claro como funciona este modo, pero recordar que la
    /// señal que se genera en el pin A procede de comparar con el comparador A.
    /// Pero si usamos este como TOP, no vamos a generar una PWM. Asi que, en
    /// general, no llamar a esta función. Usar mejor el ICR o los valores
    /// fijos de top.
    static void top_OCRA(counter_type top0);

    /// El contador va desde 0 hasta el valor escrito en ICR.
    static void top_ICR(counter_type top0);


    // 2.- ¿Qué generar en los pines A/B?

    /// Este modo de funcionamiento creo que genera una onda cuadrada. ¿Por
    /// qué aparece aquí? Ni idea. Ver pag. 170 datasheet.
//    static void toggle_pin_A_on_compare_match_pin_B_disconnected();

    /// Generamos en el pin A una señal PWM.
    /// En este modo si ponemos 300 en el comparador A y ICR = 1000, 
    /// el duty cycle es del 30% (suponiendo haber llamado a top_ICR())
    static void pin_A_non_inverting_mode();

    /// Generamos en el pin A una señal PWM.
    /// En este modo si ponemos 300 en el comparador A y ICR = 1000, 
    /// el duty cycle es del 70% (suponiendo haber llamado a top_ICR())
    static void pin_A_inverting_mode();

    /// Generamos en el pin B una señal PWM.
    /// En este modo si ponemos 300 en el comparador A y ICR = 1000, 
    /// el duty cycle es del 30% (suponiendo haber llamado a top_ICR())
    static void pin_B_non_inverting_mode();

    /// Generamos en el pin A una señal PWM.
    /// En este modo si ponemos 300 en el comparador A y ICR = 1000, 
    /// el duty cycle es del 70% (suponiendo haber llamado a top_ICR())
    static void pin_B_inverting_mode();

    // 3.- Que no se olvide encender el Timer llamando a la función on().
};


inline void Timer1_fast_PWM::top_0x00FF()
{
    // 0101
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);
}


inline void Timer1_fast_PWM::top_0x01FF()
{
    // 0110
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1_fast_PWM::top_0x03FF()
{
    // 0111
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
    
}

inline void Timer1_fast_PWM::top_OCRA(Timer1::counter_type top0)
{
    // 1111
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);

    output_compare_register_A(top0);
}

inline void Timer1_fast_PWM::top_ICR(Timer1::counter_type top0)
{
    // 1110
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
    
    input_capture_register(top0);
}


//inline void Timer1_fast_PWM::toggle_pin_A_on_compare_match_pin_B_disconnected()
//{   // 01
//    atd::Register(TCCR1A).write_zero_bit<COM1A1>();
//    atd::Register(TCCR1A).write_one_bit <COM1A0>();
//}

inline void Timer1_fast_PWM::pin_A_non_inverting_mode()
{   // 10
    atd::write_bits<COM1A1, COM1A0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}

inline void Timer1_fast_PWM::pin_A_inverting_mode()
{   // 11
    atd::write_bits<COM1A1, COM1A0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}


inline void Timer1_fast_PWM::pin_B_non_inverting_mode()
{   // 10
    atd::write_bits<COM1B1, COM1B0>::to<1,0>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}

inline void Timer1_fast_PWM::pin_B_inverting_mode()
{   // 11
    atd::write_bits<COM1B1, COM1B0>::to<1,1>::in(TCCR1A);

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}



}// namespace

#endif


