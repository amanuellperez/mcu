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

#ifndef __BCLOCK_MAIN_H__
#define __BCLOCK_MAIN_H__


#include "dev.h"
#include "cfg.h"
#include <dev_clocks.h>
#include <avr_time.h>
#include <user_time.h>

/*!
 *  \brief  Application.
 *
 */
class Main {
public:
    Main();
    void run();

private:
// Types
    using Date_time = System_clock::Date_time;

// Hardware
    Screen scr_;
    Keyboard keyboard_;


// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_system_clock();
    void reset(Date_time&);

// Window: main
    void window_main();
    void show_window_main();

// Window : set_time
    void window_set_time();

// print_time
    void print(const Date_time& t, uint8_t x0, uint8_t y0);

// get_time
    void user_get_time(Date_time& t, uint8_t x0, uint8_t y0);
//    std::time_t user_get_time(const std::time_t& t0, uint8_t x0, uint8_t y0);
//    System_clock::time_point user_get_time(const System_clock::time_point& t0,
//              uint8_t x0, uint8_t y0);
};


inline void wait_release_key()
{
    mcu::Micro::wait_ms(time_wait_release_key);
}



#endif

