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

//    // No meto este wait en init_keyboard para que se vea explicitamente
//    wait_ms(200);   // Garantizamos que los botones dejen de hacer bouncing
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


void Main::init_time(RTC::Time_point& t)
{
    atd::Generic_time<RTC::Time_point> gt{t};

    gt.day(1);
    gt.month(1);
    gt.year(2021);

    gt.hours(12);
    gt.minutes(0);
    gt.seconds(0);

    // Que no se olvide encenderlo!!!
    t.clock_on =  true;
}


// precondition: que el LCD esté funcionando. 
void Main::init_rtc_clock()
{
    RTC::Time_point t;
    rtc_.read(t);    

    if (rtc_.error())
	return;

    if (!t.clock_on){
	lcd_.clear();
	lcd_ << "RTC v0.0";  // presentación y sirve para depurar
	wait_ms(500);
	init_time(t);
	window_set_time(t);
    }
}



void Main::window_set_time()
{
    RTC::Time_point t;
    rtc_.read(t);

    window_set_time(t);
}


void Main::window_set_time(RTC::Time_point& t)
{
    atd::Generic_time<RTC::Time_point> gt{t};

    lcd_.clear();

    print_time(gt, 0, 0);
    user_get_time(gt, 0, 0);

    rtc_.write(t);

    wait_release_key();

}



int main()
{
    Main app;
    app.run();
}



