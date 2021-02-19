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


#include "main.h"

void Main::run()
{
    show_window_main();

    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	wait_ms(100);
    }
}


void Main::window_main()
{
    bool redraw = false;

    if (ok_key_.is_pressed()){
	if (on_)
	    turn_off();
	else
	    turn_on();

	redraw = true;
    }

    if (keyboard_.key<UP_KEY>().is_pressed()){
	freq_gen_add(1);
	freq_gen_ += avr::Hertz{1};
	redraw = true;
    }

    if (keyboard_.key<DOWN_KEY>().is_pressed()){
	freq_gen_substract(1);
	freq_gen_ -= avr::Hertz{1};
	redraw = true;
    }

    if (redraw)
	show_window_main();

}


void Main::show_window_main()
{
    lcd_.clear();
    lcd_ << freq_gen_ << " Hz\n";

    if (on_)
	lcd_ << "ON";
    else
	lcd_ << "OFF";
}



