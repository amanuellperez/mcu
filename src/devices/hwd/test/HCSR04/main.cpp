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


#include <limits>
#include <avr_atmega.h>
#include <dev_miniclock.h>


// Micro
// -----
namespace mcu = atmega; 
using Micro   = mcu::Micro;
#include "../../dev_HCSR04.h" // TODO: posicion

// pins
constexpr uint8_t trigger_pin = 13;
constexpr uint8_t echo_pin    = 14;


// Devices
// -------
using Miniclock_us = dev::Miniclock_us<mcu::Micro, mcu::Time_counter1_g>;
using HCSR04_cfg = dev::HCSR04_cfg<Micro, Miniclock_us,
				   trigger_pin, echo_pin>;
using HCSR04 = dev::HCSR04<HCSR04_cfg>;


// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}




void hello()
{
    mcu::UART_iostream uart;
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

    mcu::UART_iostream uart;

    switch(res){
	break; case Result::ok:	uart << "ok\n";
	break; case Result::time_out:
			uart << "Time out\n";

    }	    

}

void test_sci_meter()
{
    mcu::UART_iostream uart;
    using Number = atd::Sci_number<uint8_t>;
    using Kilometer  = atd::Kilometer<Number>;
    using Meter      = atd::Meter<Number>;
    using Centimeter = atd::Centimeter<Number>;

    Meter m{2};
    uart << "meters = " << m << '\n';

    Centimeter cm = m;
    uart << "centimeters = " << cm << '\n';

    Kilometer km = m;
    uart << "kilometers = " << km << '\n';

    uart << "meters to centimeters";
    if(cm == Centimeter(Number(2).E(2)))
	uart << "OK\n";
    else 
	uart << "FAIL\n";

    uart <<  "meters to kilometers";

    if(km == Kilometer(Number(2).E(-3)))
	uart << "OK\n";
    else 
	uart << "FAIL\n";


    m = Meter(Number(117).E(4));
    m /= Number(1'000'000);

    uart << "meter / 10^6";
    if(m == Meter(Number(117).E(-2)))
	uart << "OK\n";
    else 
	uart << "FAIL\n";


}

int main()
{
    init_uart();
    HCSR04::init();

    hello();

    mcu::UART_iostream uart;

    while(1){
	auto distance = HCSR04::read();
	if (HCSR04::last_operation_is_ok()){
	    uart << "distance = " << distance << '\n';
	}
	else
	    print_result(HCSR04::result_last_operation());

uart << "\n\n----------------------------\n";
test_sci_meter();
uart << "----------------------------\n";
	Micro::wait_ms(1000);
    }

}



