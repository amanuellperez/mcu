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

#ifndef __AVR_NOT_GENERIC_H__
#define __AVR_NOT_GENERIC_H__
// Defino este fichero para tener localizado aquellos archivos que hay 
// que modificar para generalizar el código. 
// Ejemplo: avr_SPI_basic.cpp usa los pines definidos en mega_cfg.h
// NO ES GENÉRICO. Al compilarlo lo estamos compilando solo para el atmega.
// Podemos:
//  (1) Renombrarlo llamandolo atmega_SPI_basic.cpp ya que es lo que realmente
//      es.
//  (2) Si la implementación es la misma para attiny convendría parametrizarlo
//      con los pines de tal manera que se pueda reutilizar.
//  (3) ???
//
//  Lo mismo le ocurre a avr_TWI_basic.h que usa TWI_MASK_STATUS_BITS, valor
//  que depende del micro usado.
//
//  NOTA: si se ve que resulta más liado intentar reutilizar código que
//  duplicar código, duplicarlo!!! Que cada micro tenga su propio:
//  atmega_pin_basic.h, attiny_pin_basic.h ... 
#include "mega_cfg.h"


#endif


