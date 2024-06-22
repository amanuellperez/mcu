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

#include "../../dev_DHT11.h"

#include <avr_atmega.h>
#include <dev_train_of_pulses.h>

// Micro
// -----
namespace my_mcu = atmega; 
using Micro   = my_mcu::Micro;

// pins
constexpr uint8_t sensor_pin = 15;


// Hwd Devices
// -----------
using DHT_protocol = dev::DHT_protocol;
using DHT11 = dev::DHT11<Micro, sensor_pin>;
using Result = DHT11::Result;

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
    uart << "\n\nDHT11 test\n"
	        "----------\n"
		"Connect sensor to pin " << sensor_pin 
		<< " with a pull-up resistor of 4.7k\n";

    uart << "Waiting 1 seconds for DHT11 to start working ... ";
    Micro::wait_ms(1000);
    uart << "OK\n";
}

void print_error(Result res)
{
    my_mcu::UART_iostream uart;

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

    my_mcu::UART_iostream uart;

    while(1){

	Micro::wait_ms(1200);

	uint8_t data[5];
	Result res = DHT_protocol::basic_read<Micro, sensor_pin, 20>(data);
	if (res != Result::ok){
	    uart << "basic_read error\n";
	    print_error(res);
	}


    // print data
	uart << "basic:\tH = " << (int) data[0]
	     << '.' << (int) data[1] << "%\tT = "
	     << (int) data[2]
	     << '.' << (int) data[3] << "ºC\tcheck_sum = "
	     << (int) data[4]
	     << '\n';
	


	Micro::wait_ms(1000); // 1 segundo entre cada medida

	auto [T, H] = DHT11::read();

	if (DHT11::last_operation_fail()){
	    uart << "read error\n";
	    print_error(DHT11::result_last_operation());

	} else{
		uart << "read :\tH = " << H << "%\t"
		     << "T = " << T << " ºC\n";
	}

	Micro::wait_ms(1000); // 1 segundos entre medida segun datasheet

	auto T1 = DHT11::read_temperature();
	if (DHT11::result_last_operation() != Result::ok){
	    uart << "read error\n";
	    print_error(DHT11::result_last_operation());

	} else{
		uart << "read_temperature: " << T1 << " ºC\n";
	}

	Micro::wait_ms(1000); // 1 segundos entre medida segun datasheet

	auto H1 = DHT11::read_humidity();
	if (DHT11::result_last_operation() != Result::ok){
	    uart << "read error\n";
	    print_error(DHT11::result_last_operation());

	} else{
		uart << "read_humidity: " << H1 << " %\n";
	}



    }

}



