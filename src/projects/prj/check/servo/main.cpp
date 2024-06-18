// Copyright (C) 2020 Manuel Perez 
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

#include <avr_UART.h>
#include <avr_timer1_basic.h>
#include <avr_time.h>


using Timer = avr::Timer1_fast_PWM;

constexpr uint16_t period_in_us = 1;

void interactivo();
void automatico();

int main()
{
    // init_uart();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

    // init_timer();
    Timer::top_ICR(20000UL); // 20000 us = 20 ms
    Timer::output_compare_register_A(1000UL); // 1 ms = posición del 0 +-

    Timer::pin_A_non_inverting_mode();
    Timer::on<period_in_us>();

    // menu();
    while(1){
	uart << "\n\nMenu:\n"
	     << "[a]utomático\n"
	     << "[i]nteractivo\n";

	char c{};
	uart >> c;

	if (c == 'a' or c == 'A')
	    automatico();
	else
	    interactivo();
    }

}


void interactivo()
{
    avr::UART_iostream uart;

    while(1){
	uint16_t t = 0;
	uart << "Duración del pulso (de 1000 a 2000 us): ";
	uart >> t;
	
	uart << t << "\n\r"; // echo
	if (t == 0)
	    return;
	Timer::output_compare_register_A(t);

	wait_ms(1000);
    }
}


void automatico()
{
    avr::UART_iostream uart;

    uint16_t incr = 100;
    uint16_t t_min = 900;
    uint16_t t_max = 2500;

    while(1){
        uart << "\n\nPruebas automáticas: de " << t_min << " a " << t_max
             << " con incr = " << incr << "\n";
        for (uint16_t t = t_min; t < t_max; t+=incr){
	    uart << "t = " << t << "; ";

	    Timer::output_compare_register_A(t);

	    wait_ms(1000);
	}

	uart << "\n\nVolver al menu? (s/n)";
	char res{};
	uart >> res;
	if (res == 's' or res == 'S')
	    return;

	uart << "\n\nValor minimo = ";
	uart >> t_min;
	uart << t_min << '\n';
	uart << "Valor maximo = ";
	uart >> t_max;
	uart << t_max << '\n';
	uart << "Incremento = ";
	uart >> incr;
	uart << incr << '\n';
    }
}

