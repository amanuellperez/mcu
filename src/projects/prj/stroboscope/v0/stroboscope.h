// Copyright (C) 2021 Manuel Perez 
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
#ifndef __STROBOSCOPE_STROBOSCOPE_H__
#define __STROBOSCOPE_STROBOSCOPE_H__

#include <mega.h>

template <uint32_t freq_mcu>
struct __Stroboscope_cfg;

template <>
struct __Stroboscope_cfg<1000000u>
{
    constexpr static uint16_t freq_min = 0;
    constexpr static uint16_t freq_max = 1000;
    constexpr static uint16_t freq0 = 15; 
    constexpr static uint16_t div0 = 64;
};


template <>
struct __Stroboscope_cfg<8000000u>
{
    constexpr static uint16_t freq_min = 0;
    constexpr static uint16_t freq_max = 3000;
    constexpr static uint16_t freq0 = 123;
    constexpr static uint16_t div0 = 256;
};

class Stroboscope{
public:

    using Timer = atmega::Timer1;

    Stroboscope();

    // genera la frecuencia indicada encendiendo el stroboscope
    void generate(uint16_t freq);

    void on();
    void off();

    void freq(uint16_t x) {generate(x);}
    void freq_plus(uint16_t x);
    void freq_minus(uint16_t x);

    // state
    uint16_t freq() const {return freq_;}
    bool is_on() const {return on_;}
    bool is_off() const {return !on_;}

    // rango de frecuencias que generamos
    constexpr static uint32_t freq_mcu = MCU_CLOCK_FREQUENCY_IN_HZ;
    constexpr static uint16_t freq_min = __Stroboscope_cfg<freq_mcu>::freq_min;
    constexpr static uint16_t freq_max = __Stroboscope_cfg<freq_mcu>::freq_max;

private:
// state
    uint16_t freq_ = 500;
    bool on_ = false;



    uint16_t bound(uint16_t freq);
    void generate_1MHz();
    void generate_8MHz();
    void generate();

    
};

inline Stroboscope::Stroboscope() 
{ 
    Timer::mode_fast_PWM_top_ICR1(); 
    Timer::PWM_pin_A_non_inverting_mode();
}

inline void Stroboscope::on() { on_ = true; generate();}
inline void Stroboscope::off() { on_ = false; Timer::off(); }

inline void Stroboscope::generate()
{
    if constexpr (MCU_CLOCK_FREQUENCY_IN_HZ == 8000000u)
	generate_8MHz();

    else{    // default:
	generate_1MHz();
    }

}



#endif

