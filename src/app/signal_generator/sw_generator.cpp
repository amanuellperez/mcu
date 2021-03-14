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

#include <atd_eng_magnitude.h>

void Main::window_sw_generator()
{
    show_first_window_sw_generator();
    wait_release_key();

    while(1){
	bool redraw = true;

	uint8_t c = sw_generator_scan_keyboard();
	switch (c){
	    case OK_KEY:
		if (on_)
		    turn_off();
		else
		    turn_on();

		break;

	    case UP_KEY:
		next_frequency();
		break;

	    case DOWN_KEY:
		previous_frequency();
		break;

	    case CANCEL_KEY:
		turn_off();
		wait_release_key();
		return;

	    default:
		redraw = false;
		break;
	}

	if (redraw)
	    show_window_sw_generator();

	wait_ms(time_scan_keyboard);
    }
}


uint8_t Main::sw_generator_scan_keyboard()
{
    if (keyboard_.key<OK_KEY>().is_pressed()
	    and keyboard_.key<DOWN_KEY>().is_pressed()){
	return CANCEL_KEY;
    }

    if (ok_key_.is_pressed())
	return OK_KEY;

    if (keyboard_.key<DOWN_KEY>().is_pressed())
	return DOWN_KEY;

    if (keyboard_.key<UP_KEY>().is_pressed())
	return UP_KEY;

    return NO_KEY;

}

void Main::show_first_window_sw_generator()
{
    lcd_.clear();

    lcd_.cursor_pos(0,1);
    lcd_ << atd::to_eng_magnitude<uint16_t>(speaker_.min_frequency())
         << ", "
	 << atd::to_eng_magnitude<uint16_t>(speaker_.max_frequency());

    show_window_sw_generator();
}


void Main::show_window_sw_generator()
{
    lcd_.cursor_pos(0,0);

    lcd_ << freq_gen_ << "   ";

    lcd_.cursor_pos(13, 0);
    if (on_)
	lcd_ << "ON ";
    else
	lcd_ << "OFF";

}

