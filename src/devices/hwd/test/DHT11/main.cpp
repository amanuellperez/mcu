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
namespace mcu = atmega; 
using Micro   = mcu::Micro;

// pins
constexpr uint8_t sensor_pin = 15;


// Hwd Devices
// -----------
using DHT11 = dev::DHT11<Micro, sensor_pin>;

static constexpr uint8_t nmax_pulses = 42; // ACK + 5 bytes data + end
using Cfg_polling =
    dev::Train_of_pulses_poll_receiver_cfg<Micro,
				      sensor_pin, 
				      nmax_pulses>;

using Train_of_pulses_poll_receiver = 
	    dev::Train_of_pulses_poll_receiver<Cfg_polling>;
using Train_of_pulses = dev::Train_of_pulses<nmax_pulses>;

// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}


void print_received(const Train_of_pulses& pulse)
{
    mcu::UART_iostream uart;
    uart << "\n\nReceived " << pulse.size() << " pulses:\n"
	        "---------"    "-----"    "--------\n";

    uart << "Polarity = ";
    if (pulse.polarity())
	uart << "1\n"
	        "\tlow\thigh";
    else
	uart << "0"
	        "\thigh\tlow";

    uart << '\n';


    for (uint8_t i = 0; i < pulse.size(); ++i){
	uart << (int) i << '\t';

	if (pulse.polarity())
	    uart << pulse[i].time_low << '\t' << pulse[i].time_high;
	else
	    uart << pulse[i].time_high << '\t' << pulse[i].time_low;

	uart << '\n';
    }
}


void hello()
{
    mcu::UART_iostream uart;
    uart << "\n\nDHT11 test\n"
	        "----------\n"
		"Connect sensor to pin " << sensor_pin 
		<< " with a pull-up resistor of 4.7k\n";

    uart << "Waiting 1 seconds for DHT11 to start working ... ";
    Micro::wait_ms(1000);
    uart << "OK\n";
}

void print_error()
{
    mcu::UART_iostream uart;
    using Res = DHT11::Result;

    switch(DHT11::result_last_operation()){
	break; case Res::ok:	uart << "ok\n";
	break; case Res::train_of_pulses_bad_polarity:
			uart << "Bad polarity\n";

	break; case Res::train_of_pulses_bad_size:
			uart << "Bad size\n";
	break; case Res::bad_check_sum:
			uart << "Wrong check sum\n";
    }	    

}


int main()
{
    init_uart();
    hello();

    mcu::UART_iostream uart;

    while(1){

	Micro::wait_ms(1200);

	uint8_t data[5];
	if (!DHT11::basic_read(data)){
	    uart << "basic_read error\n";
	    print_error();
	}


    // print data
	uart << "basic:\tH = " << (int) data[0]
	     << '.' << (int) data[1] << "%\tT = "
	     << (int) data[2]
	     << '.' << (int) data[3] << "ºC\tcheck_sum = "
	     << (int) data[4]
	     << '\n';
	


	Micro::wait_ms(1200); // 1 segundo entre cada medida
	atd::Decimal<uint16_t, 2> T;
	atd::Decimal<uint16_t, 2> RH;
	if (!DHT11::read(T, RH)){
	    uart << "read error\n";
	    print_error();

	} else{
		uart << "read :\tH = " << RH << "%\t"
		     << "T = " << T << "ºC\n";
	}

    }

}



