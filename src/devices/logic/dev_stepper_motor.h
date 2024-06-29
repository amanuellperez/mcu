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

#ifndef __DEV_STEPPER_MOTOR_H__
#define __DEV_STEPPER_MOTOR_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver de un stepper motor genérico.	
 *
 * HISTORIA
 *    Manuel Perez
 *    26/06/2024 Escrito
 *
 ****************************************************************************/
#include <utility>  // std::pair
#include <cstdint>

#include <atd_magnitude.h>
#include <atd_minifloat.h>

namespace dev{


// (RRR) ¿por qué pasar nsteps_per_turn como int16_t y no como uint16_t?
//       Los ángulos los voy a definir positivos y negativos ==> todos los
//       números con los que opero tienen que ser pos/neg.
//
// (RRR) ¿ángulos positivos/negativos?
//	 En geometría es habitual de hablar de ángulos positivos y negativos.
//	 Puedo decir: gira -90º con una velocidad de 1 vuelta/segundo.
//
//	 En cinemática las velocidades claramente son positivas y negativas.
//
//	 Pero cuando hablamos de un coche no es habitual dar la distancia
//	 recorrida con signo, sino en valor absoluto. Al principio pense en
//	 poner esa restricción a los ángulos, pero es natural decir gira -90º
//	 con tal velocidad. No puedo imponer una restricción a la forma normal
//	 de hablar. Por eso tanto ángulos como velocidades pueden ser
//	 negativas.
template <typename Driver0, int16_t nsteps_per_turn>
class Stepper_motor{
public:
// Types
    using Driver      = Driver0;
    using Degree      = atd::Degree<atd::Float16>; 
    using Angle       = Degree;	

    using Degrees_per_second = atd::Degrees_per_second<atd::Float16>;
    using Angular_speed      = Degrees_per_second;

    using Direction = typename Driver::Direction;

    using NSteps_t  = typename Driver::NSteps_t;				     
    using Frequency = typename Driver::Frequency;
    using Hertz     = typename Driver::Hertz;

// Cfg
    // Número de steps que da el motor para dar una vuelta
    // Generalmente 200 steps.
    static_assert(nsteps_per_turn > 0, "nsteps can't be negative");
    static constexpr Degree step_angle 
			    = Degree{360} / Degree::Rep{nsteps_per_turn};

// Movement
    static void turn(const Angle& a, const Angular_speed& w);


// -------
// Depurar
    static std::pair<Frequency, NSteps_t> 
		    angle2freq(const Angle& a, const Angular_speed& w);

    static Direction angle2direction(const Angle& a, const Angular_speed& w);
private:
// types
    
};

template <typename C, int16_t N>
auto Stepper_motor<C, N>::angle2direction(const Angle& a, const Angular_speed& w)
    -> Direction
{
    if (a >= Angle{0}) {
	if (w >= Angular_speed{0}) return Direction::positive;
	else        return Direction::negative;

    }else {
	if (w >= Angular_speed{0}) return Direction::negative;
	else        return Direction::positive;
    }

}


template <typename C, int16_t N>
auto Stepper_motor<C, N>::angle2freq(const Angle& a0, const Angular_speed& w0)
    -> std::pair<Frequency, NSteps_t>
{
    Angle a = atd::abs(a0);
    Angular_speed w = atd::abs(w0);

    Angle::Rep nsteps0 = a / step_angle;
    NSteps_t nsteps = nsteps0.integer_part<NSteps_t>(); // truncamos el Float16
    
    atd::Float16 wf = w.value();
    atd::Float16 af = a.value();

    atd::Float16 freq = (wf / af) * atd::Float16{nsteps};
    typename Hertz::Rep f = freq.integer_part<typename Hertz::Rep>();

    return {Hertz{f}, nsteps};
}


template <typename C, int16_t N>
void Stepper_motor<C, N>::turn(const Angle& a, const Angular_speed& w)
{
    Driver::direction(angle2direction(a, w));

    // TODO: 
    // switch (Driver::mode()){
    //	break; case Driver::Mode::full_step: 
    //	break; case Driver::Mode::half_step: nsteps *= 2;
    //	break; case Driver::Mode::quarter_step: nsteps *= 4;
    //	...
    // }

    auto [freq_gen, nsteps] = angle2freq(a, w);
    Driver::step(freq_gen, nsteps);
}



// Motors
// ------
template <typename Driver>
using NEMA17 = Stepper_motor<Driver, 200>;

}// namespace
 
#endif


