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

#include "../../../dev_push_button.h"
#include <avr_atmega.h>

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// Pin connections
// ---------------
static constexpr uint8_t button_pin = 14;


// Devices
// -------
using Button = dev::Push_button_level_change<Micro, button_pin>;


// Main
// ----
int main()
{
// init_uart()
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();

    Button button;

    while(1){
	if (button.is_pressed())
	    uart << "press\n";
	else 
	    uart << "not press\n";

	Micro::wait_ms(500);

    }
}



