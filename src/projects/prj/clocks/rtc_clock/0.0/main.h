// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __RTC_CLOCK_MAIN_H__
#define __RTC_CLOCK_MAIN_H__


#include "dev.h"
#include "cfg.h"
#include <mcu_clock.h>
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
    LCD lcd_;
    Keyrow keyboard_;
    RTC rtc_;


// init: hardware
    void init_TWI();
    void init_lcd();
    void init_keyboard() { }
    void init_rtc_clock();
    void init_time(RTC::Time_point& t);

// Window: main
    void window_main();
    void show_window_main();

// Window : set_time
    void window_set_time();
    void window_set_time(RTC::Time_point& t);

// Helping functions
    void print_time(atd::Date_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0);
    void user_get_time(atd::Date_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0);

    bool user_press_change_time();
    void wait_user_release_change_time();

    void error();

};


inline bool Main::user_press_change_time()
{
    return (keyboard_.key<OK_KEY>().is_pressed()
	and keyboard_.key<DOWN_KEY>().is_pressed());
}

inline void Main::wait_user_release_change_time()
{
    while (keyboard_.key<OK_KEY>().is_pressed()
	 or keyboard_.key<DOWN_KEY>().is_pressed())
	my_mcu::Micro::wait_ms(100);
}


inline void wait_release_key()
{
    my_mcu::Micro::wait_ms(time_wait_release_key);
}



#endif

