// Copyright (C) 2024 Manuel Perez 
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

#include "../../dev_ULN2003.h"
#include <mega.h>

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
static constexpr uint8_t IN1_pin = 11;
static constexpr uint8_t IN2_pin = 12;
static constexpr uint8_t IN3_pin = 13;
static constexpr uint8_t IN4_pin = 14;


// Devices
// -------
using ULN2003_cfg = dev::ULN2003_basic_cfg<Micro, 
					   IN1_pin, IN2_pin, IN3_pin, IN4_pin>;

using ULN2003 = dev::ULN2003_basic<ULN2003_cfg>;


// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void hello()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nULN2003 test\n"
	        "------------\n"
		"TODO: sin acabar!!! De hecho solo es el principio\n"
		"Connections:\n"
		"\tIN1 = " << (int) IN1_pin << 
		"; IN2 = " << (int) IN2_pin << 
		"; IN3 = " << (int) IN3_pin << 
		"; IN4 = " << (int) IN4_pin << 
		"\n\n";
}


// Main
// ----
int main() 
{
    init_uart();
    ULN2003::init();

    hello();

    my_mcu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Step\n";

	char opt{};
	uart >> opt;

	uint8_t i = 0;
	switch (opt){
	    break; case '1': 
		for (i = 0; i < 200; ++i)
		ULN2003::step();

	    break; default: uart << "What???\n";

	}
    }
}


