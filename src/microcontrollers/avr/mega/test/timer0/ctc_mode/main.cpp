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

#include "../../../mega_cfg.h"
#include "../../../avr_timer0_basic.h"
#include <avr_time.h>
#include "../../../avr_UART_iostream.h"


// Microcontroller
// ---------------
namespace my_mcu = avr_;

// Hwd devices
// -----------
using Timer = my_mcu::Timer0;

// Types
// -----
using namespace my_mcu::literals;


// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
//constexpr uint16_t top = 15625; // 15625 miniticks * 64 us/minitick = 1 seg

/***************************************************************************
 *			    FUNCTIONS
 ***************************************************************************/

void timer_on_1MHz(uint16_t period_in_us)
{
    if constexpr (my_mcu::clock_frequency == 1_MHz){
	my_mcu::UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::clock_frequency_no_prescaling(); break;
	    case 8: Timer::clock_frequency_divide_by_8(); break;
	    case 64: Timer::clock_frequency_divide_by_64(); break;
	    case 256: Timer::clock_frequency_divide_by_256(); break;
	    case 1024: Timer::clock_frequency_divide_by_1024(); break;
	    default:
		uart << "Invalid period_in_us [" << period_in_us << "]";
		break;
	}
    }
}


void timer_on_8MHz(uint16_t period_in_us)
{
    if constexpr (my_mcu::clock_frequency == 8_MHz){// si no se pone aunque no se llame a 
	    // timer_on_8MHz (por ser a 1MHz) la compila, generando error!!!
	my_mcu::UART_iostream uart;

	switch(period_in_us){
	    case 1: Timer::clock_frequency_divide_by_8(); break;
	    case 8: Timer::clock_frequency_divide_by_64(); break;
	    case 32: Timer::clock_frequency_divide_by_256(); break;
	    case 128: Timer::clock_frequency_divide_by_1024(); break;
	    default:
		uart << "Invalid period_in_us [" << period_in_us << "]\n";
		break;
	}
    }
}



void timer_on(uint16_t period_in_us)
{
    if constexpr (my_mcu::clock_frequency == 1_MHz)
	timer_on_1MHz(period_in_us);

    else if constexpr (my_mcu::clock_frequency == 8_MHz)
	timer_on_8MHz(period_in_us);

}


void print_state(const Timer::counter_type& top, uint16_t period_in_us)
{
    uint32_t period = uint32_t{2} * (uint32_t{top} + 1)* uint32_t{period_in_us};

    my_mcu::UART_iostream uart;
    uart << "\n\nState\n"
		"-----\n"
	      "top          = " << (int) top <<
	    "\nperiod_in_us = " << period_in_us <<
//		"\nTimer::clock_period() = " << Timer0::clock_period() <<
	    "\nPeriodo a ver en el osciloscopio "
	    "(= 2*(top + 1)*period_in_us):\n\t" 
	    << period << " us +- error\n";

}


/***************************************************************************
 *			INTERACTIVE FUNCTIONS
 ***************************************************************************/
uint16_t select_period_1MHz()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nSelect period in us of the clock timer (avr at 1MHz):\n"
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
    my_mcu::UART_iostream uart;

    uart << "\n\nSelect period in us of the clock timer (avr at 8MHz):\n"
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

uint16_t _select_period()
{
    if constexpr (my_mcu::clock_frequency == 1_MHz)
	return select_period_1MHz();

    else if constexpr (my_mcu::clock_frequency == 8_MHz)
	return select_period_8MHz();

    else{
	my_mcu::UART_iostream uart;
	uart << "ERROR: select_period(), unknown frecuency\n";
	return 1;
    }
}

uint16_t select_period(uint16_t period_in_us)
{
    Timer::off();
    period_in_us = _select_period();
    timer_on(period_in_us);

    return period_in_us;
}


Timer::counter_type select_top(Timer::counter_type top)
{
    my_mcu::UART_iostream uart;

    uart << "\ntop (max " << (int) Timer::max() << ") = ";

    uint16_t top16{};
    uart >> top16;
    uart << top16 << '\n'; // echo
    top = static_cast<uint8_t>(top16);
    Timer::output_compare_register_A(top);

    return top;
}


/***************************************************************************
 *				MENUS	
 ***************************************************************************/
void oca_menu()
{
    my_mcu::UART_iostream uart;

    uart << "\nOCA menu:\n"
	    "[d]isconnect\n"
	    "[t]oggle on compare match\n"
	    "[c]lear on compare match\n"
	    "[s]et on compare match\n"
	    "Observar que 'clear' y 'set' no hacen nada.\n"
	    "Supongo (?) que la idea es que el pin cambie transcurrido\n"
	    "un tiempo (el tiempo que tarda el timer en llegar al TOP\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case 'd': Timer::pin_A_disconnected();
	break; case 't': Timer::CTC_pin_A_toggle_on_compare_match();
	break; case 'c': Timer::CTC_pin_A_clear_on_compare_match();
	break; case 's': Timer::CTC_pin_A_set_on_compare_match();
    }
}

void ocb_menu()
{
    my_mcu::UART_iostream uart;

    uart << "\nOCB menu:\n"
	    "[d]isconnect\n"
	    "[t]oggle on compare match\n"
	    "[c]lear on compare match\n"
	    "[s]et on compare match\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case 'd': Timer::pin_B_disconnected();
	break; case 't': Timer::CTC_pin_B_toggle_on_compare_match();
	break; case 'c': Timer::CTC_pin_B_clear_on_compare_match();
	break; case 's': Timer::CTC_pin_B_set_on_compare_match();
    }

}


void main_menu(Timer::counter_type& top, uint16_t& period_in_us)
{
    my_mcu::UART_iostream uart;

    print_state(top, period_in_us);

    uart << "\nMenu\n"
		"----\n"
	    "1. Turn on\n"
	    "2. Turn off\n"
	    "3. Select period_in_us\n"
	    "4. Select top          (This option turn off the timer. Remember tu turn on)\n"
	    "5. OCA menu\n"
	    "6. OCB menu\n";

    char option{};
    uart >> option;
    switch(option){
	break; case '1': timer_on(period_in_us);
	break; case '2': Timer::off();
	break; case '3': period_in_us = select_period(period_in_us);
	break; case '4': top = select_top(top);
	break; case '5': oca_menu();
	break; case '6': ocb_menu();
	break; default : uart << "Unknown option";
    }

}

/***************************************************************************
 *				MAIN
 ***************************************************************************/
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}

void init_timer(const Timer::counter_type& top, uint16_t period_in_us)
{
    Timer::CTC_mode();
    Timer::output_compare_register_A(top);
    Timer::CTC_pin_A_toggle_on_compare_match(); // para que se vea algo al ppio
    Timer::CTC_pin_B_toggle_on_compare_match();
    timer_on(period_in_us);
}


// Presentación
void main_hello()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nCTC mode test\n"
                "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{Timer::OCA_pin()} << " and " << uint16_t{Timer::OCB_pin()}
         << "\n\nYou can test this program with F_CPU=1 MHz or F_CPU = 8 MHz\n"
	    "To change F_CPU change its value in the makefile and recompile\n\n";
}


int main()
{
    uint16_t period_in_us = 1;
    Timer::counter_type top = 200;

    init_uart();
    init_timer(top, period_in_us);

    main_hello();

    while(1){
	main_menu(top, period_in_us);
    }
}




