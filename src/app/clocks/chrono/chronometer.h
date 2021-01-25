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


#include <chrono>
#include <atd_cast.h>

namespace dev{

// Tenemos 3 periodos al manejar timers (aunque escribo frecuencia son
// periodos, suena mejor con frecuencia):
//  1. T_clock = frecuencia con la que opera el reloj del avr.
//  2. T_timer = frecuencia a la que opera el timer. Al timer le pasamos un
//	         preescaler, un divisor 'd' que hace que T_timer = d*T_clock.
//  3. T_interrupt = frecuencia a la que se genera la interrupción. 
//               DUDA: T_int = top * T_timer ó T_int = (top + 1)*T_timer???
template <typename Timer, uint16_t timer_period_in_us>
constexpr inline typename Timer::counter_type __Chronometer_top()
{
    constexpr uint32_t one_microsecond_in_us = 1000u;
    constexpr uint32_t top = one_microsecond_in_us/timer_period_in_us;

    static_assert(top < Timer::max(),
                  "Top too great for this timer. Try another period or choose "
                  "a different F_CPU.");
    return atd::safe_static_cast<typename Timer::counter_type, uint32_t, top>();
}


// Restricciones: solo un Chronometer por aplicación (culpa de que usa static
// microseconds_).
template <typename Timer, uint16_t timer_period_in_us>
struct Chronometer{
    using rep        = uint32_t;

    /// init chronometer
    //TODO: usar Generic_timer
    constexpr static void init()
    {
	Timer::enable_output_compare_A_match_interrupt();

        Timer::top_OCRA(__Chronometer_top<Timer, timer_period_in_us>() );
    }

    static rep now() noexcept {return microseconds_;}

    /// Reinicia el cronómetro y lo enciende.
    static void start()
    {
	reset();
	on();
    }

    /// Para el cronómetro sin borrar el tiempo actual.
    static void stop()
    {
	Timer::off();
    }

    
    /// Damos un tick al clock. Esta función se llamará desde la interrupción
    /// correspondiente.
    static void tick() { ++microseconds_;}


private:
    inline static volatile rep microseconds_;

    static void reset() 
    { 
	microseconds_ = 0u; 
	Timer::counter(0);
    }


    static void on()
    {
        Timer::template on<timer_period_in_us>();
    }
};



}// dev

#endif


