// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __CHRONO_MAIN_H__
#define __CHRONO_MAIN_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Reloj de horno
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    30/01/2021 v0.0
 *
 ****************************************************************************/
#include "dev.h"
#include "cfg.h"

#include <avr_time.h>
#include <user_time.h>

// Experimento: utilización de estos 'Clocks' para controlar el bouncing del
// teclado.
class Clock{
public:
    Clock(uint8_t n)
    {
        if (n == 0)
            imax_ = 0;
        else 
	    imax_ = n - 1;
    }

    void reset() {i_ = 0;}
    void tick() 
    {
	if (i_ < imax_)
	    ++i_;
	else 
	    i_ = 0;
    }

    uint8_t count() {return i_;}

    // Al usar '0' al empezar empieza sonando.
    // El reloj suena cada vez que da una vuelta
    bool ring() {return i_ == 0;}

private:
    uint8_t i_  = 0;
    uint8_t imax_;
};

template <uint16_t time_in_ms>
class Main_clock{
public:
    Main_clock(Clock& clock) : clock_{&clock} { }

    void wait()
    {
	wait_ms(time_in_ms);// CUIDADO: wait_ms es una macro!!!
	clock_->tick();
    }

private:
   Clock* clock_; 
};



/*!
 *  \brief  Application.
 *
 */
class Main {
public:
    Main();
    void run();

private:
// Hardware
    Screen scr_;
    Keyboard keyboard_;

// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_stopwatch();


// Window: main
    void window_main();

    void print_time();


// keyboard
    uint8_t scan_keyboard();

    constexpr static uint8_t keyboard_clock_imax = 2; // 2 * 50 = 100
    constexpr static uint16_t period_main_clock_ms = 50;

    Clock keyboard_clock_;
    Main_clock<period_main_clock_ms> mclock_; // main_clock
};


inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}



#endif

