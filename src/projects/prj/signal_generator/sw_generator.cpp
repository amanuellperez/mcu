// Copyright (C) 2021 Manuel Perez 
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

#include <atd_eng_magnitude.h>
#include "metronome.h"

void Main::window_sw_generator()
{
    show_first_window_sw_generator();
    wait_release_key();


    Metronome<avr::Frequency, metronome_d1, metronome_d0> metronome;

    Key old_key = Key::none;

    while(1){
	bool redraw = true;

	Key key = generator_scan_keyboard();

	if (old_key != key)
	    metronome.reset();
	else if (key != Key::none)
	    metronome.tick();

	old_key = key;

	switch (key){
	    case Key::ok:
		if (on_)
		    turn_off();
		else
		    turn_on();

		break;

	    case Key::up:
		if (metronome.trigger())
		    next_frequency(metronome.incr());
		break;

	    case Key::down:
		if (metronome.trigger())
		    previous_frequency(metronome.incr());
		break;

	    case Key::cancel:
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


Main::Key Main::generator_scan_keyboard()
{
    if (keyboard_.key<OK_KEY>().is_pressed()
	    and keyboard_.key<DOWN_KEY>().is_pressed()){
	return Key::cancel;
    }

    if (ok_key_.is_pressed())
	return Key::ok;

    if (keyboard_.key<DOWN_KEY>().is_pressed())
	return Key::down;

    if (keyboard_.key<UP_KEY>().is_pressed())
	return Key::up;

    return Key::none;

}

void Main::show_first_window_sw_generator()
{
    lcd_.clear();

    lcd_.cursor_pos(0,1);
    print_without_decimals(lcd_, speaker_.min_frequency());
    lcd_ << ",";
    print_without_decimals(lcd_, speaker_.max_frequency());

    show_window_sw_generator();
}


void Main::show_window_sw_generator()
{
    lcd_.cursor_pos(0,0);

    print(lcd_, freq_gen_);
    lcd_ << "   ";

    lcd_.cursor_pos(13, 0);
    if (on_)
	lcd_ << "ON ";
    else
	lcd_ << "OFF";

}

