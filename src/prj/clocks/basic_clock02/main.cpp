// Copyright (C) 2022 A.Manuel L.Perez 
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
}



void Main::init_lcd()
{
    Font::load(scr_);
}


std::time_t Main::init_time_t()
{
    std::tm mt;
    std::memset(&mt, 0, sizeof(std::tm)); // fundamental inicializarlo todo

    atd::Generic_time_view<std::tm> gt{mt};

    gt.day(1);
    gt.month(1);
    gt.year(2021);

    gt.hours(12);
    gt.minutes(0);
    gt.seconds(0);

    return std::mktime(&mt);
}


void Main::init_system_clock()
{
    System_clock::init();

    time_t t0 = init_time_t();
    System_clock::time_point t = std::chrono::system_clock::from_time_t(t0);

    window_set_time(t);
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

	wait_ms(100);
    }
}



int main()
{
    Main app;
    app.run();
}



