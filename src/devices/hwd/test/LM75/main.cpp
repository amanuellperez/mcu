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

// OJO: para que el TWI funcione a 100kHz es necesario operar
// a 8 MHz!!
// Conectar 1 termómetros LM75 con dirección 1.
#include <avr_TWI.h>
#include "../../dev_LM75.h"

#include <avr_UART.h>
#include <avr_time.h>


// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART;
using UART_iostream = mcu::UART_iostream<UART>;

using namespace myu; // TODO




using LM75_1 = dev::Thermometer_LM75<1>;

int main() 
{
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

    uart << "\nPrueba del LM75\n";
    uart << "---------------\n";

    // OJO: para que el TWI funcione a 100kHz es necesario operar
    // a 8 MHz!!
    TWI::enable_and_set_SCL_frequency_in_kHz<100>();

    LM75_1 lm75_1;

    while (1) {
	if (lm75_1.refresh())
	    uart << "Error al intentar actualizar la temperatura\n";

        uart << static_cast<uint16_t>(lm75_1.T()) << '.'
	     << static_cast<uint16_t>(lm75_1.T_d()) << '\n';

        wait_ms(1000);
    } // while(1)
}



