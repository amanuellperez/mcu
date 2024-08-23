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

#ifndef __MCU_SPI_H__
#define __MCU_SPI_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuración genérica de SPI	
 *
 * HISTORIA
 *    Manuel Perez
 *    22/08/2024 SPI concept
 *
 ****************************************************************************/
#include <cstdint>

namespace mcu {

// Este es un selector básico que no hay por qué usar. Cada aplicación podrá
// (y deberá) de escribir sus propios selectores
//
// Por defecto el pin es no_CS, esto es, para seleccionarlo escribimos 0, y
// para deseleccionarla un 1.
template <typename Micro, uint8_t npin>
class SPI_pin_selector{
public:
    using Pin = typename Micro::Pin<npin>;

    // Fundamental dejar deseleccionado el spi device.
    static void init() 
    {
	Pin::as_output();
	deselect();
    }

    static void select()
    { 
	Pin::write_zero(); }

    static void deselect()
    { 
	Pin::write_one(); 
    }

    SPI_pin_selector() 
    { select(); }

    ~SPI_pin_selector() 
    { deselect(); }

private:
};


} // namespace mcu

#endif


