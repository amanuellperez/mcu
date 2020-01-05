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

#ifndef __DEV_SYSTEM_CLOCK_H__
#define __DEV_SYSTEM_CLOCK_H__

#include <chrono>

#include <ctime>
#include <time.h>
#include <atd_cast.h>

namespace dev{

template <typename Timer, uint16_t timer_period_in_us>
constexpr inline typename Timer::counter_type __system_clock_top()
{
    constexpr uint32_t one_second_in_us = 1000000u;
    constexpr uint32_t top = one_second_in_us/timer_period_in_us;

    static_assert(top < Timer::max(),
                  "Top too great for this timer. Try another period or choose "
                  "a different F_CPU.");
    return atd::safe_static_cast<typename Timer::counter_type, uint32_t, top>();
}


// Timer requirements:
//	Timer::on<period_in_us>   - enables timer with period = period_in_us
//	Timer::enable_output_compare_A_match_interrupt
//	Timer::top_OCRA
//
// TODO: What is a timer? What concept? 
// This class has to work with ANY timer (avr timer, pic timer...). Which are
// the correct names for the timer requirements? 
//
template <typename Timer>
struct System_clock : public std::chrono::system_clock {

    template <uint16_t timer_period_in_us>
    constexpr static void init()
    {
        Timer::template on<timer_period_in_us>();

	Timer::enable_output_compare_A_match_interrupt();

        Timer::top_OCRA(__system_clock_top<Timer, timer_period_in_us>() );
    }

    /// Ponemos en hora el reloj.
    static void set(const time_point& t0) { ::set_system_time(to_time_t(t0)); }

    /// Damos un tick al clock.
    static void tick() {::system_tick();}

};

}// namespace

#endif


