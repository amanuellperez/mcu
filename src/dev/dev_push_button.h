// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_PUSHBUTTON_H__
#define __DEV_PUSHBUTTON_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Push_button
 *
 *   - HISTORIA:
 *    A.Manuel L.Perez  - 24/07/2017 Escrito
 *
 ****************************************************************************/
#include "avr_pin.h"

namespace dev{

/*!
 *  \brief  Push_button conectado a un pin.
 *
 *  El pulsador se conecta directamente al pin, por un lado, y a tierra, por
 *  el otro. El pin se configurará con la resistencia pull-up. De esta forma, 
 *  cuando no esté pulsado el pin leerá un 1, y al pulsarse leerá un 0.
 *
 *  Este pulsador es básico, no tiene en cuenta el bouncing. Es para, por
 *  ejemplo, encender un led cuando está pulsado un botón o cosas así.
 *
 *  El pin de entrada lo configuramos como con resistencia pull-up
 *
 */
template <uint8_t n>
class Push_button{
public:
    Push_button() {avr::Pin<n>::as_input_with_pullup();}

    /// ¿está pulsado?
    bool is_pressed()
    {return avr::Pin<n>::is_zero();}

    /// ¿no está pulsado?
    bool is_not_pressed()
    {return !is_pressed();}

    /// Espera hasta que se pulsa el interruptor.
    void wait_until_is_pressed()
    { while(!is_pressed()) ; }
};



}// namespace

#endif
