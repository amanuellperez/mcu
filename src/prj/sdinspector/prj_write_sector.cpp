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

#include "sdc_print.h"
#include "dev_print.h"

void Main::ask_modify_block(SDCard::Block data)
{
    // TODO: preguntar
    data[0] = 0xAB;
    data[1] = 0xCD;
    data[2] = 0xEF;
}

void Main::write_block()
{
    mcu::UART_iostream uart;

    atd::print(uart, msg_main_write);
    print_question(uart, msg_address, false);

    SDCard::Address add;
    uart >> add;
    uart << add << '\n';

    uint8_t data[SDCard::block_size];
    if (read_sector(add, data)){
	ask_modify_block(data);
	auto res = SDCard::write(add, data);
	print(uart, res);
    }

    
}
