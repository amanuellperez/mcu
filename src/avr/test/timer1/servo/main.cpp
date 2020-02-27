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

// Conectar los pines +- del servo a la fuente de alimentación (independiente
// de la del avr). Conectar las tierras de la fuente de alimentación y del
// servo juntas. Y conectar el pin OC1A (15) al de control del servo.
// A través de UART darle los valores de los pulsos que se quiera. 
// (Con los servos que tengo van de 7 ms hasta 2.7 ms)
#include "../../../avr_UART.h"
#include "../../../avr_timer1.h"
#include "../../../avr_time.h"

#include <stdlib.h>
#include <std_type_traits.h>

using namespace avr;

using Timer = Timer1_fast_PWM;

constexpr uint16_t period_in_us = 1;


int main()
{
    UART_ostream uart;

    Timer::top_ICR1(20000UL);
    Timer::registro_ICR1(20000UL);
    Timer::pin_A_non_inverting_mode();
    Timer::on<period_in_us>();


    while(1){
	uint16_t t;
	uart << "Duración del pulso (de 1000 a 2000 us): ";
	UART::receive(t);
	
	uart << t << "\n\r"; // echo
	Timer::comparadorA(t);

	wait_ms(1000);
    }
}



