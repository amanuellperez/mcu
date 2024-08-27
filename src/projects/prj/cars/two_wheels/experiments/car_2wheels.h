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
template <typename Right_wheel_t, typename Left_wheel_t>
class Car_2_wheels{
public:
    using Right_wheel = Right_wheel_t;
    using Left_wheel  = Left_wheel_t;

// Constructor
    Car_2_wheels() = delete;	// static interface

// Moving functions
    static void stop();

    static void forward(const atd::Percentage& p);
    static void forward_turn_right_(const atd::Percentage& right, 
				   const atd::Percentage& left);

    static void forward_turn_right(const atd::Percentage& right, 
				   const atd::Percentage& left);
    static void forward_turn_left();

    static void backward(const atd::Percentage& p);
    static void backward_turn_right();
    static void backward_turn_left();

    static void lateral_movement();

// Movimiento genérico: para probar 
    // Se mueve hacia adelante o hacia atras con las velocidades de las ruedas
    // indicadas en right/left.
    static void move(atd::Sign sign, const atd::Percentage& right, 
				     const atd::Percentage& left);

    // En este caso el sentido de giro de las ruedas es diferente
    static void move_opposite(atd::Sign sign, const atd::Percentage& right, 
				     const atd::Percentage& left);

// nombres con significado
    static void right_wheel(atd::Sign sign, const atd::Percentage& p);
    static void left_wheel(atd::Sign sign, const atd::Percentage& p);
};

template <typename R, typename L>
inline 
void Car_2_wheels<R,L>::right_wheel(atd::Sign sign, const atd::Percentage& p)
{ Right_wheel::voltage(sign, p); }

template <typename R, typename L>
inline 
void Car_2_wheels<R,L>::left_wheel(atd::Sign sign, const atd::Percentage& p)
{ Left_wheel::voltage(sign, p); }

template <typename R, typename L>
void Car_2_wheels<R,L>::move(atd::Sign sign, const atd::Percentage& right, 
				 const atd::Percentage& left)
{
    right_wheel(sign, right);
    left_wheel (sign, left);
}

template <typename R, typename L>
void Car_2_wheels<R,L>::move_opposite(atd::Sign sign, const atd::Percentage& right, 
				 const atd::Percentage& left)
{
    right_wheel(sign, right);
    left_wheel (atd::opposite(sign), left);
}

template <typename R, typename L>
void Car_2_wheels<R,L>::stop()
{
    Right_wheel::stop();
    Left_wheel::stop();
}


template <typename R, typename L>
void Car_2_wheels<R,L>::forward(const atd::Percentage& p)
{
    right_wheel(atd::Sign::positive, p);
    left_wheel (atd::Sign::positive, p);
}

template <typename R, typename L>
void Car_2_wheels<R,L>::backward(const atd::Percentage& p)
{
    right_wheel(atd::Sign::negative, p);
    left_wheel (atd::Sign::negative, p);
}

template <typename R, typename L>
void Car_2_wheels<R,L>::forward_turn_right(const atd::Percentage& right,
					 const atd::Percentage& left)
{
    right_wheel(atd::Sign::positive, right);
    left_wheel (atd::Sign::positive, left);
}

template <typename R, typename L>
void Car_2_wheels<R,L>::forward_turn_left()
{
    right_wheel(atd::Sign::positive, atd::Percentage{50});
    left_wheel (atd::Sign::positive, atd::Percentage{0});
}

template <typename R, typename L>
void Car_2_wheels<R,L>::backward_turn_right()
{
    right_wheel(atd::Sign::negative, atd::Percentage{0});
    left_wheel (atd::Sign::negative, atd::Percentage{50});
}

template <typename R, typename L>
void Car_2_wheels<R,L>::backward_turn_left()
{
    right_wheel(atd::Sign::negative, atd::Percentage{50});
    left_wheel (atd::Sign::negative, atd::Percentage{0});
}



template <typename R, typename L>
void Car_2_wheels<R,L>::lateral_movement()
{
    for (uint8_t i = 0; i < 10; ++i){
	forward_turn_right(atd::Percentage{0}, atd::Percentage{50});
	forward_turn_left();
    }
}
}// namespace
 
#endif


