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
#include <cstdint>
#include <atd_magnitude.h>
#include <atd_minifloat.h>

namespace dev{


// (RRR) ¿por qué pasar nsteps_per_turn como int16_t y no como uint16_t?
//       Los ángulos los voy a definir positivos y negativos ==> todos los
//       números con los que opero tienen que ser pos/neg.
template <typename Cfg, int16_t nsteps_per_turn>
class Stepper_motor{
public:
// Types
    using Driver      = typename Cfg::Driver;
    using Degree      = atd::Degree<atd::Float16>; 
    using Angle       = Degree;	

    using Degrees_per_second = atd::Degrees_per_second<atd::Float16>;
    using Angular_speed      = Degrees_per_second;

    using Direction = typename Driver::Direction;

// Cfg
    // Número de steps que da el motor para dar una vuelta
    // Generalmente 200 steps.
    static_assert(nsteps_per_turn > 0, "nsteps can't be negative");
    static constexpr Degree step_angle 
			    = Degree{360} / Degree::Rep{nsteps_per_turn};

// Movement
    static void turn(const Angle& a, const Angular_speed& w);

private:
// types
    using NSteps_t  = typename Driver::NSteps_t;				     
    using Frequency = typename Driver::Frequency;
    using Hertz     = typename Driver::Hertz;
    
};


template <typename C, int16_t N>
void Stepper_motor<C, N>::turn(const Angle& a, const Angular_speed& w)
{
    if (a > Angle{0})
	Driver::direction(Direction::positive);
    else
	Driver::direction(Direction::negative);

    Angle::Rep nsteps0 = a / step_angle;
    NSteps_t nsteps = nsteps0.integer_part<NSteps_t>(); // truncamos el Float16
    
    // TODO: 
    // switch (Driver::mode()){
    //	break; case Driver::Mode::full_step: 
    //	break; case Driver::Mode::half_step: nsteps *= 2;
    //	break; case Driver::Mode::quarter_step: nsteps *= 4;
    //	...
    // }

    atd::Float16 wf = w.value();
    atd::Float16 af = a.value();

    atd::Float16 nsteps_f{nsteps};
    atd::Float16 freq = (wf / af) * nsteps_f;
    typename Hertz::Rep f = freq.integer_part<typename Hertz::Rep>();
    Hertz freq_gen{f};

    Driver::step(freq_gen, nsteps);

}



// Motors
// ------
template <typename C>
using NEMA17 = Stepper_motor<C, 200>;

}// namespace
 
#endif


