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

#ifndef __AVR_TIMER0_BASIC_H__
#define __AVR_TIMER0_BASIC_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del timer 0 del AVR. Es un timer de 8 bits.
 *
 *  - TODO:
 *    Faltan implementar los modos:
 *	    * Fast PWM
 *	    * PWM, Phase correct
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *
 *      06/08/2017 v0.0
 *      18/07/2019 Reescrito.
 *      04/01/2020 Creado traductor.
 *
 ****************************************************************************/

#include <atd_register.h>
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
    using counter_type = TIMER0_counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom() {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr counter_type num_pin_A() {return TIMER0_num_pin_A; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr counter_type num_pin_B() {return TIMER0_num_pin_B;}


// CONFIGURACIÓN DEL RELOJ
    // Selección del reloj y de su velocidad (según tabla 19-10)
    /// No hacemos preescalado.
    static void clock_speed_no_preescaling();

    /// Dividimos la frecuencia del reloj entre 8.
    static void clock_frequency_entre_8();

    /// Dividimos la frecuencia del reloj entre 64.
    static void clock_frequency_entre_64();

    /// Dividimos la frecuencia del reloj entre 256.
    static void clock_frequency_entre_256();

    /// Dividimos la frecuencia del reloj entre 1024.
    static void clock_frequency_entre_1024();

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
    template<uint16_t periodo
	    , uint32_t clock_frecuencia_en_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void clock_period_in_us();

// ENCENDIDO/APAGADO DEL TIMER
    /// Enciende el Timer, usando como reloj el reloj de periodo indicado.
    /// 'periodo' es el periodo en microsegundos al que va a funcionar el timer.
    /// clock_frequency_in_hz = es la frecuencia del reloj del AVR.
    template<uint16_t periodo
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {clock_period_in_us<periodo, clock_frequency_in_hz>();}

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


// INTERRUPCIONES
    /// Se captura con ISR_TIMER0_OVF
    static void enable_overflow_interrupt();

    /// Se captura con ISR_TIMER0_COMPA
    static void enable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER0_COMPB
    static void enable_output_compare_B_match_interrupt();


private:
    template<uint16_t periodo>
    static void clock_period_in_us_1MHz();

    template<uint16_t periodo>
    static void clock_period_in_us_8MHz();

}; // Timer0





// avr clock at 1MHz
// -----------------
template<uint16_t periodo>
inline void Timer0::clock_period_in_us_1MHz() 
{
    if constexpr (periodo == 1u)
	clock_speed_no_preescaling();
    
    else if constexpr (periodo == 8u)
	clock_frequency_entre_8();

    else if constexpr (periodo == 64u)
	clock_frequency_entre_64();

    else if constexpr (periodo == 256u)
	clock_frequency_entre_256();

    else if constexpr (periodo == 1024u)
	clock_frequency_entre_1024();

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

template<uint16_t periodo>
inline void Timer0::clock_period_in_us_8MHz() 
{
    if constexpr (periodo == 1u)
	clock_frequency_entre_8();

    else if constexpr (periodo == 8u)
	clock_frequency_entre_64();

    else if constexpr (periodo == 32u)
	clock_frequency_entre_256();
 
    else if constexpr (periodo == 128u)
	clock_frequency_entre_1024();
 
    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer0 period. Try another one.");
}


template<uint16_t period
	, uint32_t clock_frequency_in_hz>
inline void Timer0::clock_period_in_us()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	clock_period_in_us_1MHz<period>();

    else if constexpr (clock_frequency_in_hz == 8000000UL)
	clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


/*!
 *  \brief  Timer0 in normal mode.
 *
 */
class Timer0_normal_mode : public Timer0 {
public:
    using counter_type = Timer0::counter_type;

    /// Encendemos el Timer0
    template<uint16_t periodo
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on();

    /// Ponemos el contador a cero.
    static void reset() { TCNT0 = 0; }


private:
    static void set_normal_mode();
    static void pin_AB_disconnected();
};



template<uint16_t periodo
	, uint32_t clock_frequency_in_hz>
inline void Timer0_normal_mode::on()
{
    set_normal_mode();
    pin_AB_disconnected();
    Timer0::on<periodo, clock_frequency_in_hz>();
}


inline void Timer0_normal_mode::set_normal_mode()
{
    atd::Register(TCCR0B).write_zero_bit<WGM02>	 ();
    atd::Register(TCCR0A).write_zero_bit<WGM01, WGM00>();
}


inline void Timer0_normal_mode::pin_AB_disconnected()
{   // 00
    atd::Register(TCCR0A).write_zero_bit <COM0A0, COM0A0>();
    atd::Register(TCCR0A).write_zero_bit <COM0B0, COM0B0>();
}



/*!
 *  \brief  CTC mode. 
 *
 *
 *  Una de las utilidades de este modo es generar una onda cuadrada en los
 *  pines A y B. Tal como funciona el Timer0 genera la misma señal en ambos
 *  pines.
 *
 *  Ver el test como ejemplo.
 */
class Timer0_CTC_mode:public Timer0 {
public:
    using counter_type = Timer0::counter_type;

    // Configuración del CTC.
    // 1.- Definir el top.
    /// El TCNT va desde 0 hasta el TOP.
    static void top_OCRA(const counter_type& top0);

    // 2.- ¿Qué generar en los pines A/B cuando sucede un compare match? 
    // ¿Una onda cuadrada (toggle)?  ¿set/clear el pin?
    static void pin_A_toggle_on_compare_match();
    static void pin_A_set_on_compare_match();
    static void pin_A_clear_on_compare_match();

    static void pin_B_toggle_on_compare_match();
    static void pin_B_set_on_compare_match();
    static void pin_B_clear_on_compare_match();

    // 3.- Recordar encender el Timer (llamar a on).

};



inline void Timer0_CTC_mode::top_OCRA(const Timer0::counter_type& top0)
{
    // 010
    atd::Register(TCCR0B).write_zero_bit <WGM02>();
    atd::Register(TCCR0A).write_one_bit<WGM01>();
    atd::Register(TCCR0A).write_zero_bit<WGM00>();

    output_compare_register_A(top0);
}



// Table 19-3.
inline void Timer0_CTC_mode::pin_A_toggle_on_compare_match()
{   // 01
    atd::Register(TCCR0A).write_zero_bit<COM0A1>();
    atd::Register(TCCR0A).write_one_bit <COM0A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}

inline void Timer0_CTC_mode::pin_A_clear_on_compare_match()
{   // 10
    atd::Register(TCCR0A).write_one_bit <COM0A1>();
    atd::Register(TCCR0A).write_zero_bit<COM0A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}


inline void Timer0_CTC_mode::pin_A_set_on_compare_match()
{   // 11
    atd::Register(TCCR0A).write_one_bit <COM0A1, COM0A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::as_output();
}

// Table 19-6
inline void Timer0_CTC_mode::pin_B_toggle_on_compare_match()
{   // 01
    atd::Register(TCCR0A).write_zero_bit<COM0B1>();
    atd::Register(TCCR0A).write_one_bit <COM0B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}

inline void Timer0_CTC_mode::pin_B_clear_on_compare_match()
{   // 10
    atd::Register(TCCR0A).write_one_bit <COM0B1>();
    atd::Register(TCCR0A).write_zero_bit<COM0B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}


inline void Timer0_CTC_mode::pin_B_set_on_compare_match()
{   // 11
    atd::Register(TCCR0A).write_one_bit <COM0B1, COM0B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::as_output();
}







inline void Timer0::off()
{ // 000
    atd::Register(TCCR0B).write_zero_bit<CS02, CS01, CS00>();
}


inline void Timer0::clock_speed_no_preescaling() 
{   // 001
    atd::Register(TCCR0B).write_zero_bit<CS02, CS01>();
    atd::Register(TCCR0B).write_one_bit <CS00>	();
}

/// Frecuencia del reloj preescalado =  clk_io/8
inline void Timer0::clock_frequency_entre_8()
{   // 010
    atd::Register(TCCR0B).write_zero_bit<CS02, CS00>	();
    atd::Register(TCCR0B).write_one_bit <CS01>	();
}

/// Frecuencia del reloj preescalado =  clk_io/64
inline void Timer0::clock_frequency_entre_64()
{   //011
    atd::Register(TCCR0B).write_zero_bit<CS02>	();
    atd::Register(TCCR0B).write_one_bit <CS01, CS00>	();
}

/// Frecuencia del reloj preescalado =  clk_io/256
inline void Timer0::clock_frequency_entre_256()
{   // 100
    atd::Register(TCCR0B).write_one_bit <CS02>	();
    atd::Register(TCCR0B).write_zero_bit<CS01, CS00>	();
}

/// Frecuencia del reloj preescalado =  clk_io/1024
inline void Timer0::clock_frequency_entre_1024()
{   // 101
    atd::Register(TCCR0B).write_one_bit <CS02, CS00>	();
    atd::Register(TCCR0B).write_zero_bit<CS01>	();
}


inline void Timer0::external_clock_falling_edge()
{// 110
    atd::Register(TCCR0B).write_one_bit <CS02, CS01>	();
    atd::Register(TCCR0B).write_zero_bit<CS00>	();
}

inline void Timer0::external_clock_rising_edge()
{// 111
    atd::Register(TCCR0B).write_one_bit <CS02, CS01, CS00>();
}





// Interrupciones
// --------------
inline void Timer0::enable_overflow_interrupt()
{
    atd::Register(TIMSK0).write_one_bit<TOIE0> ();
    avr::enable_all_interrupts();
}


inline void Timer0::enable_output_compare_A_match_interrupt()
{
    atd::Register(TIMSK0).write_one_bit<OCIE0A> ();
    avr::enable_all_interrupts();
}


inline void Timer0::enable_output_compare_B_match_interrupt()
{
    atd::Register(TIMSK0).write_one_bit<OCIE0B> ();
    avr::enable_all_interrupts();
}





}// namespace avr


#endif

