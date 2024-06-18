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

#ifndef __DEV_LED_H__
#define __DEV_LED_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	LED
 *
 * HISTORIA
 *    Manuel Perez
 *    24/07/2017 Escrito
 *    18/09/2023 Eliminadas dependencias de avr.
 *
 ****************************************************************************/
#include <stdint.h>

namespace dev{

/*!
 *  \brief  Es un led vulgar y corriente.
 *
 */
// template <uint8_t n>
template <typename Micro, uint8_t n>
class LED{
public:
    using Pin = typename Micro::Pin<n>;

    LED() {init();}

    static void init() {Pin::as_output();}

    /// Encendemos el led
    static void turn_on() {Pin::write_one();}

    /// Apagamos el led
    static void turn_off() {Pin::write_zero();}

    /// Si está encendido lo apaga, y si está apagado lo enciende.
    static void toggle() {Pin::toggle();}
};



}// namespace

#endif
