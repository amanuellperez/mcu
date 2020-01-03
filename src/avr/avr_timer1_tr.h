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

#ifndef __AVR_TIMER1_TR_H__
#define __AVR_TIMER1_TR_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Traductor del timer 1 del AVR. Es un timer de 16 bits.
 *
 *	La salida de este timer está en los pines 15 y 16.
 *   
 *   - HISTORIA:
 *           alp  - 05/08/2017 Escrita v0.0
 *		    18/07/2019 v0.1
 *	
 *  TODO: faltan 3 modos de funcionamiento:
 *	+ El de capturar en el ICR.
 *	+ El phase correct mode.
 *	+ El phase and frequency correct.
 *
 *  TODO: meter todas las funciones dentro de un .cpp
 *
 ****************************************************************************/
#include <atd_register.h>

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
//	TCCRA	 8 bits
//	TCCRB	 8 bits
//
// 2. Posiciones de los pines dentro del avr:
//	pin de entrada: T0, T1 y ICP.
//	pines de salida: A y B.
//
class Timer1{
public:
    // Características del Timer
    // -------------------------
    // TODO: la resolucion se puede deducir del número de bits de TCNT!!!
    // Hacerlo automático!!!
    static constexpr uint32_t resolution() {return TIMER1_resolution;}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr uint8_t num_pin_A() {return TIMER1_num_pin_A; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr uint8_t num_pin_B() {return TIMER1_num_pin_B;}


    // Configuración del reloj
    // -----------------------
    static void clock_speed_no_preescaling();
    static void clock_frequency_entre_8();
    static void clock_frequency_entre_64();
    static void clock_frequency_entre_256();
    static void clock_frequency_entre_1024();

    /// Definimos el periodo del reloj que usa el timer.
    /// clock_frecuencia_en_hz = es la frecuencia del reloj del AVR.
    template<uint16_t periodo
	    , uint32_t clock_frecuencia_en_hz = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void clock_period_in_us();


    // Encendido/apagado del timer
    // ---------------------------
    /// Enciende el Timer, usando como reloj el reloj de periodo indicado.
    /// 'periodo' es el periodo en microsegundos al que va a funcionar el timer.
    /// clock_frecuencia_en_hz = es la frecuencia del reloj del AVR.
    template<uint16_t periodo
	    , uint32_t clock_frecuencia_en_hz = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void on() {clock_period_in_us<periodo, clock_frecuencia_en_hz>();}

    /// Paramos el timer.
    static void off();


    // Selección de reloj externo
    // --------------------------
    /// External clock source on T1 pin. Clock on falling edge.
    static void external_clock_falling_edge();

    /// External clock source on T1 pin. Clock on rising edge.
    static void external_clock_rising_edge();


    // Acceso a registros
    // ------------------
    static uint16_t counter();		/// Lectura del counter.
    static void counter(uint16_t x);	/// Escritura del counter.

    static uint16_t output_compare_register_A();      /// read OCRA
    static void output_compare_register_A(uint16_t x);/// write OCRA

    static uint16_t output_compare_register_B();      /// read OCRB
    static void output_compare_register_B(uint16_t x);/// write OCRB

    static uint16_t input_capture_register();	 /// Lectura del ICR.
    static void input_capture_register(uint16_t x);/// Escritura del ICR.


    // Interrupciones
    // --------------
    /// Cuando se produce un overflow generamos la interrupción
    /// correspondiente. Se captura con ISR_TIMER1_OVF
    static void enable_overflow_interrupt();

    /// Se captura con ISR_TIMER1_CAPT
    static void enable_input_capture_interrupt();

    /// Se captura con ISR_TIMER1_COMPA
    static void enable_on_compareA_match();

    /// Se captura con ISR_TIMER1_COMPB
    static void enable_on_compareB_match();

}; // Timer1


inline void Timer1::off()
{ // 000
    atd::Register(TCCR1B).write_zero_bit<CS12, CS11, CS10>();
}

inline void Timer1::clock_speed_no_preescaling()
{ // 001
    atd::Register(TCCR1B).write_zero_bit<CS12, CS11>	();
    atd::Register(TCCR1B).write_one_bit <CS10>	();
}
    
inline void Timer1::clock_frequency_entre_8()
{// 010
    atd::Register(TCCR1B).write_zero_bit<CS12, CS10>	();
    atd::Register(TCCR1B).write_one_bit <CS11>	();
}

inline void Timer1::clock_frequency_entre_64()
{// 011
    atd::Register(TCCR1B).write_zero_bit<CS12>	();
    atd::Register(TCCR1B).write_one_bit <CS11, CS10>	();
}

inline void Timer1::clock_frequency_entre_256()
{// 100
    atd::Register(TCCR1B).write_one_bit <CS12>	();
    atd::Register(TCCR1B).write_zero_bit<CS11, CS10>	();
}

inline void Timer1::clock_frequency_entre_1024()
{// 101
    atd::Register(TCCR1B).write_one_bit <CS12, CS10>	();
    atd::Register(TCCR1B).write_zero_bit<CS11>	();
}

inline void Timer1::external_clock_falling_edge()
{// 110
    atd::Register(TCCR1B).write_one_bit <CS12, CS11>	();
    atd::Register(TCCR1B).write_zero_bit<CS10>	();
}

inline void Timer1::external_clock_rising_edge()
{// 111
    atd::Register(TCCR1B).write_one_bit <CS12, CS11, CS10>();
}



// reloj del avr a 1MHz
// --------------------
// a 1 us
template<>
inline void Timer1::clock_period_in_us<1u, 1000000UL>() 
{clock_speed_no_preescaling();}

// a 8 us
template<>
inline void Timer1::clock_period_in_us<8u, 1000000UL>() 
{clock_frequency_entre_8();}

// a 64 us
template<>
inline void Timer1::clock_period_in_us<64u, 1000000UL>() 
{clock_frequency_entre_64();}

// a 256 us
template<>
inline void Timer1::clock_period_in_us<256u, 1000000UL>() 
{clock_frequency_entre_256();}

// a 1024 us
template<>
inline void Timer1::clock_period_in_us<1024u, 1000000UL>() 
{clock_frequency_entre_1024();}

// reloj del avr a 8MHz
// --------------------
// a 125 ns
//template<>
//inline void Timer1::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

// a 1 us
template<>
inline void Timer1::clock_period_in_us<1u, 8000000UL>() 
{clock_frequency_entre_8();}

// a 8 us
template<>
inline void Timer1::clock_period_in_us<8u, 8000000UL>() 
{clock_frequency_entre_64();}

// a 32 us
template<>
inline void Timer1::clock_period_in_us<32u, 8000000UL>() 
{clock_frequency_entre_256();}

// a 128 us
template<>
inline void Timer1::clock_period_in_us<128u, 8000000UL>() 
{clock_frequency_entre_1024();}






// Devuelve el valor del contador.
// Importante: según la datasheet, pag. 153,  al leer los uint16_t 
// hay que bloquear las interrupciones. Por eso el lock.  
inline uint16_t Timer1::counter() 
{
    Interrupts_lock l;
    auto res = TCNT1;

    return res;
}

inline void Timer1::counter(uint16_t x) 
{
    Interrupts_lock l;
    TCNT1 = x;
}


inline uint16_t Timer1::output_compare_register_A() 
{
    Interrupts_lock l;
    auto res = OCR1A;

    return res;
}

// Escribimos x en el comparador A
inline void Timer1::output_compare_register_A(uint16_t x)
{
    Interrupts_lock l;
    OCR1A = x;
}


inline uint16_t Timer1::output_compare_register_B() 
{
    Interrupts_lock l;
    auto res = OCR1B;

    return res;
}

// Escribimos x en el comparador B
inline void Timer1::output_compare_register_B(uint16_t x)
{
    Interrupts_lock l;
    OCR1B = x;
}


inline uint16_t Timer1::input_capture_register() 
{
    Interrupts_lock l;
    auto res = ICR1;

    return res;
}

// Escribimos x en el comparador B
inline void Timer1::input_capture_register(uint16_t x)
{
    Interrupts_lock l;
    ICR1 = x;
}










inline void Timer1::enable_overflow_interrupt()
{
    atd::Register(TIMSK1).write_one_bit<TOIE1> ();
    Interrupt::enable_all_interrupts();
}

inline void Timer1::enable_input_capture_interrupt()
{
    atd::Register(TIMSK1).write_one_bit<ICIE1> ();
    Interrupt::enable_all_interrupts();
}


inline void Timer1::enable_on_compareA_match()
{
    atd::Register(TIMSK1).write_one_bit<OCIE1A> ();
    Interrupt::enable_all_interrupts();
}


inline void Timer1::enable_on_compareB_match()
{
    atd::Register(TIMSK1).write_one_bit<OCIE1B> ();
    Interrupt::enable_all_interrupts();
}

/*!
 *  \brief  Timer1 funcionando en normal mode.
 *
 */
class Timer1_normal_mode: public Timer1{
public:
    /// Encendemos el Timer1
    template<uint16_t periodo
	    , uint32_t clock_frecuencia_en_hz = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void on();

