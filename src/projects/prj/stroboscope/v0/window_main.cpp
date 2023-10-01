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

#include <user_choose_number.h>

void Main::window_main()
{
    while (1){
	show_window_main();

        dev::user_choose_number_lineal(*this, lcd_, keyboard_)
            .pos(0, 0)
            .between(stroboscope_.freq_min, stroboscope_.freq_max)
            .callback(&Main::choose_freq)
            .choose4(stroboscope_.freq());

        // Se sale de choose_number cuando se pulsa OK
	stroboscope_.off();

	//show_window_main_off();

	wait_release_key();
    }	
}


void Main::show_window_main()
{
    lcd_.cursor_pos(0,0);
    atd::print(lcd_, stroboscope_.freq(), nm::Width{4});
    lcd_ << " Hz\n";
    //lcd_ << std::setw(4) << stroboscope_.freq() << " Hz\n";
    lcd_.cursor_pos(0,1);
    lcd_ << "   ";

}

void Main::show_window_main_off()
{
    lcd_.cursor_pos(0,0);
    atd::print(lcd_, stroboscope_.freq(), nm::Width{4});
    lcd_ << " Hz\n";
    //lcd_ << std::setw(4) << stroboscope_.freq() << " Hz\n";
    lcd_.cursor_pos(0,1);
    lcd_ << "OFF";
}

void Main::choose_freq(uint16_t freq)
{
    stroboscope_.freq(freq);
}
