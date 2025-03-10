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

// conectar al pin 14 un led. Tiene que parpadear.

#include "../../avr_time.h"

#include <avr/io.h>

// Microcontroller
// ---------------
namespace myu = avr_;

int main()
{
    DDRB = 0b00000001;

    while(1){
	PORTB = 0b00000001;
	myu::wait_ms(1000);

	PORTB = 0b00000000;
	myu::wait_ms(1000);
    }

}


