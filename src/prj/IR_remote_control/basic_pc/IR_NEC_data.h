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

#ifndef __IR_NEC_DATA_H__
#define __IR_NEC_DATA_H__

#include "IR_NEC_protocol.h"

enum class Remote_control_protocol{
    NEC, RC5 // ...
};

// (RRR) ¿Cómo definir los comandos del control remoto?
//       Opción 1:
//	    En prj_dev.h se define el control remoto a usar:
//		
//		using Remote_control = dev::Remote_control<Elegoo>;
//	    
//	    y en el programa se usaría un control remoto genérico lo cual
//	    facilita la labor del programador. 
//
//	    De momento pruebo a ir viendo cómo implementar esto.
template <typename T>
struct Remote_control{ };

struct Elegoo {};


template <>
struct Remote_control<Elegoo>{

Remote_control_protocol protocol = Remote_control_protocol::NEC;

constexpr static uint16_t first_burst = 9000;

// Como suele ser habitual usar uint8_t genera problemas, por eso lo defino
// como int16_t
using index_type = int16_t;

// OJO: estas defininiciones tienen que estar de acuerdo con
// Remote_control_elegoo
// Estos son los nombres de los comandos, por si el programador los necesita.
// Ejemplo: transmit(cmd[asterisk]); <-- esto es legible.
enum index : index_type {
    first = 0,
    n1 = first,
    n2, n3, n4, n5, 
    n6, n7, n8, n9, 
    asterisk,
    n0,
    hashtag,
    up  , down,
    left, right,
    ok,
    last = ok,
    null
};

// TODO: a PROGMEM
constexpr static NEC_message cmd[] =
{                               // boton de Elegoo
    {0x00, 0xFF, 0x16, 0xE9},	// 1
    {0x00, 0xFF, 0x19, 0xE6},	// 2
    {0x00, 0xFF, 0x0D, 0xF2},	// 3
    {0x00, 0xFF, 0x0C, 0xF3},	// 4
    {0x00, 0xFF, 0x18, 0xE7},	// 5
    {0x00, 0xFF, 0x5E, 0xA1},	// 6
    {0x00, 0xFF, 0x08, 0xF7},	// 7
    {0x00, 0xFF, 0x1C, 0xE3},	// 8
    {0x00, 0xFF, 0x5A, 0xA5},	// 9
    {0x00, 0xFF, 0x42, 0xBD},	// asterisk
    {0x00, 0xFF, 0x52, 0xAD},	// 0
    {0x00, 0xFF, 0x4A, 0xB5},	// hashtag
    {0x00, 0xFF, 0x46, 0xB9},	// up
    {0x00, 0xFF, 0x15, 0xEA},	// down
    {0x00, 0xFF, 0x44, 0xBB},	// left
    {0x00, 0xFF, 0x43, 0xBC},	// right
    {0x00, 0xFF, 0x40, 0xBF}	// ok
};

};

 
#endif


