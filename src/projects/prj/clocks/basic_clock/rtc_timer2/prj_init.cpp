// Copyright (C) 2023 Manuel Perez 
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


void Main::init_uart()
{
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}


Main::Main()
{
    show_menu_ = false;

    init_uart();

    Micro::sleep_mode(my_mcu::Sleep::mode::power_save);
    //Micro::sleep_mode(my_mcu::Sleep::mode::idle);

    if (Clock::turn_on() == false)
	uart << "Error: can't initialize Clock\n";

    Button::init();
    Button::enable_interrupt();

    Micro::enable_interrupts();


}

