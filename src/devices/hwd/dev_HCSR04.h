// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_HCSR04_H__
#define __DEV_HCSR04_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor y driver del HC-SR04
 *
 * HISTORIA
 *    Manuel Perez
 *    16/10/2023 v0.0
 *
 ****************************************************************************/
#include <atd_minifloat.h>
#include <atd_magnitude.h>

namespace dev{

// (RRR) La datasheet indica que el sensor puede medir de 2cm a 4 m.
//       Suponiendo, como hace la datasheet, que la velocidad del sonido son
//       340 m/s, el rango de tiempos que tenemos que medir con el miniclock
//       es:
//	    0.04 m / 340 = 117 us
//	    8 m    / 340 = 23530 us
//	 Por eso el static_assert, para garantizar que el miniclock mide lo
//	 suficiente.
//
template <typename Micro0,
	  typename Miniclock_us0,
	  uint8_t trigger_pin0,
	  uint8_t echo_pin0>
struct HCSR04_cfg{
    using Micro = Micro0;
    using Miniclock_us = Miniclock_us0;
    using Trigger_pin = typename Micro::Pin<trigger_pin0>;
    using Echo_pin    = typename Micro::Pin<echo_pin0>;
    
    static_assert (Miniclock_us::counter_max() > 23600,
		   "Need to read at least 23600 us");
};


// TODO: ¿cómo se pueden pasarle los pines con nombre?
// Hacer lo que en el LCD HD44780 pero menos verbose.
// Ejemplo: HCSR04<Micro, {HCSR04::trigger_pin{13}, HCSR04::echo_pin{14}}>;
//
// No se hace dueño del miniclock de forma que otros devices puedan usarlo.
template <typename Cfg>
class HCSR04{
public:
// Types
    enum class Result{ok, time_out};
    using Meter      = atd::Meter<atd::uFloat16>; // mide hasta 4 metros

// Construction
    static void init();

    // TODO/DUDA: Los de arduino pasan la temperatura para hacer un cálculo
    // más preciso. ¿Merece la pena? Ver el % de error del sensor y el % de
    // error de la fórmula correspondiente y mirar si merece la pena
    // implementarlo o no. La fórmula es sencilla.
    static Meter read(uint16_t time_out_us = max_time_out_us);

// Gestión de errores
    static Result result_last_operation() {return result_;}
    static bool last_operation_is_ok() {return result_ == Result::ok;}
    static bool last_operation_fail() {return !last_operation_is_ok();}

private:
// Data
    inline static Result result_;

// Internal types
    using Micro       = Cfg::Micro;
    using Miniclock_us= Cfg::Miniclock_us;
    using Trigger_pin = Cfg::Trigger_pin;
    using Echo_pin    = Cfg::Echo_pin;

// Cfg
    static constexpr 
	uint16_t max_time_out_us = std::numeric_limits<uint16_t>::max();
    
    static constexpr uint16_t speed_sound = 340; // 340 m/s
						 
// Helpers
    static uint32_t read_time(uint16_t time_out_us = max_time_out_us);
};

template <typename Cfg>
void HCSR04<Cfg>::init()
{
    Trigger_pin::write_zero();
    Trigger_pin::as_output();

    Echo_pin::as_input_without_pullup();
}

// Precondicion: Se ha llamado a init()
// (observar que esta precondición se puede eliminar metiendo la llamada a
// init dentro de read. Eso sería útil si se va a multiplexar los pines. Pero
// ¿se multiplexará en la práctica? Que el uso nos lo diga)
template <typename C>
HCSR04<C>::Meter HCSR04<C>::read(uint16_t time_out_us)
{
    auto T = read_time(time_out_us); // = 2*t

    if (result_ != Result::ok)
	return Meter{0u};

    auto d = (speed_sound * T / 2);

    Meter m{d};
    m /= Meter::Rep{1'000'000}; 

    return m;
}


// Observar que si aquí se llama a init() cuando no se llame a read, se
// podrían usar los pines para otras cosas. ¿Podría multiplexar los pines y
// conectarlos a diferentes dispositivos? Eso es habitual? No? 
template <typename C>
uint32_t HCSR04<C>::read_time(uint16_t time_out_us)
{
    result_ = Result::ok;

    Miniclock_us::init();

    typename Micro::Disable_interrupts lock;

// pulso de 10us
    Trigger_pin::write_one();
    Micro::wait_us(10);
    Trigger_pin::write_zero();

// Wait to read
    Miniclock_us::start();
    while (Echo_pin::is_zero()){
	if ( Miniclock_us::time() >= time_out_us){
	    result_ = Result::time_out;
	    return 0;
	}
     }

// read
    Miniclock_us::start();

    while (Echo_pin::is_one()){
	if (Miniclock_us::time() >= time_out_us){
	    result_ = Result::time_out;
	    return 0;
	}
    }

    Miniclock_us::stop();

    return Miniclock_us::time();
}


}// dev

#endif


