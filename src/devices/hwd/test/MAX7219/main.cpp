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


#include <avr_atmega.h>
#include <mcu_SPI.h>

#include <rom_font_dogica_8x8_cr.h>
#include <atd_draw.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
#include "../../dev_MAX7219.h"

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
static constexpr uint8_t noCS = 16;
static constexpr uint8_t strip0 = noCS;
static constexpr uint8_t strip1 = 15;
static constexpr uint8_t strip2 = 14;
static constexpr uint8_t strip3 = 13;
static constexpr uint8_t matrix_nstrips = 4;

// SPI protocol
// ------------
using SPI = myu::SPI_master;
constexpr uint32_t spi_frequency_in_hz = 500'000; // máx. 10MHz
static_assert (noCS == myu::SPI_master::noCS_pin_number);


// Devices
// -------
struct MAX7219_cfg{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= mcu::SPI_pin_selector<Micro, myu::SPI_master::noCS_pin_number>;
};

struct MAX7219_cfg_matrix{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= 
	mcu::SPI_pin_array_selector<Micro, strip0, strip1, strip2, strip3>;
};

using MAX7219        = dev::MAX7219_basic<MAX7219_cfg>;
using MAX7219_array  = dev::MAX7219_array<MAX7219_cfg, 4>;
using MAX7219_matrix = dev::MAX7219_matrix<MAX7219_cfg_matrix, 
						    matrix_nstrips, 4>;

using Bitmatrix = MAX7219_matrix::Bitmatrix;
using Coord = Bitmatrix::Coord_ij;

// Helpers
// -------
template <size_t nrows, size_t ncols>
void print(std::ostream& out, const atd::Bitmatrix_col_1bit<nrows, ncols>& m)
{
    for (uint8_t i = 0; i < m.rows(); ++i){
	for (uint8_t j = 0; j < m.cols(); ++j){
	    if (m(i, j) == 1)
		out << 'X';
	    else
		out << '.';
	}
	out << '\n';

    }
}
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


// inicializamos 1 display solo
void init_max7219()
{
    MAX7219::init(); 
    MAX7219::scan_all_digits();
//    MAX7219::intensity(10);
    MAX7219::disable_decode_mode();
    MAX7219::normal_mode();
}

void init_max7219_array()
{
    MAX7219_array::init(); 
    MAX7219_array::intensity(0x00);
    MAX7219_array::turn_on();
}

void init_max7219_matrix()
{
    myu::UART_iostream uart;
    Test test{uart};
    CHECK_TRUE(test, MAX7219_matrix::nstrips == matrix_nstrips, 
						    "MAX7219_matrix::nstrips");
    CHECK_TRUE(test, MAX7219_matrix::modules_per_strip == 4, 
					"MAX7219_matrix::modules_per_strip");

    MAX7219_matrix::init(); 
    MAX7219_matrix::intensity(0x00);
    MAX7219_matrix::turn_on();
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nMAX7219 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect MAX7219 to SPI via " << (int) noCS << " pin\n"
		"\tConnecto other modules to pins " << (int) strip1
		<< "\n\n";
}

// Para 1 display
void test_basic1()
{
    init_max7219();

    // CUIDADO: de [1..8] !!!
    for (uint8_t i = 1; i <= 8; ++i){
	MAX7219::digit(i, 0x00);
    }


    for (uint8_t i = 0; i < 8; ++i){
	for (uint8_t j = 1; j <= 8; ++j) 
	    MAX7219::digit(j, 1 << i);

	Micro::wait_ms(1000);
    }
}


void test_array()
{
    myu::UART_iostream uart;
    uart << "Testing array of " << (int) MAX7219_array::size() << " MAX7219\n";

    init_max7219_array();

    uint8_t x[] = {0x01, 0x02, 0x04, 0x08};

    MAX7219_array::clear();

    for (uint8_t i = 0; i < MAX7219_array::rows(); ++i){
	MAX7219_array::row(i, x);
	Micro::wait_ms(800);

	MAX7219_array::clear(i);
    }

    MAX7219_array::clear();


}

