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

#ifndef __IR_NEC_REMOTE_H__
#define __IR_NEC_REMOTE_H__

#include "IR_NEC_protocol.h"


// DUDA: Tengo 2 comandos similares, uno de Elegoo y otro de Nisupa cada uno
// de ellos con comandos diferentes. ¿No hay nada estandar???
class Remote_control_Elegoo{
public:
// Types
    using Command_type = int;

    // OJO: estas defininiciones tienen que estar de acuerdo con el array
    // cmd[].
    // Estos son los nombres de los comandos, por si el programador los 
    // necesita. Suministro dos interfaces: [first, last] para saber si el
    // comando es válido, o un nombre para poder escribir código del tipo:
    //			remote.transmit(RC::asterisk); 
    enum Command : Command_type{
	first = 0,
	n0 = first,
	n1, n2, n3, n4, n5, n6, n7, n8, n9, 
	up, down, left, right,
	asterisk, hashtag, ok,
	last = ok,
	null
    };

// Functions
    /// Enviamos todos los comandos a esta dirección.
    Remote_control_Elegoo(uint8_t address = default_address);

    template <typename Clock_us, typename SWG>
    bool transmit(Command_type i);


private:
// Data
    uint8_t address_; 
    uint8_t inv_address_; 
    
// static data
    constexpr static uint16_t first_burst_in_us = 9000;
    constexpr static uint8_t default_address    = 0x00;

    struct Cmd {uint8_t cmd; uint8_t inv_cmd;};

    // TODO: inv_cmd == ~cmd??? <-- sí? almacenar solo cmd.
    // TODO: a PROGMEM
    constexpr static Cmd cmd[] =
    {
	{0x52, 0xAD},	// 0
	{0x16, 0xE9},	// 1
	{0x19, 0xE6},	// 2
	{0x0D, 0xF2},	// 3
	{0x0C, 0xF3},	// 4
	{0x18, 0xE7},	// 5
	{0x5E, 0xA1},	// 6
	{0x08, 0xF7},	// 7
	{0x1C, 0xE3},	// 8
	{0x5A, 0xA5},	// 9
	{0x46, 0xB9},	// up
	{0x15, 0xEA},	// down
	{0x44, 0xBB},	// left
	{0x43, 0xBC},	// right
	{0x42, 0xBD},	// asterisk
	{0x4A, 0xB5},	// hashtag
	{0x40, 0xBF}	// ok
    };
};

inline Remote_control_Elegoo::Remote_control_Elegoo(uint8_t address)
    :address_{address}
{ 
    inv_address_ = ~address;	// si lo pongo como : inv_address_{~address}
				// me da un warning de narrowing ???
}
 

template <typename Clock_us, typename SWG>
bool Remote_control_Elegoo::transmit(Command_type i)
{ 
    if (first <= i and i <= last){
	NEC_message msg{address_, inv_address_, cmd[i].cmd, cmd[i].inv_cmd};
	NEC_protocol::transmit<Clock_us, SWG>(first_burst_in_us, msg); 
	return true;
    }

    return false;
}

#endif


