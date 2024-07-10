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

#ifndef __ADEV_MOTOR_H__
#define __ADEV_MOTOR_H__

#include <atd_percentage.h>
#include <atd_math.h>

namespace adev{ // ampliacion de dev. Son clases que puede que sea interesante
		// pasarlas a dev

// Example: Dual_H_Bridge = L298N
// TODO: solo es un motor, bastaría con pasar un puente H, no un dual-
//
// Dudas paranoicas
// ----------------
//	    ¿Qué dirección es positiva y cual negativa?
//
// La pregunta es: yo soy el motor, o yo miro al motor.
// Voy a considerar que yo MIRO al motor, ya que es lo que habitualmente hago.
// Aunque si la clase es DC_Motor, ¿no quiere decir que es el motor? Pero
// claro, cuando yo le pido a la clase: DC_Motor.turn_right() le estoy
// diciendo que gire a la derecha ¿desde MI punto de vista o desde SU punto de
// vista? xD
//
// Voy a suponer (el uso dirá si es correcta o no la suposición) que yo
// me pongo DELANTE del motor y miro cómo gira. Si gira en el sentido
// matemático de ángulos positivos eso es girar hacia la izquierda;
// el sentido negativo es girar hacia la derecha
template <typename Dual_H_Bridge>
class DC_Motor{
public:
    using H_Bridge  = Dual_H_Bridge;

    using Direction = atd::Sign;

// Constructor
    DC_Motor() = delete;	// static interface

// Moving functions
    static void stop();

    static void turn_right(const atd::Percentage& right);
    static void turn_left( const atd::Percentage& left);

    static void turn(Direction dir, const atd::Percentage& p);
};


template <typename D>
inline 
void DC_Motor<D>::turn_right(const atd::Percentage& p)
{ H_Bridge::voltage1(atd::Sign::negative, p); }

template <typename D>
inline 
void DC_Motor<D>::turn_left(const atd::Percentage& p)
{ H_Bridge::voltage1(atd::Sign::positive, p); }

template <typename D>
void DC_Motor<D>::turn(Direction dir, const atd::Percentage& p)
{
    if (dir == Direction::positive)
	turn_left(p);
    else
	turn_right(p);
}


template <typename D>
inline 
void DC_Motor<D>::stop()
{ H_Bridge::stop1(); }




/***************************************************************************
 *			    SPEED CONTROL MOTOR
 ***************************************************************************/
// (???) ¿Cómo llamarlo en ingles? 
// Es un motor en el que se puede fijar la velocidad de giro.
class Speed_control_motor{
public:
   

private:
    
};
}// namespace
 
#endif


