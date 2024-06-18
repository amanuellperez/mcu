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

#include <atd_ostream.h>

#include "dev.h"
#include "init.h"
#include "print.h"
#include "strings.h"
#include "read_block.h"

void press_key_to_continue()
{
    mcu::UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_press_key_to_continue);
    char c{};
    uart >> c;
}

void read_status()
{
    mcu::UART_iostream uart;

    auto r2 = SDCard::send_status();
    print(uart, r2);
}

void ask_modify_block(SDCard::Block data)
{
    // TODO: preguntar
    data[0] = 0xAB;
    data[1] = 0xCD;
    data[2] = 0xEF;
}

void write_block()
{
    mcu::UART_iostream uart;

    atd::print(uart, msg_main_write);
    print_question(uart, msg_address, false);

    SDCard::Address add;
    uart >> add;
    uart << add << '\n';

    uint8_t data[SDCard::block_size];
    if (read_block(add, data)){
	ask_modify_block(data);
	auto res = SDCard::write(add, data);
	print(uart, res);
    }

    
}


int main()
{
// init_UART();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();

    Chip_select::init();

    atd::print(uart, msg_hello);
    atd::print(uart, msg_main_menu);

    char ans{};
    uart >> ans;

    if (ans == '1')
	step_by_step_init();

    else
	automatic_init();

    
    while(1){
	atd::print(uart, msg_main_menu2);
	uart >> ans;

	switch(ans){
	    break; case '1': read_status();
	    break; case '2': read_block();
	    break; case '3': write_block();
	}
    }
}





