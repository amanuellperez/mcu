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
#include "../../../mega_timer1_basic.h"
#include <avr_time.h>
#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

using Timer = myu::Timer1;
using namespace myu::literals;


void timer_on_1MHz(uint16_t period_in_us)
{
    if constexpr (myu::clock_frequency == 1_MHz){
	UART_iostream uart;

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
    if constexpr (myu::clock_frequency == 8_MHz){// si no se pone aunque no se llame a 
	    // timer_on_8MHz (por ser a 1MHz) la compila, generando error!!!
	UART_iostream uart;

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
	    "[c]hange value of ocr1a\n"
	    "[d]isconnect\n"
	    "[i]nverting mode\n"
	    "[n]on inverting mode\n"
	    "[t]oggle on compare match (Solo para modo top_OCR1A)\n";


    uint16_t tmp{};
    char c{};
    uart >> c;
    switch(c){
	case 'c':
	    uart << "OCR1A = ";
	    uart >> tmp;
	    uart << tmp << '\n';
	    Timer::unsafe_output_compare_register_A(tmp);
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


void ocb_menu()
{
    UART_iostream uart;

    uart << "\nOCB menu:\n"
	    "[c]hange value of ocr1b\n"
	    "[d]isconnect\n"
	    "[i]nverting mode\n"
	    "[n]on inverting mode\n";
//	    "[t]oggle on compare match\n";


    uint16_t tmp{};
    char c{};
    uart >> c;
    switch(c){
	case 'c':
	    uart << "OCR1B = ";
	    uart >> tmp;
	    uart << tmp << '\n';
	    Timer::unsafe_output_compare_register_B(tmp);
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

enum class Mode{
    top_0x00FF, top_0x01FF, top_0x03FF, top_ICR1, top_OCR1A
};

Mode mode_menu()
{
    UART_iostream uart;
    uart << "Select mode:\n"
	    "[1] top 0x00FF\n"
	    "[2] top 0x01FF\n"
	    "[3] top 0x03FF\n"
	    "[4] top ICR1\n"
	    "[5] top OCR1A\n";

    char c{};
    uart >> c;
    switch (c){
	case '1': 
	    Timer::fast_PWM_mode_top_0x00FF();
	    return Mode::top_0x00FF;

	case '2': 
	    Timer::fast_PWM_mode_top_0x01FF();
	    return Mode::top_0x01FF;

	case '3': 
	    Timer::fast_PWM_mode_top_0x03FF();
	    return Mode::top_0x03FF;

	case '4': 
	    Timer::fast_PWM_mode_top_ICR1();
	    return Mode::top_ICR1;

	case '5': 
	    Timer::fast_PWM_mode_top_OCR1A();
	    return Mode::top_OCR1A;
    }

    return Mode::top_ICR1;
}


void print(Mode mode)
{
    UART_iostream uart;

    myu::Time period;
    myu::Time T_a;
    myu::Time T_b;

    switch(mode){
	case Mode::top_0x00FF:
	    uart << "\nmode = top_0x00FF";
	    period = (uint32_t{0x00FF} + 1)* Timer::clock_period();
	    T_a= Timer::unsafe_output_compare_register_A()* Timer::clock_period();
	    T_b= Timer::unsafe_output_compare_register_B()* Timer::clock_period();
	    break;

	case Mode::top_0x01FF:
	    uart << "\nmode = top_0x01FF";
	    period = (uint32_t{0x01FF} + 1)* Timer::clock_period();
	    T_a= Timer::unsafe_output_compare_register_A()* Timer::clock_period();
	    T_b= Timer::unsafe_output_compare_register_B()* Timer::clock_period();
	    break;

	case Mode::top_0x03FF:
	    uart << "\nmode = top_0x03FF";
	    period = (uint32_t{0x03FF} + 1)* Timer::clock_period();
	    T_a= Timer::unsafe_output_compare_register_A()* Timer::clock_period();
	    T_b= Timer::unsafe_output_compare_register_B()* Timer::clock_period();
	    break;

	case Mode::top_ICR1:
	    uart << "\nmode = top_ICR1";
	    period = (Timer::unsafe_input_capture_register() + 1)* Timer::clock_period();
	    T_a= Timer::unsafe_output_compare_register_A()* Timer::clock_period();
	    T_b= Timer::unsafe_output_compare_register_B()* Timer::clock_period();
	    break;

	case Mode::top_OCR1A:
	    uart << "\nmode = top_OCR1A";
	    uart << "\npin B desconectado!!! Señal solo se ve en pin A\n";
	    period = (Timer::unsafe_output_compare_register_A() + 1)* Timer::clock_period();
	    T_b= Timer::unsafe_output_compare_register_B()* Timer::clock_period();
	    T_a = 0_us;
	    break;
    }

    uart << "\nA ver en el osciloscopio:\n"
	    "      periodo = " << period << " us\n"
	    "      T_a     = " << T_a << " us\n"
	    "      T_b     = " << T_b << " us\n";
}

int main()
{
// init_uart()
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// init_timer()
    Mode mode = Mode::top_ICR1;

    uint16_t period_in_us = 64;
    Timer::counter_type top = 1000;

    // configuración inicial, para que se vea algo en el osciloscopio
    Timer::fast_PWM_mode_top_ICR1();
    Timer::unsafe_input_capture_register(1000);
    Timer::unsafe_output_compare_register_A(500);
    Timer::unsafe_output_compare_register_B(200);
    Timer::PWM_pin_A_non_inverting_mode();
    Timer::PWM_pin_B_inverting_mode();
    timer_on(period_in_us);

    uart << "\n\nFast PWM test\n"
	        "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{Timer::OCA_pin()} << " and " << uint16_t{Timer::OCB_pin()}
         << '\n';

    while(1){
	uart << "\n\nState"
	          "\n-----";
	print(mode);

	uart <<  "\nICR1 = " << Timer::unsafe_input_capture_register() <<
		 "\nOCRA = " << Timer::unsafe_output_compare_register_A() << 
		 "\nOCRB = " << Timer::unsafe_output_compare_register_B() << 
		 "\nTimer::clock_period() = " << Timer::clock_period() << '\n';

	uart << "\nMenu\n"
	          "----\n"
                "o[f]f\n"
		"[o]n\n"
                "[p]eriod_in_us (apaga el timer. Llamar a on)\n"
		"[m]ode\n"
		"OC[A] menu\n"
		"OC[B] menu\n"
		"[I]CR1 select\n";


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

	    case 'm':
	    case 'M':
		mode = mode_menu();
		break;

	    case 'p':
		Timer::off();
		period_in_us = select_period();
		timer_on(period_in_us);
		break;

	    case 'i':
	    case 'I':
		uart << "\nICR1 (max " << Timer::max() << ") = ";
		uart >> top;
		uart << top << '\n';
		Timer::unsafe_input_capture_register(top);
		break;

	    default:
		uart << "I don't understand.\n";
		break;

	}
    }
}



