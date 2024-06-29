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

#include "../../dev_A4988.h"
#include <avr_atmega.h>

#include <atd_magnitude.h>
#include <atd_test.h>

constexpr const char* msg_hello = "\n\n"
		"*************************************************************\n"
		"                        A4988 TEST\n"
		"*************************************************************\n"
		"                        IMPORTANT\n"
		" Don't forget to connect an electrolytic capacitor of 100 uF\n"
		" (minimum 47 uF) between VMOT and GND!!!\n"
		"*************************************************************\n";

using namespace test;

// Microcontroller
// ---------------
// myu = my_mcu (???) <-- today, I like myu! Tomorrow... :?
namespace myu = atmega;
using Micro   = myu::Micro;

template <uint8_t n>
using Pin = typename Micro::Pin<n>;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
struct A4988_pins{
//     ----------
// Aunque no es necesario indicar los floatings pins, a la hora
// de mantener el programa es mucho más sencillo ya que queda documentado
// en código cómo se conectan los pins
// INSTRUCCIONES
//	Probar a conectar los pines al micro o dejarlos flotantes, el programa
//  cambiará completamente.
//static constexpr uint8_t NO_ENABLE= mcu::Pin_connection::floating;
static constexpr uint8_t NO_ENABLE= 9;

static constexpr uint8_t MS1= 10;
static constexpr uint8_t MS2= 11;
static constexpr uint8_t MS3= 12;
//static constexpr uint8_t MS1= mcu::Pin_connection::floating;
//static constexpr uint8_t MS1= mcu::Pin_connection::to_VCC;
//static constexpr uint8_t MS1= mcu::Pin_connection::to_GND;

//static constexpr uint8_t MS2= mcu::Pin_connection::floating;
//static constexpr uint8_t MS2= mcu::Pin_connection::to_VCC;
//static constexpr uint8_t MS2= mcu::Pin_connection::to_GND;

//static constexpr uint8_t MS3= mcu::Pin_connection::floating;
//static constexpr uint8_t MS3= mcu::Pin_connection::to_VCC;
//static constexpr uint8_t MS3= mcu::Pin_connection::to_GND;

// En lugar de dejarlos floating conectar no_reset con no_sleep
//static constexpr uint8_t NO_RESET= 13;
static constexpr uint8_t NO_RESET= mcu::Pin_connection::floating;
//static constexpr uint8_t NO_SLEEP= 14;
static constexpr uint8_t NO_SLEEP= mcu::Pin_connection::floating;

static constexpr uint8_t DIR     = 15;
static constexpr uint8_t STEP_pin= 16;

using STEP = myu::SWG1_pin<STEP_pin>;
};

// Devices
// -------
using A4988 = dev::A4988_basic<Micro, A4988_pins>;


// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

template <uint8_t npin>
void print_pin_number(const char* name)
{
    myu::UART_iostream uart;

    uart << name << " = ";

    if (Micro::Pin<npin>::is_a_valid_pin())
	uart << (int) npin;

    else if (npin == mcu::Pin_connection::floating)
	uart << "floating";

    else if (npin == mcu::Pin_connection::to_VCC)
	uart << "to VCC";

    else if (npin == mcu::Pin_connection::to_GND)
	uart << "to GND";

    uart << "; ";
}


void hello()
{
    myu::UART_iostream uart;
    uart << msg_hello <<
		"Connections:\n"
		"\tDIR = " << (int) A4988_pins::DIR << 
		"; STEP = " << (int) A4988_pins::STEP_pin;


    uart << "\n\t";
    print_pin_number<A4988_pins::NO_ENABLE>("NO ENABLE");
    print_pin_number<A4988_pins::NO_SLEEP>("NO SLEEP");
    print_pin_number<A4988_pins::NO_RESET>("NO RESET");

    uart << "\n\t";
    print_pin_number<A4988_pins::MS1>("MS1");
    print_pin_number<A4988_pins::MS2>("MS2");
    print_pin_number<A4988_pins::MS3>("MS3");
    uart << "\n\n";
}

void test_step()
{
    myu::UART_iostream uart;
    uart << "\nFrequency (in Hz): ";
    uint32_t freq;
    uart >> freq;

    if (freq == 0)
	return;

    uart << "Number of steps: ";
    A4988::NSteps_t nsteps;
    uart >> nsteps;

    A4988::step(atd::Hertz<uint32_t>{freq}, nsteps);
}


void test_direction()
{
    myu::UART_iostream uart;
    uart << "\nDirection (+/-): ";
    char d{};
    uart >> d;

    auto dir = A4988::Direction::positive;

    if (d == '-')
	dir = A4988::Direction::negative;

    A4988::direction(dir);
}

// no enable
// ---------
template <uint8_t no_enable_pin>
void print_menu_enable()
    requires (!Pin<no_enable_pin>::is_a_valid_pin())
{ }

template <uint8_t no_enable_pin>
void print_menu_enable()
    requires (Pin<no_enable_pin>::is_a_valid_pin())
{ 
    myu::UART_iostream uart;
    uart << "4. enable/disable\n";
}

template <uint8_t no_enable_pin>
void test_enable()
    requires (!Pin<no_enable_pin>::is_a_valid_pin())
{ }

template <uint8_t no_enable_pin>
void test_enable()
    requires (Pin<no_enable_pin>::is_a_valid_pin())
{
    myu::UART_iostream uart;
    uart << "\nEnable/disable (e/d): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': A4988::enable();

	break; case 'd':
	       case 'D': A4988::disable();

	break; default: uart << "What?\n";
    }
}


// no sleep
// --------
template <uint8_t no_sleep_pin>
void print_menu_sleep()
{ 
    if constexpr (Pin<no_sleep_pin>::is_a_valid_pin()){
	myu::UART_iostream uart;
	uart << "5. sleep/awake\n";
    }
}

