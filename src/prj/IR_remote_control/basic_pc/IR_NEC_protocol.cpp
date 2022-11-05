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
#include <atd_string.h>	    // binary_char_to
#include <atd_ostream.h>    // print_int_as_hex

using Message_it = atd::CArray_view<Pulse>::iterator;

// Aunque la especificación del NEC indica que el pulso de start tiene que ser
// 9ms low seguido de 4'5 ms high, tengo un mando que envía 4'5 ms low y luego
// 4'5 ms high. De momento lo implemento así.
bool NEC_is_start_pulse(const Pulse& pulse)
{
    if (!(is_equal(pulse.time_low, 9000) or is_equal(pulse.time_low, 4500)))
	    return false;

    return is_equal(pulse.time_high, 4500);
}


Message_it NEC_look_for_start(Message_it p, Message_it pe)
{
    for (; p != pe; ++p){
	if (NEC_is_start_pulse(*p))
	    return p;
    }

    return pe;
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

static Message_it print_NEC_byte_in_binary(std::ostream& out, 
					Message_it p, Message_it pe)
{
    out << "0b";
    for (int8_t i = 7; i >= 0 and p != pe; --i, ++p){

	if (is_equal(p->time_low, 562))
	    out << NEC_pulse_to_bit(p->period());

	else 
	    out << 'x';
    }

    return p;
}

static Message_it print_NEC_byte_in_hex(std::ostream& out, 
					Message_it p, Message_it pe)
{
    char res[9];
    res[8] = '\0';
    for (uint8_t i = 0; i < 8 and p != pe; ++i, ++p){
	if (is_equal(p->time_low, 562))
	    res[i] = NEC_pulse_to_bit(p->period());
	
	else
	    res[8] = 'x';
    }

    if (res [8] == '\0'){
	atd::print_int_as_hex(out, atd::binary_char_to<uint8_t>(res));
	return p;
    }
    else {
	out << "0x????";
	return pe;
    }

}

// The 4 bytes of data bits are each sent least significant bit first.
// La ventaja de imprimir es que permite averiguar errores en el formato
// recibido: imprimirá '?' si el pulso enviado es diferente del esperado, o
// 'x' si el burst no es de la longitud esperada.
static Message_it print_NEC_byte(std::ostream& out,
					    Message_it p0, Message_it pe)
{
    auto p = print_NEC_byte_in_binary(out, p0, pe);

    out << '\t';

    print_NEC_byte_in_hex(out, p0, pe);

    return p;
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
bool print_NEC_protocol(std::ostream& out, const atd::CArray_view<Pulse>& pulse)
{
    auto pe = pulse.end();
    auto p = NEC_look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "\nNEC\n"
	    "---\n";


    ++p;    // quitamos el start pulse
    out <<   "Address         : ";
    p = print_NEC_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nInverted address: ";
    p = print_NEC_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nCommand         : ";
    p = print_NEC_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nInverted command: ";
    p = print_NEC_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << '\n';

// DUDA: miramos si quedan más bytes para imprimir?

    return true;
}


bool print_min_NEC_protocol(std::ostream& out, const atd::CArray_view<Pulse>& pulse)
{
    auto pe = pulse.end();
    auto p = NEC_look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "NEC: ";


    ++p;    // quitamos el start pulse
    p = print_NEC_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_NEC_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_NEC_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_NEC_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << '\n';

// DUDA: miramos si quedan más bytes para imprimir?

    return true;
}
