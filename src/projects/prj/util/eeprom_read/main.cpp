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

/****************************************************************************
 *
 *   - DESCRIPCION: Aplicación que sirve para leer el contenido de una EEPROM
 *
 *
 *   - HISTORIA:
 *	Manuel Perez- 22/09/2019 v0.0
 *
 ****************************************************************************/
#include <dev_EEPROM_iostream.h>
#include <dev_EEPROM_lineal.h>
#include <dev_25LC256_hwd.h>

#include <mega.h>

#include <atd_cstddef.h>
#include <atd_istream.h>
#include <atd_ostream.h>

#include "dev_debug_eeprom.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

constexpr uint8_t period_in_us = 16;

constexpr uint8_t num_pin_chip_select = 16;

using EEPROM_lineal =
    dev::EEPROM_lineal<dev::EEPROM_25LC256<num_pin_chip_select>>;

using EEPROM_ostream = dev::EEPROM_ostream<EEPROM_lineal>;
using EEPROM_istream = dev::EEPROM_istream<EEPROM_lineal>;


constexpr const char ERROR[] = "ERROR ";

constexpr uint8_t sz = 255;

void init_UART()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void init_SPI()
{
    myu::SPI_master::clock_period_in_us<period_in_us>();
    myu::SPI_master::turn_on();
}


void presentacion(const char title[])
{
    UART_iostream uart;

    uart << '\n' << title << '\n';
    uart <<     "-----------\n";
    uart << "Pulsa una tecla\n";
    char c;
    uart.get(c);
}


std::pair<uint16_t, uint8_t> user_read_peticion()
{
    UART_iostream uart;

    uint16_t addr = 0;
    uart << "\n¿Dirección a partir de la que leer? (0 por defecto)\n";
    uart >> addr;
    if (!uart){
	uart.clear();
	addr = 0;
    }
    uart << addr << "\n";

    uint8_t n = sz;
    uart << "¿Cuántos bytes leemos? (máximo " << atd::write_as_int(sz)
         << ") (30 por defecto)\n";
    uart >> atd::read_as_uint8_t(n);
    if (!uart){
	uart.clear();
	n = 30;
    }
    uart << n << "\n";

    return {addr, n};
}


void run()
{
    auto [addr, n] = user_read_peticion();

    EEPROM_lineal eeprom;
    UART_iostream uart;
    dev::EEPROM_debug::send(eeprom, uart, addr, n);
}


int main()
{
    init_UART();
    init_SPI();

    while(1){
	presentacion("Read EEPROM");
	run();
    }
}
