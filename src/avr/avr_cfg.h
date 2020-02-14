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

#ifndef __AVR_CFG_H__
#define __AVR_CFG_H__
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
 *    A.Manuel L.Perez
 *    27/01/2020 v0.0
 *
 ****************************************************************************/

#include <avr/io.h> // registros: DDRB... PORT...




#if MCU == atmega328p
#include "avr_atmega328p.h"

#else
#error "No se han definido los pines de este microcontrolador"
#error "Escribirlos en el fichero avr_pins.h"
#endif


// CONFIGURACIÓN DEL RELOJ
    constexpr uint32_t MCU_CLOCK_FREQUENCY_IN_HZ = F_CPU;



#endif

