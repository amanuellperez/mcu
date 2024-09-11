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
#include <rom_font_dogica_8x8_rf.h>
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
// dejo solo 3 para poder probar 
static constexpr uint8_t matrix_nstrips = 3;

// SPI protocol
// ------------
using SPI = myu::SPI_master;
constexpr uint32_t spi_frequency_in_hz = 500'000; // máx. 10MHz
static_assert (noCS == myu::SPI_master::noCS_pin_number);


// Devices
// -------
struct MAX7219_cfg_by_rows{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= 
	mcu::SPI_pin_array_selector<Micro, strip0, strip1, strip2>;

    static constexpr bool by_rows{};

// Font no lo necesita MAX7219_matrix, pero lo uso en las pruebas
    using Font = rom::font_dogica_8x8_rf::Font;
    static constexpr bool is_by_rows = true;
    static constexpr bool is_by_cols = false;
};

struct MAX7219_cfg_by_columns{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= 
	mcu::SPI_pin_array_selector<Micro, strip0, strip1, strip2>;

    static constexpr bool by_columns{};

// Font no lo necesita MAX7219_matrix, pero lo uso en las pruebas
    using Font = rom::font_dogica_8x8_cr::Font;
    static constexpr bool is_by_rows = false;
    static constexpr bool is_by_cols = true;
};

// Elegir para probar por filas o por columnas
//using Cfg = MAX7219_cfg_by_columns;
using Cfg = MAX7219_cfg_by_rows;

using MAX7219_matrix = dev::MAX7219_matrix<Cfg, matrix_nstrips, 4>;

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

template <size_t nrows, size_t ncols>
void print(std::ostream& out, const atd::Bitmatrix_row_1bit<nrows, ncols>& m)
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


