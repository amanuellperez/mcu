// Copyright (C) 2024 Manuel Perez 
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

#ifndef __CAR_2WHEELS_H__
#define __CAR_2WHEELS_H__

#include <atd_percentage.h>
#include <atd_math.h>

namespace adev{ // ampliacion de dev. Son clases que puede que sea interesante
		// pasarlas a dev

// Example: Dual_H_Bridge = L298N
// TODO: mejor pasar como parámetro Right_H_Bridge and Left_H_Bridge
template <typename Dual_H_Bridge>
class Car_2_wheels{
public:
    using H_Bridge = Dual_H_Bridge;
    // TODO:
    // Right_wheel = ...
    // Left_wheel  = ...

// Constructor
    Car_2_wheels() = delete;	// static interface

// Moving functions
    static void stop();
    static void forward(const atd::Percentage& p);
    static void backward(const atd::Percentage& p);
    static void forward_turn_right();
    static void forward_turn_left();
    static void lateral_movement();

private:
    
};

template <typename D>
void Car_2_wheels<D>::stop()
{
    H_Bridge::stop1();
    H_Bridge::stop2();
}


template <typename D>
void Car_2_wheels<D>::forward(const atd::Percentage& p)
{
    H_Bridge::voltage1(atd::Sign::positive, p);
    H_Bridge::voltage2(atd::Sign::positive, p);
}

template <typename D>
void Car_2_wheels<D>::backward(const atd::Percentage& p)
{
    H_Bridge::voltage1(atd::Sign::negative, p);
    H_Bridge::voltage2(atd::Sign::negative, p);
}

// TODO: values ???
template <typename D>
void Car_2_wheels<D>::forward_turn_right()
{
    H_Bridge::voltage1(atd::Sign::positive, 0);
    H_Bridge::voltage2(atd::Sign::positive, 50);
}

template <typename D>
void Car_2_wheels<D>::forward_turn_left()
{
    H_Bridge::voltage1(atd::Sign::positive, 50);
    H_Bridge::voltage2(atd::Sign::positive, 0);
}


template <typename D>
void Car_2_wheels<D>::lateral_movement()
{
    for (uint8_t i = 0; i < 10; ++i){
	forward_turn_right();
	forward_turn_left();
    }
}
}// namespace
 
#endif


