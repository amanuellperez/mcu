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
#include <dev_system_clock.h>
#include <atd_time.h>

void Main::run()
{
    while(1){
//	if (errno_)
//	    error();
//	else 
	    pantalla_principal();

	wait_ms(100);
    }
}


void Main::pantalla_principal()
{
    show_pantalla_principal();

    if (keyboard_.key<OK_KEY>().is_pressed())
	pantalla_cambio_hora(System_clock::now());
}


void Main::show_pantalla_principal()
{
    print(System_clock::now());
}


void Main::print(const System_clock::time_point& t0)
{
    std::time_t sec = System_clock::to_time_t(t0);
    std::tm t;
    ::gmtime_r(&sec, &t);

    lcd_.cursor_pos(0,0);
    lcd_ << atd::only_date(t);
    lcd_.cursor_pos(0,1);
    lcd_ << atd::only_time(t);
}


