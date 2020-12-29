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
    init_TWI();
    init_lcd();
    init_keyboard();
    init_rtc_clock();
}


void Main::init_TWI()
{
    TWI::on<TWI_frecuency>();
}

void Main::init_lcd()
{
    lcd_.screen().stop_brcorner(true);// I'm not going to use it as a terminal
    lcd_.screen().nowrap(); 
}


void Main::init_rtc_clock()
{
    RTC::Clock t;
    rtc.read(t);    // valor inicial que trae por defecto el RTC
    rtc.init(t);
}


void Main::window_set_time()
{
    RTC::Clock t;
    rtc.read(t);

    atd::Generic_time<RTC::Clock> gt{t};

    lcd_.clear();

    dev::user_get_time(lcd_, keyboard_, gt, 0, 0);
    wait_release_key();

    rtc.write(t);
//    if (rtc.error())
//	uart << "Error al intentar escribir la hora\n";

}



int main()
{
    Main app;
    app.run();
}