    /// Ponemos el contador a cero.
    static void reset();


private:
    static void set_normal_mode();
    static void pin_AB_disconnected();
};


template<uint16_t periodo
	, uint32_t clock_frecuencia_en_hz>
inline void Timer1_normal_mode::on()
{
    Interrupts_lock l;

    set_normal_mode();
    pin_AB_disconnected();
    Timer1::on<periodo, clock_frecuencia_en_hz>();
}


// Modo de funcionamiento del timer: normal. Cuenta de 0 a 0xFFFF
// y vuelve a empezar.
inline void Timer1_normal_mode::set_normal_mode()
{
//    Interrupts_lock l; Son de 8 bits, no es necesario bloquearlo

    atd::Register(TCCR1A).write_zero_bit<WGM10, WGM11>();
    atd::Register(TCCR1B).write_zero_bit<WGM12, WGM13>();
}

inline void Timer1_normal_mode::pin_AB_disconnected()
{   // 00
    atd::Register(TCCR1A).write_zero_bit <COM1A1, COM1A0>();
    atd::Register(TCCR1A).write_zero_bit <COM1B1, COM1B0>();
}



inline void Timer1_normal_mode::reset()
{
    Interrupts_lock l; 
    TCNT1 = 0; 
}




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
    // Configuración del CTC.
    // 1.- ¿Dónde anotamos el valor del TOP? ¿En OCRA o en ICR?
    // Recordar definir el valor de OCR1A/ICR elegido.
    /// Define el valor del TOP como el indicado en OCR1A.
    /// El TCNT va desde 0 hasta el valor escrito en OCR1A.
    static void top_OCRA(const uint16_t& top0);

