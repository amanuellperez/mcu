// Copyright (C) 2019-2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#include "../../../avr_signal_generator.h"
#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"



using SG = avr::PWM_generator;

constexpr uint16_t period_in_us = 1;


int main()
{
    avr::Hertz freq{100};
    uint16_t duty_cycle = 50;

// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

// init_signal_generator()
    SG::frequency(freq);
    SG::on<period_in_us>();


    uart << "\n\nDC motor\n"
	        "--------\n";
		
    while(1){
	uart << "State\n"
	        "-----\n"
		"freq       = " << freq << " Hz\n"
		"duty_cycle = " << duty_cycle << " %\n";


	uart << "\nMenu\n"
	          "----\n"
		  "[o]n\n"
		  "o[f]f\n"
		  "[s]elect freq\n"
		  "[d]uty cycle\n";
	
	uint32_t tmp;
	char c{};
	uart >> c;
	switch(c){
	    case 'o':
		SG::ch1_on(duty_cycle);
		break;

	    case 'f':
		SG::ch1_off();
		break;

	    case 's':
		uart << "freq (in Hz) = ";
		uart >> tmp;
		uart << tmp << '\n';
		freq = avr::Hertz{tmp};
		SG::frequency(freq);
		break;

	    case 'd':
		uart << "duty_cycle = ";
		uart >> duty_cycle;
		uart << duty_cycle << " %\n";
		SG::ch1_on(duty_cycle);
		break;
	}

//	uint16_t t = 1000;
//	for (int i = 1; i < 10; ++i){
//	    t += 1000UL;
//	    SG::ch1_on(t);
//	    wait_ms(2000);
//	}
    }
}



