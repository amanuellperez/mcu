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

    uart << "Message (write everything in hexadecimal):\n"
	    "Address: ";

//    NEC_message msg{0x00, 0x30, 0x0D, 0xF2}; // bajar volumen
//    uart >> msg.address;
//    uart << "Has escrito [" << msg.address << "]\n";

    // NEC_message msg{0xFF, 0xFF, 0xFF, 0xFF};
    NEC_message msg_down{0x07, 0x07, 0x0B, 0xF4}; // bajar volumen
    NEC_message msg_up{0x07, 0x07, 0x07, 0xF8}; // subir volumen

    while(true){
	uart << "Envio\n";
    transmit(time_first_burst_in_us, msg_down); // pasar la longitud del primer burst
    wait_ms(1000);
    transmit(time_first_burst_in_us, msg_up); // pasar la longitud del primer burst
    wait_ms(1000);

    }
}

