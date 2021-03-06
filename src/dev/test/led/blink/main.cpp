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

// Conectar un led al pin 14. Lo hacemos parpadear.

#include "../../../dev_led.h"
#include <avr_time.h>

using namespace dev;


int main() 
{
    // ponemos un LED en el pin 14
    LED<14> led;

    uint16_t time = 500; // probar con 10: ¡no parpadea!!!

    while(1){
	led.on();
	wait_ms(time);

	led.off();
	wait_ms(time);
    }
}


