// Copyright (C) 2021 Manuel Perez 
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

#include "../../../mega_cfg_hwd.h"
#include "../../../mega_timer0_hwd.h"
#include <avr_time.h>
#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

using namespace myu::literals;
using Timer = myu::Timer0;

// Probar cada periodo con diferentes frecuencias: 1 MHz y 8 MHz.
// Para los 8 MHz hay que definir el fuse correspondiente y F_CPU en el
// makefile.
// Medir la salida con el osciloscopio.

// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
//constexpr uint16_t top = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


void timer_on_1MHz(uint16_t period_in_us)
{
    if constexpr (myu::clock_frequency == 1_MHz){
	UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::clock_frequency_no_prescaling(); break;
	    case 8: Timer::clock_frequency_divide_by_8(); break;
	    case 64: Timer::clock_frequency_divide_by_64(); break;
	    case 256: Timer::clock_frequency_divide_by_256(); break;
	    case 1024: Timer::clock_frequency_divide_by_1024(); break;
	    default:
		uart << "period_in_us [" << period_in_us << "] no válido\n";
		break;
	}
    }
}


void timer_on_8MHz(uint16_t period_in_us)
{
    if constexpr (myu::clock_frequency == 8_MHz){// si no se pone aunque no se llame a 
	    // timer_on_8MHz (por ser a 1MHz) la compila, generando error!!!
	UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::clock_frequency_divide_by_8(); break;
	    case 8: Timer::clock_frequency_divide_by_64(); break;
	    case 32: Timer::clock_frequency_divide_by_256(); break;
	    case 128: Timer::clock_frequency_divide_by_1024(); break;
	    default:
		uart << "period_in_us [" << period_in_us << "] no válido\n";
		break;
	}
    }
}



void timer_on(uint16_t period_in_us)
{
    if constexpr (myu::clock_frequency == 1_MHz)
	timer_on_1MHz(period_in_us);

    else if constexpr (myu::clock_frequency == 8_MHz)
	timer_on_8MHz(period_in_us);

}

uint16_t select_period_1MHz()
{
    UART_iostream uart;

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
    UART_iostream uart;

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
    if constexpr (myu::clock_frequency == 1_MHz)
	return select_period_1MHz();

    else if constexpr (myu::clock_frequency == 8_MHz)
	return select_period_8MHz();

    else{
	UART_iostream uart;
	uart << "ERROR: select_period(), frecuencia desconocida\n";
	return 1;
    }
}



void oca_menu()
{
    UART_iostream uart;

    uart << "\nOCA menu:\n"
	    "[d]isconnect\n"
	    "[n]on inverting mode\n"
	    "[i]nverting mode\n"
	    "t[o]ggle on compare match\n"
	    "[t]op (OCRA)\n";


    char c{};
    uart >> c;
    uint16_t top{};

    switch(c){
	case 'd':
	    Timer::pin_A_disconnected();
	    break;

	case 'n': 
	    Timer::PWM_pin_A_non_inverting_mode();
	    break;

	case 'i': 
	    Timer::PWM_pin_A_inverting_mode();
	    break;

	case 'o': 
	    Timer::PWM_pin_A_toggle_on_compare_match();
	    break;

	case 't':
	    uart << "top = ";
	    uart >> top;
	    uart << top << '\n';
	    Timer::output_compare_register_A(static_cast<Timer::counter_type>(top));
	    break;
    }
}

void ocb_menu()
{
    UART_iostream uart;

    uart << "\nOCB menu:\n"
	    "[d]isconnect\n"
	    "[n]on inverting mode\n"
	    "[i]nverting mode\n"
	    "[t]op (OCRB)\n";


    uint16_t top{};
    char c{};
    uart >> c;
    switch(c){
	case 'd':
	    Timer::pin_B_disconnected();
	    break;

	case 'n': 
	    Timer::PWM_pin_B_non_inverting_mode();
	    break;

	case 'i': 
	    Timer::PWM_pin_B_inverting_mode();
	    break;

	case 't':
	    uart << "top = ";
	    uart >> top;
	    uart << top << '\n';
	    Timer::output_compare_register_B(static_cast<Timer::counter_type>(top));
	    break;
    }

}


enum class Mode{
    top_0xFF, top_OCRA
};


int main()
{
// init_uart()
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// init_timer()
    uint16_t period_in_us = 1;
    Mode mode = Mode::top_0xFF;

    Timer::mode_fast_PWM_top_0xFF();
    Timer::output_compare_register_A(50);
    Timer::output_compare_register_B(50);
    Timer::PWM_pin_A_non_inverting_mode(); // para que se vea algo al ppio
    Timer::PWM_pin_B_non_inverting_mode();
    timer_on(period_in_us);

    uart << "\n\nCTC mode test\n"
            "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{Timer::OCA_pin()} << " and " << uint16_t{Timer::OCB_pin()}
         << '\n';

    while(1){
	myu::Time period{};
	if (mode == Mode::top_0xFF)
	    period = Timer::clock_period() * (0xFF + 1);
	else 
	    period = Timer::clock_period() * (Timer::output_compare_register_A() + 1);

	uart << "\n\nState\n"
		    "-----"
		"\nperiod_in_us          (wanted) = " << period_in_us <<
		"\nTimer::clock_period() (real)   = " << Timer::clock_period() <<
	        "\nmode = mode_fast_PWM_top_";
	if (mode == Mode::top_0xFF)
	    uart << "0xFF";
	else {
	    uart << "OCRA"
	            "\nEn este modo la frecuencia la controla OCRA"
		    "\nel pin_A desconectarlo, ya que no se genera señal ahí"
		    "\nOCRB controla el duty_cycle. Solo se genera señal en pin_B\n";
	}

	uart <<	"\nOCRA = " << (int) Timer::output_compare_register_A() << 
		"\nOCRB = " << (int) Timer::output_compare_register_B() << 
                "\nPeriodo a ver en el osciloscopio: " <<
		period << " us +- error\n";
                

        uart << "\nMenu\n"
	            "----\n"
                "o[f]f\n"
		"[o]n\n"
                "[p]eriod_in_us (apaga el timer. Llamar a on)\n"
		"[m]ode\n"
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
		oca_menu();
		break;

	    case 'b':
	    case 'B':
		ocb_menu();
		break;

	    case 'p':
		Timer::off();
		period_in_us = select_period();
		timer_on(period_in_us);
		break;

	    case 'm':
		uart << "top_OCR[A]\n"
		        "top_0xF[F]\n";
		uart >> c;
		if (c == 'a' or c == 'A'){
		    mode = Mode::top_OCRA;
		    Timer::mode_fast_PWM_top_OCRA();
		}
		else{
		    mode = Mode::top_0xFF;
		    Timer::mode_fast_PWM_top_0xFF();
		}


	    default:
		uart << "I don't understand.\n";
		break;

	}

    }
}



