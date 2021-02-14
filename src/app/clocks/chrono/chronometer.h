// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __CHRONOMETER_H__
#define __CHRONOMETER_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Cronómetro básico
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    30/01/2021 v0.0
 *
 ****************************************************************************/
#include <chrono>
#include <atd_cast.h>
#include <atd_math.h> // div
#include <tuple>	// tie

namespace dev{

// De momento voy a usar el chronometro para medir tiempos. La diferencia con
// un reloj es su resolución: capaz de medir los milisegundos.
// Observar que no es más que una std::duration representada en sexagesimal.
// (???) Esta clase realmente es sexagesimal_milliseconds, esto es, un número
//       de milisegundos dados en sexagesimal. ¿Por qué no definirla en
//       general? Por la resolución. En el chronometro incluyo hasta el número
//       de horas, pero se podría contar también el dia, mes y año. ¿cómo
//       hacerlo genérico? ¿Merece la pena?
struct __Chronometer_sexagesimal_ms{
    int16_t milliseconds; // 00-999
    int8_t seconds;// 00-59
    int8_t minutes;// 00-59
    int8_t hours;  // 00-23

    __Chronometer_sexagesimal_ms() {}

    // conversión de ms a sexagesimal
    __Chronometer_sexagesimal_ms(int32_t ms);

    // conversión de sexagesimal a ms
    int32_t to_milliseconds() const;

    // std usa count(). La suministro por si en el futuro lo generalizo y lo
    // podemos integrar con std::duration.
    int32_t count() const {return to_milliseconds();}

};



inline bool operator==(const __Chronometer_sexagesimal_ms& a,
	               const __Chronometer_sexagesimal_ms& b)
{
    return a.milliseconds == b.milliseconds
	and a.seconds == b.seconds
	and a.minutes == b.minutes
	and a.hours == b.hours;
}

inline bool operator!=(const __Chronometer_sexagesimal_ms& a,
	               const __Chronometer_sexagesimal_ms& b)
{
    return !(a == b);
}






// Tenemos 3 periodos al manejar timers (aunque escribo frecuencia son
// periodos, suena mejor con frecuencia):
//  1. T_clock = frecuencia con la que opera el reloj del avr.
//  2. T_timer = frecuencia a la que opera el timer. Al timer le pasamos un
//	         preescaler, un divisor 'd' que hace que T_timer = d*T_clock.
//  3. T_interrupt = frecuencia a la que se genera la interrupción. 
//               DUDA: T_int = top * T_timer ó T_int = (top + 1)*T_timer???
template <typename Timer, uint16_t timer_period_in_us>
constexpr inline typename Timer::counter_type __Chronometer_ms_top()
{
    constexpr uint32_t one_millisecond_in_us = 1000u;
    constexpr uint32_t top = one_millisecond_in_us/timer_period_in_us;

    static_assert(top < Timer::max(),
                  "Top too great for this timer. Try another period or choose "
                  "a different F_CPU.");
    return atd::safe_static_cast<typename Timer::counter_type, uint32_t, top>();
}


// * Restricciones: solo un Chronometer_ms por aplicación (culpa de que usa static
//   milliseconds_).
// * (RRR) Voy a implementarlo siguiendo el formato de los relojes de std.
// * tick_up = indica si cada tick incrementa o decrementa el contador.
//	     Esto es, si el tiempo va hacia adelante o hacia atras.
//
// * mejoras (???): parametrizarlo con la representación y el ratio, de esta
//   forma se puede elegir que sea un chronometro que funcione en ms ó us.
//   ¿Merece la pena hacerlo?
template <typename Timer, uint16_t timer_period_in_us, bool tick_up = true>
struct Chronometer_ms{
    using duration   = std::chrono::duration<int32_t, std::milli>;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<Chronometer_ms, duration>;

    // milliseconds in sexagesimal representation
    using Sexagesimal_ms = __Chronometer_sexagesimal_ms;


    /// init chronometer
    //TODO: usar Generic_timer
    constexpr static void init()
    {
	Timer::enable_output_compare_A_match_interrupt();

	Timer::mode_CTC_top_OCR1A();
        Timer::output_compare_register_A(
            __Chronometer_ms_top<Timer, timer_period_in_us>());

        reset();
    }

    /// Reinicia el cronómetro y lo enciende.
    static void start()
    {
        Timer::template on<timer_period_in_us>();
    }

    /// Para el cronómetro sin borrar el tiempo actual.
    static void stop() { Timer::off(); }


// Lectura
    // la incluyo para que sea similar a un clock std
    static time_point now() noexcept 
    {return time_point{duration{milliseconds_}};}

    // en un chronometro parece más útil esta funcion que no now() (<-- la
    // elimino?) 
    static rep count() {return milliseconds_;}
    static Sexagesimal_ms sexagesimal_count() 
    { return Sexagesimal_ms{milliseconds_}; }

// Escritura
    static void count(const Sexagesimal_ms& sexag)
    { milliseconds_ = sexag.count(); }



    // precondition: state == stop
    static void add(duration incr_t0) 
    {
	duration incr_t{incr_t0};
	milliseconds_ += incr_t.count();
    }
 

    // precondition: state == stop
    static void substract(duration incr_t0) 
    {
	duration incr_t{incr_t0};
	if (milliseconds_ > incr_t.count())
	    milliseconds_ -= incr_t.count();
	else 
	    milliseconds_ = 0;
    }


    /// Damos un tick al clock. Esta función se llamará desde la interrupción
    /// correspondiente.
    static void tick() 
    {
	if constexpr (tick_up)
	    ++milliseconds_;
	else
	    --milliseconds_;
    }


private:
    // por culpa de que tiene que ser volatile no se puede definir como
    // duration (sus operators no son volatiles)
    inline static volatile rep milliseconds_;

    static void reset() 
    { 
	milliseconds_ = 0; 
	Timer::counter(0);
    }

};



}// dev

#endif


