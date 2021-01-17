// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __SENSOR_MAIN_H__
#define __SENSOR_MAIN_H__

#include "dev.h"
#include "cfg.h"

class Main{
public:
    Main();
    void run();

private:
// Hardware
    LCD_ostream lcd_;
    Keyboard keyboard_;

    Sensor sensor_;
    RTC rtc_;


// init: hardware
    void init_TWI();
    void init_lcd();
    void init_keyboard() { }
    void init_sensor();
    void init_rtc_clock();


// Window: main
    void window_main();
    void show_window_main();

// Window : set_time
    void window_set_time();
    void window_set_time(RTC::Time_point& t);

// Helping functions
// user
    bool user_press_change_time();
    void wait_user_release_change_time();

// time
    void init_time(RTC::Time_point& t);
    void print_datetime(atd::Generic_time<RTC::Time_point> t, uint8_t x0, uint8_t y0);
    void user_get_datetime(atd::Generic_time<RTC::Time_point> t, uint8_t x0, uint8_t y0);

// sensor
    void print_sensor();

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
	wait_ms(100);
}

inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}


#endif

