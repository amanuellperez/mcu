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
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

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
    UART_iostream uart;
    myu::UART_basic_cfg<baud_rate>();
    uart.turn_on();
}



void hello()
{
    UART_iostream uart;
    uart << "\n\nSPI_pin_array_selector test\n"
	        "----------------------------\n"
		"Connections:\n"
		"\tConnect a LED with R = 10k to pins " 
		<< (int) npin0 << ", " << (int) npin1  << ", "
		<< (int) npin2 << " and " << (int) npin3
		<< "\n\n";
}

void test_SPI_pin_array_selector()
{
    UART_iostream uart;
    uart << "SPI_pin_array_selector test\n"
	    "---------------------------\n";

    // select es write_zero. Dejamos los leds apagados
    SPI_array::select<0>();
    SPI_array::select<1>();
    SPI_array::select<2>();
    SPI_array::select<3>();

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
void test_SPI_selector_with_deselect_delay()
{
    UART_iostream uart;
    uart << "SPI_selector_with_deselect_delay test\n"
	    "-------------------------------------\n";

    using SPI_selector = mcu::SPI_selector_with_deselect_delay<SPI_array, 500'000>;


// init
// ----
    uart << "init: check all the pins are HIGH\n";
    SPI_selector::init();
    uart << "Press key to continue";
    char c{};
    uart >> c;
    uart << '\n';

// dynamic interface
// -----------------
    uart << "(1) LED " << (int) npin0 << " blinking 4 times ... ";
    for (uint8_t i = 0; i < 4; ++i){
	Micro::wait_ms(500);
	SPI_selector spi{0};
	// el destructor hace wait_us(500'000)
    }
    uart << "OK\n";

    uart << "(1) LED " << (int) npin1 << " blinking 4 times ... ";
    for (uint8_t i = 0; i < 4; ++i){
	Micro::wait_ms(500);
	SPI_selector spi{1};
	// el destructor hace wait_us(500'000)
    }
    uart << "OK\n";


    uart << "(2) LED " << (int) npin0 << " blinking 4 times ... ";
    for (uint8_t i = 0; i < 4; ++i){
	SPI_selector::select(0);
	Micro::wait_ms(500);
	SPI_selector::deselect(0);
	// deselect hace wait_us(500'000)
    }
    uart << "OK\n";

    uart << "(2) LED " << (int) npin1 << " blinking 4 times ... ";
    for (uint8_t i = 0; i < 4; ++i){
	SPI_selector::select(1);
	Micro::wait_ms(500);
	SPI_selector::deselect(1);
	// deselect hace wait_us(500'000)
    }
    uart << "OK\n";


}

// Main
// ----
int main()
{
    init_uart();

    hello();
    
    SPI_array::init();
    UART_iostream uart;

    while(1){
//	test_SPI_pin_array_selector();
	test_SPI_selector_with_deselect_delay();
    }
}




