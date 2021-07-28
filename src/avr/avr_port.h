// Copyright (C) 2019-2020 A.Manuel L.Perez 
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

#ifndef __AVR_PORT_H__
#define __AVR_PORT_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Clases para manejar los puertos enteros
 *
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 03/06/2019 Escrito
 *
 ****************************************************************************/
#include <avr/io.h>

namespace avr{

/*!
 *  \brief  Es el puerto B con todos los pines definidos como salida.
 *
 *  La ventaja de esta clase frente a llamar a PORTB directamente es que no 
 *  hay que recordar inicializar el puerto como salida, ya que es automático.
 *
 */
struct PuertoB_de_salida{
    PuertoB_de_salida() {DDRB = 0xFF;}
    PuertoB_de_salida& operator=(uint8_t x)
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
struct PuertoD_de_salida{
    PuertoD_de_salida() {DDRD = 0xFF;}
    PuertoD_de_salida& operator=(uint8_t x)
    {
        PORTD = x;
        return *this;
    }
};

}// namespace

#endif
