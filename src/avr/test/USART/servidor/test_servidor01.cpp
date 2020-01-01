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

// Problema:
//  ¿cómo comunicarse con el microcontrolador desde el ordenador?
//  Una posible solución es usar USART.
//
//  Este programa responde a instrucciones dadas desde el ordenador. Al pulsar
//  los números del 1 al 8 se encienden uno de los 8 leds.
//
//  Lo pruebo con el programa 'screen' de linux y funciona perfectamente.
//  Ahora el segundo paso es hacer yo el programa desde linux que envíe por
//  puerto serie las instrucciones al avr. (ahora uso el cable de FTDI pero en
//  un futuro compraré un chip MAX232)
//
//  Este programa dialoga con el programa test_cliente01.cpp
#include "../../mcu_led.h"
#include "../../avr_time.h"
#include "../../avr_USART.h"

using namespace avr;

int main(void) 
{

    LED led[8] = {9_pin, 10_pin, 11_pin, 12_pin, 13_pin, 14_pin, 15_pin, 16_pin};

    auto usart = USART::init();

    while(1){
	char res;
    
	if (USART::receive(res)){
	    usart << res;
	    switch(res){
		case '1': led[0].on(); break;
		case '2': led[1].on(); break;
		case '3': led[2].on(); break;
		case '4': led[3].on(); break;
		case '5': led[4].on(); break;
		case '6': led[5].on(); break;
		case '7': led[6].on(); break;
		case '8': led[7].on(); break;

		default:
			for (uint8_t i = 0; i < 8; ++i)
			    led[i].off();

	    }
	}


    }

    return 0;
}


