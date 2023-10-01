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

#include "main.h"
#include "time.h"

#include <user_time.h>
#include <cstring>  // memset

/***************************************************************************
 *				  INIT
 ***************************************************************************/
Main::Main()
{
// init_hardware():
    init_lcd();
    init_keyboard();

    init_system_clock();

    Micro::enable_interrupts();
}



void Main::init_lcd()
{
    Font::load(scr_);
}


void Main::reset(Date_time& t)
{
    t.day(1);
    t.month(1);
    t.year(2021);

    t.hours(12);
    t.minutes(0);
    t.seconds(0);
}


void Main::init_system_clock()
{
    System_clock::turn_on();
    Date_time t;
    reset(t);
    System_clock::set(t);

    window_set_time();
}



/***************************************************************************
 *				    MAIN
 ***************************************************************************/
void Main::run()
{
    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	Micro::wait_ms(100);
    }
}



int main()
{
    Main app;
    app.run();
}



