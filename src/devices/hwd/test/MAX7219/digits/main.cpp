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


#include "../../../dev_MAX7219.h"
#include <avr_atmega.h>
#include <mcu_SPI.h>

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
static constexpr uint8_t noCS = 16;
static constexpr uint8_t spi_no_select_pin = noCS;


// SPI protocol
// ------------
using SPI = myu::SPI_master;
constexpr uint32_t spi_frequency_in_hz = 500'000; // máx. 10MHz
static_assert (noCS == myu::SPI_master::noCS_pin_number);


// Devices
// -------
//using MAX7219_matrix_cols = dev::MAX7219_matrix<MAX7219_cfg_by_columns, matrix_nstrips, 4>;
struct MAX7219_cfg{
    using SPI_master   = myu::SPI_master;
    using SPI_selector = mcu::SPI_pin_selector<Micro, spi_no_select_pin>;
};

//using MAX7219 = dev::MAX7219_basic<MAX7219_cfg>;
using MAX7219 = dev::MAX7219_digits<MAX7219_cfg>;


// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void init_spi()
{
    SPI::clock_frequency_in_hz<spi_frequency_in_hz>();
    SPI::turn_on();
}


void init_max7219()
{
    myu::UART_iostream uart;

    MAX7219::init(); 
    MAX7219::intensity(0x00);
//    MAX7219::enable_decode_mode();
//    MAX7219::scan_all_digits();
    MAX7219::turn_on();
    MAX7219::display_test_on();
    Micro::wait_ms(1000);
    MAX7219::display_test_off();
}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nMAX7219 digits test\n"
	        "-------------------\n"
		"Connections:\n"
		"\tConnect MAX7219 to SPI via " << (int) noCS << " pin\n"
		<< "\n\n";
}

template <uint16_t ms>
void blink(uint8_t d, uint8_t value)
{
    MAX7219::digit(d, value);
    Micro::wait_ms(ms);
    MAX7219::clear(d);
}

void test_basic()
{
    myu::UART_iostream uart;

    static constexpr uint16_t ms = 500;

    uart << "\nBasic test\n"
	      "----------\n";
    for (uint8_t i = 0; i < 8; ++i){
	uart << "writing " << (int) i << " ... ";
	blink<ms>(i, i);
	uart << "OK\n";
    }
}

// Main
// ----
int main() 
{
    init_uart();
    init_spi();
    init_max7219();

    hello();

    myu::UART_iostream uart;
    
    MAX7219::clear();
    while (1){
	uart << "\nMenu\n"
		  "----\n"
		  "0. Clear\n";

	char opt{};
	uart >> opt;
	test_basic();
	

//	switch (opt){
//	    break; case '5': test_display<MAX7219_matrix>();
//	    break; default:
//			uart << "What?\n";
//	}
    }                                                 
}


