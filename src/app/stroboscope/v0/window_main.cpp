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

#include <user_choose_number.h>

void Main::window_main()
{
    show_window_main();

    while (1){
	dev::user_choose_number_lineal(*this, lcd_, keyboard_).pos(0, 0)
				     .between(0, 5000)
				     .callback(&Main::choose_freq)
				     .choose4(stroboscope_.freq());

	// Se sale de choose_number cuando se pulsa OK
	stroboscope_.off();

	wait_ms(500);
	wait_release_key();
    }	
}


void Main::show_window_main()
{
    lcd_.cursor_pos(0,0);
    lcd_ << stroboscope_.freq() << "  Hz\n";
}


void Main::choose_freq(uint16_t freq)
{
    stroboscope_.freq(freq);
}
