// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../dev_25LC256_basic.h"
#include <atd_cast.h>

#include <algorithm>

#include <mega.h>

// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART_8bits;
using UART_iostream = mcu::UART_iostream<UART>;

using namespace dev;


constexpr uint8_t num_pin_chip_select = 16;
constexpr uint8_t periodo_en_us = 16;
using EEPROM  = EEPROM_25LC256<num_pin_chip_select>;


void print_table(EEPROM& eeprom)
{
    UART_iostream uart;

    uart << "Address value\r\n";
    uint8_t r[10];
    for (uint8_t i = 0; i < 10; ++i)
	eeprom.read(10*i, &r[i], 1);

    for (uint8_t i = 0; i < 10; ++i)
	uart << "   " << 10 * i << "   " << r[i]
	     << "\r\n";
}

void print_eeprom(EEPROM& eeprom)
{
    UART_iostream uart;
    
    constexpr uint16_t sz = EEPROM::page_size() * 2;

    uint8_t r[sz];

    EEPROM::size_type n = eeprom.read(0, r, sz);
    if (n != sz)
        uart << "Error: se han leido [" << n << "] bytes en lugar de sz = ["
             << sz << "]\n\r";

    for (EEPROM::size_type i = 0; i < sz; ++i){
	if (!(i % 10)){
	    uart << "\n\r" << i << " |";
	}
	uart << "  " << r[i];
    }

    uart << "\r\n";
}

uint8_t read_uint8_t(const char* msg, UART_iostream& uart)
{
    uart << msg;

    uint16_t m;
    uart >> m;
    uint8_t n = atd::bounded_cast<uint8_t>(m);

    uart << n << "\n\r";

    return n;
}


void test_write(EEPROM& eeprom)
{
    UART_iostream uart;

    uart << "¿A partir de qué dirección quieres escribir?\r\n";
    uint16_t address;
    uart >> address;
    uart << address << "\r\n";
    uint8_t n0 = read_uint8_t("¿A partir de qué número escribimos?\n\r", uart);
    uint8_t n = read_uint8_t("¿Cuántos números escribimos?\n\r", uart);

    uint8_t b[255];
    uint8_t sz = std::min<uint8_t>(n, 255);
    for (uint8_t i  = 0; i < sz; ++i)
	b[i] = uint8_t{static_cast<uint8_t>(n0 + i)};

    while (eeprom.write_in_process(eeprom.read_status_register()))
    {;}

    // OJO: primero enable, luego write!!!
    eeprom.write_enable();
    eeprom.write(address, &b[0], sz);
    uart << "\n\r";
}



void test_uint8_t() {
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    myu::SPI_master::clock_period_in_us<periodo_en_us>();
    myu::SPI_master::turn_on();

    EEPROM eeprom;
    eeprom.cfg_SPI();


    while(1){
	uart << "\r\nPrueba EEPROM\r\n"
		    "-------------\r\n";

	// print_table(eeprom);
	print_eeprom(eeprom);

//	uart << " [e] to erase all memory\r\n";
	uart << " [w] to write byte to memory\r\n\r\n";
	
	char ans;
	if (!uart.get(ans)){
	    uart.clear();
	    uart << "Error de lectura\n\r";
	}

	uart << ans << "\r\n";
	switch(ans){
//	    case 'e':
//		uart << "Borrando memoria ... ";
//		eeprom.clear();
//		uart << "OK\r\n";
//		break;

	    case 'w':
		test_write(eeprom);

		break;

	    default:
		uart << "what?\r\n";
		break;
	}

    }// while
}



int main()
{
    test_uint8_t();
}
