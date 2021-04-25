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

void Main::window_main()
{
    show_window_main();

    while (1){
	bool update_screen = true;

	switch(scan_keyboard()){
	    case Key::ok:
		if (stroboscope_.is_on())
		    stroboscope_.off();
		else
		    stroboscope_.on();

		break;

	    case Key::down:
		stroboscope_.freq_minus(1);
		break;

	    case Key::up:
		stroboscope_.freq_plus(1);
		break;

	    case Key::none:
		update_screen = false;
		break;
	}

	if (update_screen)
	    show_window_main();

	wait_release_key();
    }
}


void Main::show_window_main()
{
    lcd_.clear();
    lcd_ << stroboscope_.freq() << " Hz\n";
    if (stroboscope_.is_on())
	lcd_ << "ON ";
    else
	lcd_ << "OFF";
}


