// Copyright (C) 2025 Manuel Perez 
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

#include "../../mega0.h"


// Microcontroller
// ---------------
namespace myu = atmega4809_40;

using Micro = myu::Micro;

// Conectar un LED al pin 'led_pin'. Tiene que parpadear.
constexpr uint8_t led_pin = 4;

using Led = Micro::Pin<led_pin>;

constexpr uint8_t blink_time = 200;

int main()
{
    myu::init();

    Led::as_output();

    while (1)
    {
	Led::write_one();
	myu::wait_ms(blink_time);

	Led::write_zero();
	myu::wait_ms(blink_time);

    }

}


