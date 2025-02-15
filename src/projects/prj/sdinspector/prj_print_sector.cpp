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

#include <atd_ostream.h>

void Main::print_sector()
{
    UART_iostream uart;
    print_line(uart);
    print_sector(uart, sector);

}

bool Main::print_sector_fromto_ask(Sector::size_type& from, size_t& sz)
{
    UART_iostream uart;

    uart << '\n';
    print_line(uart);

    print_question(uart, msg_print_sector_from, false);
    uart >> from;
    if (from > sector.size()){
	atd::print(uart, msg_print_sector_from_to_big);
	uart << sector.size()<< '\n';
	return false;
    }

    uart << '\n';
    print_question(uart, msg_print_sector_size, false);
    sz = 0;
    uart >> sz; // TODO: si se da retorno de carro modifica 'sz' ????
		// De hecho hace cosas raras. Para no depurarlo le indico al
		// usuario que escriba explícitamente '0'. TODO: depurarlo!!!
		// Al rato ha empezado a funcionar bien (???)

    if (sz == 0)    // imprimimos hasta el final
	sz = sector.size() - from;

    else if (from + sz > sector.size()){
	atd::print(uart, msg_print_sector_size_to_big);
	uart << '\n';
	return false;
    }

    uart << '\n';

    return true;
}


void Main::print_sector_fromto()
{
    UART_iostream uart;

    Sector::size_type from{};
    size_t sz{};
    if (!print_sector_fromto_ask(from, sz))
	return;

    uart << '\n';
    print_line(uart);
    print_sector(uart, sector, from, sz);

}



template <size_t n>
void print_as_str(std::ostream& out, std::span<const uint8_t, n> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}

void print_as_str(std::ostream& out, std::span<uint8_t> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}
