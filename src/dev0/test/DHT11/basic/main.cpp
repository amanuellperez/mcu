// Copyright (C) 2022 Manuel Perez 
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

// Este programa es para probar el Generic_timer_counter, aunque pone la base
// para programar el DHT11.

#include <atd_bit.h>
#include <avr_atmega.h>

constexpr uint8_t sensor_pin = 15;
using Counter = atmega::Time_counter0_g;
using Pin = atmega::Pin<sensor_pin>;


// Esperamos de 0 a 255 ms
void wait(const Counter::counter_type& t)
{
    Counter::reset();
    Counter::turn_on_with_clock_period_of<1024>::us();

    while (Counter::value() < t)
    { ; }
}


bool read()
{
    atmega::UART_iostream uart;

// 1. Pulso mínimo de 18 ms
    Pin::as_output();
    Counter::turn_on_with_clock_period_of<1024>::us(); // T_tick = 1024 us
    Counter::reset();
    Pin::write_zero();

    while (Counter::value() < 20) { ; } // 1.024 * 20 = 20.5 ms
					// pero el reloj del avr tiene un 10%
					// de incertidumbre (-10% son 18 ms +-)

    Pin::as_input_without_pullup();


// 2. Leemos la respuesta del sensor
    Counter::turn_on_with_clock_period_of<1>::us();	// T_tick = 1 us
    Counter::reset();

    // El sensor mantiene HIGH durante 20-40 us
    while (Pin::is_one()){
	if (Counter::value() > 60){
	    uart << "2. sensor HIGH > 60 us\n";
	    return false;
	}
    }


// Preambulo: pulso de 160 us (80 us LOW y 80 us HIGH)
// ---------------------------------------------------
    // todo lo que sigue lo medimos en microsegundos
    Counter::turn_on_with_clock_period_of<1>::us();

    Counter::reset();
    while (Pin::is_zero()){ // 80 +- 10% us = de 72 a 88 us
	if (Counter::value() > 100){
	    uart << "Pulso 80 us LOW > 100 us\n";
	    return false;
	}
    }

//    if (Counter::value() < 72)
//	return false;


    Counter::reset();
    while (Pin::is_one()){ // 80 +- 10% us = de 72 a 88 us
	if (Counter::value() > 100){
	    uart << "Pulso 80 us HIGH > 100 us\n";
	    return false;
	}
    }

//    if (Counter::value() < 72)
//	return false;


// Lectura: 50 us LOW, data HIGH 
// -----------------------------
    uint8_t data[5];

    for (uint8_t i = 0; i < 5; ++i){
	for (int8_t j = 7; j >= 0; --j){
	    // 50 us LOW
	    Counter::reset();
	    while (Pin::is_zero()){ // 50 +- 10% us = de 45 a 55 us
		if (Counter::value() > 70){
		    uart << "data zero > 70 us\n";
		    return false;
		}
	    }

	    // data: 28 us es 0, 70 us es 1
	    Counter::reset();
	    while (Pin::is_one()){ 
		if (Counter::value() > 90){
		    uart << "data one > 90 us\n";
		    return false;
		}
	    }

	    Counter::counter_type value = Counter::value();

	    // Al probarlo me da un valor mínimo de 18 us!!! 
	    // Según la datasheet tiene que ser de 26-28 us (con un 10% menos
	    // de incertidumbre del reloj del avr sería 23, muy lejos del
	    // 18!!!) ¿por qué?
	    if (10 <= value and value <= 35)
		atd::write_zero(data[i], j);

	    else if (60 <= value and value <= 80)
		atd::write_one(data[i], j);

	    else {
		uart << "data no valido:\n"
		     << "   Leyendo (" << (int) i << ", " << (int) j << ") = "  << (int) value << '\n';

		return false;
	    }
	}
    }

// Comprobamos que el checksum es correcto
// TODO: aclarar este checksum
    uint8_t check = (data[0] + data[1] + data[2] + data[3]) & 0xFF;

    if (check != data[4])
	return false;

    uart << "Recibido: " << (int) data[0]
	 << '.' << (int) data[1]
	 << "; " << (int) data[2]
	 << '.' << (int) data[3]
	 << '\n';

    return true;
}


int main()
{
    atmega::UART_iostream uart;
    atmega::basic_cfg(uart);
    uart.on();

    Counter::init();

    uart << "\n--------------\n";
    uart <<   "Testing DHT11!\n";
    uart <<   "--------------\n";

    uart << "Waiting 2 seconds for DHT11 to start working";
    for (uint8_t i = 0; i < 8; ++i){
	wait(250);
	uart << '.';
    }
    uart << " OK\n";


    while(1){
	if (!read())
	    uart << "Reading error\n";

	uart << "Waiting 1 second";
	for (uint8_t i = 0; i < 4; ++i){
	    wait(250);
	uart << '.';
    }
    uart << '\n';

    }
}



