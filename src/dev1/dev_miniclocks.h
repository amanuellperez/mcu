// Copyright (C) 2022 A.Manuel L.Perez 
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

#ifndef __DEV_MINICLOCKS_H__
#define __DEV_MINICLOCKS_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Un clock es un medidor de tiempo. Va siempre hacia adelante y tiene
 *	una máxima resolución (= máximo valor que puede contar)
 *  
 *	Distingo dos tipos de clocks básicos:
 *	    1. Miniclocks:  su resolución es muy pequeña, la del timer interno.
 *			Por ello solo suministro Miniclock_us y
 *			Miniclock_ms. 
 *
 *			En principio, miniclock NO es un reloj, no necesito
 *			ponerlo en hora. Un nombre más correcto sería
 *			Medidor_de_tiempo_ms pero es muy largo.
 *
 *			Ejemplo: para medir la duración de un pulso en 1
 *			pin. (ver IR_remote_control).
 *
 *	    2. Clocks    :  su resolución viene parametrizada por el tipo
 *	                indicado, pudiendo ser muy grande. 
 *	                Es necesario capturar la interrupción
 *	                correspondiente para que funcione bien.
 *	                Suministro dos: Clock_ms y Clock_s.
 *
 *	                Ejemplo: system_clock es un tipo de Clock_s
 *	    
 *
 *  RELACIÓN CON TIMER_COUNTER
 *
 *	Timer_counter
 *	-------------
 *	    (1) Podemos configurar el periodo del reloj interno, que nos da la
 *	        unidad de medida del counter. Podemos medir en (1us, 2us, 20us,
 *	        ...???).
 *	    (2) Podemos activar las interrupciones o no.
 *
 *	Miniclock_ms
 *	------------
 *	Particulariza Timer_counter:
 *	    (1) Solo mide ms. 
 *	    (2) Interrupciones desactivadas.
 *	    (3) Va hacia adelante.
 *
 *	Clock_ms
 *	--------
 *	Particulariza Timer_counter:
 *	    (1) Solo mide ms. 
 *	    (2) Interrupciones activadas.
 *	    (3) Va hacia adelante.
 *       
 *
 *
 *  HISTORIA
 *    A.Manuel L.Perez
 *    10/12/2022 Miniclock_ms/us
 *
 ****************************************************************************/
#include <dev_concepts.h>
#include <avr_atmega.h>	    // TODO: borrame. No genérico
			    
namespace dev{
/***************************************************************************
 *			    MINICLOCK_MS
 ***************************************************************************/
namespace dev_{
// De momento no suministro funciones write/reset/resume porque no parece que
// tenga mucho sentido. Si se necesita en el futuro añadirlas.
template <typename Timer_counter, int period>
class Miniclock{
public:
// Types
    using Timer        = Timer_counter;
    // DUDA: counter_type o value_type???
    // Estamos contando milisegundos: counter_type tiene sentido.
    using counter_type = Timer::counter_type;

// Operations
    // static interface
    Miniclock() = delete;

    /// Construye el reloj
    static void init(); // ¿init o cfg?

    /// Enciende el reloj desde 0.
    static void start();

    /// Para el reloj sin borrar el valor que estuviese mostrando.
    static void stop();
    
    /// Devuelve el número de milisegundos transcurridos desde start()
    static counter_type read(); // DUDA: mejor time()??? time_in_ms()???


    /// Espera t ticks de reloj. Si el tick son microsegundos, espera t
    /// microsegundos. Si el tick son milisegundos, t milisegundos.
    static void wait(const counter_type& t)
	    requires Unsafe_device<Timer>;

    static void wait(const counter_type& t)
	    requires Safe_device<Timer>;

// Info
    /// Devuelve el valor máximo que puede alcanzar el counter sin dar
    /// overflow: counter_max() + 1 == 0
    static counter_type counter_max();

};


template <typename T, int p>
inline void Miniclock<T, p>::init()
{
    Timer::init();
}

template <typename T, int period>
void Miniclock<T, period>::start()
{
    Timer::reset();
    Timer::template on<period>(); // 1024 us = 1000 +- 10% = 1 ms
}

template <typename T, int p>
inline void Miniclock<T, p>::stop()
{
    Timer::off();
}

template <typename T, int p>
inline Miniclock<T, p>::counter_type Miniclock<T, p>::read()
{
    return Timer::value();
}


template <typename T, int p>
void Miniclock<T, p>::wait(const counter_type& t)
	    requires Unsafe_device<Timer>

{
    atmega::Disable_interrupts lock;	// TODO: esto no es genérico

    Timer::unsafe_reset();
    
    while (Timer::unsafe_value() < t)
    { ; }
}


template <typename T, int p>
void Miniclock<T, p>::wait(const counter_type& t)
	    requires Safe_device<Timer>
{
    Timer::reset();
    
    while (Timer::value() < t)
    { ; }
}


template <typename T, int p>
inline Miniclock<T, p>::counter_type Miniclock<T, p>::counter_max()
{ 
    return Timer::max_top(); 
}
}// namespace dev_


// Miniclocks
// ----------
template <typename T>
using Miniclock_us = dev_::Miniclock<T, 1>;

template <typename T>
using Miniclock_ms = dev_::Miniclock<T, 1024>;



}// namespace


#endif


