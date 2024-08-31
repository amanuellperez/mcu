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

#include "../../mcu_SPI.h"
#include <avr_atmega.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
constexpr uint8_t npin0 = 11;
constexpr uint8_t npin1 = 12;
constexpr uint8_t npin2 = 13;
constexpr uint8_t npin3 = 14;

// Devices
// -------
using SPI_array = mcu::SPI_pin_array_selector<Micro, npin0, npin1, npin2, npin3>;


// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}



void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nSPI_pin_array_selector test\n"
	        "----------------------------\n"
		"Connections:\n"
		"\tConnect a LED with R = 10k to pins " 
		<< (int) npin0 << ", " << (int) npin1  << ", "
		<< (int) npin2 << " and " << (int) npin3
		<< "\n\n";
}



// Main
// ----
int main()
{
    init_uart();

    hello();
    
    SPI_array::init();
    myu::UART_iostream uart;

    // select es write_zero. Dejamos los leds apagados
    SPI_array::select<0>();
    SPI_array::select<1>();
    SPI_array::select<2>();
    SPI_array::select<3>();
    while(1){
// static interface
// ----------------
	uart << "(1) LED " << (int) npin0 << " blinking 4 times ... ";
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array::deselect<0>();
	    Micro::wait_ms(500);
	    SPI_array::select<0>();
	}
	uart << "OK\n";

	uart << "(1) LED " << (int) npin1 << " blinking 4 times ... ";
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array::deselect<1>();
	    Micro::wait_ms(500);
	    SPI_array::select<1>();
	}
	uart << "OK\n";

	uart << "(1) LED " << (int) npin2 << " blinking 4 times ... ";
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array::deselect<2>();
	    Micro::wait_ms(500);
	    SPI_array::select<2>();
	}
	uart << "OK\n";

	uart << "(1) LED " << (int) npin3 << " blinking 4 times ... ";
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array::deselect<3>();
	    Micro::wait_ms(500);
	    SPI_array::select<3>();
	}
	uart << "OK\n";

// dynamic interface
// -----------------
	uart << "(2) LED " << (int) npin0 << " blinking 4 times ... ";
	SPI_array::deselect<0>();
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array spi{0};
	    Micro::wait_ms(500);
	}
	SPI_array::select<0>();
	uart << "OK\n";

	uart << "(2) LED " << (int) npin1 << " blinking 4 times ... ";
	SPI_array::deselect<1>();
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array spi{1};
	    Micro::wait_ms(500);
	}
	SPI_array::select<1>();
	uart << "OK\n";


	uart << "(2) LED " << (int) npin2 << " blinking 4 times ... ";
	SPI_array::deselect<2>();
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array spi{2};
	    Micro::wait_ms(500);
	}
	SPI_array::select<2>();
	uart << "OK\n";

	uart << "(2) LED " << (int) npin3 << " blinking 4 times ... ";
	SPI_array::deselect<3>();
	for (uint8_t i = 0; i < 4; ++i){
	    Micro::wait_ms(500);
	    SPI_array spi{3};
	    Micro::wait_ms(500);
	}
	SPI_array::select<3>();
	uart << "OK\n";
    }
}




