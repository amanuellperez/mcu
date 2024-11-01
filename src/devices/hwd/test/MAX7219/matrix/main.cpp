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

#include <rom_font_dogica_8x8_cr.h>
#include <rom_font_dogica_8x8_rf.h>

#include <rom_font_upheavtt_13x14_cr.h>
#include <rom_font_upheavtt_13x14_rf.h>

#include <atd_draw.h>
#include <atd_display.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART>;

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
struct MAX7219_cfg_by_rows{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= 
	mcu::SPI_pin_array_selector<Micro, strip0, strip1, strip2, strip3>;

    static constexpr bool by_rows{};

    static constexpr bool is_by_rows = true;
    static constexpr bool is_by_cols = false;
};

struct MAX7219_cfg_by_columns{
    using SPI_master	= myu::SPI_master;
    using SPI_selector	= 
	mcu::SPI_pin_array_selector<Micro, strip0, strip1, strip2, strip3>;

    static constexpr bool by_columns{};

// Font no lo necesita MAX7219_matrix, pero lo uso en las pruebas
    static constexpr bool is_by_rows = false;
    static constexpr bool is_by_cols = true;
};

// Elegir para probar por filas o por columnas

using MAX7219_matrix_cols = dev::MAX7219_matrix<MAX7219_cfg_by_columns, matrix_nstrips, 4>;
using MAX7219_matrix_rows = dev::MAX7219_matrix<MAX7219_cfg_by_rows, matrix_nstrips, 4>;

// Elegir por filas o columnas para probar
using MAX7219_matrix = MAX7219_matrix_cols;
//using MAX7219_matrix = MAX7219_matrix_rows;



// Text_display
template <typename Cfg>
using Text_display = atd::Monochromatic_text_display<Cfg>;

template <typename Font0, typename Display0, int rows, int cols>
struct Display_cfg{
// Text buffer
    using index_type = int;
    //using index_type = unsigned int;
    static constexpr index_type text_rows = rows;
    static constexpr index_type text_cols = cols;

// Ventana que vemos del buffer
    static constexpr bool cylinder_type{};
    
// Fuente
    using Font = Font0;

// Display output
    using Display    = Display0;
};

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
void print_uints(std::ostream& out, 
			const atd::Bitmatrix_row_1bit<nrows, ncols>& m)
{
    using Bitmatrix = atd::Bitmatrix_row_1bit<nrows, ncols>;
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


template <typename C>
void print_bg(std::ostream& out, const atd::Text_block_with_view<C>& txt)
{
    using index_t = atd::Text_block<C>::index_type;

    out << "\n>>>-------------------------------\n";
    for (index_t i = 0; i < txt.bg_rows(); ++i){
	for (index_t j = 0; j < txt.bg_cols(); ++j){
	    char c = txt.bg_read({i, j});
	    if (c == '\0')
		out << "_";
	    else
		out << c;
	}
	out << '\n';
    }
    out << "<<<-------------------------------\n";
}

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg<baud_rate>();
    uart.turn_on();
}

void init_spi()
{
    SPI::clock_frequency_in_hz<spi_frequency_in_hz>();
    SPI::turn_on();
}


void init_max7219_matrix()
{
    UART_iostream uart;
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
    UART_iostream uart;
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
    using Bitmatrix  = typename MAX7219_matrix::Bitmatrix;

    UART_iostream uart;
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

	MAX7219_matrix::write(bm);

	Micro::wait_ms(200);
    }

    MAX7219_matrix::clear();


}

