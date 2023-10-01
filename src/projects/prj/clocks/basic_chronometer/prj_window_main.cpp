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

#include "prj_main.h"

#include <user_choose_number.h>

void Main::wait_press_key()
{
    wait_release_key();

    // Aquí sería mejor dormir el micro y despertarlo solo cuando se pulse la
    // tecla correspondiente. Habría que usar interrupciones con el keyboard_.
    while (keyboard_.key<OK_KEY>().is_not_pressed()){
	Micro::wait_ms(100);	// ¿por qué 100 ms?
    }
}


void Main::window_stop()
{
    wait_press_key();

}


void Main::window_reset()
{
    Chronometer::reset();
    print_time();

    wait_press_key();
}


void Main::window_running()
{
    Chronometer::start();
    print_time();

    wait_release_key();

    while(!keyboard_.key<OK_KEY>().is_pressed()){
	print_time();
//	Micro::wait_ms(100); // si se descomenta va mas despacio el lcd
    }

    Chronometer::turn_off();
    Micro::wait_ms(200);
}



void Main::print_time()
{
    lcd_.cursor_pos(0,0);

    auto t = Chronometer::now_as_time();

    lcd_.print(t.hours, nm::Width{2});
    lcd_.print(':');
    lcd_.print(t.minutes, nm::Width{2});
    lcd_.print(':');
    lcd_.print(t.seconds, nm::Width{2});
    lcd_.print(':');
    lcd_.print(t.milliseconds, nm::Width{3});
}


