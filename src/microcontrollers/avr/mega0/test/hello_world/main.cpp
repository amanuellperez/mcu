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

// conectar al pin 1 un led. Tiene que parpadear.

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    PORTC.DIR |= PIN0_bm;

    while (1)
    {
        PORTC.OUT |= PIN0_bm;
        _delay_ms(500);
        PORTC.OUT &= ~PIN0_bm;
        _delay_ms(500);
    }

}


