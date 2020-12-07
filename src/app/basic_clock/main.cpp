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

#include "main.h"

#include <user_time.h>

Main::Main()
{
// init_hardware():
    init_lcd();
    init_keyboard();
    init_system_clock();
}



void Main::init_lcd()
{
    lcd_.screen().stop_brcorner(true);// I'm not going to use it as a terminal
    lcd_.screen().nowrap(); 
}


void Main::init_system_clock()
{
    System_clock::init<system_clock_timer_period_in_us>();

    System_clock::time_point t0;    // = zero
    window_set_time(t0);
}


void Main::window_set_time(System_clock::time_point t0)
{
    lcd_.clear();

    t0 = dev::user_get_time(lcd_, keyboard_, t0, 0, 0);
    wait_release_key();

    System_clock::set(t0);
}



int main()
{
    Main app;
    app.run();
}



