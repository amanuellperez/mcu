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

#ifndef __MCU_TIME_H__
#define __MCU_TIME_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar tiempo	
 *
 * HISTORIA
 *    Manuel Perez
 *    10/07/2024 wait_till_at_most_ms
 *
 ****************************************************************************/
#include <cstdint>

namespace mcu{

// wait_till_at_most_ms
// --------------------
// Hay dos formas básicas (mínimo) de abortar la ejecución de una función:
//  1) Transcurrido un determinado tiempo
//  2) Que el usuario pulse una tecla para cancelar
//
// La siguiente función es un primer intento de sistematizar 1). 
// Observar que bloquea el micro y mide cada milisegundo.
// Otra posibilidad sería que midiese cada 500 us, o 250 us, o 100 us. ¿Qué
// será mejor? Probemos y que el uso lo dicte.
template <typename Wait_1_ms, typename Predicate>
bool wait_till_at_most_ms(uint16_t time_ms, Predicate f, Wait_1_ms wait_1_ms)
{
    if (time_ms == 0)
	return true;

    while (f()){
	wait_1_ms();
	--time_ms;
	if (time_ms == 0)
	    return true;
    }

    return false;
}

// Esta función esta pensada para ser usada:
//  struct Wait_1_ms{
//	void operator()() {Micro::wait_ms(1);}
//  };
//
//  template <typename Pred>
//  using wait_till = mcu::wait_till<Wait_1_ms, Pred>;
//
//  if (wait_till([]{ return condicion(); })
//		._at_most_ms(abort_time)
//			    )
//	{ abort ; }
//
template <typename Wait_1_ms, typename Predicate>
struct wait_till{

    wait_till(Predicate p): pred{p} { }

    bool _at_most_ms(uint16_t wait_time_in_ms)
    { return wait_till_at_most_ms(wait_time_in_ms, pred, Wait_1_ms{});}

    Predicate pred;
};

}// namespace mcu
 
#endif


