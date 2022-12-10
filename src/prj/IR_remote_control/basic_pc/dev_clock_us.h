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

#ifndef __DEV_CLOCK_US_H__
#define __DEV_CLOCK_US_H__



#undef wait_ms
#undef wait_us

namespace dev{

// Clock que mide en microsegundos con poco alcance (valor máx = 2^16 ó 2^8
// dependiendo del Timer usado)
// Auque es obvio que `wait` es `wait_us` ya que el Clock es en microsegundos,
// me resulta más claro usar `wait_us` que `wait`. Por eso de momento lo llamo
// `wait_us`.
template <typename Timer>
class Clock_us{
public:
    using counter_type = Timer::counter_type;

    static void init() { Timer::safe_init(); }

    static void on();
    static void off(){ Timer::off(); };

    static counter_type unsafe_value() { return Timer::unsafe_value(); }
    static counter_type safe_value()   { return Timer::safe_value(); }

    static void unsafe_reset() {Timer::unsafe_reset(); }
    static void safe_reset()   {Timer::safe_reset(); }

    static void wait_us(Timer::counter_type t);
};


template <typename Timer>
void Clock_us<Timer>::on()
{
    Timer::safe_top(Timer::max_top());
    Timer::template on<1>();
}

template <typename Timer>
void Clock_us<Timer>::wait_us(Timer::counter_type t)
{
    atmega::Disable_interrupts lock;	// TODO: esto no es genérico

    Timer::unsafe_reset();
    
    while (Timer::unsafe_value() < t)
    { ; }

}

}// namespace

#endif
