// Copyright (C) 2019-2021 Manuel Perez 
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
#include "../../../mega_timer1_hwd.h"
#include <avr_time.h>
#include "../../../mega_interrupt.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_micro.h"


// Microcontroller
// ---------------
namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

using namespace myu::literals;
using Timer = hwd::Timer1;

// Probar cada periodo con diferentes frecuencias: 1 MHz y 8 MHz.
// Para los 8 MHz hay que definir el fuse correspondiente y F_CPU en el
// makefile.
// Medir la salida con el osciloscopio.

// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
//constexpr uint16_t top = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


void timer_on_1MHz(uint16_t period_in_us)
{
    if constexpr (hwd::clock_cpu() == 1'000'000){
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
    if constexpr (hwd::clock_cpu() == 8'000'000){// si no se pone aunque no se llame a 
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
    if constexpr (hwd::clock_cpu() == 1'000'000)
	timer_on_1MHz(period_in_us);

    else if constexpr (hwd::clock_cpu() == 8'000'000)
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
    if constexpr (hwd::clock_cpu() == 1'000'000)
	return select_period_1MHz();

    else if constexpr (hwd::clock_cpu() == 8'000'000)
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
	    "[t]oggle on compare match\n"
	    "[c]lear on compare match\n"
	    "[s]et on compare match\n"
	    "Observar que 'clear' y 'set' no hacen nada.\n"
	    "Supongo (?) que la idea es que el pin cambie transcurrido\n"
	    "un tiempo (el tiempo que tarda el timer en llegar al TOP\n";


    char c{};
    uart >> c;
    switch(c){
	case 'd':
	    Timer::pin_A_disconnected();
	    break;

	case 't': 
	    Timer::CTC_pin_A_toggle_on_compare_match();
	    break;

	case 'c': 
	    Timer::CTC_pin_A_clear_on_compare_match();
	    break;

	case 's': 
	    Timer::CTC_pin_A_set_on_compare_match();
	    break;
    }
}

void ocb_menu()
{
    UART_iostream uart;

    uart << "\nOCB menu:\n"
	    "[d]isconnect\n"
	    "[t]oggle on compare match\n"
	    "[c]lear on compare match\n"
	    "[s]et on compare match\n";


    char c{};
    uart >> c;
    switch(c){
	case 'd':
	    Timer::pin_B_disconnected();
	    break;

	case 't': 
	    Timer::CTC_pin_B_toggle_on_compare_match();
	    break;

	case 'c': 
	    Timer::CTC_pin_B_clear_on_compare_match();
	    break;

	case 's': 
	    Timer::CTC_pin_B_set_on_compare_match();
	    break;
    }

}




int main()
{
// init_uart()
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// init_timer()
    uint16_t period_in_us = 1;
    Timer::counter_type top = 1000;

    //Timer::mode_CTC_top_OCRA();
    //Timer::output_compare_register_A(top);
    Timer::CTC_mode_top_ICR();
    { myu::Disable_interrupts l; Timer::unsafe_input_capture_register(top);}
    Timer::CTC_pin_A_toggle_on_compare_match(); // para que se vea algo al ppio
    Timer::CTC_pin_B_toggle_on_compare_match();
    timer_on(period_in_us);

    uart << "\n\nCTC mode test\n"
            "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{Timer::OCA_pin()} << " and " << uint16_t{Timer::OCB_pin()}
         << '\n';

    while(1){
        uint32_t period = uint32_t{2} * (uint32_t{top} + 1)* uint32_t{period_in_us};

	uart << "\n\nState\n"
		    "-----\n"
	        "top = " << top <<
		"\nperiod_in_us                = " << period_in_us <<
//		"\nTimer::clock_period_in_us() = " << Timer::clock_period() <<
                "\nPeriodo a ver en el osciloscopio: " <<
		period << " us +- error ("
                "2*(top + 1)*period_in_us)\n";

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
	    break; case 'o': timer_on(period_in_us);
	    break; case 'f': Timer::off();
	    break; case 'a':
	           case 'A': oca_menu();
	    break; case 'b':
		   case 'B': ocb_menu();
	    break; case 'p':
		    Timer::off();
		    period_in_us = select_period();
		    timer_on(period_in_us);

	    break; case 't':
		uart << "\ntop (max " << Timer::max() << ") = ";
		uart >> top;
		uart << top << '\n';
		//Timer::output_compare_register_A(top);
		{ myu::Disable_interrupts l; 
		  Timer::unsafe_input_capture_register(top);}

	    break; default: uart << "Unknown option\n";

	}

    }
}



