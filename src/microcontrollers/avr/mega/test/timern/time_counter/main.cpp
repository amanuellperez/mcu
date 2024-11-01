// Copyright (C) 2022 Manuel Perez 
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


#include "../../../mega_timer0.h"
#include "../../../mega_timer1.h"
#include "../../../mega_timer2.h" 
#include "../../../mega_debug.h"

#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_cfg.h"
#include "../../../mega_pin.h"

// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART>;

// Pin conections
// --------------
// Eligo pins que no tienen nada que ver con los Timers
constexpr uint8_t npin0 = 26;
constexpr uint8_t npin1 = 27;
constexpr uint8_t npin2 = 28;

// Devices
// -------
using Counter0  = myu::Time_counter0;
using Counter1  = myu::Time_counter1;
using Counter2  = myu::Time_counter2;

using Pin0 = myu::Pin<npin0>;
using Pin1 = myu::Pin<npin1>;
using Pin2 = myu::Pin<npin2>;


// CUIDADO: hay mucha diferencia entre tener el reloj del micro a 1MHz o a 8
// MHz. Para tiempos precisos (del orden de 50 us (???)) usar 8MHz. Para el
// resto seguramente que valga 1 MHz. Con todo hacer pruebas.
//
// Resultados: Timer0
//
// Como se ve con T_clock = 1 us se genera mucho error. Además al mirar la
// onda cuadrada generada en el osciloscopio se ve que no es simétrica, que
// tarda un poco más al hacer el while(1). (es un jump; para verlo sustituir
// el while(1) por un goto y se ve que la señal generada no cambia)
//
// Oscilan un poco las frecuencias generadas. No se por qué.
//
// period_in_us = 1
//			     1 MHz	    |        8 MHz
//	value() < 1  ==> T = 14.14 us :(    |	    en torno a 2 us (oscila)
//		< 10 ==>   = 30.32 us :(    |	    ¿en torno a 2 us????
//		< 100 =>   =                |       201 us :)
//
// period_in_us = 64
//	value() < 1 ==> T =  127-131 us :)
//		< 2 ==> T =  255 us     :)
//	
// period_in_us = 1024
//	value() < 1 ==> T =  2.07 ms :)
//	        < 2 ==>   =  4.14 ms :)
//	        < 10 =>   = 20.70 ms :)
//

// Types
// -----
using Max_type = Counter1::counter_type;


void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}

void main_hello()
{
    UART_iostream uart;

    uart << "\n\nTime counter test\n"
	        "-----------------\n"
		"Connect oscilloscope to pins:\n" 
		"\t\t" << (int) Pin0::number << " (for Time_counter0)\n"
		"\t\t" << (int) Pin1::number << " (for Time_counter1)\n"
		"\t\t" << (int) Pin2::number << " (for Time_counter2)\n"
		"\n";

}

void init_pins()
{
    Pin0::as_output();
    Pin1::as_output();
    Pin2::as_output();
}

template <typename Counter, uint16_t period>
void generate(uint32_t top)
{
    UART_iostream uart;
    Counter::init(static_cast<typename Counter::counter_type>(top));
    Counter::template turn_on_with_clock_period_of<period>::us();
    Counter::enable_top_interrupt();

    uint32_t T = 2 * top * period;
    uart << "Generating signal of period " << T << " us\n"
	    "\t(timer clock period = " << period << 
	    " us, max_value = " << top << ")\n";

}

char choose_timer()
{
    UART_iostream uart;
    uart << "\nMenu\n"
	    "----\n"       
	    "0. Timer 0\n"
	    "1. Timer 1\n"
	    "2. Timer 2\n";

    char timer{};
    uart >> timer;

    return timer;
}

