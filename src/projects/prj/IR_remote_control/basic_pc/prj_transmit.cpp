// Copyright (C) 2022 Manuel Perez 
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
#include "dev_NEC_protocol.h"
#include <atd_istream.h>    // read_int_as_hex
#include <atd_ostream.h>    // print_int_as_hex

static void ask_NEC_message(dev::NEC_message& msg)
{
    UART uart;

    atd::print(uart, msg_transmit_data_menu_first_burst);

    char res{};
    uart >> res;

    msg.time_first_pulse = 9000;

    if (res == '2')
	msg.time_first_pulse = 4500;

    atd::print(uart, msg_ask_NEC_message);

    atd::print(uart, msg_address);
    atd::read_int_as_hex(uart, msg.address);
    atd::print_int_as_hex(uart, msg.address);

    uart << '\n';
    atd::print(uart, msg_inverted_address);
    atd::read_int_as_hex(uart, msg.inv_address);
    atd::print_int_as_hex(uart, msg.inv_address);

    uart << '\n';
    atd::print(uart, msg_command);
    atd::read_int_as_hex(uart, msg.command);
    atd::print_int_as_hex(uart, msg.command);

    uart << '\n';
    atd::print(uart, msg_inverted_command);
    atd::read_int_as_hex(uart, msg.inv_command);
    atd::print_int_as_hex(uart, msg.inv_command);
}


void Main::send_message()
{
    UART uart;
    atd::print(uart, msg_transmit_data);

    atd::print(uart, msg_transmit_data_menu_protocol);
    uart << '\n';


    dev::NEC_message msg{9000, 0xFF, 0xFF, 0xFF, 0xFF};
    ask_NEC_message(msg);
    
    uart << '\n';
    atd::print(uart, msg_sending);
    uart << msg << '\n'; // si se pone a continuación del const char* da error!!!
					   //
    dev::NEC_protocol::transmit<Miniclock_us, SWG>(msg);
}

