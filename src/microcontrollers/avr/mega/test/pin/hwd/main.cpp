// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../../mega_registers.h"
#include "../../../mega_pin_hwd.h"
#include "../../../mega_micro.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>

namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

constexpr uint8_t npin = 13;
using Pin = hwd::Pin<npin>;

void blink()
{
    Pin::as_output();

    UART_iostream uart;
    for (uint8_t i = 0; i < 10; ++i){
	uart << '.';

	Pin::write_one();
	myu::wait_ms(500);

	Pin::write_zero();
	myu::wait_ms(500);
    }
    uart << " DONE\n";
}



int main()
{
// uart_init();
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// menu
    uart << "\n--------\n"
	      "Pin test\n"
	      "--------\n";

    while (1){
	uart << "\nMenu\n"
		"1. Blink LED pin number " << (int) npin << '\n' <<
		"2. As output\n"
		"3. As input with pullup\n"
		"4. As input without pullup\n"
		"5. Read value pin\n"
		"6. Write zero\n"
		"7. Write one\n\n";

	char ans{};
	uart >> ans;
	switch(ans){
	    break; case '2'	: Pin::as_output();
	    break; case '3'	: Pin::as_input_with_pullup();
	    break; case '4'	: Pin::as_input_without_pullup();
	    break; case '5': 
		    uart << "\nRead:"
			    "\n\tpin.is_zero() = " << (int) Pin::is_zero()
			 << "\n\tpin.is_one() = " << (int) Pin::is_one()
			 << "\n\tpin.read() = " << (int) Pin::read() 
			 << '\n';

	    break; case '6': Pin::write_zero();
	    break; case '7': Pin::write_one();
	    break; default	: blink();

	}
    }
}


