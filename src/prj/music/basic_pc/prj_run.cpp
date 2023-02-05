// Copyright (C) 2022 Manuel Perez 
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

#include "prj_main.h"


void Main::run()
{
    UART uart;
    uart << "\n\nTesting music functions\n"
	    "Connect a buzzer (or speaker) to pin " << (int) speaker_pin;

    while (1){
	uart << "\n-----------------------\n"
		"1. Music scale (normal)\n"
	        "2. Music scale (all)\n"
		"3. Organ toy (basic piano? :)\n"
		"4. Play a song\n"
		"5. Sirens\n";

	char ans{};
	uart >> ans;
	switch(ans){
	    break; case '1': musical_scale_one_line();
	    break; case '2': musical_scale_all();
	    break; case '3': organ_toy();
	    break; case '4': play_song();
	    break; case '5': play_siren();
	}

    }
}


