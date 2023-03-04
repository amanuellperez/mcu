// Copyright (C) 2019-2020 Manuel Perez 
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

// Conectar un led al pin 14, y un pulsador
// entre resistencias al pin 4

#include "../../avr_atmega328p_cfg.h"
#include "../../avr_pin.h"
#include "../../avr_micro.h"
#include "../../avr_UART_iostream.h"

namespace mcu = avr_;
using Micro = mcu::Micro;

constexpr uint8_t num_pin = 15;


void blink()
{
    mcu::Pin<num_pin> pin;
    pin.as_output();

    mcu::UART_iostream uart;
    for (uint8_t i = 0; i < 10; ++i){
	uart << '.';

	pin.write_one();
	Micro::wait_ms(500);

	pin.write_zero();
	Micro::wait_ms(500);
    }
    uart << " DONE\n";
}


void test_output_pin()
{
    mcu::UART_iostream uart;
    uart << "Output_pin test\n";

    mcu::Output_pin<num_pin> pin;

    uart << "\n---------------\n"
	    "0. Write zero\n"
	    "1. Write one\n"
	    "2. Read value\n"
	    "3. return\n";

    while(1){
	char ans{};
	uart >> ans;
	switch(ans){
	    break; case '0': pin.write_zero();
	    break; case '1': pin.write_one();
	    break; case '2': 
		    uart << "\nRead:"
			    "\n\tpin.is_zero() = " << (int) pin.is_zero()
			 << "\n\tpin.is_one() = " << (int) pin.is_one()
			 << "\n\tpin.read() = " << (int) pin.read() 
			 << '\n';

	    break; case '3': return;
	}
    }

}


int main()
{
// uart_init();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

// menu
    uart << "\n--------\n"
	      "Pin test\n"
	      "--------\n";

    while (1){
	uart << "\nMenu\n"
		"1. Blink LED pin number " << (int) num_pin << '\n' <<
		"2. Test Output_pin\n\n";

	char ans{};
	uart >> ans;
	switch(ans){
	    break; case '2'	: test_output_pin();
	    break; default	: blink();
	}
    }
}


