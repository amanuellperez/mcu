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

#ifndef __DEV_A4988_H__
#define __DEV_A4988_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del driver del stepper motor A4988	
 *
 * HISTORIA
 *    Manuel Perez
 *    22/06/2024 Escrito
 *
 ****************************************************************************/
#include <atd_math.h>	// Sign

#include <mcu_pin.h>	

namespace dev{

// Cfg
// ---
// Admite una estructura con los números de pines correspondientes.
// Si no se pasa un pin se da por supuesto que está flotante.
template <typename Pins>
struct A4988_pins{
    using Pin = mcu::Pin_connection;
    static constexpr uint8_t NO_ENABLE(){ return Pin::floating;}
    static constexpr uint8_t NO_ENABLE()
		requires requires {Pins::NO_ENABLE;}
		{ return Pins::NO_ENABLE;}

    static constexpr uint8_t MS1() { return Pin::floating; }
    static constexpr uint8_t MS1()
		requires requires {Pins::MS1;}
		{ return Pins::MS1; }

    static constexpr uint8_t MS2() { return Pin::floating; }
    static constexpr uint8_t MS2()
		requires requires {Pins::MS2;}
		{ return Pins::MS2; }

    static constexpr uint8_t MS3() { return Pin::floating; }
    static constexpr uint8_t MS3()
		requires requires {Pins::MS3;}
		{ return Pins::MS3; }


    static constexpr uint8_t NO_RESET() { return Pin::floating; }
    static constexpr uint8_t NO_RESET()
		requires requires {Pins::NO_RESET;}
		{ return Pins::NO_RESET; }

    static constexpr uint8_t NO_SLEEP() { return Pin::floating; }
    static constexpr uint8_t NO_SLEEP()
		requires requires {Pins::NO_SLEEP;}
		{ return Pins::NO_SLEEP; }


    using STEP = typename Pins::STEP;
    static constexpr uint8_t DIR() { return Pins::DIR; }
    
};



// Class
// -----
template <typename Micro0, typename Pins_numbers>
class A4988_basic {
public:
// Types
    using Micro = Micro0;

    // pins
    using Pins = A4988_pins<Pins_numbers>;

    // de control de movimiento
    using STEP =  typename Pins::STEP;
    using DIR  =  typename Micro::Pin<Pins::DIR()>;

    // de configuración
    using NO_ENABLE = typename Micro::Pin<Pins::NO_ENABLE()>;
    using NO_RESET = typename Micro::Pin<Pins::NO_RESET()>;
    using NO_SLEEP = typename Micro::Pin<Pins::NO_SLEEP()>;

    // tipo de movimiento
    using MS1 = typename Micro::Pin<Pins::MS1()>;
    using MS2 = typename Micro::Pin<Pins::MS2()>;
    using MS3 = typename Micro::Pin<Pins::MS3()>;


    // types
    using Direction = atd::Sign;
    using Frequency = typename STEP::SW_signal::Frequency;
    using NSteps_t  = STEP::NPulses_t;

// Constructor 
    A4988_basic() = delete; // static interface
    
    static void init();

// Motor
    static void direction(Direction);

    // Envía al motor nsteps pulsos a la frecuencia indicada.
    static void step(const Frequency& freq, const NSteps_t& nsteps);

    // (???) ¿Tiene algún sentido estas dos funciones? 
    // step(Frequency){STEP::genera(freq);}; // el motor gira sin parar 
    // stop(){STEP::stop();};		     // se para el motor

// Cfg
    static void enable()
	    requires (mcu::Pin_connection::is_valid(NO_ENABLE::number));
private:
// Types
    using SW_signal = typename STEP::SW_signal;
};

template <typename M, typename P>
void A4988_basic<M, P>::init()
{
    DIR::as_output();
}


// DUDA: ¿hay que parar el motor antes de cambiar dirección? 
//	¿Qué pasa si con SQG el sofwador hace que el motor vaya dando pasos y
//	cambia la dirección? ¿Puede generar algún problema? Si el driver es
//	robusto, no. ¿Lo es?
template <typename M, typename P>
void A4988_basic<M, P>::direction(Direction dir) 
{
// stop() (???)
    if (dir == Direction::positive) 
	DIR::write_one();

    else
	DIR::write_zero();
}


template <typename M, typename P>
void A4988_basic<M, P>::step(const Frequency& freq, const NSteps_t& nsteps)
{
    SW_signal sw{freq};
    STEP::generate(sw, nsteps); 
}


}// namespace


#endif