void test_array2()
{
    myu::UART_iostream uart;
    uart << "Testing array of " << (int) MAX7219_array::size() << " MAX7219\n";

    init_max7219_array();

    MAX7219_array::clear();

    for (uint8_t i = 0; i < 8; ++i){
	MAX7219_array::write(1, i, 0xFF);
	Micro::wait_ms(500);
	MAX7219_array::clear();
    }

    MAX7219_array::clear();


}

void test_write_bitmatrix()
{
    myu::UART_iostream uart;
    uart << "write::MAX7219_matrix\n";

    static uint8_t j = 0;
    Bitmatrix bm;

    bm.clear(); // FUNDAMENTAL!!!
    for (uint8_t i = 0; i < bm.rows(); ++i)
	bm(i,j) = 1;
    ++j;

    if (j == bm.cols())
	j = 0;

    MAX7219_matrix::write(bm);

    Micro::wait_ms(800);
    MAX7219_matrix::clear();
    Micro::wait_ms(800);


}

void test_font()
{
    myu::UART_iostream uart;
    uart << "Font test\n"
	    "---------\n";
    using Font = rom::font_dogica_8x8_cr::Font;

    MAX7219_matrix::clear();

    Bitmatrix bm;

    bm.clear();
    atd::write<Font>(bm, Coord{0, 0}, 'H');
    atd::write<Font>(bm, Coord{0, 8}, 'e');
    atd::write<Font>(bm, Coord{0, 16}, 'l');
    atd::write<Font>(bm, Coord{0, 24}, 'l');
//    print(uart, bm);
    MAX7219_matrix::write(bm);
    Micro::wait_ms(800);
    MAX7219_matrix::clear();
    Micro::wait_ms(800);
}

void test_basic()
{
    MAX7219_matrix::clear();
    for (uint8_t i = 0; i < MAX7219_matrix::nstrips; ++i){
	MAX7219_matrix::display_test_on(i);
	Micro::wait_ms(1000);
	MAX7219_matrix::display_test_off(i);
	Micro::wait_ms(1000);
    }
}

void test_write_column()
{
    myu::UART_iostream uart;
    uart << "Write column test\n"
	    "-----------------\n";
    MAX7219_matrix::clear();


    uint8_t x0[] = {0x01, 0x02, 0x04, 0x08};
    uint8_t x1[] = {0x07, 0x0E, 0x1C, 0x38};
    uint8_t x2[] = {0x04, 0x08, 0x10, 0x20};

    for (uint8_t nstrip = 0; nstrip < 2; ++nstrip){
	for (uint8_t j = 0; j + 2 < MAX7219_matrix::strip_cols; ++j){
	    MAX7219_matrix::write(nstrip, j + 0, x0);
	    MAX7219_matrix::write(nstrip, j + 1, x1);
	    MAX7219_matrix::write(nstrip, j + 2, x2);
	    Micro::wait_ms(1000);
	    MAX7219_matrix::clear();
	}
    }
}

void test_write()
{
    myu::UART_iostream uart;
    uart << "Write test\n"
	    "----------\n";
    MAX7219_matrix::clear();

    uint8_t x[] = {0x01, 0x02, 0x04, 0x08};
    for (uint8_t i = 0; i < 8; ++i){
	MAX7219_matrix::write(0, i, x);
//	Micro::wait_ms(500);
    }
    Micro::wait_ms(500);
    MAX7219_matrix::clear();
    Micro::wait_ms(800);


//    uint8_t y[] = {0x08, 0x04, 0x02, 0x01};
//    for (uint8_t i = 0; i < 8; ++i){
//	MAX7219_matrix::write(1, i, y);
//	Micro::wait_ms(500);
//    }

}

void test_matrix()
{
    myu::UART_iostream uart;
    uart << "\nTesting array of " << (int) MAX7219_array::size() << " MAX7219\n";

    init_max7219_matrix();
    
//    test_basic();
//   test_write();
//    test_write_column();
//    test_write_bitmatrix();
    test_font();
}

// Main
// ----
int main() 
{
    init_uart();
    init_spi();

    hello();

    myu::UART_iostream uart;
    while (1) 
    {
//	test_basic1();
//	test_array();
//	test_array2();
	test_matrix();

    }                                                 
}


