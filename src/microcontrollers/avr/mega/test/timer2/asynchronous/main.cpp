// Copyright (C) 2023 Manuel Perez 
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

#include "../../../mega_cfg.h"
#include "../../../avr_UART_iostream.h"
#include "../../../avr_timer2_basic.h"
#include <avr_time.h>
#include "../../../avr_debug.h"


#include <atd_ostream.h>
#include <stdlib.h>
#include <std_type_traits.h>

// Microcontroller
// ---------------
namespace my_mcu = avr_;

// Hwd devices
// -----------
using Timer = my_mcu::Timer2;

// Cfg
// ---
constexpr uint16_t period_in_us = 128;

// Global variables
// ----------------
volatile uint32_t counter = 0;


// FUNCTIONS
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}


// Este es el protocolo a seguir para inicializar el timer en modo asíncrono
// (ver punto "22.9 Asynchronous Operation of Timer/Counter2" de la datasheet)
void timer_init_asynchronous_mode()
{
    my_mcu::UART_iostream uart;

    Timer::disable_interrupts();
    Timer::enable_asynchronous_mode();
    Timer::normal_mode();
    Timer::clock_frequency_divide_by_128();  // arranca el timer
					      //

    uart << "wait_till_registers_are_ready ...\n"
	    "ASSR values = ";
//    Timer::wait_till_registers_are_ready(); <-- FUNDAMENTAL LLAMARLA!!!
//    No llamo a wait_till_registers_are_ready para ver el proceso de
//    arranque. En la práctica no escribir este bucle while, mejor llamar a
//    wait_till_registers_are_ready
    while (!Timer::are_registers_ready()){

	atd::print_int_as_hex(uart, ASSR);
	uart << " ";
	my_mcu::wait_ms(400);
    }
    uart << "OK\n";

    Timer::clear_pending_interrupts();
    Timer::enable_overflow_interrupt(); 
}

int main()
{
    init_uart();

    my_mcu::UART_iostream uart;
    uart << "\nTimer2 counter test\n"
	      "-------------------\n"
	      "Connect a crystal of 32kHz to pins TOSC1, TOSC2\n"
	      "without capacitors, directly to pins TOSC1 and TOSC2\n";
// Al principio le conectaba 2 condensadores de 20 pF, pero no son necesarios.
//	      "with 2 capacitors 18 pF (datasheet reference??? )\n";


    timer_init_asynchronous_mode();
    my_mcu::print_registers_timer2(uart);

// start:
    my_mcu::enable_interrupts();
					      


    while(1){
	{// atomic
	    my_mcu::Disable_interrupts di;
	    uart << (int) counter << " s\n";
	}

	my_mcu::wait_ms(1000);
    }
}



ISR_TIMER2_OVF
{
//    my_mcu::UART_iostream uart;
//    uart << "int\n";
    counter = counter + 1;
}


