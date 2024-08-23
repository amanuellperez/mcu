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


#include "../../dev_MAX7219.h"
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

// SPI protocol
// ------------
using SPI = myu::SPI_master;
constexpr uint32_t spi_frequency_in_hz = 500'000; // máx. 10MHz


// Devices
// -------
struct MAX7219_cfg{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= mcu::SPI_pin_selector<Micro, myu::SPI_master::noCS_pin_number>;
};

using MAX7219 = dev::MAX7219<MAX7219_cfg>;

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
    MAX7219::init(); 
    MAX7219::scan_all_digits();
//    MAX7219::intensity(10);
    MAX7219::disable_decode_mode();
    MAX7219::normal_mode();
}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nMAX7219 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect MAX7219 to SPI via CS pin"
		<< "\n\n";
}


// Main
// ----
int main() 
{
    init_uart();
    init_spi();
    init_max7219();

    hello();

    // CUIDADO: de [1..8] !!!
    for (uint8_t i = 1; i <= 8; ++i){
	MAX7219::digit(i, 0x00);
    }
    
    myu::UART_iostream uart;
    while (1) 
    {
	for (uint8_t i = 0; i < 8; ++i){
	    for (uint8_t j = 1; j <= 8; ++j) 
		MAX7219::digit(j, 1 << i);

	    Micro::wait_ms(1000);
	}

    }                                                 
}


