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

#include "mega_UART_hal.h"
#include "mega_import_avr.h"

namespace mega_{
namespace hal{

int UART_8bits::flush(uint16_t time_out_ms)
{
    while (!USART::is_transmit_complete())
    {
	wait_ms(1);
	if (time_out_ms == 0)
	    return -1;

	--time_out_ms;
    }

    // Datasheet: The TXC0 Flag bit is automatically cleared
    // when a transmit complete interrupt is executed, 
    // or it can be cleared by writing a one to its bit location.
    
    // USART::clear_transmit_complete_flag();
    atd::write_bit<TXC0>::to<1>::in(UCSR0A);

    return 0;
}

}// namespace
}// namespace
