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

// Conectar un led al pin 14. Lo hacemos parpadear.

#include "../../../dev_led.h"
#include <avr_atmega.h>

namespace mcu = atmega;


int main() 
{
    // ponemos un LED en el pin 14
    dev::LED<14> led;

    uint16_t time = 500; // probar con 10: ¡no parpadea!!!

    while(1){
	led.on();
	mcu::Micro::wait_ms(time);

	led.off();
	mcu::Micro::wait_ms(time);
    }
}


