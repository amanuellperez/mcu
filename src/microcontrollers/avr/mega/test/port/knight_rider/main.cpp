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
#include "../../../mega_port_basic.h"
#include "../../../mega_private.h" // wait_ms


int main()
{
    mega_::Output_portB portB;

    while(1){
	uint8_t v = 0b00000001;
	for (uint8_t i = 0; i < 8; ++i){
	    portB = v;
	    mega_::wait_ms(200);
	    v <<= 1;
	}
    }
}



