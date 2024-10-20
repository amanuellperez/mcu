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

// (???) Este programa es genérico salvo los archivos de cabecera y el
// namespace myu. Se podría usar para probar cualquier micro???
#include "../../../mega0_pin.h"
#include "../../../mega0.h"

#include <avr_time.h>

// Microcontroller
// ---------------
namespace myu = atmega4809_40;

// pines
constexpr uint8_t push_button_pin = 3;
constexpr uint8_t led_pin         = 2;

using Led  = myu::Pin<led_pin>;
using Push = myu::Pin<push_button_pin>;

int main()
{
    Led::as_output();
    Push::as_input_with_pullup();

    Led::write_one();

    while (1)
    {
	if (Push::is_one() and Led::is_zero())
	    Led::write_one();

	else if (Push::is_zero() and Led::is_one())
	    Led::write_zero();

    }

}