char choose_period(char timer)
{
    UART_iostream uart;

    uart << "\nPeriod:\n"
	    "0. Turn off\n"
	    "1.    1 us\n"
	    "2.    8 us\n";

    if (timer == '2')
	uart << "3.   32 us\n";

    uart << "4.   64 us\n";

    if (timer == '2')
	uart << "5.  128 us\n";

    uart <<	"6.  256 us\n"
	    "7. 1024 us\n"
	    "p. print registers\n";


    char period{};
    uart >> period;

    return period;
}


Max_type choose_max_value(char timer, Max_type max)
{
    UART_iostream uart;
    
    uart << "Max. value (from 0 to ";

    if (timer == '0') uart << (int) Counter0::max_top();
    else if (timer == '1') uart << (int) Counter1::max_top();
    else if (timer == '2') uart << (int) Counter2::max_top();
    else {
	uart << "Unknown timer\n";
	return max;
    }

    uart << ")\n";
    uart >> max;

    return max;
}

void test_bugs()
{// Posibles bugs encontrados en otros programas.
 // Aqui compruebo automaticamente si son realmente bugs de Time_counter o no
    UART_iostream uart;

    Counter1::turn_on_with_overflow_to_count_1s();

// CHECK_TRUE:
    uart << "turn_on_with_overflow_to_count_1s ... ";
    if (OCR1AH == 0x3D and OCR1AL == 0x09)
	uart << " OK\n";

    else
	uart << "ERROR!!!\n";

//    myu::print_registers_timer1(uart);
    Counter1::turn_off();

}


int main()
{
    init_uart();
    init_pins();

    myu::enable_interrupts();

    main_hello();

    test_bugs();

    UART_iostream uart;


    Max_type max_value = 10;

    while(1){
	char timer = choose_timer();
	char period = choose_period(timer);
	max_value = choose_max_value(timer, max_value);


	if (timer == '0'){
	    uart << "Timer 0: ";
	    switch(period){
		break; case '0': Counter0::disable_top_interrupt();
				 Counter0::turn_off();
		break; case '1': generate<Counter0, 1>(max_value);
		break; case '2': generate<Counter0, 8>(max_value);
		break; case '4': generate<Counter0, 64>(max_value);
		break; case '6': generate<Counter0, 256>(max_value);
		break; case '7': generate<Counter0, 1024>(max_value);
		break; case 'p': 
			    uart << '\n';
			    myu::print_registers_timer1(uart);
		break; default: uart << "Unknwon option\n";
	    }
	}

	else if (timer == '1'){
	    uart << "Timer 1: ";
	    switch(period){
		break; case '0': Counter1::disable_top_interrupt();
				 Counter1::turn_off();
		break; case '1': generate<Counter1, 1>(max_value);
		break; case '2': generate<Counter1, 8>(max_value);
		break; case '4': generate<Counter1, 64>(max_value);
		break; case '6': generate<Counter1, 256>(max_value);
		break; case '7': generate<Counter1, 1024>(max_value);
		break; case 'p': 
			    uart << '\n';
			    myu::print_registers_timer1(uart);

		break; default: uart << "Unknwon option\n";
	    }
	}

	else if (timer == '2'){
	    uart << "Timer 2: ";
	    switch(period){
		break; case '0': Counter2::disable_top_interrupt();
				 Counter2::turn_off();
		break; case '1': generate<Counter2, 1>(max_value);
		break; case '2': generate<Counter2, 8>(max_value);
		break; case '3': generate<Counter2, 32>(max_value);
		break; case '4': generate<Counter2, 64>(max_value);
		break; case '5': generate<Counter2, 128>(max_value);
		break; case '6': generate<Counter2, 256>(max_value);
		break; case '7': generate<Counter2, 1024>(max_value);
		break; case 'p': 
			    uart << '\n';
			    myu::print_registers_timer2(uart);

		break; default: uart << "Unknwon option\n";
	    }
	}
    }
}


// INTERRUPTS
// ----------
ISR_TIMER0_COMPA{
    Pin0::toggle();
}

ISR_TIMER1_COMPA{
    Pin1::toggle();
}

ISR_TIMER2_COMPA{
    Pin2::toggle();
}
