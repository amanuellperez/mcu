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

namespace dev{

// Cfg
// ---
// Nombres
template <uint8_t pin_NO_ENABLE,
	  uint8_t pin_NO_RESET,
	  uint8_t pin_NO_SLEEP>
struct A4988_NO_ENABLE_RESET_SLEEP{
    static constexpr uint8_t NO_ENABLE = pin_NO_ENABLE;
    static constexpr uint8_t NO_RESET  = pin_NO_RESET;
    static constexpr uint8_t NO_SLEEP  = pin_NO_SLEEP;
};

// pines MS1, MS2 y MS3
template <uint8_t pin_MS1,
	  uint8_t pin_MS2,
	  uint8_t pin_MS3>
struct A4988_MS123{
    static constexpr uint8_t MS1 = pin_MS1;
    static constexpr uint8_t MS2 = pin_MS2;
    static constexpr uint8_t MS3 = pin_MS3;
};

// Para que el motor avance un step, hay que enviarle un pulso. ¿Cómo generar
// ese pulso? Lo delego al cliente. El cliente puede usar un pin normal y
// bloquear el micro mientras genera los pulsos, o usar un SQG_pin del micro.
// Por eso paso como parámetro pin_SQG_step en lugar del número de pin.
template <typename pin_SQG_step,
	  uint8_t pin_DIR>
struct A4988_STEP_AND_DIR{
    using STEP = pin_SQG_step;
    static constexpr uint8_t DIR  = pin_DIR;
};

template <typename pin_STEP_AND_DIR>
//	  typename pin_MS123,
//	  typename pin_NO_ENABLE_RESET_SLEEP>
struct A4988_pins{
//    static constexpr uint8_t NO_ENABLE = pin_NO_ENABLE_RESET_SLEEP::NO_ENABLE;
//
//    static constexpr uint8_t MS1       = pin_MS123::MS1;
//    static constexpr uint8_t MS2       = pin_MS123::MS2;
//    static constexpr uint8_t MS3       = pin_MS123::MS3;
//
//    static constexpr uint8_t NO_RESET  = pin_NO_ENABLE_RESET_SLEEP::NO_RESET;
//    static constexpr uint8_t NO_SLEEP  = pin_NO_ENABLE_RESET_SLEEP::NO_SLEEP;

    using STEP = typename pin_STEP_AND_DIR::STEP;
    static constexpr uint8_t DIR       = pin_STEP_AND_DIR::DIR;
    
};



// Class
// -----

template <typename Micro0, typename Pins>
class A4988_basic {
public:
// Types
    using Micro = Micro0;

    // pins
    using STEP =  typename Pins::STEP;
    using DIR  =  typename Micro::Pin<Pins::DIR>;
    // TODO: añadir resto de pines

    using Direction = atd::Sign;
    using Frequency = typename STEP::SW_signal::Frequency;
    using NSteps_t  = STEP::NPulses_t;

// Constructor 
    A4988_basic() = delete; // static interface
    
    static void init();

// Motor
    static void direction(Direction);
    // gira (angulo, speed) con angulo + o -
    // gira (num_steps, speed)
    // da_vueltas(speed)    
    // stop(); deja de dar vueltas

// Para ver cómo funciona un motor y experimentar con un nuevo motor
// suministro la siguiente función:
//	Envía al motor nsteps pulsos a la frecuencia indicada.
    static void step(const Frequency& freq, const NSteps_t& nsteps);
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


