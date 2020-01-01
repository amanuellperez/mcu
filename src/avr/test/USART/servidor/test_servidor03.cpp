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

//  Este programa dialoga con el programa test_cliente02.cpp
//
//  Enviamos un fichero desde el AVR al pc (es el paso previo para hacer un
//  termómetro logger)
//
#include "../../mcu_led.h"
#include "../../avr_time.h"
#include "../../avr_USART.h"

using namespace avr;

int main() 
{
    auto usart = USART::init();

    // ¡¡el nombre_programa tiene que ir en flash memory!!!
    const char abc[] = "abcdefghijklmnñopqrstuvwxyz0123456789";

    while(1){
	for (uint8_t i = 0; i < sizeof(abc); ++i)
	    usart << abc[i];

	wait_ms(1000);
    }

    return 0;
}


