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

#include "IR_NEC_protocol.h"

bool is_NEC_protocol(const uint16_t* msg, uint8_t n)
{
    if (n < 2)
	return false;

    return (is_equal(msg[0], 9000) and is_equal(msg[1], 4500));
}


// NEC bit format (pulse distance encoding):
//	* logical '0': a 562.5us pulse burst followed by 562.us space, with
//	               a total transmit time of 1.125 ms.
//	* logical '1': a 562.5us pulse burst followed by a 1.678 ms space,
//	               with a total transmit time of 2.25 ms.
static char NEC_pulse_to_bit(const uint16_t T)
{
    if (is_equal(T, 1125))
	return '0';

    else if (is_equal(T, 2250))
	return '1';

    else 
	return '?';
}


// The 4 bytes of data bits are each sent least significant bit first.
// La ventaja de imprimir es que permite averiguar errores en el formato
// recibido: imprimirá '?' si el pulso enviado es diferente del esperado, o
// 'x' si el burst no es de la longitud esperada.
static void print_NEC_byte(std::ostream& out, const uint16_t* p)
{
    out << "0b";
    for (int8_t i = 7; i >= 0; --i){

	if (is_equal(p[2*i], 562))
	    out << NEC_pulse_to_bit(p[2*i] + p[2*i + 1]);

	else 
	    out << 'x';
    }

}


// precondition: sabemos que msg[0] == 9000, y msg[1] == 4500
// NEC:
//	9 ms leading pulse burst
//	4.5 ms space
//	the 8-bit address for receiving device
//	the 8-bit logical inverse address
//	the 8-bit command
//	the 8-bit logical inverse of the command
//	a final 562.5 us burst to signify the end fo message transmission
//
//  
bool print_NEC_protocol(std::ostream& out, const uint16_t* msg, uint8_t n)
{
    out << "\nNEC\n"
	    "---\n";

    // 33 bits = 1 bit preambulo + 4 bytes (32 bits)
    if (n < 33*2 + 1){
	out << "Error: no se han recibido todos los bytes\n";
	return false;
    }

// msg[0] = 9000 us; msg[1] = 4500 us
    out <<   "Address         : ";
    print_NEC_byte(out, &msg[2]);

    out << "\nInverted address: ";
    print_NEC_byte(out, &msg[2 + 16*1]);	// 16 = 2 * 8 bits que tiene 1 byte

    out << "\nCommand         : ";
    print_NEC_byte(out, &msg[2 + 16*2]);

    out << "\nInverted command: ";
    print_NEC_byte(out, &msg[2 + 16*3]);
    out << '\n';

//    check_end_NEC(&msg[2 + 16*4]);
    return true;
}


