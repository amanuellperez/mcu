// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

uint8_t Main::scan_keyboard()
{
    if (keyboard_.key<START_KEY>().is_pressed())
	return START_KEY;

    if (keyboard_.key<STOP_KEY>().is_pressed())
	return STOP_KEY;

    return NO_KEY;
}

// Observar que tiene prioridad 'start' sobre 'stop'
void Main::window_main()
{
    
    if (ktimer_.is_off()){
	uint8_t c = scan_keyboard();

	switch(c){
	    break; case START_KEY: Stopwatch::start();
	    break; case STOP_KEY : Stopwatch::stop();
	}

	if (c != NO_KEY) // si se ha pulsado una tecla esperemos el bouncing
	    ktimer_.on();
    }

    print_time();
}



void Main::print_time()
{
    scr_.cursor_pos(2,0);
    auto sexag = Stopwatch::sexagesimal_count();

    if constexpr (Font::cols <= 2){
	Font::print_number(scr_, sexag.minutes, nm::Width{2});
	Font::print_colon(scr_);
    }

    Font::print_number(scr_, sexag.seconds, nm::Width{2});
    Font::print_colon(scr_);
    Font::print_number(scr_, sexag.milliseconds, nm::Width{3});
}


