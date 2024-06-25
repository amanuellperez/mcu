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

bool read_block(SDCard::Address addr, SDCard::Block data)
{
    myu::UART_iostream uart;

    auto r = SDCard::read(addr, data);
    print(uart, r);

    if (r.ok()){
	atd::print(uart, msg_read_ok);

	atd::print(uart, msg_address);
	uart << ' ' << addr << '\n';
	print_block(uart, data);
	return true;
    }

    return false;
}

void read_block()
{
    myu::UART_iostream uart;

    atd::print(uart, msg_main_read);
    uart << (int) SDCard::block_size << '\n';

    print_question(uart, msg_address, false);
    SDCard::Address add;
    uart >> add;
    uart << add << '\n';

    uint8_t data[SDCard::block_size];
    read_block(add, data);
}


