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
#pragma once

#ifndef __NEC_PRINT_TCC__
#define __NEC_PRINT_TCC__

#include <atd_string.h>	    // binary_char_to

namespace dev{

template <typename Message_it>
Message_it NEC_protocol::look_for_start(Message_it p, Message_it pe)
{
    for (; p != pe; ++p){
	if (is_start_pulse(*p))
	    return p;
    }

    return pe;
}


// Se envia el least significant bit first, por eso hay que cambiar el orden.
template <typename Message_it>
Message_it NEC_protocol::print_byte_in_binary(std::ostream& out, 
					Message_it p, Message_it pe)
{
    out << "0b";

    if (pe - p < 8) {
	out << "---";
	return pe;
    }

    for (int8_t i = 7; i >= 0; --i){

	if (is_equal(p[i].time_low, t_burst))
	    out << decode_period_as_char(p[i].period());

	else 
	    out << 'x';
    }

    return (p + 8);
}


template <typename Message_it>
Message_it NEC_protocol::print_byte_in_hex( std::ostream& out
				, Message_it p, Message_it pe)
{
    char res[9];
    res[8] = '\0';
    for (int8_t i = 7; i >= 0 and p != pe; --i, ++p){
	if (is_equal(p->time_low, t_burst))
	    res[i] = decode_period_as_char(p->period());
	
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


template <typename Message_it>
Message_it NEC_protocol::print_byte(std::ostream& out,
					    Message_it p0, Message_it pe)
{
    auto p = print_byte_in_binary(out, p0, pe);

    out << '\t';

    print_byte_in_hex(out, p0, pe);

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
template <size_t N>
bool NEC_protocol::print_verbose(std::ostream& out, 
					const dev::Train_of_pulses<N>& pulse)
{
    auto pe = pulse.end();
    auto p = look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "\nNEC\n"
	    "---\n";


    out <<   "First pulse     : (" << p->time_low << ", " << p->time_high << ") us\n";
    ++p;   

    out <<   "Address         : ";
    p = print_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nInverted address: ";
    p = print_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nCommand         : ";
    p = print_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }


    out << "\nInverted command: ";
    p = print_byte(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << '\n';

// DUDA: miramos si quedan más bytes para imprimir?

    return true;
}


// NO CAMBIAR!!!
//	Aunque podemos escribir esta función convirtiendo el tren de pulsos
//	primero en un msg NEC, no hacerlo. Esta función print añade
//	funcionalidad: en caso de que no entienda algún bit lo imprime como
//	'x' ó '?' cosa que no haríamos si la implementamos usando primero
//	`convert(ptrain).into(msg)` y luego imprimiendo el mensaje.
template <size_t N>
bool NEC_protocol::print(std::ostream& out
			    , const dev::Train_of_pulses<N>& pulse)
{
    auto pe = pulse.end();
    auto p = look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "NEC(" << p->time_low << "): ";


    ++p;    // quitamos el start pulse
    p = print_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << ' ';
    p = print_byte_in_hex(out, p, pe);

    if (p == pe){
	out << '\n';
	return true; // es NEC
    }

    out << '\n';

// DUDA: miramos si quedan más bytes para imprimir?

    return true;
}

}// namespace
 
#endif
