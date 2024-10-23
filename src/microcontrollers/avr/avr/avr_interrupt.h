// Copyright (C) 2024 Manuel Perez 
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

#ifndef __AVR_INTERRUPT_H__
#define __AVR_INTERRUPT_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar interrupciones
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    27/07/2017 Pruebas iniciales.
 *    22/01/2019 La migro a templates y defino macros.
 *    06/11/2022 Quito la llamada de enable_all_interrupts(). Esto no
 *	         pertenece al traductor. Si usas el AVR tienes que saber que
 *	         tienes que llamar a enable_all_interrupts().
 *
 *	         disable_pin
 *
 *    03/12/2022 Rename: enable_all_interrupts -> enable_interrupts (es más
 *		         corto y se entiende bien)
 *
 *		 Enable_interrupt
 *    12/12/2022 Enable_interrupts/Disable_interrupts a dev_interrupt.h
 *    07/09/2023 Vuelvo a meter Enable_interrupts/Disable_interrupts en avr_.
 *
 ****************************************************************************/
#include <avr/interrupt.h>

namespace avr_{

/***************************************************************************
 *			    INTERRUPTS
 ***************************************************************************/
/// Habilita el uso de interrupciones. Además, recordar habilitar
/// cada interrupción por separado.
inline void enable_interrupts() {sei();}

/// Deshabilita el uso de interrupciones
inline void disable_interrupts() {cli();}


// Disable_interrupts
// ------------------
class Disable_interrupts{
public:
    Disable_interrupts()
    {
	sreg_ = SREG;
	disable_interrupts();
    }

    ~Disable_interrupts()
    { SREG = sreg_; }


private:
    unsigned char sreg_;
};


// Enable_interrupts
// -----------------
class Enable_interrupts{
public:
    Enable_interrupts()
    {
	sreg_ = SREG;
	enable_interrupts();
    }

    ~Enable_interrupts()
    { SREG = sreg_; }


private:
    unsigned char sreg_;
};

} // namespace

#endif
