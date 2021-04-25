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
#ifndef __STROBOSCOPE_STROBOSCOPE_H__
#define __STROBOSCOPE_STROBOSCOPE_H__

#include <avr_timer1_basic.h>

class Stroboscope{
public:
    static_assert(MCU_CLOCK_FREQUENCY_IN_HZ == 1000000u);

    using Timer = avr::Timer1;

    Stroboscope();

    // genera la frecuencia indicada encendiendo el stroboscope
    void generate(uint16_t freq);

    void on();
    void off();

    void freq_plus(uint16_t x);
    void freq_minus(uint16_t x);

    // state
    uint16_t freq() const {return freq_;}
    bool is_on() const {return on_;}
    bool is_off() const {return !on_;}


public:
// state
    uint16_t freq_ = 500;
    bool on_ = false;


    // rango de frecuencias que generamos
    constexpr static uint16_t freq_min = 0;
    constexpr static uint16_t freq_max = 1000;

    uint16_t bound(uint16_t freq);
    void generate();
};

inline Stroboscope::Stroboscope() { Timer::mode_fast_PWM_top_ICR1(); }

inline void Stroboscope::on() { on_ = true; generate();}
inline void Stroboscope::off() { on_ = false; Timer::off(); }



#endif

