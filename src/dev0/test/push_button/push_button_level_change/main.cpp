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

#include "../../../dev_push_button.h"
#include <avr_atmega.h>

namespace mcu = atmega;

// Conectar un led al pin 15 y un pulsador al pin 23
// Al pulsar el pulsador se enciende el led y al soltarlo se apaga
int main()
{
// init_uart()
    atmega::UART_iostream uart;
    atmega::basic_cfg(uart);
    uart.on();

    dev::Push_button_level_change<23> push_button;

    while(1){
	if (push_button.is_pressed())
	    uart << "press\n";
	else 
	    uart << "not press\n";

	mcu::wait_ms(500);

    }
}



