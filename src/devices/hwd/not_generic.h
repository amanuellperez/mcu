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

#ifndef __NOT_GENERIC_H__
#define __NOT_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Fichero temporal para ir desvinculando del namespace
 *	inicial avr. 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    08/12/2022 Escrito
 *
 ****************************************************************************/
#include <mega_private.h> // wait_ms
#include <mega_pin.h>
#include <mega_memory.h>    // lo usa logic (dev_LCD_font_xxx.h)
			    // y LCD_HD44780_generic.h

// TODO: Como esto lo he ido desarrollando poco a poco, a medida que he ido
// aprendiendo la mayoria de los dispositivos acceden directamente a las
// funciones del atmega (que al principio defini como avr). Para identificar
// bien aquellas partes que no son genéricas voy a marcarlas como
// `not_generic` para saber que las tengo que ir reescribiendo.
// Cuando en los devices haya eliminado por completo este namespace,
// eliminarlo.
namespace not_generic{
using namespace mega_;
}

#endif


