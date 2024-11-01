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

#include "../../dev_DHT22.h"

#include <avr_atmega.h>

// Micro
// -----
namespace myu = atmega; 
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART>;

// pins
constexpr uint8_t sensor_pin = 15;


// Hwd Devices
// -----------
using DHT_protocol = dev::DHT_protocol;
using DHT22 = dev::DHT22<Micro, sensor_pin>;
using Result = DHT22::Result;

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}




void hello()
{
    UART_iostream uart;
    uart << "\n\nDHT22 test\n"
	        "----------\n"
		"Connect sensor to pin " << sensor_pin 
		<< " with a pull-up resistor of 4.7k\n";

    uart << "Waiting 1 seconds for DHT22 to start working ... ";
    Micro::wait_ms(1000);
    uart << "OK\n";
}

void print_error(Result res)
{
    UART_iostream uart;

    switch(res){
	break; case Result::ok:	uart << "ok\n";
	break; case Result::train_of_pulses_bad_polarity:
			uart << "Bad polarity\n";

	break; case Result::train_of_pulses_bad_size:
			uart << "Bad size\n";
	break; case Result::bad_check_sum:
			uart << "Wrong check sum\n";
    }	    

}


int main()
{
    init_uart();
    hello();

    UART_iostream uart;

    while(1){

	Micro::wait_ms(2000); // 2 segundos entre medida segun datasheet

	auto [T, H] = DHT22::read();

	if (DHT22::last_operation_fail()){
	    uart << "read error\n";
	    print_error(DHT22::result_last_operation());

	} else{
		uart << "read :\tH = " << H << "%\t"
		     << "T = " << T << "ºC\n";
	}

	Micro::wait_ms(2000); // 2 segundos entre medida segun datasheet

	auto T1 = DHT22::read_temperature();
	if (DHT22::result_last_operation() != Result::ok){
	    uart << "read error\n";
	    print_error(DHT22::result_last_operation());

	} else{
		uart << "read_temperature: " << T1 << " ºC\n";
	}

	Micro::wait_ms(2000); // 2 segundos entre medida segun datasheet

	auto H1 = DHT22::read_humidity();
	if (DHT22::result_last_operation() != Result::ok){
	    uart << "read error\n";
	    print_error(DHT22::result_last_operation());

	} else{
		uart << "read_humidity: " << H1 << " %\n";
	}
    }

}



