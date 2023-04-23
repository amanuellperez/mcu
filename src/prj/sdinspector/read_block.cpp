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


#include "read_block.h"
#include "strings.h"
#include "print.h"

void print_block(std::ostream& out, SDCard::Block data)
{
    atd::print(out, msg_line);
    for (uint16_t i = 0; i < SDCard::block_size; ++i){
	if (i % 20 == 0 and i != 0)
	    out << '\n';
	atd::print_int_as_hex(out, data[i], false);
	out << ' ';
    }
    out << '\n';
}

// Esta función la usan write_block y read_block
bool read_block(SDCard::Address addr, SDCard::Block data)
{
    mcu::UART_iostream uart;

    auto r = SDCard::read(addr, data);

    if (r.ok()){
	return true;
    }
    else {
	// TODO: poner "error!!!: DETALLES"
	print(uart, r);
    }

    return false;
}

void read_block()
{
    mcu::UART_iostream uart;

    atd::print(uart, msg_main_read);
    uart << (int) SDCard::block_size << '\n';

    print_question(uart, msg_address, false);
    SDCard::Address add;
    uart >> add;
    uart << add << '\n';

    Sector sector; // TODO: a Main
    if (read_block(add, sector)){
	sector.address = add;
	sector.print(uart);
    }

}


