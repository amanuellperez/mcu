// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#include <dev_system_clock.h>
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
// Hardware
    LCD_ostream lcd_;
    Keyboard keyboard_;


// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_system_clock();
    std::time_t init_time_t();

// Window: main
    void window_main();
    void show_window_main();

// Window : set_time
    void window_set_time(System_clock::time_point t0);

// print_time
    void print(const System_clock::time_point& t0);
    void print(atd::Generic_time<std::tm> t, uint8_t x0, uint8_t y0);

// get_time
    void user_get_time(atd::Generic_time<std::tm> t, uint8_t x0, uint8_t y0);
    std::time_t user_get_time(const std::time_t& t0, uint8_t x0, uint8_t y0);
    System_clock::time_point user_get_time(const System_clock::time_point& t0,
              uint8_t x0, uint8_t y0);
};


inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}



#endif

