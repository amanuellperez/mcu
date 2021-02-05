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

// Ejemplo básico de uso del Timer como contador.
// Genera una señal de 1MHz en el pin OC1A (pin 15)
// y en el pin OC1B (pin 16). Conectar el osciloscopio a estos pines para
// comprobarlo.
#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"
#include "../../../avr_UART_iostream.h"


using namespace avr::literals;
using Timer = avr::Timer1_CTC_mode;

// Probar cada periodo con diferentes frecuencias: 1 MHz y 8 MHz.
// Para los 8 MHz hay que definir el fuse correspondiente y F_CPU en el
// makefile.
// Medir la salida con el osciloscopio.

// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
//constexpr uint16_t top = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


void timer_on_1MHz(uint16_t period_in_us)
{
    if constexpr (avr::clock_frequency == 1_MHz){
	avr::UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::on<1>(); break;
	    case 8: Timer::on<8>(); break;
	    case 64: Timer::on<64>(); break;
	    case 256: Timer::on<256>(); break;
	    case 1024: Timer::on<1024>(); break;
	    default:
		uart << "period_in_us [" << period_in_us << "] no válido\n";
		break;
	}
    }
}


void timer_on_8MHz(uint16_t period_in_us)
{
    if constexpr (avr::clock_frequency == 8_MHz){// si no se pone aunque no se llame a 
	    // timer_on_8MHz (por ser a 1MHz) la compila, generando error!!!
	avr::UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::on<1>(); break;
	    case 8: Timer::on<8>(); break;
	    case 32: Timer::on<32>(); break;
	    case 128: Timer::on<128>(); break;
	    case 1024: Timer::on<1024>(); break;
	    default:
		uart << "period_in_us [" << period_in_us << "] no válido\n";
		break;
	}
    }
}



void timer_on(uint16_t period_in_us)
{
    if constexpr (avr::clock_frequency == 1_MHz)
	timer_on_1MHz(period_in_us);

    else if constexpr (avr::clock_frequency == 8_MHz)
	timer_on_8MHz(period_in_us);

}

uint16_t select_period_1MHz()
{
    avr::UART_iostream uart;

    uart << "\n\nperiod_in_us (avr a 1MHz):\n"
	    "1\n"
	    "8\n"
	    "64\n"
	    "256\n"
	    "1024\n";

    uint16_t res{};
    uart >> res;
    switch(res){
	case 1:
	case 8:
	case 64:
	case 256:
	case 1024:
	    return res;
    }

    return 1;
}

uint16_t select_period_8MHz()
{
    avr::UART_iostream uart;

    uart << "\n\nperiod_in_us (avr a 8MHz):\n"
	    "1\n"
	    "8\n"
	    "32\n"
	    "128\n"
	    "1024\n";

    uint16_t res{};
    uart >> res;
    switch(res){
	case 1: 
	case 8: 
	case 32:
	case 128: 
	case 1024:
	    return res;
    }

    return 1;
}


uint16_t select_period()
{
    if constexpr (avr::clock_frequency == 1_MHz)
	return select_period_1MHz();

    else if constexpr (avr::clock_frequency == 8_MHz)
	return select_period_8MHz();

    else{
	avr::UART_iostream uart;
	uart << "ERROR: select_period(), frecuencia desconocida\n";
	return 1;
    }
}


void run_timer(uint16_t period_in_us, Timer::counter_type top)
{
    Timer::top_OCRA(top);
    Timer::pin_A_toggle_on_compare_match();
    Timer::pin_B_toggle_on_compare_match();
    timer_on(period_in_us);
}


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();


    uint16_t period_in_us = 1;
    Timer::counter_type top = 1000;

    while(1){
	uart << "\n\nState:\n"
	        "top = " << top <<
		"\nperiod_in_us                = " << period_in_us <<
		"\nTimer::clock_period_in_us() = " << Timer::clock_period_in_us();
//	avr::Microsecond us = Timer::clock_period();
//		uart << "\nTimer::clock_period()       = " << us << '\n';
//		"\nTimer::clock_period()       = " << Timer::clock_period() << '\n';

        uint32_t period = uint32_t{2} * uint32_t{top} * uint32_t{period_in_us};
        uart << "\n\nMenu:\n"
                "o[f]f\n"
		"[o]n\n"
                "[p]eriod_in_us (apaga el timer. Llamar a on)\n"
                "[t]op (apaga el timer. Llamar a on)\n"
                "\nPeriodo a ver en el osciloscopio: "
                "2*top*period_in_us = " << 
		period << " us +- error\n\n"
		"Elige opción del menu\n";

        char c{};
	uart >> c;
	switch(c){
	    case 'o':
		run_timer(period_in_us, top);
		break;

	    case 'f':
		Timer::off();
		break;

	    case 'p':
		Timer::off();
		period_in_us = select_period();
		run_timer(period_in_us, top);
		break;

	    case 't':
		Timer::off();
		uart << "\ntop = ";
		uart >> top;
		uart << top << '\n';
		run_timer(period_in_us, top);
		break;

	    default:
		uart << "No entiendo\n";
		break;

	}

    }
}



