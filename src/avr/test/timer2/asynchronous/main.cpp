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

// El siguiente ejemplo es (inicialmente) copia literal del dado en
// https://onlinedocs.microchip.com/pr/GUID-93DE33AC-A8E1-4DD9-BDA3-C76C7CB80969-en-US-2/index.html?GUID-76536716-D66D-4486-8E08-BD5405A1AECA
// Comparar el estilo: sin los comentarios no se entiende el programa. El
// programa que yo suministro son los comentarios que da Microchip: el código
// se puede leer sin necesidad de comentarios!!!
// Esa es una de las ideas fundamentales: el código se tiene que poder leer,
// que no sean necesarios los comentarios.
#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_UART.h"
#include "../../../avr_timer2_basic.h"
#include "../../../avr_time.h"


namespace mcu = avr_;

using Timer = mcu::Timer2;

volatile time_t counter = 0;


 

int main()
{
// UART_init();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

// clock_init();
    Timer::enable_external_asynchronous_clock();
    Timer::CTC_pin_A_clear_on_compare_match();
    Timer::clock_frequency_divide_by_1024();
    Timer::output_compare_register_A(32);
    Timer::wait_till_registers_are_ready();
    Timer::clear_pending_interrupts();
    Timer::enable_output_compare_A_match_interrupt();

    while(1){
	mcu::wait_ms(1000);
    }
}


ISR_TIMER2_COMPA{
    counter = counter + 1;
    mcu::UART_iostream uart;
    uart << counter;   
}


