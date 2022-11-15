// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "main.h"
#include "IR_NEC_protocol.h"
#include <atd_istream.h>    // read_int_as_hex
#include <atd_ostream.h>    // print_int_as_hex

static void read_NEC_message(NEC_message& msg)
{
    avr::UART_iostream uart;

    uart << "Message (write everything in hexadecimal 0x...):\n";

    uart << "Address: ";
    atd::read_int_as_hex(uart, msg.address);
    atd::print_int_as_hex(uart, msg.address);

    uart << "\nInverted address: ";
    atd::read_int_as_hex(uart, msg.inv_address);
    atd::print_int_as_hex(uart, msg.inv_address);

    uart << "\nCommand: ";
    atd::read_int_as_hex(uart, msg.command);
    atd::print_int_as_hex(uart, msg.command);

    uart << "\nInverted command: ";
    atd::read_int_as_hex(uart, msg.inv_command);
    atd::print_int_as_hex(uart, msg.inv_command);
}


void Main::transmit_data()
{
    avr::UART_iostream uart;
    uart << "Transmit data\n"
	    "-------------\n";

    uart << "Protocol:\n"
	    "(1) NEC (now the only option)\n\n";

    uart << "Length of first burst:\n"
	    "(1) 9.0 ms\n"
	    "(2) 4.5 ms\n";

    char res{};
    uart >> res;
    Timer::counter_type time_first_burst_in_us = 9000;

    if (res == '2')
	time_first_burst_in_us = 4500;

    NEC_message msg{0xFF, 0xFF, 0xFF, 0xFF};
    read_NEC_message(msg);
    
    uart << "Sending: ";
    uart << msg << '\n'; // si se pone a continuación del const char* da error!!!
    transmit(time_first_burst_in_us, msg); // pasar la longitud del primer burst
}

