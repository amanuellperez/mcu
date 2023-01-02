// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_pin.h"
#include "../../../avr_timer0_generic.h"
#include "../../../avr_timer1_generic.h"
#include "../../../avr_UART.h"

namespace mcu = avr_;

constexpr uint8_t pin_test = 15;

using Counter0  = mcu::Time_counter0_g;
using Counter1 = mcu::Time_counter1_g;

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

template <typename Counter, uint16_t period_in_us>
void generate(const typename Counter::counter_type& max_value, uint16_t nrep)
{
    mcu::UART_iostream uart;
    uart << "\n----------------------------\n"
	    "Generating " << nrep << " times from 0 to " << (int) max_value << "\n"
	    "\tTimer period in us = " << period_in_us << '\n';

    if constexpr (std::is_same_v<Counter, Counter0>)
	uart << "\tTimer 0";

    else
	uart << "\tTimer 1";

    auto T = 2*(period_in_us * max_value);
    uart << "\tPeriod of the signal generate =? " << T << '\n';

    mcu::Output_pin<pin_test> pin;

    Counter::init();
    Counter::template on<period_in_us>();

    for (uint16_t i = 0; i < nrep; ++i){
	Counter::reset();   // DUDA: 1º el reset y luego el write o al revés?
	pin.write_zero();
	while (Counter::value() < max_value) { ; }

	Counter::reset();
	pin.write_one();
	while (Counter::value() < max_value) { ; }
    }

    uart << "DONE\n";
    pin.write_zero();
}


int main()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

    uart << "\n\nTime counter test\n"
	        "----------------\n"
		"Connect oscilloscope to pin " << (int) pin_test << '\n';


constexpr Counter1::counter_type max_value = 1;
    uint16_t nrep = 10000;
    while(1){
	uart << "--------------------------\n"
	        "0. Timer 0\n"
		"1. Timer 1\n";

	char timer{};
	uart >> timer;
    
	uart << "\nPeriod:\n"
	        "[1]    1 us\n"
		"[2]    8 us\n"
		"[3]   64 us\n"
		"[4]  256 us\n"
		"[5] 1024 us\n";

	char period{};
	uart >> period;

	if (timer == '0')
	    switch(period){
		break; case '1': generate<Counter0, 1>(max_value, nrep);
		break; case '2': generate<Counter0, 8>(max_value, nrep);
		break; case '3': generate<Counter0, 64>(max_value, nrep);
		break; case '4': generate<Counter0, 256>(max_value, nrep);
		break; case '5': generate<Counter0, 1024>(max_value, nrep);
		break; default: generate<Counter0, 1>(max_value, nrep);
	    }

	else
	    switch(period){
		break; case '1': generate<Counter1, 1>(max_value, nrep);
		break; case '2': generate<Counter1, 8>(max_value, nrep);
		break; case '3': generate<Counter1, 64>(max_value, nrep);
		break; case '4': generate<Counter1, 256>(max_value, nrep);
		break; case '5': generate<Counter1, 1024>(max_value, nrep);
		break; default: generate<Counter1, 1>(max_value, nrep);
	    }
	

    }
}



