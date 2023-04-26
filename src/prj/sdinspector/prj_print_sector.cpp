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


#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"


void Main::print_sector()
{
    mcu::UART_iostream uart;
    print_line(uart);
    sector.print(uart);

}

bool Main::print_sector_fromto_ask(Sector::Address& from, size_t& sz)
{
    mcu::UART_iostream uart;

    uart << '\n';
    print_line(uart);

    print_question(uart, msg_print_sector_from, false);
    uart >> from;
    if (from > Sector::sector_size){
	atd::print(uart, msg_print_sector_from_to_big);
	uart << Sector::sector_size << '\n';
	return false;
    }

    uart << '\n';
    print_question(uart, msg_print_sector_size, false);
    uart >> sz;
    if (from + sz > Sector::sector_size){
	atd::print(uart, msg_print_sector_size_to_big);
	uart << '\n';
	return false;
    }

    uart << '\n';

    return true;
}


void Main::print_sector_fromto()
{
    mcu::UART_iostream uart;

    Sector::Address from{};
    size_t sz{};
    if (!print_sector_fromto_ask(from, sz))
	return;

    print_line(uart);
    sector.print(uart, from, sz);

}