    /// Define el valor del TOP como el indicado en ICR.
    /// El TCNT va desde 0 hasta el valor escrito en ICR.
    static void top_ICR(const uint16_t& top0);

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



inline void Timer1_CTC_mode::top_OCRA(const uint16_t& top0)
{
    // 0100
    atd::Register(TCCR1B).write_zero_bit<WGM13>	 ();
    atd::Register(TCCR1B).write_one_bit <WGM12>	 ();
    atd::Register(TCCR1A).write_zero_bit<WGM11, WGM10>();

    output_compare_register_A(top0);
}


inline void Timer1_CTC_mode::top_ICR(const uint16_t& top0)
{
    // 1100
    atd::Register(TCCR1B).write_one_bit<WGM13, WGM12> ();
    atd::Register(TCCR1A).write_zero_bit<WGM11, WGM10>();

    input_capture_register(top0);
}


// Table 20-3.
inline void Timer1_CTC_mode::pin_A_toggle_on_compare_match()
{   // 01
    atd::Register(TCCR1A).write_zero_bit <COM1A1>();
    atd::Register(TCCR1A).write_one_bit<COM1A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::de_salida();
}

inline void Timer1_CTC_mode::pin_A_clear_on_compare_match()
{   // 10
    atd::Register(TCCR1A).write_one_bit <COM1A1>();
    atd::Register(TCCR1A).write_zero_bit<COM1A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::de_salida();
}


inline void Timer1_CTC_mode::pin_A_set_on_compare_match()
{   // 11
    atd::Register(TCCR1A).write_one_bit <COM1A1, COM1A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::de_salida();
}


inline void Timer1_CTC_mode::pin_B_toggle_on_compare_match()
{   // 01
    atd::Register(TCCR1A).write_zero_bit <COM1B1>();
    atd::Register(TCCR1A).write_one_bit<COM1B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::de_salida();
}

inline void Timer1_CTC_mode::pin_B_clear_on_compare_match()
{   // 10
    atd::Register(TCCR1A).write_one_bit <COM1B1>();
    atd::Register(TCCR1A).write_zero_bit<COM1B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::de_salida();
}


inline void Timer1_CTC_mode::pin_B_set_on_compare_match()
{   // 11
    atd::Register(TCCR1A).write_one_bit <COM1B1, COM1B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::de_salida();
}



/*!
 *  \brief  Hacemos que el Timer 1 funcione en el modo fast PWM.
 *
 *
 */
class Timer1_fast_PWM:public Timer1{
public:
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
    static void top_OCRA(uint16_t top0);

