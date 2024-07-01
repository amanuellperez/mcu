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
 * TODO
 *	El problema de parametrizar los drivers es que duplicamos código:
 *	    using Driver1 = ...
 *	    using Driver2 = ...
 *
 *	    using Motor1 = dev::Stepper_motor<Driver1>;
 *	    using Motor2 = dev::Stepper_motor<Driver2>;
 *
 *	Como los Driver1 y Driver2 son clases distintas, Motor1 y Motor2 lo
 *	son también duplicando código de forma innecesaria.
 *
 *	PROBLEMA: manteniendo el interfaz que tiene ahora Stepper_motor evitar
 *	la duplicación de código (¿puedo usar herencia? ¿wraper?)
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
template <typename Driver0, int16_t nsteps_per_turn0>
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

    using Mode      = typename Driver::Mode;

// Cfg
    // Número de steps que da el motor para dar una vuelta
    // Generalmente 200 steps.
    static_assert(nsteps_per_turn0 > 0, "nsteps can't be negative");

    static int16_t nsteps_per_turn();
    static Degree step_angle();

    // DUDA: 
    // (1) static void enable()  requires requires {Driver::enable();};
    // versus
    // (2) static void enable();
    // La versión (1) tiene la ventaja de que el cliente puede hacer
    // compilación condicional dependiendo de si está definida enable o no.
    // La segunda tiene la ventaja de que si se intenta usar Motor::enable()
    // y no está definida en el Driver, el compilador muestra los requirements
    // del Driver. En este caso sería "NO_ENABLE::is_a_valid_pin" dando una
    // pista al programador de por qué no esta definida (cosa que la primera
    // no hace).
    // De momento experimento con la (2) porque estoy seguro de que al
    // escribir dentro de unos meses una aplicacion usando motores, no me
    // acordaré de si puedo usar enable o no, y agradeceré que el compilador me
    // diga que no he conectado el pin correspondiente.
    // enable
    static void enable()  {Driver::enable();}
    static void disable() {Driver::disable();}

    // sleep
    static void sleep() {Driver::sleep();}
    static void awake() {Driver::awake();}

    // engage
    static void engage()    {Driver::engage();}
    static void disengage() {Driver::disengage();}

    // mode
    static Mode mode() {return Driver::mode();}

    static void full_step_mode()	{ Driver::full_step_mode();}
    static void half_step_mode()	{ Driver::half_step_mode();}
    static void quarter_step_mode()	{ Driver::quarter_step_mode();}
    static void eighth_step_mode()	{ Driver::eighth_step_mode();}
    static void sixteenth_step_mode()	{ Driver::sixteenth_step_mode();}


// Movement
    static void turn(const Angle& a, const Angular_speed& w);


// -------
// Depurar
    static std::pair<Frequency, NSteps_t> 
		    angle2freq(const Angle& a, const Angular_speed& w);

    static Direction angle2direction(const Angle& a, const Angular_speed& w);

private:
// Cfg
    static constexpr int16_t nsteps_per_turn_ = nsteps_per_turn0;
    
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

    Angle::Rep nsteps0 = a / step_angle();
    NSteps_t nsteps = nsteps0.integer_part<NSteps_t>(); // truncamos el Float16
    
    atd::Float16 wf = w.value();
    atd::Float16 af = a.value();

    atd::Float16 freq = (wf / af) * atd::Float16{nsteps};
    typename Hertz::Rep f = freq.integer_part<typename Hertz::Rep>();

    return {Hertz{f}, nsteps};
}

template <typename C, int16_t N>
int16_t Stepper_motor<C, N>::nsteps_per_turn()
{
    switch (Driver::mode()){
	break; case Driver::Mode::full_step	: return 1*nsteps_per_turn_;
	break; case Driver::Mode::half_step	: return 2*nsteps_per_turn_;
	break; case Driver::Mode::quarter_step	: return 4*nsteps_per_turn_;
	break; case Driver::Mode::eighth_step	: return 8*nsteps_per_turn_;
	break; case Driver::Mode::sixteenth_step: return 16*nsteps_per_turn_;
	break; default: return 0; // aquí nunca debería de llegar
    }
}


// (FUTURO???) Cada vez que llamamos a step_angle() estamos recalculándolo.
// Pero este número solo varía si varia el modo. Sería más eficiente en tiempo
// almacenar step_angle y solo actualizarlo cuando el usuario actualice el
// modo. ¿Merecerá la pena?
template <typename C, int16_t N>
inline auto Stepper_motor<C, N>::step_angle() -> Degree
{ return Degree{360} / (Degree::Rep{nsteps_per_turn()}); }


template <typename C, int16_t N>
void Stepper_motor<C, N>::turn(const Angle& a, const Angular_speed& w)
{
    Driver::direction(angle2direction(a, w));

    auto [freq_gen, nsteps] = angle2freq(a, w);

    Driver::step(freq_gen, nsteps);
}



// Motors
// ------
template <typename Driver>
using NEMA17 = Stepper_motor<Driver, 200>;

}// namespace
 
#endif


