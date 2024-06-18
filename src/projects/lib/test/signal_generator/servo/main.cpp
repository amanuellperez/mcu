// Copyright (C) 2019-2021 Manuel Perez 
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

// Conectar los pines +- del servo a la fuente de alimentación (independiente
// de la del avr). Conectar las tierras de la fuente de alimentación y del
// servo juntas. Y conectar el pin OC1A (15) al de control del servo.
// A través de UART darle los valores de los pulsos que se quiera. 
// (Con los servos que tengo van de 7 ms hasta 2.7 ms)
#include "../../../dev_signal_generator.h"
#include <avr_UART.h>
#include <avr_timer1_generic.h>
#include <avr_time.h>


using PWM = dev::PWM_generator<avr::Timer1>;
using namespace avr::literals;

constexpr uint16_t period_in_us = 1;


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

// init_pwm()
    PWM::on<period_in_us>();
    PWM::period(20_ms);
    PWM::ch1_duty_cycle(1500_us);
    PWM::ch1_non_inverting_mode();

    uart << "\n\nServo\n"
	        "-----\n";
    while(1){
	uint16_t t;
	uart << "Duración del pulso (de 1000 a 2000 us): ";
	uart >> t;
	uart << t << "\n";

	PWM::ch1_duty_cycle(avr::Time{t, -6});

	wait_ms(1000);
    }
}



