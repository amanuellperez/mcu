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
#pragma once

#ifndef __IR_NEC_PROTOCOL_H__
#define __IR_NEC_PROTOCOL_H__


#include <cstdint>
#include <iostream>
#include <atd_array.h>
#include <atd_string.h>	    // binary_char_to
#include <atd_ostream.h>    // print_int_as_hex
			    
#include "pulse.h"
#include "dev.h"    // Clock_us::counter_type


struct NEC_message{
    uint8_t address;
    uint8_t inv_address;
    uint8_t command;
    uint8_t inv_command;
};

std::ostream& operator<<(std::ostream& out, const NEC_message& msg);
//std::ostream& operator<<(avr::UART_iostream& out, const NEC_message& msg);

void transmit(Clock_us::counter_type time_first_burst_in_us
	     , const NEC_message& msg);

// Devuelve true si a = b +- 20%
// Esto es, si b - 20% <= a <= b + 20%
// (cojo el 20% ya que:
//	1. El clock del avr tiene un 10% de incertidumbre
//	2. Doy un 10% más de margen de error
// )
template <typename Int1, typename Int2>
inline bool is_equal(const Int1& a, const Int2& b)
{ return (b*0.8 <= a and a <= b*1.2); }





// Un mensaje es un array de pulsos
bool NEC_is_start_pulse(const Pulse& pulse);
char NEC_pulse_to_bit(const uint16_t T);


template <typename Message_it>
Message_it NEC_look_for_start(Message_it p, Message_it pe)
{
    for (; p != pe; ++p){
	if (NEC_is_start_pulse(*p))
	    return p;
    }

    return pe;
}


// Se envia el least significant bit first, por eso hay que cambiar el orden.
template <typename Message_it>
Message_it print_NEC_byte_in_binary(std::ostream& out, 
					Message_it p, Message_it pe)
{
    out << "0b";

    if (pe - p < 8) {
	out << "---";
	return pe;
    }

    for (int8_t i = 7; i >= 0; --i){

	if (is_equal(p[i].time_low, 562))
	    out << NEC_pulse_to_bit(p[i].period());

	else 
	    out << 'x';
    }

    return (p + 8);
}


template <typename Message_it>
Message_it print_NEC_byte_in_hex( std::ostream& out
				, Message_it p, Message_it pe)
{
    char res[9];
    res[8] = '\0';
    for (int8_t i = 7; i >= 0 and p != pe; --i, ++p){
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


template <typename Message_it>
Message_it print_NEC_byte(std::ostream& out,
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
template <size_t N>
bool print_NEC_protocol(std::ostream& out, const Train_of_pulses<N>& pulse)
{
    auto pe = pulse.end();
    auto p = NEC_look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "\nNEC\n"
	    "---\n";


    out <<   "First pulse     : (" << p->time_low << ", " << p->time_high << ") us\n";
    ++p;   

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


template <size_t N>
bool print_min_NEC_protocol(std::ostream& out
			    , const Train_of_pulses<N>& pulse)
{
    auto pe = pulse.end();
    auto p = NEC_look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    out << "NEC(" << p->time_low << "): ";


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


#endif
