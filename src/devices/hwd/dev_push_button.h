// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __DEV_PUSHBUTTON_H__
#define __DEV_PUSHBUTTON_H__
/****************************************************************************
 *
 * DESCRIPCION: Push_button
 *
 * HISTORIA
 *    Manuel Perez
 *    24/07/2017 Push_button v0.0
 *    19/02/2021 Push_button_level_change v0.0
 *    18/09/2023 Eliminando dependencias de avr. 
 *               enable/disable_interrupt
 *
 ****************************************************************************/
#include <stdint.h>

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
 *  El pin de entrada lo configuramos como con resistencia pull-up.
 *
 *  Dos formas de usarlo:
 *	1) La del vago:	
 *		Push_button<23> b;
 *		if (b.is_pressed()) ...
 *
 *	   No tenemos que hacer nada.
 *
 *	2) Como interfaz: 
 *		using Enter_button = Push_button<23>;
 *		Enter_button::init(); // no olvidarlo!!!
 *
 *		if (Enter_button::is_pressed()) ...
 *
 *  Si el compilador optimiza el código deberían de generar el mismo código
 *  las dos.
 */
//template <uint8_t n>
template <typename Micro, uint8_t n>
class Push_button{
public:
// Types
// -----
    using Pin = typename Micro::Pin<n>;

// Cfg
// ---
    static constexpr uint8_t pin_number = n;

// Constructors
// ------------
    constexpr Push_button() {init();}

    /// Inicializamos el pulsador.
    static constexpr void init() 
    { Pin::as_input_with_pullup(); }


// Press/not press
// ---------------
    /// ¿está pulsado?
    static constexpr bool is_pressed()
    //{return not_generic::Pin<n>::is_zero();}
    {return Pin::is_zero();}

    /// ¿no está pulsado?
    static constexpr bool is_not_pressed()
    {return !is_pressed();}

    /// Espera hasta que se pulsa el interruptor.
    static constexpr void wait_until_is_pressed()
    { while(!is_pressed()) ; }

// Interrupts
// ----------
// Recordar definir la ISR correspondiente!!!
    static void enable_interrupt()
    { Pin::enable_change_level_interrupt(); }

    static void disable_interrupt()
    { Pin::disable_change_level_interrupt(); }
};


/*!
 *  \brief  Pulsador que devuelve press solo cuando se produce un cambio de
 *  nivel.
 *
 * ¿Cómo manejar un pulsador?
 * Se puede mirar:
 *    1. Si se encuentra pulsado o no. Ese es Push_button.
 *    2. Si se ha pulsado desde la última vez que se miró. Este es este
 *       Push_button_level_change.
 *
 */
// template <uint8_t n>
template <typename Micro, uint8_t n>
class Push_button_level_change{
public:
// Types
// -----
    using Pin = typename Micro::Pin<n>;

// Cfg
// ---
    static constexpr uint8_t pin_number = n;

// Constructors
// ------------
    constexpr Push_button_level_change() {init();}

    /// Inicializamos el pulsador.
    static constexpr void init() 
    { Pin::as_input_with_pullup(); }


// Press/not press
// ---------------
    /// ¿está pulsado?
    static bool is_pressed();

    /// ¿no está pulsado?
    static constexpr bool is_not_pressed()
    {return !is_pressed();}

// Interrupts
// ----------
// Recordar definir la ISR correspondiente!!!
    static void enable_interrupt()
    { Pin::enable_change_level_interrupt(); }

    static void disable_interrupt()
    { Pin::disable_change_level_interrupt(); }

private:
    inline static bool look_ = true;

};


template <typename M, uint8_t n>
bool Push_button_level_change<M, n>::is_pressed()
{
    if (Pin::is_zero()){
	if (look_){
	    look_ = false;
	    return true;
	}
    }
    else
	look_ = true;

    return false;
}


}// namespace

#endif
