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

#ifndef __MEGA_HWD_PORT_H__
#define __MEGA_HWD_PORT_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Clases para manejar los puertos enteros
 *
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    03/06/2019 Escrito
 *
 ****************************************************************************/
#include <avr/io.h>

namespace mega_{

/*!
 *  \brief  Es el puerto B con todos los pines definidos como salida.
 *
 *  La ventaja de esta clase frente a llamar a PORTB directamente es que no 
 *  hay que recordar inicializar el puerto como salida, ya que es automático.
 *
 */
struct Output_portB{
    Output_portB() {DDRB = 0xFF;}
    Output_portB& operator=(uint8_t x)
    {
        PORTB = x;
        return *this;
    }
};

/*!
 *  \brief  Es el puerto D con todos los pines definidos como salida.
 *
 *  La ventaja de esta clase frente a llamar a PORTD directamente es que no 
 *  hay que recordar inicializar el puerto como salida, ya que es automático.
 *
 */
struct Output_portD{
    Output_portD() {DDRD = 0xFF;}
    Output_portD& operator=(uint8_t x)
    {
        PORTD = x;
        return *this;
    }
};

}// namespace

#endif
