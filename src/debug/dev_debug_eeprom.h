// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __DEV_DEBUG_EEPROM_H__
#define __DEV_DEBUG_EEPROM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz para depurar una EEPROM.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	22/11/2019 v0.0 Escrito.
 *
 ****************************************************************************/

#include <ctype.h>	    // isalnum
#include <cstddef>    // uint8_t
#include <stdint.h>
#include <ostream>


namespace dev{


class EEPROM_debug{
public:

// Vuelca como máximo n bytes de la eeprom en el flujo out. 
// Vuelca los bytes [addr, addr + n) siempre y cuando no alcance el final de
// la memoria.
// Returns: el número de bytes volcados.
// TODO: ¿No hay posibilidad de ningún tipo de error? Raro...
template <typename EEPROM>
// requires: EEPROM es lineal.
static uint16_t
send(EEPROM& eeprom, std::ostream& out, uint16_t addr, uint8_t n)
{
    constexpr uint8_t sz = 255; // n <= 255 ya que es uint8_t

    uint8_t buf[sz];

    uint16_t leidos = eeprom.read(addr, buf, n);
    dump_buffer_fmt(out, addr, buf, leidos);

    return leidos;
}

private:
// Vuelca el contenido del buffer buf con formato
static void dump_buffer_fmt(std::ostream& out, 
		uint16_t addr0, const uint8_t* buf, uint8_t n);
};// EEPROM_debug



}// namespace

#endif


