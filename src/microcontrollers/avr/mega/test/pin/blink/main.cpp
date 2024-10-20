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
#include "../../../mega_pin.h"

#include <avr_time.h>

// Microcontroller
// ---------------
namespace myu = avr_;

// Conectar un LED al pin 'led_pin'. Tiene que parpadear.
constexpr uint8_t led_pin = 15;

using Pin = myu::Pin<led_pin>;

// ¿Compila is_a_valid_pin()?
static_assert(myu::Pin<led_pin>::is_a_valid_pin());

static_assert(!myu::Pin<mcu::Pin_connection_type::floating>::is_a_valid_pin());
static_assert(myu::Pin<mcu::Pin_connection_type::floating>::is_a_valid_connection());

//DONT_COMPILE(static_assert(!myu::Pin<100>::is_a_valid_pin());)

constexpr uint16_t time_ms = 500;

int main()
{

    Pin::as_output();

    while (1)
    {
	Pin::write_one();
	myu::wait_ms(time_ms);

	Pin::write_zero();
	myu::wait_ms(time_ms);

	Pin::toggle();
	myu::wait_ms(time_ms);

	Pin::toggle();
	myu::wait_ms(time_ms);
    }

}


