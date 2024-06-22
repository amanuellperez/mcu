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


#include "main.h"
#include <dev_clocks.h>
#include <atd_time.h>

void Main::run()
{
    while(1){
	if (rtc_.error())
	    error();
	else 
	    window_main();

	my_mcu::Micro::wait_ms(100); // ¿se puede poner 500 ms? No, dejaría de funcionar el
		      // teclado ya que solo se miraría cada 500 ms si se
		      // ha pulsado una tecla o no.
    }
}




void Main::window_main()
{
    show_window_main();

    if (user_press_change_time())
    {
	wait_user_release_change_time();
	window_set_time();
    }
}


void Main::show_window_main()
{
    RTC::Time_point t;
    rtc_.read(t);
    print_time(atd::Date_time_view<RTC::Time_point>{t}, 0, 0);
}



void Main::error()
{
    lcd_.clear();
    lcd_ << "RTC error";
}