    /// El contador va desde 0 hasta el valor escrito en ICR.
    static void top_ICR(uint16_t top0);


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
    atd::Register(TCCR1B).write_zero_bit<WGM13>	();
    atd::Register(TCCR1B).write_one_bit <WGM12>	();
    atd::Register(TCCR1A).write_zero_bit<WGM11> ();
    atd::Register(TCCR1A).write_one_bit <WGM10>	();
}


inline void Timer1_fast_PWM::top_0x01FF()
{
    // 0110
    atd::Register(TCCR1B).write_zero_bit<WGM13>	();
    atd::Register(TCCR1B).write_one_bit <WGM12>	();
    atd::Register(TCCR1A).write_one_bit <WGM11>	();
    atd::Register(TCCR1A).write_zero_bit<WGM10> ();
}

inline void Timer1_fast_PWM::top_0x03FF()
{
    // 0111
    atd::Register(TCCR1B).write_zero_bit<WGM13>	 ();
    atd::Register(TCCR1B).write_one_bit <WGM12>	 ();
    atd::Register(TCCR1A).write_one_bit <WGM11, WGM10>();
    
}

inline void Timer1_fast_PWM::top_OCRA(uint16_t top0)
{
    // 1111
    atd::Register(TCCR1B).write_one_bit<WGM13, WGM12>();
    atd::Register(TCCR1A).write_one_bit<WGM11, WGM10>();

    output_compare_register_A(top0);
}

inline void Timer1_fast_PWM::top_ICR(uint16_t top0)
{
    // 1110
    atd::Register(TCCR1B).write_one_bit <WGM13, WGM12>	();
    atd::Register(TCCR1A).write_one_bit <WGM11>		();
    atd::Register(TCCR1A).write_zero_bit<WGM10>		();
    
    input_capture_register(top0);
}


//inline void Timer1_fast_PWM::toggle_pin_A_on_compare_match_pin_B_disconnected()
//{   // 01
//    atd::Register(TCCR1A).write_zero_bit<COM1A1>();
//    atd::Register(TCCR1A).write_one_bit <COM1A0>();
//}

inline void Timer1_fast_PWM::pin_A_non_inverting_mode()
{   // 10
    atd::Register(TCCR1A).write_one_bit <COM1A1>();
    atd::Register(TCCR1A).write_zero_bit<COM1A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::de_salida();
}

inline void Timer1_fast_PWM::pin_A_inverting_mode()
{   // 11
    atd::Register(TCCR1A).write_one_bit <COM1A1, COM1A0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_A()>::de_salida();
}


inline void Timer1_fast_PWM::pin_B_non_inverting_mode()
{   // 10
    atd::Register(TCCR1A).write_one_bit <COM1B1>();
    atd::Register(TCCR1A).write_zero_bit<COM1B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::de_salida();
}

inline void Timer1_fast_PWM::pin_B_inverting_mode()
{   // 11
    atd::Register(TCCR1A).write_one_bit <COM1B1, COM1B0>();

    // Obligatorio definirlo como de salida. 
    Pin<num_pin_B()>::de_salida();
}



}// namespace

#endif


