// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __AVR_CONSTANTS_H__
#define __AVR_CONSTANTS_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Configuración del avr que vamos a usar
 *
 *   - COMENTARIOS: Este fichero desvincula nuestro programa del avr
 *
 *   CUIDADO: Estas constantes aparecen en .cpp, luego hay que compilar una
 *   biblioteca diferente para cada microcontrolador!!!
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    27/01/2020 v0.0
 *
 ****************************************************************************/
#include "avr_types.h"


namespace avr_{

// DUDA: ¿Cuál de las siguientes dos versiones usar?
// (1) Para parámetro de template queda mejor el uint32_t.
// (2) Para el resto mejor `Frequency`.
// De momento suministro las dos y que el uso elija.
//
constexpr uint32_t clock_frequency_in_hz = F_CPU;
constexpr Frequency clock_frequency{F_CPU};

}// namespace

// F_CPU lo usa <util/delay.h>. No se puede hacer el undef
// sin incluir antes delay.h
//#undef F_CPU




#endif

