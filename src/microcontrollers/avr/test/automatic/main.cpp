// Copyright (C) 2024 Manuel Perez 
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

// Ejemplo básico de uso del Timer como contador
#include "../../avr_atmega328p_cfg.h"
#include "../../avr_UART_iostream.h"
#include "../../avr_timer0_basic.h"
#include "../../avr_timer1_basic.h"
#include "../../avr_time.h"

#include <atd_ostream.h>
#include <stdlib.h>
#include <std_type_traits.h>
#include <atd_test.h>

using namespace test;

// Microcontroller
// ---------------
namespace myu = avr_;

					 
// Hwd devices
// -----------


// FUNCTIONS
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg(uart);
    uart.turn_on();
}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nAutomatic test\n"
	        "--------------\n";
}

void press_key_to_continue()
{
    myu::UART_iostream uart;
    uart << "\nPress key to continue\n";
    char key{};
    uart >> key;
}

void test_timer0()
{
    myu::UART_iostream uart;
    auto test = Test::interface(uart, "Timer0");

    using Timer = myu::Timer0;

    Timer::off();

// counter
    Timer::counter(25u);
    CHECK_TRUE(test, Timer::counter() == 25u, "counter");

// modes
    Timer::normal_mode();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::normal, "normal_mode");

    Timer::CTC_mode();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::CTC, "CTC_mode");

    Timer::fast_PWM_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::fast_PWM_top_OCRA, 
						    "fast_PWM_mode_top_OCRA");

    Timer::fast_PWM_mode_top_0xFF();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::fast_PWM_top_0xFF, 
						    "fast_PWM_mode_top_0xFF");

    Timer::PWM_phase_correct_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::PWM_phase_correct_top_OCRA, 
						    "PWM_phase_correct_mode_top_OCRA");

    Timer::PWM_phase_correct_mode_top_0xFF();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::PWM_phase_correct_top_0xFF, 
						    "PWM_phase_correct_mode_top_0xFF");

// prescalers
    Timer::off();
    CHECK_TRUE(test, Timer::prescaler() == 0, "off");

    Timer::clock_frequency_no_preescaling();
    CHECK_TRUE(test, Timer::prescaler() == 1, 
					    "clock_frequency_no_preescaling");

    Timer::clock_frequency_divide_by_8();
    CHECK_TRUE(test, Timer::prescaler() == 8, "clock_frequency_divide_by_8");

    Timer::clock_frequency_divide_by_64();
    CHECK_TRUE(test, Timer::prescaler() == 64, "clock_frequency_divide_by_64");

    Timer::clock_frequency_divide_by_256();
    CHECK_TRUE(test, Timer::prescaler() == 256, "clock_frequency_divide_by_256");

    Timer::clock_frequency_divide_by_1024();
    CHECK_TRUE(test, Timer::prescaler() == 1024, "clock_frequency_divide_by_1024");


    // Garantizo apagarlo
    Timer::off();
}

