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

#include "../../mega0_usart_basic.h"
#include "../../mega0_uart.h"
#include "../../mega0_registers.h"
#include <mcu_default_cfg.h>

namespace myu = mega0_;

// Descomentar para elegir qué USART probar
//using USART_cfg = myu::cfg_40_pins::USART0;
using USART_cfg = myu::cfg_40_pins::USART1; // pines 0,1
//using USART_cfg = myu::cfg_40_pins::USART2;

using USART = myu::USART_basic<USART_cfg>;
using UART_8bits = myu::UART_8bits<USART_cfg>;


void init()
{
    myu::cfg_40_pins::pins::init();

    // probamos que se configure correctamente
    UART_8bits::init<mcu::default_cfg::UART_8bits_9600_bauds>(); 

    USART::enable_transmitter();
    USART::enable_receiver();
}

void print(char c)
{
    while (!USART::is_transmit_data_register_empty()) { ; }

    USART::transmit_data_register_low_byte(c);
}

void print(const char str[])
{
    for(size_t i = 0; i < strlen(str); i++)
        print(str[i]);
}

char read()
{
    while (!USART::are_there_unread_data()) { ; }
    
    return USART::receive_data_register_low_byte();
}

void hello()
{
    print ("\r\n\nUSART basic test\r\n");
    print ("----------------\r\n\n");
}


int main()
{
    init();
 
    hello();

    while (1){
	print("Write a letter: ");
	char c = read();
	print ("\r\nYou have written: [");
	print (c);
	print ("]\r\n");
    }
}