template <typename MAX7219, typename Font>
void test_font_dogica()
{
    using Bitmatrix = typename MAX7219::Bitmatrix;
    using Coord = typename Bitmatrix::Coord_ij;

    UART_iostream uart;
    uart << "Font dogica\n";

    static constexpr auto h = Font::rows;
    static constexpr auto w = Font::cols;

    MAX7219::clear();

    Bitmatrix bm;

    bm.clear();

    atd::write<Font>(bm, Coord{0, 0}  , '1');
    atd::write<Font>(bm, Coord{0, w}  , '2');
    atd::write<Font>(bm, Coord{0, w*2}, '3');
    atd::write<Font>(bm, Coord{0, w*3}, '4');

    atd::write<Font>(bm, Coord{h, 0}  , '5');
    atd::write<Font>(bm, Coord{h, w*1}, '6');
    atd::write<Font>(bm, Coord{h, w*2}, '7');
    atd::write<Font>(bm, Coord{h, w*3}, '8');

    atd::write<Font>(bm, Coord{h*2, w*0}, '9');
    atd::write<Font>(bm, Coord{h*2, w*1}, 'A');
    atd::write<Font>(bm, Coord{h*2, w*2}, 'B');
    atd::write<Font>(bm, Coord{h*2, w*3}, 'C');

    atd::write<Font>(bm, Coord{h*3, w*0}, 'D');
    atd::write<Font>(bm, Coord{h*3, w*1}, 'E');
    atd::write<Font>(bm, Coord{h*3, w*2}, 'F');
    atd::write<Font>(bm, Coord{h*3, w*3}, 'G');

    MAX7219::write(bm);
}



template <typename MAX7219, typename Font>
void test_font_upheavtt()
{
    using Bitmatrix = typename MAX7219::Bitmatrix;
    using Coord = typename Bitmatrix::Coord_ij;

    UART_iostream uart;
    uart << "Font upheavtt\n";

    static constexpr auto h = Font::rows;
    static constexpr auto w = Font::cols;

    MAX7219::clear();

    Bitmatrix bm;

    bm.clear();

    atd::write<Font>(bm, Coord{0, 0}  , '1');
    atd::write<Font>(bm, Coord{0, w}  , '2');
    atd::write<Font>(bm, Coord{0, 2*w}, '3');
    uart << "Only half of number 3 can be written\n";

    atd::write<Font>(bm, Coord{h, 0}  , '4');
    atd::write<Font>(bm, Coord{h, w}  , '5'); 
    atd::write<Font>(bm, Coord{h, 2*w}, '6'); 
    uart << "Only half of number 6 can be written\n";


    MAX7219::write(bm);
}

// Esto es raro... @_@ ¿por qué no sobrecargar la función?
//                     ¬_¬ no se por qué, pero no quiero pasar MAX7219 como
//                     parametro (seguro que la proxima vez lo cambio xD)
template <typename MAX7219>
    requires (std::is_same_v<MAX7219, MAX7219_matrix_cols>)
void test_font()
{
    UART_iostream uart;
    uart << "Font test\n"
	    "---------\n";

    using Dogica = rom::font_dogica_8x8_cr::Font;
    test_font_dogica<MAX7219, Dogica>();
    Micro::wait_ms(1000);

    using Upheavtt = rom::font_upheavtt_13x14_cr::Font;
    test_font_upheavtt<MAX7219, Upheavtt>();

}


template <typename MAX7219>
    requires (std::is_same_v<MAX7219, MAX7219_matrix_rows>)
void test_font()
{
    UART_iostream uart;
    uart << "Font test\n"
	    "---------\n";

    using Dogica = rom::font_dogica_8x8_rf::Font;
    test_font_dogica<MAX7219, Dogica>();
    Micro::wait_ms(1000);

    using Upheavtt = rom::font_upheavtt_13x14_rf::Font;
    test_font_upheavtt<MAX7219, Upheavtt>();

}


void test_basic()
{
    UART_iostream uart;
    uart << "\nBasit test\n"
	      "----------\n";
	    

    MAX7219_matrix::clear();
    for (uint8_t i = 0; i < MAX7219_matrix::nstrips; ++i){
	uart << "Strip " << (int) i << " ON ... ";
	MAX7219_matrix::display_test_on(i);
	Micro::wait_ms(400);
	uart << "OFF\n";
	MAX7219_matrix::display_test_off(i);
	Micro::wait_ms(800);
    }
}


void test_write()
{
    UART_iostream uart;
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
	    Micro::wait_ms(200);
	}
    }
    Micro::wait_ms(500);
    MAX7219_matrix::clear();
    Micro::wait_ms(800);
}

