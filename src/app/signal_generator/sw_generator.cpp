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


void Main::window_sw_generator()
{
    show_window_sw_generator();
    wait_release_key();

    while(1){
//	uint8_t c = keyboard_.read();
//
//	switch(c){
//	    case OK_KEY:
//	    case CANCEL_KEY:
//	    case UP_KEY:
//	    case DOWN_KEY:
//	}


	if (sw_generator_run()){
	    turn_off();
	    wait_release_key();
	    return;
	}

	wait_ms(time_scan_keyboard);
    }
}

// TODO: esta función mezcla 2 responsabilidades:
//	    1. scanea el teclado
//	    2. decide que hacer en función de ello.
//	Al crear keyboard_.read() se separarán y quedará bien.
bool Main::sw_generator_run()
{
    bool redraw = false;

    if (keyboard_.key<OK_KEY>().is_pressed()
	    and keyboard_.key<DOWN_KEY>().is_pressed()){
	return true;
    }

    if (ok_key_.is_pressed()){
	if (on_)
	    turn_off();
	else
	    turn_on();

	redraw = true;
    }


    if (keyboard_.key<UP_KEY>().is_pressed()){
	next_frequency();
	redraw = true;
    }

    if (keyboard_.key<DOWN_KEY>().is_pressed()){
	previous_frequency();
	redraw = true;
    }

    if (redraw)
	show_window_sw_generator();

    return false;
}


void Main::show_window_sw_generator()
{
    lcd_.clear();
    lcd_ << freq_gen_ << " Hz\n";

    if (on_)
	lcd_ << "ON";
    else
	lcd_ << "OFF";
}

