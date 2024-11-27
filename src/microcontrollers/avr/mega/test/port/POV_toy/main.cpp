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

// Conectar el puerto B a 8 leds. Hacemos el coche fantástico.
#include "../../../mega_port_hwd.h"
#include "../../../mega_import_avr.h" // wait_ms



void POVDisplay(uint8_t fila_glyph)
{
    PORTB = fila_glyph;
    mega_::wait_ms(2);
}

int main()
{
    mega_::hwd::Output_portB portB;

    while(1){
	POVDisplay(0b00001110);
	POVDisplay(0b00011000);
	POVDisplay(0b10111101);
	POVDisplay(0b01110110);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b01110110);
	POVDisplay(0b10111101);
	POVDisplay(0b00011000);
	POVDisplay(0b00001110);

	portB = 0;
	mega_::wait_ms(10);
    }
}



