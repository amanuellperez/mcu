// Copyright (C) 2023 Manuel Perez 
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

#include "avr_UART.h"

namespace avr_{

void UART_flush()
{
    while (!UART_basic::is_transmit_complete())
    {;}

    // Datasheet: The TXC0 Flag bit is automatically cleared
    // when a transmit complete interrupt is executed, 
    // or it can be cleared by writing a one to its bit location.
    
    atd::write_bit<TXC0>::to<1>::in(UCSR0A);
}

}// namespace
