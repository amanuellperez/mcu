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

// Conectar 3 pulsadores a los pines indicados. La salida la mostramos por
// UART.
#include "../../dev_keyboard.h"

#include <avr_time.h>
#include <avr_USART.h>


constexpr uint8_t keyboard_enter_pin = 23;
constexpr uint8_t keyboard_up_pin = 24;
constexpr uint8_t keyboard_down_pin  = 25;

constexpr uint8_t ENTER_KEY = 0;
constexpr uint8_t UP_KEY    = 1;
constexpr uint8_t DOWN_KEY  = 2;

using Keyboard = dev::Keyboard3<keyboard_enter_pin, ENTER_KEY,
			        keyboard_up_pin, UP_KEY,
				keyboard_down_pin, DOWN_KEY>;



void test_keyboard()
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    Keyboard keyboard;

    while (1){
	if (keyboard.key<ENTER_KEY>().is_pressed())
	    uart << "enter\n" << std::flush;

	if (keyboard.key<UP_KEY>().is_pressed())
	    uart << "up\n" << std::flush;

	if (keyboard.key<DOWN_KEY>().is_pressed())
	    uart << "down\n" << std::flush;

	wait_ms(100);
    }
}

int main()
{
    test_keyboard();
}



