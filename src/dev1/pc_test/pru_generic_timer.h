// Copyright (C) 2022 Manuel Perez 
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

#ifndef __PRU_GENERIC_TIMER_H__
#define __PRU_GENERIC_TIMER_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Generic timers para probar dev1.
 *
 * HISTORIA
 *    Manuel Perez
 *    10/12/2022 Escrito
 *
 ****************************************************************************/
namespace pru{ // de pruebas

class Timer_generic_counter{
public:
    using counter_type = int;

    static void init() 
    { 
	reset(); 
	off(); 
    }

    template <int period_in_us>
    static void on() 
    {
	on_ = true;

	if constexpr (period_in_us == 1024)
	    incr_ = 1000;

	else 
	    incr_ = period_in_us;
    }

    static void off() {on_ = false;}
    
    static void reset() {counter_ = 0;}

    static counter_type value() { return counter_;}

// Funciones para poder simular el Timer
    static void tick() {counter_ += incr_;}

private:
    inline static bool on_ = false;

    inline static counter_type counter_= 0; 
    inline static counter_type incr_ = 1;
};

} // namespace

#endif


