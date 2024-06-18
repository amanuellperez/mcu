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

#include "../../dev_TMP36.h"
#include <avr_UART.h>
#include <avr_time.h>

using namespace avr;

constexpr uint8_t num_pin_sensor = 25;
constexpr uint16_t Vcc_en_mV = 5000;

// Conectar al pin 25 el sensor de temperatura TMP36
void test_1()
{
    UART_ostream usart;

    ADC::single_mode(ADC::AREF::selection_external);

    usart << "Probando TMP36\n\r"
	     "--------------\n\r";

    while (1) {
        usart << "10T = "
              << Termometro_TMP36::temperatura_en_decigrados<num_pin_sensor,
                                                             Vcc_en_mV>()
              << "\n\r";
        delay_ms(1000);
    }                                      
}

int main() 
{
    test_1();
//    test_2();

}


