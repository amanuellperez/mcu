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

void Main::clone_remote_control()
{
    UART uart;
    uart << '\n';
    atd::print(uart, msg_clone_remote_control);

    while (1){
	if (receive())
	    clone_pulses();

	else if (!clone_read_uart())
	    return;

    }

}

void Main::clone_pulses()
{
    UART uart;

    if (msg.full()){
	atd::print(uart, msg_clone_msg_full);
	return;
    }

    dev::NEC_message tmp;
    if (!atd::convert(pulse).into(tmp)){
	atd::print(uart, msg_error);
	return;
    }

    msg.push_back(tmp);

    uart << (msg.size() - 1) << '\t' << tmp << '\n';

}

bool Main::clone_read_uart()
{
    if (!UART::are_there_data_unread())
	return false;

    UART uart;

    char c{};
    uart >> c;

    switch(c){
	break; case 'c': 
	       case 'C': clone_msg_clear();
				
	break; case 'e': 
	       case 'E': return false;

    }

    return true;
}

void Main::clone_msg_clear()
{
    msg.clear();

    UART uart;
    atd::print(uart, msg_clone_msg_clear);
}