template <size_t nrows, size_t ncols>
void print_uints(std::ostream& out, const atd::Bitmatrix_row_1bit<nrows, ncols>& m)
{
    using index_type = typename Bitmatrix::index_type;
    for (uint8_t nstrip = 0; nstrip < MAX7219_matrix::nstrips; ++nstrip)
    {
	for (uint8_t ndigit = 0; ndigit < 8; ++ndigit){
	    index_type i = 8 * nstrip + ndigit;
	    out << (int) i << ": ";
	    for (auto p = m.row_begin(i); p !=  m.row_end(i); ++p){
		out << (int) *p << ' ';
	    }
	    out << '\n';
	}
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
    MAX7219_matrix::clear();
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nMAX7219 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect MAX7219 to SPI via " << (int) noCS << " pin\n"
		"\tConnecto other modules to pins " << (int) strip1
		    << ", " << (int) strip2 
		<< "\n\nTo test the matrix by rows or by columns you have "
		   "to choose Cfg as MAX7219_cfg_by_rows or MAX7219_cfg_by_columns"
		   "and recompile"
		<< "\n\n";
}



void test_write_bitmatrix()
{
    myu::UART_iostream uart;
    uart << "write::MAX7219_matrix\n";

    uart << "matrix of " << (int) MAX7219_matrix::rows 
		         << " rows and " 
			 << (int) MAX7219_matrix::cols
			 << " cols\n";

    Bitmatrix bm;
    for (uint8_t j = 0; j < bm.cols(); ++j){

	bm.clear(); // FUNDAMENTAL!!!
	for (uint8_t i = 0; i < bm.rows(); ++i)
	    bm(i,j) = 1;
	++j;

	MAX7219_matrix::write(bm);

	Micro::wait_ms(500);
    }

    MAX7219_matrix::clear();


}

void test_font()
{
    myu::UART_iostream uart;
    uart << "Font test\n"
	    "---------\n";
    using Font = Cfg::Font;

    MAX7219_matrix::clear();

    Bitmatrix bm;

    bm.clear();
    if (Cfg::is_by_cols){
    atd::write<Font>(bm, Coord{0, 0}  , '1');
    atd::write<Font>(bm, Coord{0, 8}  , '2');
    atd::write<Font>(bm, Coord{0, 8*2}, '3');

    atd::write<Font>(bm, Coord{8, 0}  , '4');
    atd::write<Font>(bm, Coord{8, 8*1}, '5');
    atd::write<Font>(bm, Coord{8, 8*2}, '6');

    atd::write<Font>(bm, Coord{8*2, 8*0}, '7');
    atd::write<Font>(bm, Coord{8*2, 8*1}, '8');
    atd::write<Font>(bm, Coord{8*2, 8*2}, '9');

    atd::write<Font>(bm, Coord{8*3, 8*0}, 'A');
    atd::write<Font>(bm, Coord{8*3, 8*1}, '8');
    atd::write<Font>(bm, Coord{8*3, 8*2}, 'C');
    } else {

    atd::write<Font>(bm, Coord{0, 0}  , '1');
    atd::write<Font>(bm, Coord{0, 8}  , '2');
    atd::write<Font>(bm, Coord{0, 8*2}, '3');
    atd::write<Font>(bm, Coord{0, 8*3}, '4');
    atd::write<Font>(bm, Coord{8, 0}  , '5');
    atd::write<Font>(bm, Coord{8, 8*1}, '6');
    atd::write<Font>(bm, Coord{8, 8*2}, '7');
    atd::write<Font>(bm, Coord{8, 8*3}, '8');

    atd::write<Font>(bm, Coord{8*2, 8*0}, '9');
    atd::write<Font>(bm, Coord{8*2, 8*1}, 'A');
    atd::write<Font>(bm, Coord{8*2, 8*2}, 'B');
    atd::write<Font>(bm, Coord{8*2, 8*3}, 'C');
    }


    MAX7219_matrix::write(bm);
}

void test_basic()
{
    myu::UART_iostream uart;
    uart << "\nBasit test\n"
	      "----------\n";
	    

    MAX7219_matrix::clear();
    for (uint8_t i = 0; i < MAX7219_matrix::nstrips; ++i){
	uart << "Strip " << (int) i << " ON ... ";
	MAX7219_matrix::display_test_on(i);
	Micro::wait_ms(500);
	uart << "OFF\n";
	MAX7219_matrix::display_test_off(i);
	Micro::wait_ms(1000);
    }
}

// Esta función no aporta mucho
//void test_write_digit()
//{
//    myu::UART_iostream uart;
//    uart << "write_digit test\n"
//	    "----------------\n";
//    MAX7219_matrix::clear();
//
//
//    uint8_t x0[] = {0x01, 0x02, 0x04, 0x08};
//    uint8_t x1[] = {0x07, 0x0E, 0x1C, 0x38};
//    uint8_t x2[] = {0x04, 0x08, 0x10, 0x20};
//
//    for (uint8_t nstrip = 0; nstrip < 2; ++nstrip){
//	for (uint8_t j = 0; j + 2 < MAX7219_matrix::strip_cols; ++j){
//	    MAX7219_matrix::write(nstrip, j + 0, x0);
//	    MAX7219_matrix::write(nstrip, j + 1, x1);
//	    MAX7219_matrix::write(nstrip, j + 2, x2);
//	    Micro::wait_ms(1000);
//	    MAX7219_matrix::clear();
//	}
//    }
//}

void test_write()
{
    myu::UART_iostream uart;
    uart << "\nWrite test\n"
	      "----------\n"
	      "Writing 0x01 in strip 0\n"
	      "        0x02 in strip 1\n"
	      "        0x04 in strip 2\n"
	      "and     0x08 in strip 3\n";


    MAX7219_matrix::clear();

    uint8_t x[] = {0x01, 0x02, 0x04, 0x08};
    for (uint8_t nstrip = 0; nstrip < MAX7219_matrix::nstrips; ++nstrip){
	for (uint8_t i = 0; i < 8; ++i){
	    MAX7219_matrix::write(nstrip, i, x);
	    Micro::wait_ms(500);
	}
    }
    Micro::wait_ms(500);
    MAX7219_matrix::clear();
    Micro::wait_ms(800);
}


// Main
// ----
int main() 
{
    init_uart();
    init_spi();
    init_max7219_matrix();

    hello();

    myu::UART_iostream uart;

    while (1){
	uart << "\nMenu\n"
	          "----\n"
		  "0. Clear\n"
		  "1. Basic test\n"
		  "2. Write test\n"
		  "3. Write bit matrix test\n"
		  "4. Font test\n";

	char opt{};
	uart >> opt;
	
	switch (opt){
	    break; case '0': MAX7219_matrix::clear();
	    break; case '1': test_basic();
	    break; case '2': test_write();
	    break; case '3': test_write_bitmatrix();
	    break; case '4': test_font();
	    break; default:
			uart << "What??? \n";
	}

    }                                                 
}


