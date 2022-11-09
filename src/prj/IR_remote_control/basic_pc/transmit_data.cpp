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
    uart << "Transmit_data\n";

    // NEC_message msg{0xFF, 0xFF, 0xFF, 0xFF};
    // NEC_message msg{0xE0, 0xE0, 0xD0, 0x2F}; // bajar volumen
//    NEC_message msg{0xE0, 0xE0, 0xE0, 0x1F}; // subir volumen
    NEC_message msg{0x00, 0x30, 0x0D, 0xF2}; // bajar volumen

    while(true){
	uart << "Envio\n";
    transmit(9000, msg); // pasar la longitud del primer burst
    wait_ms(1000);

    }
}

