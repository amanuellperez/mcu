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

#ifndef __AVR_TIMER0_H__
#define __AVR_TIMER0_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Traductor del timer 0 del AVR. Es un timer de 8 bits.
 *
 *   - TODO: El método para crear un interfaz a los dispositivos va a ser 
 *   el siguiente:
 *	1.- Crear un interfaz con nombre a las funciones de bajo nivel que
 *	suministra el dispositivo (esto es, en lugar de escribir en un flag le
 *	damos un nombre con significado a la acción).
 *
 *	2.- Crear diferentes clases para manejar el dispositivo. Por ejemplo,
 *	un timer se puede usar como un Contador, o como un Generador_de_PWM o
 *	como un Generador_de_waves. Cada uno de estos dispositivos tiene
 *	configuración por defecto diferente y funciones diferentes.
 *
 *   - HISTORIA:
 *           alp  - 06/08/2017 Escrita v0.0
 *		    18/07/2019 v0.1
 *
 ****************************************************************************/
#include <atd_register.h>

#include "avr_interrupt.h"
#include "avr_cfg.h"

namespace avr{

// La creo como clase para luego poderla pasar como parámetro a templates
// (por ejemplo, a Contador). Si la defino como namespace no puedo hacerlo.
class Timer0{

public:
    // Conexiones hardware del Timer
    // -----------------------------
    // Observar que tiene que ser uint16_t ya que la resolución es 256
    static constexpr uint16_t resolucion() {return TIMER0_resolucion;}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr uint8_t num_pinA() {return TIMER0_num_pin_A; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr uint8_t num_pinB() {return TIMER0_num_pin_B;}


    // Configuración del reloj
    // -----------------------
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
	    , uint32_t clock_frecuencia_en_hz = AVR_CLOCK_FRECUENCIA_EN_HZ>
    static void clock_period_in_us();


    // Modos de funcionamiento
    // -----------------------
    /// Definimos como modo de funcionamiento el normal mode.
    static void set_normal_mode();

    /// Definimos como modo de funcionamiento el CTC mode.
    static void set_CTC_mode();


    // Registros del timer
    // -------------------
    /// Reinicia el contador: vuelve a 0.
    static void reset() { TCNT0 = 0; }  

    /// Devuelve el valor del contador
    static uint8_t counter() {return TCNT0;}

    /// Escribimos x en el comparador 1
    static void comparador1(uint8_t x) {OCR0A = x;}


    // Interrupciones
    // --------------
    /// Habilitamos que se lance una interrupción cada vez que el timer
    /// pasa de 255 a 0.
    static void enable_overflow_interrupt();

// TODO: recolocar y probar lo que sigue.
    // Compare Output Mode, non PWM 
    // Para la salida en el pin OC0A (pin 12 del AVR) (table 19.3)
    static void compare_match_toggle_OC0A()
    {	// 01
	atd::Register(TCCR0A).write_zero_bit<COM0A1>	();
	atd::Register(TCCR0A).write_one_bit <COM0A0>	();
    }

    // Compare Output Mode, non PWM 
    // Para la salida en el pin OC0B (pin 11 del AVR) (table 19.6)
    static void compare_match_toggle_OC0B()
    {	// 01
	atd::Register(TCCR0A).write_zero_bit<COM0B1>	();
	atd::Register(TCCR0A).write_one_bit <COM0B0>	();
    }


    // Inicializamos el generador de ondas cuadradas en el pin A
    // Recordar que para poder usarlo:
    //		1.- Definir como salida el pin al que se conecte
    //		    (como entrada si se quiere
    //		    apagar)
    //		2.- Definir la frecuencia. La frecuencia viene dada por
    //		    la fórmula
    //			f_sw = f/(2d(1+OCR0A)
    //		    siendo:
    //		    f = la frecuencia del microcontrolador
    //		    d = el divisor de frecuencia (1, 8, 64, 256 ó 1024)
    //		    OCR0A = el valor que escribamos en el comparador1
    //		    
    //		    A partir de esta función podemos calcular d y OCR0A
    //		    conocidos f_sw y f.
    //
    //
    //	Esta función la defino como template para que de un error si se
    //	quiere conectar el generador de ondas a un pin no válido.
    //
    //	TODO: que de un mensaje más legible. Ahora simplemente dara el error
    //	diciendo que no está definida. Debería de decir: 
    //	"el generador de ondas solo se puede conectar a los pines 11 y 12.
    //	 Lo ha conectado mal. Revise las conexiones, por favor."
    //template<int num_pin>
    //void square_wave_init(Divisor divisor);
    //

    //// Implementación de la función square_wave_init para el pin A
    //inline void square_wave_init_pinA(Divisor divisor)
    //{
    //    set_CTC_mode(divisor);
    //    compare_match_toggle_OC0A();
    //}
    //
    //
    //// Implementación de la función square_wave_init para el pin B
    //inline void square_wave_init_pinB(Divisor divisor)
    //{
    //    set_CTC_mode(divisor);
    //    compare_match_toggle_OC0B();
    //}
    ////} // namespace impl
    //

    //
    //template<>
    //inline void square_wave_init<num_pinA()>(Divisor d)
    //{ square_wave_init_pinA(d); }
    //
    //template<>
    //inline void square_wave_init<num_pinB()>(Divisor d)
    //{ square_wave_init_pinB(d); }





};// Timer0


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


inline void Timer0::set_normal_mode()
{
//   Interrupts_lock l;

    // modo = 000
    atd::Register(TCCR0B).write_zero_bit<WGM02>	 ();
    atd::Register(TCCR0A).write_zero_bit<WGM01, WGM00>();
}

// Definimos el CTC_mode.
inline void Timer0::set_CTC_mode()
{
//    Interrupts_lock l;

    // modo = 010
    atd::Register(TCCR0B).write_zero_bit<WGM02> ();
    atd::Register(TCCR0A).write_one_bit <WGM01> ();
    atd::Register(TCCR0A).write_zero_bit<WGM00> ();
}

// Interrupciones
// --------------
inline void Timer0::enable_overflow_interrupt()
{
    atd::Register(TIMSK0).write_one_bit<TOIE0> ();
    Interrupt::enable_all_interrupts();
}

// reloj del avr a 1MHz
// --------------------
// a 1 us
template<>
inline void Timer0::clock_period_in_us<1u, 1000000UL>() 
{clock_speed_no_preescaling();}

// a 8 us
template<>
inline void Timer0::clock_period_in_us<8u, 1000000UL>() 
{clock_frequency_entre_8();}

// a 64 us
template<>
inline void Timer0::clock_period_in_us<64u, 1000000UL>() 
{clock_frequency_entre_64();}

// a 256 us
template<>
inline void Timer0::clock_period_in_us<256u, 1000000UL>() 
{clock_frequency_entre_256();}

// a 1024 us
template<>
inline void Timer0::clock_period_in_us<1024u, 1000000UL>() 
{clock_frequency_entre_1024();}



}// namespace avr


#endif

