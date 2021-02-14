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

#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"
#include "../../../avr_UART_iostream.h"


using Timer = avr::Timer1;
using namespace avr::literals;


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


void oca_menu(uint16_t& ocr1a)
{
    avr::UART_iostream uart;

    uart << "\nOCA menu:\n"
	    "[c]hange value of ocr1a\n"
	    "[d]isconnect\n"
	    "[i]nverting mode\n"
	    "[n]on inverting mode\n"
	    "[t]oggle on compare match (NO FUNCIONA?? )\n";


    char c{};
    uart >> c;
    switch(c){
	case 'c':
	    uart << "ocr1a = ";
	    uart >> ocr1a;
	    uart << ocr1a << '\n';
	    Timer::output_compare_register_A(ocr1a);
	    break;

	case 'd':
	    Timer::pin_A_disconnected();
	    break;

	case 'i': 
	    Timer::PWM_pin_A_inverting_mode();
	    break;

	case 'n': 
	    Timer::PWM_pin_A_non_inverting_mode();
	    break;

	case 't': 
	    Timer::fast_PWM_pin_A_toggle_on_compare_match();
	    break;
    }
}


void ocb_menu(uint16_t& ocr1b)
{
    avr::UART_iostream uart;

    uart << "\nOCB menu:\n"
	    "[c]hange value of ocr1b\n"
	    "[d]isconnect\n"
	    "[i]nverting mode\n"
	    "[n]on inverting mode\n";
//	    "[t]oggle on compare match\n";


    char c{};
    uart >> c;
    switch(c){
	case 'c':
	    uart << "ocr1b = ";
	    uart >> ocr1b;
	    uart << ocr1b << '\n';
	    Timer::output_compare_register_B(ocr1b);
	    break;
	case 'd':
	    Timer::pin_B_disconnected();
	    break;

	case 'i': 
	    Timer::PWM_pin_B_inverting_mode();
	    break;

	case 'n': 
	    Timer::PWM_pin_B_non_inverting_mode();
	    break;

//	case 't': 
//	    Timer::fast_PWM_pin_B_toggle_on_compare_match();
//	    break;
    }
}

int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

// init_timer()
    uint16_t period_in_us = 64;
    Timer::counter_type top = 1000;
    uint16_t ocr1a = 100;
    uint16_t ocr1b = 300;

    // configuración inicial, para que se vea algo en el osciloscopio
    Timer::mode_fast_PWM_top_ICR1();
    Timer::input_capture_register(top);
    Timer::output_compare_register_A(ocr1a);
    Timer::output_compare_register_B(ocr1b);
    Timer::PWM_pin_A_non_inverting_mode();
    Timer::PWM_pin_B_inverting_mode();
    timer_on(period_in_us);

    uart << "\n\nFast PWM test\n"
	        "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{Timer::OCA_pin()} << " and " << uint16_t{Timer::OCB_pin()}
         << '\n';

    while(1){
        uint32_t period = (uint32_t{top} + 1)* uint32_t{period_in_us};
        uint32_t T_a= (uint32_t{ocr1a})* uint32_t{period_in_us};
        uint32_t T_b= (uint32_t{ocr1b})* uint32_t{period_in_us};
	uart << "\n\nState"
	          "\n-----"
		    "\ntop = " << top <<
		    "\nocra = " << ocr1a << 
		    "\nocrb = " << ocr1b << 
		    "\nperiod_in_us                = " << period_in_us <<
		    "\nTimer::clock_period_in_us() = " << Timer::clock_period() <<
                "\nA ver en el osciloscopio: " <<
		period << " us +- error ((top + 1)*period_in_us)\n"
		"OCA cambia en " << T_a << " us\n"
		"OCB cambia en " << T_b << " us\n";

	uart << "\nMenu\n"
	          "----\n"
                "o[f]f\n"
		"[o]n\n"
                "[p]eriod_in_us (apaga el timer. Llamar a on)\n"
                "[t]op (apaga el timer. Llamar a on)\n"
		"OC[A] menu\n"
		"OC[B] menu\n";

        char c{};
	uart >> c;
	switch(c){
	    case 'o':
		timer_on(period_in_us);
		break;

	    case 'f':
		Timer::off();
		break;

	    case 'a':
	    case 'A':
		oca_menu(ocr1a);
		break;

	    case 'b':
	    case 'B':
		ocb_menu(ocr1b);
		break;

	    case 'p':
		Timer::off();
		period_in_us = select_period();
		timer_on(period_in_us);
		break;

	    case 't':
		uart << "\ntop (max " << Timer::max() << ") = ";
		uart >> top;
		uart << top << '\n';
		Timer::output_compare_register_A(top);
		break;

	    default:
		uart << "I don't understand.\n";
		break;

	}
    }
}



