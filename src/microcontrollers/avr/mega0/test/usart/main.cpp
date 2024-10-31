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

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "../../mega0_usart_basic.h"
#include "../../mega0_cfg.h"

namespace myu = mega0_;
using USART = myu::USART_basic<myu::cfg::USART1>;

// TODO: calcular baud rate
// Calcula el valor del baud_register para ese baud_rate
// TODO: ¿2 versiones?
//  1) usa F_CPU: se define en tiempo de compilación
//  2) no usa F_CPU: en este micro se puede configurar dinamicamente el
//  divisor del reloj!!!
//	
//  Distinguir entre:
//	    CLK_MAIN = frecuencia fijada por los fuses, conocida en tiempo de
//		       compilacion
//	    CLK_CPU   
//	    CLK_PER  = esta es la que usa UART? Se configura dinámicamente.
//
//  ¿dos tipos de aplicaciones? ¿aquellas con F_CPU fija, y aquellas con
//  CLK_MAIN que será dinámica? (el problema es que sin F_CPU fija no se puede
//  usar _delay_ms)
constexpr uint16_t baud_rate_to_baud_register(uint32_t baud_rate)
{ return ((float)(F_CPU * 64 / (16 * (float)baud_rate)) + 0.5); }


void init(void)
{
    USART::baud(baud_rate_to_baud_register(9600));
    USART::enable_transmitter();
}

void print(char c)
{
    while (!USART::is_transmit_data_empty()) { ; }

    USART::data_register_low_byte(c);
}

void print(const char str[])
{
    for(size_t i = 0; i < strlen(str); i++)
        print(str[i]);
}

void hello_world()
{
    print("Hello world!\r\n");
    _delay_ms(500);
}


int main()
{
    init();
 
    while (1){
	hello_world();
    }
}

