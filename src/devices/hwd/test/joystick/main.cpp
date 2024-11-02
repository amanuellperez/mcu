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

#include "../../dev_joystick.h"

#include <avr_atmega.h>

// Micro
// -----
namespace myu = atmega; 
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// pins
constexpr uint8_t push_button_pin = 26; // es ADC, veamos que todo funciona
constexpr uint8_t x_pin = 27;
constexpr uint8_t y_pin = 28;


// Hwd Devices
// -----------
using ADC = myu::ADC;
constexpr uint16_t AREF_in_mV = 5'000;
constexpr uint8_t AREF_connection = ADC::AREF_connect_to_internal_AVCC;
using kk = myu::ADC_pin_single_mode<28, 5'000>;
using ADC_x_pin = myu::ADC_pin_single_mode<x_pin, AREF_in_mV>;
using ADC_y_pin = myu::ADC_pin_single_mode<y_pin, AREF_in_mV>;

using Cfg = dev::Joystick_cfg_type1<Micro, 
				    ADC_x_pin, ADC_y_pin,
				    push_button_pin>;

using Joystick = dev::Joystick_type1<Cfg>;

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}

void init_adc()
{
    ADC::single_mode<AREF_connection>();
}


void press_key_to_continue()
{
    UART_iostream uart;
    uart << "Press a key to continue\n";

    char c{};
    uart >> c;
}


void hello()
{
    UART_iostream uart;
    uart << "\n\nJoystick test\n"
	        "-------------\n"
		"IMPORTANT: Don't forget to connect AVCC to power\n"
		"Connections:\n"
		"\tx pin: " << (int) x_pin <<
		"\n\ty pin: " << (int) y_pin << 
		"\n\tpush button: " << (int) push_button_pin 
		<< '\n';

    press_key_to_continue();
}



int main()
{
    init_uart();
    hello();
    init_adc();

    Joystick joystick;

    UART_iostream uart;
    
    while(1){
	
	uart << '(' << joystick.x_as_volts() <<
	        ", " << joystick.y_as_volts() << 
		"); Button = " << joystick.is_pressed() <<
		'\n';


	uart << joystick.point() << '\n';

	Micro::wait_ms(1'000);
    }

}



