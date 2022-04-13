// Copyright (C) 2019-2022 A.Manuel L.Perez 
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

#ifndef __DEV_CLOCKS_H__
#define __DEV_CLOCKS_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Differents clocks:
 *	
 *	+ system_clock: reloj de sistema (similar a std::system_clock).
 *	+ Chronometer_ms: reloj que mide el tiempo en milisegundos.
 *
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    ??/??/2020 Escrito
 *    26/02/2021 Basado en Generic_timer
 *    11/04/2022 Reestructurado. Chronometer_ms
 *
 ****************************************************************************/

#include <chrono>

#include <ctime>
#include <time.h>
#include <atd_cast.h>

namespace dev{

/***************************************************************************
 *			    SYSTEM_CLOCK
 ***************************************************************************/
template <typename Timer, uint16_t timer_period_in_us>
constexpr inline typename Timer::counter_type __system_clock_top()
{
    constexpr uint32_t one_second_in_us = 1000000u;
    constexpr uint32_t top = one_second_in_us/timer_period_in_us;

    static_assert(top < Timer::timer_counter_max_top(),
                  "Top too great for this timer. Try another period or choose "
                  "a different F_CPU.");
    return atd::safe_static_cast<typename Timer::counter_type, uint32_t, top>();
}


// Timer: es un Generic_timer.
// (RRR) timer_period_in_us solo lo necesito en init. ¿Por qué parametrizar
//       todo el System_clock con timer_period_in_us cuando no es necesario?
//       Para simplificar el uso. En el archivo 'dev.h' se define el
//       System_clock, y el resto del programa no necesita recordar que el
//       init tienes que pasarle el timer_period_in_us. Queda más claro el
//       código (y posiblemente más portable)
template <typename Timer, uint16_t timer_period_in_us>
struct System_clock : public std::chrono::system_clock {

    constexpr static void init()
    {
	Timer::mode_timer_counter(
			    __system_clock_top<Timer, timer_period_in_us>());
        Timer::template on<timer_period_in_us>();
    }

    /// Ponemos en hora el reloj.
    static void set(const time_point& t0) { ::set_system_time(to_time_t(t0)); }

    /// Damos un tick al clock.
    static void tick() {::system_tick();}

};



/***************************************************************************
 *			    CHRONOMETER_MS
 ***************************************************************************/

/// El tiempo lo podemos medir en milisegundos o escribirlo como
/// hh:mm::ss::ms.
/// Ejemplo: 2h 3min 4s 25ms = 7384025 ms
/// Esta clase es responsable de pasar de una representación a otra.
//
// ¿Por qué devolver las horas? ¿Por qué no devolver también días?
// La resolución la marca el tipo usado para representar los milisegundos.
// Si usamos int32_t podemos representar hasta 2^31 - 1 ms = que vienen a ser
// unas 600 horas. 
// Si en lugar de int32_t se usara int64_t podríamos representar hastsa
// 2^63 - 1 ms = con lo que se podría representar años perfectamente.
// En principio la idea es hacer stopwatch y timers no necesitando en la
// práctica  medir hasta las horas (usar un int16_t no serviría ya que solo se
// podrían representar hasta 30 segundos, demasiado poco tiempo).
struct _Sexagesimal_ms{
    int16_t milliseconds; // 00-999
    int8_t seconds;// 00-59
    int8_t minutes;// 00-59
    int8_t hours;  // 00-23

    _Sexagesimal_ms() {}

    // conversión de ms a sexagesimal
    _Sexagesimal_ms(int32_t ms);

    // conversión de sexagesimal a ms
    int32_t to_milliseconds() const;

    // std usa count(). La suministro por si en el futuro lo generalizo y lo
    // podemos integrar con std::duration.
    int32_t count() const {return to_milliseconds();}

};



inline bool operator==(const _Sexagesimal_ms& a,
	               const _Sexagesimal_ms& b)
{
    return a.milliseconds == b.milliseconds
	and a.seconds == b.seconds
	and a.minutes == b.minutes
	and a.hours == b.hours;
}

inline bool operator!=(const _Sexagesimal_ms& a,
	               const _Sexagesimal_ms& b)
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

    static_assert(top < Timer::timer_counter_max_top(),
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
template <typename Timer,   // = Generic_timer
	 uint16_t timer_period_in_us, 
	 bool tick_up = true // up or down?
	 > 
struct Chronometer_ms {
    using duration   = std::chrono::duration<int32_t, std::milli>;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<Chronometer_ms, duration>;

    // milliseconds in sexagesimal representation
    using Sexagesimal_ms = _Sexagesimal_ms;


    /// init chronometer. Lo ponemos a 0.
    constexpr static void init()
    {
	Timer::mode_timer_counter(
			    __Chronometer_ms_top<Timer, timer_period_in_us>());

        reset();
    }

    /// Enciende el cronometro. Recordar definir el tiempo antes.
    static void on()
    { Timer::template on<timer_period_in_us>(); }

    /// Para el cronómetro sin borrar el tiempo actual.
    static void off() { Timer::off(); }


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
    static void reset() 
    { 
	milliseconds_ = 0; 
	Timer::timer_counter_reset();
    }

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

};

}// dev

#endif


