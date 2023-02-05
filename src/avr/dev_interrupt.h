// Copyright (C) 2022 Manuel Perez 
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

#ifndef __DEV_INTERRUPT_H__
#define __DEV_INTERRUPT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Gestión genérica de interrupciones de un microcontrolador.
 *
 *	(RRR) ¿por qué meterlo en avr y no en dev1?
 *	      Para programar esto no necesito conocer la datasheet, realmente
 *	      pertenece a dev1.
 *
 *	      Pero:
 *	      (1) Los dispositivos de `avr` necesitan bloquear interrupciones.
 *	          Necesito esta clase en avr.
 *
 *	      (2) A fin de cuentas esto es ampliar el sistema de
 *	          interrupciones de un microcontrolador: tiene sentido que
 *	          esté dentro de los micros, y no en dev.
 *
 *	      Si implemento más micros el orden debería de ser:
 *		    /mcu/.		# aqui lo meto
 *		        /avr/.		# cosas comunes a este dir
 *		            /atmega
 *			    /attiny
 *		        /pic/.
 *		        ...
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    12/12/2022 Enable_interrupts/Disable_interrupts
 *
 ****************************************************************************/

namespace dev{

/// Enable_interrupts/Disable_interrupts
//--------------------------------------
// Idioma: RRI. Habilitamos/deshabilitamos las interrupciones y el destructor
// las deja en el estado en el que estuvieran inicialmente.
// Ejemplo de bloqueo en la datasheet: ver pag. 153 por ejemplo.
template <typename Micro>
// requires Micro == microcontrolador, pero qué es un microcontrolador???
class Disable_interrupts{
public:
    Disable_interrupts()
    {
	flag_ = Micro::global_flag_interrupt();	
	Micro::disable_interrupts();
    }

    ~Disable_interrupts()
    {
	Micro::global_flag_interrupt(flag_); // Restore global interrupt flag
    }


private:
    Micro::Global_flag_interrupt flag_;
};


template <typename Micro>
// requires Micro == microcontrolador, pero qué es un microcontrolador???
class Enable_interrupts{
public:
    Enable_interrupts()
    {
	flag_ = Micro::global_flag_interrupt();	
	Micro::enable_interrupts();
    }

    ~Enable_interrupts()
    {
	Micro::global_flag_interrupt(flag_); // Restore global interrupt flag
    }


private:
    Micro::Global_flag_interrupt flag_;
};

}// namespace

#endif


