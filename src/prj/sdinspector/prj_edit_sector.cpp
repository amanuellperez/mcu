// Copyright (C) 2023 Manuel Perez 
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

#include "prj_main.h"
#include <atd_ostream.h>
#include <atd_istream.h>

#include "sdc_print.h"
#include "dev_print.h"

void Main::edit_sector()
{
    mcu::UART_iostream uart;

    uart << '\n';
    print_line(uart);
    atd::print(uart, msg_edit_sector_instructions);
    print_question(uart, msg_edit_sector_from, false);

    Sector::Address addr;
    uart >> addr;
    uart << '\n';

    edit_sector_from(addr);
    
}

void Main::edit_sector_from(Sector::Address addr)
{
    mcu::UART_iostream uart;

    while (addr < sector.size()) {
	uart.fill('0');
	uart.width(3);
	uart << addr << ':';
	atd::print_int_as_hex(uart, sector[addr], false);
	uart << " -> ";

	uint8_t data{}; 
	if (atd::read_int_as_hex(uart, data, false) == 0)
	    return;

	atd::print_int_as_hex(uart, data, false); // echo


	sector[addr] = data;

	++addr;
	uart << '\n';
    }

}

