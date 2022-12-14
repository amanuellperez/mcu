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

// Para probarlo conectar el pin 15 al osciloscopio y medir la señal generada.

constexpr uint8_t num_pin = 15;
using Counter = avr_::Time_counter0_g;
using Counter1 = avr_::Time_counter1_g;

//constexpr uint16_t period_in_us = 1u; 
constexpr uint16_t period_in_us = 8u; 
//constexpr uint16_t period_in_us = 64u; 
//constexpr uint16_t period_in_us = 256u; 
//constexpr uint16_t period_in_us = 1024u; 
constexpr Counter::counter_type max_value = 1;

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

int main()
{
    avr_::Output_pin<num_pin> pin;
    Counter::init();
    Counter::on<period_in_us>();

    while(1){

	Counter::reset();   // DUDA: 1º el reset y luego el write o al revés?
	pin.write_zero();
	while (Counter::value() < max_value) { ; }

	Counter::reset();
	pin.write_one();
	while (Counter::value() < max_value) { ; }

    }
}



