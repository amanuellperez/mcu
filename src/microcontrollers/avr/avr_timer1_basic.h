// Copyright (C) 2019-2022 Manuel Perez 
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

#ifndef __AVR_TIMER1_BASIC_H__
#define __AVR_TIMER1_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del timer 1 del AVR. 
 *   
 *  - HISTORIA:
 *   Manuel Perez
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
 *    07/11/2022  disable_..._interrupt()
 *		  Y no era un traductor puro, ya que al activar las
 *		  interrupciones también activaba las interrupciones
 *		  del avr, cosa que no viene en la datasheet. Recordar
 *		  que un traductor se limita a traducir la datasheet
 *		  sin añadir/quitar nada (no cierto del todo ya que
 *		  aquí estoy añadiendo la posibilidad de pasar la
 *		  frecuencia de trabajo del Timer en lugar de usar los
 *		  divisores de frecuencia).
 *
 *    08/11/2022  Lo dejo como traductor puro. 
 *		  Para simplificar el uso estaba bloqueando las
 *		  interrupciones al acceder a los registros de 16 bits.
 *		  Sin embargo, eso hace que la lectura se ralentice, afectando
 *		  la eficiencia del programa. Además, en la datasheet habla de
 *		  que hay que bloquear las interrupciones: esto es un
 *		  traductor, NO tiene que cambiar la funcionalidad. Da la
 *		  impresión de que es mejor bloquear las interrupciones en el
 *		  Generic_timer.
 *
 *     08/07/2024 mode(), prescaler()
 *
 ****************************************************************************/
#include <limits>
#include <array>

#include <atd_bit.h>
#include <atd_type_traits.h>

#include <avr/io.h> // registros: DDRB... PORT...
#include "avr_pin.h"
#include "avr_cfg.h"	// clock_frequency_in_hz

