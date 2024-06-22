// Copyright (C) 2022 Manuel Perez 
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

#ifndef __MCU_MINICLOCK_H__
#define __MCU_MINICLOCK_H__
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
 *    Manuel Perez
 *    10/12/2022 Miniclock_ms/us
 *    31/12/2022 unsafe_time/unsafe_reset
 *
 ****************************************************************************/
#include "mcu_concepts.h"
			    
namespace mcu{
/***************************************************************************
 *			    MINICLOCK_XX
 ***************************************************************************/
namespace mcu_{
// De momento no suministro funciones write/reset/resume porque no parece que
// tenga mucho sentido. Si se necesita en el futuro añadirlas.
//
//
// (RRR) Es responsabilidad del cliente llamar explicitamente a `start` para
//       que funcione el reloj. Si se llama a Miniclock::wait() sin haber
//       llamado a `start` no hará nada.
//
//       ¿Por qué? El Miniclock_us está pensado para medir microsegundos. Si
//       cada vez que llamo a wait tengo que perder tiempo encendiéndolo
//       estámos perdiendo mucho tiempo. Esta pretende ser una clase eficiente
//       de bajo nivel.
template <typename Micro_t,   // micro que estamos usando
	 typename Timer_counter0, int period0>
class Miniclock{
public:
// Types
    using Micro	       = Micro_t;
    using Timer	       = Timer_counter0::Timer; // Timer usado internamente
    using Timer_counter= Timer_counter0;
    using counter_type = Timer_counter::counter_type;

// Operations
    // static interface
    Miniclock() = delete;

    /// Configura este device para poder usarlo.
    /// Configura el Time_counter interno
    static void cfg(); 

    /// Enciende el reloj desde 0. 
    static void start();

    /// Para el reloj sin borrar el valor que estuviese mostrando.
    static void stop();
    
    // time
    // ----
    /// Devuelve el número de ticks transcurridos desde start()
    static counter_type time();

    // Cuando se quiera eficiencia, deshabilitar las interrupciones (como
    // ocurre por defecto dentro de una ISR) y usar las funciones unsafe.
    // Serán un pelín más eficientes.
    static counter_type unsafe_time()
	    requires Type::Unsafe_device<Timer_counter>;

    static counter_type unsafe_time()
	    requires Type::Safe_device<Timer_counter>;

    /// Espera t ticks de reloj. Si el tick son microsegundos, espera t
    /// microsegundos. Si el tick son milisegundos, t milisegundos.
    static void wait(const counter_type& t)
	    requires Type::Unsafe_device<Timer_counter>;

    static void wait(const counter_type& t)
	    requires Type::Safe_device<Timer_counter>;

    // reset
    // -----
    /// Pone el contador a cero. No enciende el timer como start.
    static void reset();

    static void unsafe_reset()
	    requires Type::Unsafe_device<Timer_counter>;

    static void unsafe_reset()
	    requires Type::Safe_device<Timer_counter>;

// Info
    /// Devuelve el valor máximo que puede alcanzar el counter sin dar
    /// overflow: counter_max() + 1 == 0
    constexpr static counter_type counter_max();

private:
    static constexpr int period = period0;
};


// Defino todo el interfaz como inline ya que en el Miniclock_us busco que sea
// lo más eficiente posible. No podemos perder tiempo llamando a funciones.
template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::cfg()
{
    Timer_counter::init();
}

template <typename M, typename T, int period>
inline void Miniclock<M, T, period>::start()
{
    Timer_counter::reset();
    Timer_counter::template 
		turn_on_with_clock_period_of<period>::us();
					    // 1024 us = 1000 +- 10% = 1 ms
}

template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::stop()
{
    Timer_counter::turn_off();
}

template <typename M, typename T, int p>
inline Miniclock<M, T, p>::counter_type Miniclock<M, T, p>::time()
{
    return Timer_counter::value();
}

template <typename M, typename T, int p>
inline Miniclock<M, T, p>::counter_type Miniclock<M, T, p>::unsafe_time()
	    requires Type::Unsafe_device<Timer_counter>
{
    return Timer_counter::unsafe_value();
}

template <typename M, typename T, int p>
inline Miniclock<M, T, p>::counter_type Miniclock<M, T, p>::unsafe_time()
	    requires Type::Safe_device<Timer_counter>
{
    return Timer_counter::safe_value();
}


template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::wait(const counter_type& t)
	    requires Type::Unsafe_device<Timer_counter>

{
    using Disable_interrupts = Micro::Disable_interrupts;
    Disable_interrupts lock;	

    Timer_counter::unsafe_reset();
    
    while (Timer_counter::unsafe_value() < t)
    { ; }
}


template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::wait(const counter_type& t)
	    requires Type::Safe_device<Timer_counter>
{
    Timer_counter::reset();
    
    while (Timer_counter::value() < t)
    { ; }
}



template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::reset()
{
    Timer_counter::reset();
}

template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::unsafe_reset()
	requires Type::Unsafe_device<Timer_counter>
{
    Timer_counter::unsafe_reset();
}

template <typename M, typename T, int p>
inline void Miniclock<M, T, p>::unsafe_reset()
	requires Type::Safe_device<Timer_counter>
{
    Timer_counter::reset();
}


template <typename M, typename T, int p>
inline
constexpr Miniclock<M, T, p>::counter_type Miniclock<M, T, p>::counter_max()
{ 
    return Timer_counter::max_top(); 
}
}// namespace mcu_


// Miniclocks
// ----------
template <typename M, typename T>
using Miniclock_us = mcu_::Miniclock<M, T, 1>;

template <typename M, typename T>
using Miniclock_ms = mcu_::Miniclock<M, T, 1024>;



}// namespace


#endif


