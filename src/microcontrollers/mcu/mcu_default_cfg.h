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

#ifndef __MCU_DEFAULT_CFG_H__
#define __MCU_DEFAULT_CFG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Guardemos la default configuración de diferentes dispositivos de los
 *	avrs.	
 *
 * HISTORIA
 *    Manuel Perez
 *    03/11/2024 default_cfg::UART_8bis
 *
 ****************************************************************************/

namespace mcu {
namespace default_cfg{

// UART_8bits
// ----------
template <uint32_t baud_rate0>
struct UART_8bits{
    static constexpr uint32_t baud_rate      = baud_rate0;
    static constexpr bool parity_mode_enable = false;
    static constexpr bool one_stop_bit       = true; 
    static constexpr uint8_t character_size  = 8;

// De momento el max_error solo lo usa atmega328p
    static constexpr uint8_t max_error       = 2;
};

using UART_8bits_9600_bauds = UART_8bits<9'600>;

} // default_

}// mcu

#endif


