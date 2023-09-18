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

#include "../../../dev_push_button.h"
#include "../../../dev_led.h"
#include <avr_atmega.h>

// Microcontroller
// ---------------
namespace mcu = atmega;
using Micro = mcu::Micro;

// Pin connections
// ---------------
static constexpr uint8_t led_pin    = 13;
static constexpr uint8_t button_pin = 14;


// Devices
// -------
using LED    = dev::LED<Micro, led_pin>;
using Button = dev::Push_button<Micro, button_pin>;


// Main
// ----
int main()
{
    LED led;
    Button button;

    while(1){
	if (button.is_pressed())
	    led.on();
	else 
	    led.off();

	Micro::wait_ms(100);
    }
}