void test_timer1()
{
    myu::Disable_interrupts lock; // para poder usar las funciones unsafe

    myu::UART_iostream uart;
    auto test = Test::interface(uart, "Timer0");

    using Timer = myu::Timer1;

    Timer::off();

// counter
    Timer::unsafe_counter(25u);
    CHECK_TRUE(test, Timer::unsafe_counter() == 25u, "counter");

// modes
    Timer::normal_mode();
    CHECK_TRUE(test, Timer::mode() == Timer::Mode::normal, "normal_mode");

    Timer::CTC_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::CTC_top_OCRA, 
		    "CTC_mode_top_OCRA");

    Timer::CTC_mode_top_ICR();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::CTC_top_ICR, 
		    "CTC_mode_top_ICR");

    Timer::fast_PWM_mode_top_0x00FF();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::fast_PWM_top_0x00FF, 
		    "fast_PWM_mode_top_0x00FF");

    Timer::fast_PWM_mode_top_0x01FF();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::fast_PWM_top_0x01FF, 
		    "fast_PWM_mode_top_0x01FF");

    Timer::fast_PWM_mode_top_0x03FF();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::fast_PWM_top_0x03FF, 
		    "fast_PWM_mode_top_0x03FF");

    Timer::fast_PWM_mode_top_ICR();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::fast_PWM_top_ICR, 
		    "fast_PWM_mode_top_ICR");

    Timer::fast_PWM_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::fast_PWM_top_OCRA, 
		    "fast_PWM_mode_top_OCRA");

    Timer::PWM_phase_correct_mode_top_0x00FF();
    CHECK_TRUE(test, Timer::mode() == 
		    Timer::Mode::PWM_phase_correct_top_0x00FF, 
		    "PWM_phase_correct_mode_top_0x00FF");

    Timer::PWM_phase_correct_mode_top_0x01FF();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_correct_top_0x01FF, 
		"PWM_phase_correct_mode_top_0x01FF");

    Timer::PWM_phase_correct_mode_top_0x03FF();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_correct_top_0x03FF, 
		"PWM_phase_correct_mode_top_0x03FF");

    Timer::PWM_phase_correct_mode_top_ICR();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_correct_top_ICR, 
		"PWM_phase_correct_mode_top_ICR");

    Timer::PWM_phase_correct_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_correct_top_OCRA, 
		"PWM_phase_correct_mode_top_OCRA");

    Timer::PWM_phase_and_frequency_correct_mode_top_ICR();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_and_frequency_correct_top_ICR,
		"PWM_phase_and_frequency_correct_mode_top_ICR");

    Timer::PWM_phase_and_frequency_correct_mode_top_OCRA();
    CHECK_TRUE(test, Timer::mode() == 
		Timer::Mode::PWM_phase_and_frequency_correct_top_OCRA, 
		"PWM_phase_and_frequency_correct_mode_top_OCRA");

// prescalers
    Timer::off();
    CHECK_TRUE(test, Timer::prescaler() == 0, "off");

    Timer::clock_frequency_no_preescaling();
    CHECK_TRUE(test, Timer::prescaler() == 1, 
					    "clock_frequency_no_preescaling");

    Timer::clock_frequency_divide_by_8();
    CHECK_TRUE(test, Timer::prescaler() == 8, "clock_frequency_divide_by_8");

    Timer::clock_frequency_divide_by_64();
    CHECK_TRUE(test, Timer::prescaler() == 64, "clock_frequency_divide_by_64");

    Timer::clock_frequency_divide_by_256();
    CHECK_TRUE(test, Timer::prescaler() == 256, "clock_frequency_divide_by_256");

    Timer::clock_frequency_divide_by_1024();
    CHECK_TRUE(test, Timer::prescaler() == 1024, "clock_frequency_divide_by_1024");


    // Garantizo apagarlo
    Timer::off();
}

void test_pin()
{
    myu::UART_iostream uart;
    auto test = Test::interface(uart, "Pin<14>");

    using Pin = myu::Pin<14>;

    Pin::as_output();

    Pin::write_zero();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == true, "write_zero");
    CHECK_TRUE(test, Pin::is_one() == false, "write_zero");
    CHECK_TRUE(test, Pin::read() == 0, "write_zero");

    Pin::write_one();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == false, "write_one");
    CHECK_TRUE(test, Pin::is_one() == true, "write_one");
    CHECK_TRUE(test, Pin::read() != 0, "write_zero");

    Pin::toggle();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == true, "toggle");
    CHECK_TRUE(test, Pin::is_one() == false, "toggle");
    CHECK_TRUE(test, Pin::read() == 0, "toggle");

    // TODO: ¿cómo probar los pines de entrada?
    // Basta con conectar un pin con otro, uno lo definimos como salida y el
    // otro de entrada.
    // IDEA (???) Conectar el micro de alguna determinada manera fija (siempre
    // la misma) para intentar probar lo máximo de forma automática.
    //
}

int main()
{
    init_uart();
    hello();

    while(1){
	test_pin();
	press_key_to_continue();

	test_timer0();
	press_key_to_continue();

	test_timer1();
	press_key_to_continue();
    }
}