namespace avr_{
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
    using counter_type = cfg::timer1::counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom()
    {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr uint8_t OCA_pin() {return cfg::timer1::OCA_pin; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr uint8_t OCB_pin() {return cfg::timer1::OCB_pin;}


// CONFIGURACIÓN DEL RELOJ
    // (RRR) ¿por qué definir explícitamente aquí los prescaler_factor?
    //       Los podía definir en cfg::timer0, pero la implementación de esta
    //       clase conoce los prescalers en las funciones
    //       clock_frequency_no_preescaling, ... O se generalizan esas
    //       funciones metiendolas en cfg::timer0 o no mejor hacerlo todo
    //       concreto.
    //
    // Array con los posibles prescaler factors del timer.
    static constexpr std::array<uint16_t, 5> 
				prescaler_factor = {1, 8, 64, 256, 1024};

    enum class Frequency_divisor{
		    no_clock_source,    
		    off = no_clock_source,
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

    // Devuelve el prescaler correspondiente
    static uint16_t prescaler();

    /// Seleccionamos la frecuencia a la que funciona el timer usando el
    /// divisor de frecuencias (prescaler_factor)
    /// Enciende el timer (si prescaler_factor != 0) o lo apaga (si es == 0)
    static void prescaler(uint16_t prescaler_factor);

    /// Frecuencia a la que funciona internamente el timer.
    /// Se cumple que clock_frequency() = 1 / clock_period();
    template <uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static Frequency clock_frequency();


// ENCENDIDO/APAGADO DEL TIMER
    /// Paramos el timer.
    static void off();

    static bool is_on();
    static bool is_off();


// SELECCIÓN DE RELOJ EXTERNO
    /// External clock source on T1 pin. Clock on falling edge.
    static void external_clock_falling_edge();

    /// External clock source on T1 pin. Clock on rising edge.
    static void external_clock_rising_edge();


// ACCESO A REGISTROS
// (RRR) ¿Por qué llamarlos unsafe_?
//       Por una parte quiero que sea un traductor puro ==> no debo de
//       bloquear las interrupciones cuando lea los registros de 16 bits.
//       Por otra parte dentro de 6 meses cuando quiera usar este Timer no voy
//       a recordar en qué funciones tengo que bloquear las interrupciones
//       antes de llamarlas: necesito marcar todas estas funciones. Por ello,
//       pruebo a llamarlas `unsafe_` para saber que tengo que bloquear las
//       interrupciones obligatoriamente antes de llamarlas.
    /// Importante: según la datasheet, pag. 153,  al leer los uint16_t 
    /// hay que bloquear las interrupciones. Recordar bloquearlos!!!
    static counter_type unsafe_counter();	/// Lectura del counter.
    static void unsafe_counter(counter_type x);	/// Escritura del counter.

    // Bloquear las interrupciones antes de leer/escribir estos registros
    static counter_type unsafe_output_compare_register_A();      /// read OCRA
    static void unsafe_output_compare_register_A(counter_type x);/// write OCRA

    // Bloquear las interrupciones antes de leer/escribir estos registros
    static counter_type unsafe_output_compare_register_B();      /// read OCRB
    static void unsafe_output_compare_register_B(counter_type x);/// write OCRB

    // Bloquear las interrupciones antes de leer/escribir estos registros
    static counter_type unsafe_input_capture_register();	 /// Lectura del ICR.
    static void unsafe_input_capture_register(counter_type x);/// Escritura del ICR.

// WAVEFORM GENERATION MODES (table 20-6)
    enum class Mode{
	normal,
	CTC_top_OCRA,
	CTC_top_ICR,
	fast_PWM_top_0x00FF,
	fast_PWM_top_0x01FF,
	fast_PWM_top_0x03FF,
	fast_PWM_top_ICR,
	fast_PWM_top_OCRA,
	PWM_phase_correct_top_0x00FF,
	PWM_phase_correct_top_0x01FF,
	PWM_phase_correct_top_0x03FF,
	PWM_phase_correct_top_ICR,
	PWM_phase_correct_top_OCRA,
	PWM_phase_and_frequency_correct_top_ICR,
	PWM_phase_and_frequency_correct_top_OCRA,
	reserved 
    };

    static Mode mode();

// Normal mode
    static void normal_mode();

// CTC modes
    static void CTC_mode_top_OCRA();
    static void CTC_mode_top_ICR();

// Fast PWM modes
    /// El contador va desde 0 hasta 0x00FF. (top = 0x00FF)
    static void fast_PWM_mode_top_0x00FF();

    /// El contador va desde 0 hasta 0x01FF. (top = 0x01FF)
    static void fast_PWM_mode_top_0x01FF();

    /// El contador va desde 0 hasta 0x03FF. (top = 0x03FF)
    static void fast_PWM_mode_top_0x03FF();

    static void fast_PWM_mode_top_ICR();
    static void fast_PWM_mode_top_OCRA();

// PWM phase correct modes
    static void PWM_phase_correct_mode_top_0x00FF();
    static void PWM_phase_correct_mode_top_0x01FF();
    static void PWM_phase_correct_mode_top_0x03FF();
    static void PWM_phase_correct_mode_top_ICR();
    static void PWM_phase_correct_mode_top_OCRA();

// PWM phase and frequency correct modes
    static void PWM_phase_and_frequency_correct_mode_top_ICR();
    static void PWM_phase_and_frequency_correct_mode_top_OCRA();


// pins operation 
    // comunes a todas las tablas (20-3,4 y 5)
    static void pin_A_disconnected();
    static void pin_B_disconnected();

    static bool is_pin_A_disconnected();
    static bool is_pin_A_connected();

    static bool is_pin_B_disconnected();
    static bool is_pin_B_connected();

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
//  Recordar llamar a avr::enable_interrupts para que funcionen.

    /// Cuando se produce un overflow generamos la interrupción
    /// correspondiente. Se captura con ISR_TIMER1_OVF
    static void enable_overflow_interrupt();
    static void disable_overflow_interrupt();

    /// Se captura con ISR_TIMER1_CAPT
    static void enable_input_capture_interrupt();
    static void disable_input_capture_interrupt();

    /// Se captura con ISR_TIMER1_COMPA
    static void enable_output_compare_A_match_interrupt();
    static void disable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER1_COMPB
    static void enable_output_compare_B_match_interrupt();
    static void disable_output_compare_B_match_interrupt();

private:

}; // Timer1

inline bool Timer1::is_on()
{return !is_off();}

inline bool Timer1::is_off()
{ return prescaler() == 0; }

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


// Devuelve el valor del contador.
// Importante: según la datasheet, pag. 153,  al leer los uint16_t 
// hay que bloquear las interrupciones. Por eso el lock.  
inline Timer1::counter_type Timer1::unsafe_counter() 
{ return TCNT1; }

inline void Timer1::unsafe_counter(Timer1::counter_type x) 
{ TCNT1 = x; }


inline Timer1::counter_type Timer1::unsafe_output_compare_register_A() 
{ return OCR1A; }

// Escribimos x en el comparador A
inline void Timer1::unsafe_output_compare_register_A(Timer1::counter_type x)
{ OCR1A = x; }


inline Timer1::counter_type Timer1::unsafe_output_compare_register_B() 
{ return OCR1B; }

// Escribimos x en el comparador B
inline void Timer1::unsafe_output_compare_register_B(Timer1::counter_type x)
{ OCR1B = x; }


inline Timer1::counter_type Timer1::unsafe_input_capture_register() 
{ return ICR1; }

// Escribimos x en el comparador B
inline void Timer1::unsafe_input_capture_register(Timer1::counter_type x)
{ ICR1 = x; }


inline void Timer1::enable_overflow_interrupt()
{ atd::write_bits<TOIE1>::to<1>::in(TIMSK1); }

inline void Timer1::disable_overflow_interrupt()
{ atd::write_bits<TOIE1>::to<0>::in(TIMSK1); }

inline void Timer1::enable_input_capture_interrupt()
{ atd::write_bits<ICIE1>::to<1>::in(TIMSK1); }

inline void Timer1::disable_input_capture_interrupt()
{ atd::write_bits<ICIE1>::to<0>::in(TIMSK1); }

inline void Timer1::enable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE1A>::to<1>::in(TIMSK1); }

inline void Timer1::disable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE1A>::to<0>::in(TIMSK1); }

inline void Timer1::enable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE1B>::to<1>::in(TIMSK1); }

