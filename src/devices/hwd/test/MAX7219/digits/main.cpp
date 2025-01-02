// Copyright (C) 2024-2025 Manuel Perez 
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


#include <mcu_SPI.h>

#include <atd_test.h>
using namespace test;

#ifdef IF_atmega328p
#include <mega.h>

namespace myu = atmega;

void init_mcu()
{}

using UART = myu::UART_8bits;

struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
};


#elifdef IF_atmega4809

#include <mega0.h>

namespace myu = atmega4809;

void init_mcu()
{
    myu::init();
//    myu::Clock_controller::clk_main_divided_by_16(); // a 1 MHz
}

using UART = myu::UART1_8bits;


struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
    static constexpr auto mode = myu::SPI_master_cfg::normal_mode;
};

#endif


// Microcontroller
// ---------------
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<UART>;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// SPI protocol
// ------------
using SPI = myu::SPI_master<SPI_master_cfg>;

// Pin connections
// ---------------
static constexpr uint8_t spi_no_select_pin = SPI::SS_pin;


// Devices
// -------
//using MAX7219_matrix_cols = dev::MAX7219_matrix<MAX7219_cfg_by_columns, matrix_nstrips, 4>;
struct MAX7219_cfg{
    using SPI_master   = SPI;
    using SPI_selector = mcu::SPI_pin_selector<Micro, spi_no_select_pin>;

};

//using MAX7219 = dev::hwd::MAX7219<MAX7219_cfg>;
using MAX7219 = dev::MAX7219_digits<MAX7219_cfg>;


// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void init_spi()
{
    SPI::init();
}


void init_max7219()
{
    UART_iostream uart;

    MAX7219::init(); 
    MAX7219::intensity(0x00);
    MAX7219::turn_on();
}


void hello()
{
    UART_iostream uart;
    uart << "\n\nMAX7219 digits test\n"
	        "-------------------\n"
		"Connections:\n"
		"\tConnect MAX7219 to SPI via " << (int) spi_no_select_pin << " pin\n"
		<< "\n\n";
}

template <uint16_t ms, bool with_point = false>
void blink(uint8_t d, uint8_t value)
{
    if constexpr (with_point)
	MAX7219::digit_with_point(d, value);
    else
	MAX7219::digit(d, value);

    Micro::wait_ms(ms);
    MAX7219::clear(d);
}

void test_basic()
{
    UART_iostream uart;
    uart << "\nBasic test\n"
	      "----------\n";

    MAX7219::display_test_on();
    Micro::wait_ms(500);
    MAX7219::display_test_off();
}

void test_basic_counter()
{
    UART_iostream uart;

    static constexpr uint16_t ms = 500;

    uart << "\nBasic counter\n"
	      "-------------\n";

    uart << "writing numbers ... ";
    for (uint8_t i = 0; i < 8; ++i)
	blink<ms>(i, i);

    uart << "OK\n";

    uart << "Adding point ...";
    for (uint8_t i = 0; i < 8; ++i)
	blink<ms, true>(i, i);

    uart << "OK\n";
    
    uart << "Writing points\n";
    for (uint8_t i = 0; i < 8; ++i)
	MAX7219::write_point(i);
}


void test_counter()
{
    UART_iostream uart;

    uart << "\nCounter test\n"
	      "------------\n";

    uart << "Write from: ";
    uint16_t x0;
    uart >> x0;
    uart << "to: ";
    uint16_t x1;
    uart >> x1;
    uart << "In position (0..7): ";
    uint16_t i;
    uart >> i;

    for (uint16_t x = x0; x <= x1; ++x){
	MAX7219::write(i, x);
	if (x1 - x0 < 500) // si no no se ve nada
	    Micro::wait_ms(10);
    }
}

// Main
// ----
int main() 
{
    init_mcu();
    init_uart();
    init_spi();
    init_max7219();

    hello();

    UART_iostream uart;
    
    MAX7219::clear();
    while (1){
	uart << "\nMenu\n"
		  "----\n"
		  "0. Clear\n"
		  "1. Basic test\n"
		  "2. Basic counter\n"
		  "3. Counter test\n";

	char opt{};
	uart >> opt;

	

	switch (opt){
	    break; case '0': MAX7219::clear();
	    break; case '1': test_basic();
	    break; case '2': test_basic_counter();
	    break; case '3': test_counter();

	    break; default:
			uart << "What?\n";
	}
    }                                                 
}


