// Copyright (C) 2021-2024 Manuel Perez 
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

#include "../../../mega_registers.h"
#include "../../../mega_SPI_hal.h"
#include "../../../mega_pin_hwd.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_debug_hwd.h"

#include <atd_ostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

// SPI protocol
// ------------
struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

// OJO: clave periodo a 8 us. Si pongo a 2 us al slave no le da tiempo a leer
// y se lee basura y pierden datos.
    static constexpr uint32_t period_in_us = 8;
};

using SPI = myu::hal::SPI_master<SPI_master_cfg>;


struct SPI_dev_cfg{
    static constexpr bool data_order_LSB = true;
    static constexpr uint8_t polarity    = 0;
    static constexpr uint8_t phase       = 0;
};

using no_CS = hwd::Pin<SPI::SS_pin>;

void SPI_write(uint8_t x)
{
    UART_iostream uart;
    uart << "SPI_write(";
    atd::print_int_as_hex(uart, x);
    uart << ")\n";

    no_CS::write_zero();
    SPI::write(x);
    no_CS::write_one();
}


void SPI_write(uint8_t x1, uint8_t x2)
{
    UART_iostream uart;
    uart << "SPI_write(";
    atd::print_int_as_hex(uart, x1);
    uart << ", ";
    atd::print_int_as_hex(uart, x2);
    uart << ")\n";

    no_CS::write_zero();
    SPI::write(x1);
    SPI::write(x2);
    no_CS::write_one();
}

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void init_SPI()
{
    SPI::init();
    SPI::cfg<SPI_dev_cfg>();

//    SPI::mode(0,0);
//    SPI::data_order_LSB();
//    SPI::SCK_period_in_us<SPI_master_cfg::period_in_us>();
    SPI::turn_on();
}

void hello()
{
    UART_iostream uart;
    uart << "\n\nSPI master test\n"
		"---------------\n"
		"Connections:\n"
		"\tConnect oscilloscope to pins SCLK, CS, or MOIS"
		<< "\n\n";
}


void send_byte()
{
    UART_iostream uart;
    uart << "\nByte to send: ";
    uint16_t x{};
    uart >> x;
    SPI_write(static_cast<uint8_t>(x));
}

void send_two_bytes()
{
    UART_iostream uart;
    uart << "\nFirst byte: ";
    uint16_t x1{};
    uart >> x1;

    uart << "\nSecond byte: ";
    uint16_t x2{};
    uart >> x2;
    SPI_write(static_cast<uint8_t>(x1), static_cast<uint8_t>(x2));
}

void cfg_cpol_cpha()
{
    UART_iostream uart;
    uart << "\nCPOL (0/1): ";
    uint16_t cpol{};
    uart >> cpol;
    if (!(cpol == 0 or cpol == 1))
	return;

    uart << "\nCPHA (0/1): ";
    uint16_t cpha{};
    uart >> cpha;
    if (!(cpha == 0 or cpha == 1))
	return;

    SPI::mode(static_cast<uint8_t>(cpol), static_cast<uint8_t>(cpha));
}

void cfg_order()
{
    UART_iostream uart;

    uart << "\nMost Significant Bit first? (y/n) ";
    char c{};
    uart >> c;

    if (c == 'y' or c == 'Y'){
	SPI::data_order_MSB();
	uart << "\nMSB!!!\n";
    }else {
	SPI::data_order_LSB();
	uart << "\nLSB!!!\n";
    }

}

// Main
// ----
int main() 
{
    init_uart();
    init_SPI();

    hello();

    UART_iostream uart;
    while (1) {
	uart << "\n\nMenu\n"
		"----\n"
		"0. print registers\n"
		"1. send 1 byte\n"
		"2. send 2 byte\n"
		"3. Cfg CPOL, CPHA\n"
		"4. Cfg LSB or MSB\n";

	char opt{};
	uart >> opt;
	switch (opt){
	    break; case '0': 
		    uart << '\n';
		    hwd::print_registers_spi(uart);
		    uart << '\n';
	    break; case '1': send_byte();
	    break; case '2': send_two_bytes();
	    break; case '3': cfg_cpol_cpha();
	    break; case '4': cfg_order();
	}
    }
}



