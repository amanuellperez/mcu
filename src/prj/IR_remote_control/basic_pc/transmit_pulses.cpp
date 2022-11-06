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

#include "dev.h"
#include "cfg.h"

#include "pulse.h"

// Opción 1 : sin interrupciones y en cualquier pin.
// Resultado: 
//  Con el microcontrolador a 1MHz genera una señal de 15kHz!!! 
//	    OJO: con el Timer0 genera 23kHz, mientras que con el Timer1
//	    15kHz???
//  A 8MHz el micro genera 32.9kHz (<-- está quizás valdría. 38kHz - 10% =
//  34.2)
//  
////void square_wave_of(38kHz)
//void transmit_pulses(atd::CArray_view<Pulse>& pulse)
//{
//    Transmit_timer::on<1>(); // leemos microsegundos
//		    
//    avr::Output_pin<ir_transmitter_pin> pin;
//
//    while (true){
//	while (Transmit_timer::value() < 13) // 38kHz
//	{ ; }
//
//	pin.toggle();
//	Transmit_timer::reset();
//    }
//
//
//
//    Transmit_timer::off();
//}


// Opción 2: con interrupciones, en cualquier pin.
// Con el microcontrolador a 1MHz no funciona, generando 12kHz.
// Con el micro a 8MHz genera una señal de 35.5kHz que está dentro del margen
// de error del reloj (38kHz - 10% = 34.2)
volatile uint16_t i  = 0;

void transmit_pulses(atd::CArray_view<Pulse>& pulse)
{
    avr::Pin<ir_transmitter_pin>::as_output();

    Transmit_timer::top(13);
    Transmit_timer::enable_top_interrupt();

    avr::enable_all_interrupts();
    Transmit_timer::on<1>(); // leemos microsegundos
			     

    while (true){
	;
    }


    Transmit_timer::off();
}


ISR_TRANSMIT_TIMER_OVF{
    avr::Pin<ir_transmitter_pin>::toggle();
    i = i + 1;
}