template <typename MAX7219, typename Font>
void test_display_dogica()
{
    UART_iostream uart;
    uart << "\nTest display dogica\n";
    
    using Cfg = Display_cfg<Font, MAX7219, 4, 20>;
    using Display = Text_display<Cfg>;
    Display display;

    display.bg_write({0,0}, "View                "
			    "     to             "
			    "        the         "
			    "            right!  ");
    print_bg(uart, display);

    display.flush();

    uart << "bitmatrix.rows() = " << (int) Display::Bitmatrix::rows() 
	  << "; cols() = " << (int) Display::Bitmatrix::cols() << "\n";
    for (uint8_t i = 0; i < 15; ++i){
	display.view_move_right(1);
	display.flush();
	Micro::wait_ms(300);
    }

    display.bg_write({0,0}, "    tfel            "
			    "         eht        "
			    "             ot     "
			    "                weiV");
    print_bg(uart, display);
    display.flush();
    Micro::wait_ms(300);

    for (uint8_t i = 0; i < 15; ++i){
	display.view_move_left(1);
	display.flush();
	Micro::wait_ms(300);
    }

    display.bg_write({0,0}, "1  .                "
			    "2  c                "
			    "3  b                "
			    ".  a                ");
    print_bg(uart, display);
    display.flush();
    Micro::wait_ms(800);

    for (uint8_t i = 0; i < 8; ++i){
	display.view_move_down(1);
	display.flush();
	Micro::wait_ms(800);
    }
}


template <typename MAX7219, typename Font>
void test_display_upheavtt()
{
    UART_iostream uart;
    uart << "\nTest display upheavtt\n"
	    "WARNING: this font has 14 rows x 13 cols.\n"
	    "         In a 32 x 32 display we get a View of only 2 x 2\n"
	    "         (13 cols x 2 = 26 bits ==> it only uses 2 columns of the left MAX7219\n";
    
    using Cfg = Display_cfg<Font, MAX7219, 2, 20>;
    using Display = Text_display<Cfg>;
    Display display;

    display.bg_write({0,0}, "View to             "
			    "        the right!  ");
    print_bg(uart, display);

    display.flush();
    Micro::wait_ms(300);

    for (uint8_t i = 0; i < 15; ++i){
	display.view_move_right(1);
	display.flush();
	Micro::wait_ms(300);
    }

    display.bg_write({0,0}, "    tfel eht        "
			    "             ot weiV");
    print_bg(uart, display);
    display.flush();
    Micro::wait_ms(300);

    for (uint8_t i = 0; i < 15; ++i){
	display.view_move_left(1);
	display.flush();
	Micro::wait_ms(300);
    }

    display.bg_write({0,0}, "1                   "
			    " 2                  ");
    print_bg(uart, display);
    display.flush();
    Micro::wait_ms(800);

    for (uint8_t i = 0; i < 8; ++i){
	display.view_move_down(1);
	display.flush();
	Micro::wait_ms(800);
    }
}

template <typename MAX7219>
    requires (std::is_same_v<MAX7219, MAX7219_matrix_cols>)
void test_display()
{
    UART_iostream uart;
    uart << "Test display\n"
	    "------------\n";

    using Dogica = rom::font_dogica_8x8_cr::Font;
    test_display_dogica<MAX7219, Dogica>();

    using Upheavtt = rom::font_upheavtt_13x14_cr::Font;
    test_display_upheavtt<MAX7219, Upheavtt>();
    
}

template <typename MAX7219>
    requires (std::is_same_v<MAX7219, MAX7219_matrix_rows>)
void test_display()
{
    UART_iostream uart;
    uart << "Test display\n"
	    "------------\n";

    using Dogica = rom::font_dogica_8x8_rf::Font;
    test_display_dogica<MAX7219, Dogica>();

    using Upheavtt = rom::font_upheavtt_13x14_rf::Font;
    test_display_upheavtt<MAX7219, Upheavtt>();
    
}


// Main
// ----
int main() 
{
    init_uart();
    init_spi();
    init_max7219_matrix();

    hello();

    UART_iostream uart;

    while (1){
	uart << "\nMenu\n"
		  "----\n"
		  "0. Clear\n"
		  "1. Basic test\n"
		  "2. Write test\n"
		  "3. Write bit matrix test\n"
		  "4. Basic Font test\n"
		  "5. Text display\n";

	char opt{};
	uart >> opt;
	
	switch (opt){
	    break; case '0': MAX7219_matrix::clear();
	    break; case '1': test_basic();
	    break; case '2': test_write();
	    break; case '3': test_write_bitmatrix();
	    break; case '4': test_font<MAX7219_matrix>();
	    break; case '5': test_display<MAX7219_matrix>();
	    break; default:
			uart << "What?\n";
	}
    }                                                 
}