template <uint8_t no_sleep_pin>
void test_sleep()
    requires (!Pin<no_sleep_pin>::is_a_valid_pin())
{ }

template <uint8_t no_sleep_pin>
void test_sleep()
    requires (Pin<no_sleep_pin>::is_a_valid_pin())
{
    myu::UART_iostream uart;
    uart << "\nSleep/awake (s/a): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 's':
	       case 'S': A4988::sleep();

	break; case 'a':
	       case 'A': A4988::awake();

	break; default: uart << "What?\n";
    }
}

// no reset
// --------
template <uint8_t no_reset_pin>
void print_menu_reset()
{ 
    if constexpr (Pin<no_reset_pin>::is_a_valid_pin()){
	myu::UART_iostream uart;
	uart << "6. engage/disengage\n";
    }
}

template <uint8_t no_reset_pin>
void test_reset()
    requires (!Pin<no_reset_pin>::is_a_valid_pin())
{ }

template <uint8_t no_reset_pin>
void test_reset()
    requires (Pin<no_reset_pin>::is_a_valid_pin())
{
    myu::UART_iostream uart;
    uart << "\nEngage/disengage (e/d): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': A4988::engage();

	break; case 'd':
	       case 'D': A4988::disengage();

	break; default: uart << "What?\n";
    }
}

// mode
// ----
template <uint8_t ms1, uint8_t ms2, uint8_t ms3>
void print_menu_mode()
{ 
    if constexpr (Pin<ms1>::is_a_valid_pin() and
		  Pin<ms2>::is_a_valid_pin() and
		  Pin<ms3>::is_a_valid_pin()){
	myu::UART_iostream uart;
	uart << "7. Write mode\n";
    }
}

template <uint8_t ms1, uint8_t ms2, uint8_t ms3>
void test_mode()
    requires (!(Pin<ms1>::is_a_valid_pin() and
	      Pin<ms2>::is_a_valid_pin() and
	      Pin<ms3>::is_a_valid_pin()))
{ }

template <uint8_t ms1, uint8_t ms2, uint8_t ms3>
void test_mode_automatic_check()
    requires (Pin<ms1>::is_a_valid_pin() and
	      Pin<ms2>::is_a_valid_pin() and
	      Pin<ms3>::is_a_valid_pin())
{
    myu::UART_iostream uart;
    auto test = Test::interface(uart, "mode()");

    A4988::full_step_mode();
    CHECK_TRUE(test, A4988::mode() == A4988::Mode::full_step, "full_step");

    A4988::half_step_mode();
    CHECK_TRUE(test, A4988::mode() == A4988::Mode::half_step, "half_step");

    A4988::quarter_step_mode();
    CHECK_TRUE(test, A4988::mode() == A4988::Mode::quarter_step
							    , "quarter_step");

    A4988::eighth_step_mode();
    CHECK_TRUE(test, A4988::mode() == A4988::Mode::eighth_step, "eighth_step");

    A4988::sixteenth_step_mode();
    CHECK_TRUE(test, A4988::mode() == A4988::Mode::sixteenth_step
							    , "sixteenth_step");
}



template <uint8_t ms1, uint8_t ms2, uint8_t ms3>
void test_mode()
    requires (Pin<ms1>::is_a_valid_pin() and
	      Pin<ms2>::is_a_valid_pin() and
	      Pin<ms3>::is_a_valid_pin())
{
    myu::UART_iostream uart;
    uart << "\nMode\n"
	      "----\n"
	      "0. Automatic check\n"
	      "1. Full mode\n"
	      "2. Half mode\n"
	      "3. Quarter mode\n"
	      "4. Eighth mode\n"
	      "5. Sixteenth mode\n";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case '0': test_mode_automatic_check<ms1, ms2, ms3>();
	break; case '1': A4988::full_step_mode();
	break; case '2': A4988::half_step_mode();
	break; case '3': A4988::quarter_step_mode();
	break; case '4': A4988::eighth_step_mode();
	break; case '5': A4988::sixteenth_step_mode();

	break; default: uart << "What?\n";
    }
}

void test_read_mode()
{
    using Mode = A4988::Mode;

    myu::UART_iostream uart;
    uart << "\n\n";
    switch (A4988::mode()){
	break; case Mode::full_step: uart << "Mode full_step\n";
	break; case Mode::half_step: uart << "Mode half_step\n";
	break; case Mode::quarter_step: uart << "Mode quarter_step\n";
	break; case Mode::eighth_step: uart << "Mode eighth_step\n";
	break; case Mode::sixteenth_step: uart << "Mode sixteenth_step\n";
	break; default: uart << "Unknown mode\n";
    }
}



// Main
// ----
int main() 
{
    init_uart();
    A4988::init();

    hello();

    Micro::enable_interrupts();
    myu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Step\n"
		  "2. Direction\n"
		  "3. Read mode\n";

	print_menu_enable<A4988_pins::NO_ENABLE>();
	print_menu_sleep<A4988_pins::NO_SLEEP>();
	print_menu_reset<A4988_pins::NO_RESET>();
	print_menu_mode<A4988_pins::MS1, A4988_pins::MS2, 
				         A4988_pins::MS3>();


	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': test_step();
	    break; case '2': test_direction();
	    break; case '3': test_read_mode();
	    break; case '4': test_enable<A4988_pins::NO_ENABLE>();
	    break; case '5': test_sleep<A4988_pins::NO_SLEEP>();
	    break; case '6': test_reset<A4988_pins::NO_RESET>();
	    break; case '7': test_mode<A4988_pins::MS1,
		                       A4988_pins::MS2, 
				       A4988_pins::MS3>();

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    A4988_pins::STEP::handle_interrupt();
}
