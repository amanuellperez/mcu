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

#include "../../../dev_clocks.h"
#include <avr_interrupt.h>
#include <avr_UART.h>
#include <avr_time.h>
#include <avr_timer1_generic.h>
#include <atd_time.h>


// Dependiendo del valor de F_CPU hay que pasar un periodo diferente.
// El compilador tiene que mostrar mensajes adecuados a cada error.
// Probar con diferentes periodos y F_CPU.
//constexpr static uint16_t clock_timer_period_in_us = 8u;
//constexpr static uint16_t clock_timer_period_in_us = 32u;
constexpr static uint16_t clock_timer_period_in_us = 64u;


// Con el Timer0 no funciona ya que el counter del timer0 es de 8 bits y no de
// 16 bits.
using Timer = avr::Timer1_generic;
using Chronometer_ms = dev::Chronometer_ms<Timer, clock_timer_period_in_us>;



// Definimos el reloj del sistema
ISR_TIMER1_COMPA
{
    Chronometer_ms::tick();
}




void print_time(avr::UART_iostream& uart, const Chronometer_ms::Sexagesimal_ms& s)
{
    uart << std::setw(2) << s.hours << ':'
	 << std::setw(2) << s.minutes << ':'
	 << std::setw(2) << s.seconds << ':'
	 << std::setw(3) << s.milliseconds << '\n';

}

void halt()
{
    avr::UART_iostream uart;
    Chronometer_ms::off();

    uart << "\n[s]top (default)\n"
	      "[r]esume\n";

    char ans{};
    uart >> ans;
    if (ans == 'r'){
	Chronometer_ms::on();
	return;
    }

    Chronometer_ms::reset();
    print_time(uart, Chronometer_ms::sexagesimal_count());

    uart << "Press a key to start\n";
    uart >> ans;
    Chronometer_ms::on();

}


void print_menu()
{
    avr::UART_iostream uart;

    uart << "Press 'h' to halt\n";
}

// TODO: hacer esto interactivo. Que se pueda parar, encender, el cronometro.
// Que se pueda elegir si ir hacia arriba o abajo. Que se pueda poner en hora.
// Para ello necesito poder mirar si uart ha recibido alguna
void test_chronometer_clock()
{
    avr::UART_iostream uart;
    using UART = avr::UART_basic;

    uart << "\n\nChronometer_ms test\n"
	        "-------------------\n";
    print_menu();

    Chronometer_ms::on();

    while(1){
	print_time(uart, Chronometer_ms::sexagesimal_count());

	wait_ms(100);

	if(UART::are_there_data_unread()){
	    char ans = static_cast<char>(UART::data_register());
	    if(ans == 'h')
		halt();

	    print_menu();
	}

    }
}


void init_uart()
{
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();
}


void init_timer()
{
    Chronometer_ms::init();
}


int main()
{
    init_uart();
    init_timer();

    test_chronometer_clock();
}



