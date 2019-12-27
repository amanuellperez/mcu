// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "mcu_pulsador.h"
#include "mcu_led.h"

// Probamos un pulsador.
// Conectamos dos leds en los pines 15 y 16, y el pulsador
// al pin 14 directamente. El otro lado a tierra.

using namespace avr;

int main(void) 
{
    Pulsador<4> pulsador;

    LED<13> led1;
    LED<14> led2;
    
    while (1) {
	if(pulsador.no_esta_pulsado())
	    led1.on();
	else
	    led1.off();


	if(pulsador.esta_pulsado())
	    led2.on();
	else
	    led2.off();
    }


    return 0;
}


