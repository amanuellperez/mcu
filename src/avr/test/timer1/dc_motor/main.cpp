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

// Controlamos la velocidad de un motor dc usando un mosfet 2N7000.
// Circuito: libro de make, pag. 299.
#include "../../../avr_UART.h"
#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"


using namespace avr;

using Timer = Timer1_fast_PWM;

constexpr uint16_t period_in_us = 1;


int main()
{
//    UART_ostream uart;

    Timer::top_ICR(9000UL);
    Timer::pin_A_non_inverting_mode();
    Timer::on<period_in_us>();


    while(1){
	uint16_t t = 1000;
	for (int i = 1; i < 10; ++i){
	    Timer::output_compare_register_A(t);
	    t += 1000UL;
	    wait_ms(2000);
	}
    }
}



