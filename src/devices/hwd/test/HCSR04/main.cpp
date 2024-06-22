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


#include "../../dev_HCSR04.h" 

#include <limits>
#include <avr_atmega.h>
#include <mcu_miniclock.h>

// Micro
// -----
namespace my_mcu = atmega; 
using Micro   = my_mcu::Micro;

// pins
constexpr uint8_t trigger_pin = 13;
constexpr uint8_t echo_pin    = 14;


// Devices
// -------
using Miniclock_us = mcu::Miniclock_us<my_mcu::Micro, my_mcu::Time_counter1_g>;
using HCSR04_cfg = dev::HCSR04_cfg<Micro, Miniclock_us,
				   trigger_pin, echo_pin>;
using HCSR04 = dev::HCSR04<HCSR04_cfg>;


// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}




void hello()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nHCSR04 test\n"
	        "-----------\n"
		"Connect:\n"
		"\techo pin to pin\t" << (int) echo_pin << 
		"\n\ttrigger pin to pin\t" << (int) trigger_pin 
		<< "\n\n";

}

void print_result(HCSR04::Result res)
{
    using Result = HCSR04::Result;

    my_mcu::UART_iostream uart;

    switch(res){
	break; case Result::ok:	uart << "ok\n";
	break; case Result::time_out:
			uart << "Time out\n";

    }	    

}


int main()
{
    init_uart();
    HCSR04::init();

    hello();

    my_mcu::UART_iostream uart;

    while(1){
	HCSR04::Meter m = HCSR04::read();
	if (HCSR04::last_operation_is_ok()){
	    uart << "distance = " << m;
	    // La aplicación correspondiente puede elegir el tipo de
	    // centímetros que usa
	    atd::Centimeter<atd::uFloat16> cm = m;
	    uart << " = " << cm << '\n';

	    
	}
	else
	    print_result(HCSR04::result_last_operation());

	Micro::wait_ms(1000);
    }

}



