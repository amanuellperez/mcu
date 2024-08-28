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

// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8

// available: pins 9-10

static constexpr uint8_t input_pin_with_pullup = 10;

// Timer0: pins 11, 12

static constexpr uint8_t output_pin = 13;
static constexpr uint8_t input_pin_without_pullup = 14;

// Timer1: 15, 16

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28
					 
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

void press_key_to_continue()
{
    myu::UART_iostream uart;
    uart << "\nPress key to continue\n";
    char key{};
    uart >> key;
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nAutomatic test\n"
	        "--------------\n"
		"Connect pin " << (int) output_pin << " to pins to "
		<< (int) input_pin_without_pullup << " and " 
		<< (int) input_pin_with_pullup << "\n"
		"Also connect pin " << (int) input_pin_without_pullup <<
		" with a resistor to ground.\n\n";

    press_key_to_continue();
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
// pin connected
    Timer::PWM_pin_A_non_inverting_mode();
    CHECK_TRUE(test, Timer::is_pin_A_disconnected() == false, "is_pin_A_disconnected");
    CHECK_TRUE(test, Timer::is_pin_A_connected() == true, "is_pin_A_connected");

    Timer::pin_A_disconnected();
    CHECK_TRUE(test, Timer::is_pin_A_disconnected() == true, "is_pin_A_disconnected");
    CHECK_TRUE(test, Timer::is_pin_A_connected() == false, "is_pin_A_connected");

    Timer::PWM_pin_B_non_inverting_mode();
    CHECK_TRUE(test, Timer::is_pin_B_disconnected() == false, "is_pin_B_disconnected");
    CHECK_TRUE(test, Timer::is_pin_B_connected() == true, "is_pin_B_connected");

    Timer::pin_B_disconnected();
    CHECK_TRUE(test, Timer::is_pin_B_disconnected() == true, "is_pin_B_disconnected");
    CHECK_TRUE(test, Timer::is_pin_B_connected() == false, "is_pin_B_connected");


// prescalers
    Timer::off();
    CHECK_TRUE(test, Timer::is_on() == false, "is_on");
    CHECK_TRUE(test, Timer::is_off() == true, "is_off");

    CHECK_TRUE(test, Timer::prescaler() == 0, "off");

    Timer::clock_frequency_no_prescaling();
    CHECK_TRUE(test, Timer::prescaler() == 1, 
					    "clock_frequency_no_prescaling");

    CHECK_TRUE(test, Timer::is_on() == true, "is_on");
    CHECK_TRUE(test, Timer::is_off() == false, "is_off");

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
    auto test = Test::interface(uart, "Timer1");

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



// pin connected
    Timer::PWM_pin_A_non_inverting_mode();
    CHECK_TRUE(test, Timer::is_pin_A_disconnected() == false, "is_pin_A_disconnected");
    CHECK_TRUE(test, Timer::is_pin_A_connected() == true, "is_pin_A_connected");

    Timer::pin_A_disconnected();
    CHECK_TRUE(test, Timer::is_pin_A_disconnected() == true, "is_pin_A_disconnected");
    CHECK_TRUE(test, Timer::is_pin_A_connected() == false, "is_pin_A_connected");

    Timer::PWM_pin_B_non_inverting_mode();
    CHECK_TRUE(test, Timer::is_pin_B_disconnected() == false, "is_pin_B_disconnected");
    CHECK_TRUE(test, Timer::is_pin_B_connected() == true, "is_pin_B_connected");

    Timer::pin_B_disconnected();
    CHECK_TRUE(test, Timer::is_pin_B_disconnected() == true, "is_pin_B_disconnected");
    CHECK_TRUE(test, Timer::is_pin_B_connected() == false, "is_pin_B_connected");

// prescalers
    Timer::off();
    CHECK_TRUE(test, Timer::is_on() == false, "is_on");
    CHECK_TRUE(test, Timer::is_off() == true, "is_off");

    CHECK_TRUE(test, Timer::prescaler() == 0, "off");

    Timer::clock_frequency_no_prescaling();
    CHECK_TRUE(test, Timer::prescaler() == 1, 
					    "clock_frequency_no_prescaling");

    CHECK_TRUE(test, Timer::is_on() == true, "is_on");
    CHECK_TRUE(test, Timer::is_off() == false, "is_off");

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

    using Pin		= myu::Pin<output_pin>;
    using Pin_with	= myu::Pin<input_pin_with_pullup>;
    using Pin_without	= myu::Pin<input_pin_without_pullup>;

    Pin_with::as_input_with_pullup();
    CHECK_TRUE(test, Pin_with::is_input_with_pullup_pin() == true, 
						"is_input_with_pullup_pin");

    Pin_without::as_input_without_pullup();
    CHECK_TRUE(test, Pin_without::is_input_without_pullup_pin() == true, 
						"is_input_without_pullup_pin");

    Pin::as_output();
    CHECK_TRUE(test, Pin::is_output_pin() == true, "is_output_pin");

    Pin::write_zero();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == true, "write_zero");
    CHECK_TRUE(test, Pin::is_one() == false, "write_zero");
    CHECK_TRUE(test, Pin::read() == 0, "write_zero");
    CHECK_TRUE(test, Pin_with::is_zero(), "write_zero");
    CHECK_TRUE(test, Pin_without::is_zero(), "write_zero");

    Pin::write_one();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == false, "write_one");
    CHECK_TRUE(test, Pin::is_one() == true, "write_one");
    CHECK_TRUE(test, Pin::read() != 0, "write_zero");
    CHECK_TRUE(test, Pin_with::is_one(), "write_one");
    CHECK_TRUE(test, Pin_without::is_one(), "write_one");

    Pin::toggle();
    myu::wait_ms(1); // tarda un poco en cambiar 
    CHECK_TRUE(test, Pin::is_zero() == true, "toggle");
    CHECK_TRUE(test, Pin::is_one() == false, "toggle");
    CHECK_TRUE(test, Pin::read() == 0, "toggle");
    CHECK_TRUE(test, Pin_with::is_zero(), "write_zero");
    CHECK_TRUE(test, Pin_without::is_zero(), "write_zero");

    // DUDA: quería dejar Pin en tri-state mode de tal manera que pudiera
    // medir el valor de los otros pines. Pero no funciona con el pin que
    // tiene la internal pull-up resistor. Why? No idea.
//    Pin::as_input_without_pullup();
//    CHECK_TRUE(test, Pin_with::is_one(), "Pin_with");
//    CHECK_TRUE(test, Pin_without::is_zero(), "Pin_with");


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