inline void Timer1::disable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE1B>::to<0>::in(TIMSK1); }


// Modos de funcionamiento
// -----------------------
inline void Timer1::normal_mode()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::PWM_phase_correct_mode_top_0x00FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);

}

inline void Timer1::PWM_phase_correct_mode_top_0x01FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::PWM_phase_correct_mode_top_0x03FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::CTC_mode_top_OCRA()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::fast_PWM_mode_top_0x00FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);
}

inline void Timer1::fast_PWM_mode_top_0x01FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::fast_PWM_mode_top_0x03FF()
{
    atd::write_bits<WGM13, WGM12>::to<0,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::PWM_phase_and_frequency_correct_mode_top_ICR()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::PWM_phase_and_frequency_correct_mode_top_OCRA()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,1>::in(TCCR1A);
}

inline void Timer1::PWM_phase_correct_mode_top_ICR()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::PWM_phase_correct_mode_top_OCRA()
{
    atd::write_bits<WGM13, WGM12>::to<1,0>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}

inline void Timer1::CTC_mode_top_ICR()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<0,0>::in(TCCR1A);
}

inline void Timer1::fast_PWM_mode_top_ICR()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,0>::in(TCCR1A);
}

inline void Timer1::fast_PWM_mode_top_OCRA()
{
    atd::write_bits<WGM13, WGM12>::to<1,1>::in(TCCR1B);
    atd::write_bits<WGM11, WGM10>::to<1,1>::in(TCCR1A);
}



// pins
// ----
inline bool Timer1::is_pin_A_connected()
{return !is_pin_A_disconnected(); }

inline bool Timer1::is_pin_A_disconnected()
{
    return atd::read_bits<COM1A1, COM1A0>::of(TCCR1A) ==
	atd::zero<uint8_t>::with_bits<COM1A1, COM1A0>::to<0,0>();
}

inline bool Timer1::is_pin_B_connected()
{return !is_pin_B_disconnected(); }


inline bool Timer1::is_pin_B_disconnected()
{
    return atd::read_bits<COM1B1, COM1B0>::of(TCCR1A) ==
	atd::zero<uint8_t>::with_bits<COM1B1, COM1B0>::to<0,0>();
}

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


