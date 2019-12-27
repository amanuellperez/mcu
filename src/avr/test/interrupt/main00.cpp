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

#include "../../avr_interrupt.h"
//#include "../../avr_USART.h"
#include "../../mcu_led.h"
#include "../../mcu_pulsador.h"
#include "../../avr_time.h"


// Esta es la versión más básica para probar las interrupciones PCINT
using namespace avr;

constexpr uint8_t num_pin_pulsador = 4;

Pulsador pulsador{num_pin_pulsador};
LED led{13_pin};
//auto usart = USART::init();

ISR_PCINT_PIN4
{
 //   usart << ".";

    if (pulsador.esta_pulsado())
	led.on();
    else 
	led.off();
}

 
int main()
{

    Interrupt::enable_pin<num_pin_pulsador>();

    while(1){
	wait_ms(1000);
    }
}



