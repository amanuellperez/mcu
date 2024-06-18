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


void Main::sound_init()
{
    freq_gen_ = sound_generator_freq0;
    turn_off();

    lcd_.clear();
    sound_lcd_refresh();

    wait_release_key();
}

void Main::sound_main()
{
    sound_init();
    sound_run();
}


void Main::sound_run()
{
    constexpr Scalar factor = Scalar::from_internal_value(1059);

    while(1){
	bool redraw = true;

	Key key = generator_scan_keyboard();

	switch (key){
	    case Key::ok:
		if (on_)
		    turn_off();
		else
		    turn_on();
		break;

	    case Key::up:
		multiply_frequency_by(factor);
		break;

	    case Key::down:
		divide_frequency_by(factor);
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
	    sound_lcd_refresh();

	wait_ms(time_scan_keyboard);

    }
}

void Main::sound_lcd_refresh()
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

