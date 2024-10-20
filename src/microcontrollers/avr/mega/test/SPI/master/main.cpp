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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_SPI.h"
#include "../../../mega_pin.h"
#include "../../../avr_UART_iostream.h"
#include "../../../avr_debug.h"

#include <atd_ostream.h>


// Microcontroller
// ---------------
namespace myu = avr_;

// SPI protocol
// ------------
using SPI = myu::SPI_master;

// OJO: clave periodo a 8 us. Si pongo a 2 us al slave no le da tiempo a leer
// y se lee basura y pierden datos.
constexpr uint16_t period_in_us = 8;	
using no_CS = myu::Pin<SPI::SS_pin_number>;

void SPI_write(uint8_t x)
{
    myu::UART_iostream uart;
    uart << "SPI_write(";
    atd::print_int_as_hex(uart, x);
    uart << ")\n";

    no_CS::write_zero();
    SPI::write(x);
    no_CS::write_one();
}


void SPI_write(uint8_t x1, uint8_t x2)
{
    myu::UART_iostream uart;
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
    myu::UART_iostream uart;
    myu::basic_cfg<9'600>(uart);
    uart.turn_on();
}

void init_SPI()
{
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::clock_period_in_us<period_in_us>();
    SPI::turn_on();
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nSPI master test\n"
		"---------------\n"
		"Connections:\n"
		"\tConnect oscilloscope to pins SCLK, CS, or MOIS"
		<< "\n\n";
}


void send_byte()
{
    myu::UART_iostream uart;
    uart << "\nByte to send: ";
    uint16_t x{};
    uart >> x;
    SPI_write(static_cast<uint8_t>(x));
}

void send_two_bytes()
{
    myu::UART_iostream uart;
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
    myu::UART_iostream uart;
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

    SPI::spi_mode(static_cast<uint8_t>(cpol), static_cast<uint8_t>(cpha));
}

void cfg_order()
{
    myu::UART_iostream uart;

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

    myu::UART_iostream uart;
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
		    print_registers_spi(uart);
		    uart << '\n';
	    break; case '1': send_byte();
	    break; case '2': send_two_bytes();
	    break; case '3': cfg_cpol_cpha();
	    break; case '4': cfg_order();
	}
    }
}



