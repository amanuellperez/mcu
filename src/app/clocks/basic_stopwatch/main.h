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

// Experimento: mejoremos el bouncing.
// Timer que cuenta ticks. Implementación mínima para la aplicación.
class Timer_ticks{
public:
    Timer_ticks(uint8_t imax) : imax_{imax}
    { i_ = 0; } // empezamos en off

    void on() {i_ = imax_;}
    void off() { i_ = 0; }

    bool is_off() {return i_ == 0;}

    void tick() 
    { 
	if (i_ > 0)
	    --i_; 
    }

private:
    uint8_t i_;	// i_ = 0, está en off. i_ != 0, en on.
    uint8_t imax_;
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
    void init_keyboard();
    void init_stopwatch();


// Window: main
    void window_main();

    void print_time();


// keyboard
    uint8_t scan_keyboard();

    constexpr static uint8_t keyboard_clock_imax = 2; // 2 * 50 = 100
    constexpr static uint16_t period_main_clock_ms = 50;

    Timer_ticks ktimer_; // keyboard_timer
};





#endif

