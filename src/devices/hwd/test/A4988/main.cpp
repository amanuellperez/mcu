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
using UART_iostream = mcu::UART_iostream<myu::UART>;

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
//static constexpr uint8_t NO_ENABLE= mcu::Pin_connection_type::floating;
static constexpr uint8_t NO_ENABLE= 9;

//static constexpr uint8_t MS1= 10;
//static constexpr uint8_t MS2= 11;
//static constexpr uint8_t MS3= 12;

static constexpr uint8_t MS1= mcu::Pin_connection_type::floating;
static constexpr uint8_t MS2= mcu::Pin_connection_type::floating;
static constexpr uint8_t MS3= mcu::Pin_connection_type::floating;

//static constexpr uint8_t MS1= mcu::Pin_connection_type::to_VCC;
//static constexpr uint8_t MS1= mcu::Pin_connection_type::to_GND;

//static constexpr uint8_t MS2= mcu::Pin_connection_type::to_VCC;
//static constexpr uint8_t MS2= mcu::Pin_connection_type::to_GND;

//static constexpr uint8_t MS3= mcu::Pin_connection_type::to_VCC;
//static constexpr uint8_t MS3= mcu::Pin_connection_type::to_GND;

// En lugar de dejarlos floating conectar no_reset con no_sleep::
static constexpr uint8_t NO_RESET= mcu::Pin_connection_type::floating;
static constexpr uint8_t NO_SLEEP= mcu::Pin_connection_type::floating;
//static constexpr uint8_t NO_RESET= 13;
//static constexpr uint8_t NO_SLEEP= 14;

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
    UART_iostream uart;
    myu::UART_basic_cfg<baud_rate>();
    uart.turn_on();
}

template <uint8_t npin>
void print_pin_number(const char* name)
{
    UART_iostream uart;

    uart << name << " = ";

    if (Micro::Pin<npin>::is_a_valid_pin())
	uart << (int) npin;

    else if (npin == mcu::Pin_connection_type::floating)
	uart << "floating";

    else if (npin == mcu::Pin_connection_type::to_VCC)
	uart << "to VCC";

    else if (npin == mcu::Pin_connection_type::to_GND)
	uart << "to GND";

    uart << "; ";
}


void hello()
{
    UART_iostream uart;
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
    UART_iostream uart;
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
    UART_iostream uart;
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
template <typename Driver>
void print_menu_enable()
{ }

template <typename Driver>
void print_menu_enable()
    requires requires {Driver::enable(); }
{ 
    UART_iostream uart;
    uart << "4. enable/disable\n";
}

template <typename Driver>
void test_enable()
//    requires (!Pin<no_enable_pin>::is_a_valid_pin())
{ }

template <typename Driver>
void test_enable()
    requires requires {Driver::enable(); }
{
    UART_iostream uart;
    uart << "\nEnable/disable (e/d): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': Driver::enable();

	break; case 'd':
	       case 'D': Driver::disable();

	break; default: uart << "What?\n";
    }
}


// no sleep
// --------
template <typename Driver>
void print_menu_sleep()
{}

template <typename Driver>
void print_menu_sleep()
    requires requires {Driver::sleep(); Driver::awake();}
{ 
    UART_iostream uart;
    uart << "5. sleep/awake\n";
}

template <typename Driver>
void test_sleep()
{ }

template <typename Driver>
void test_sleep()
    requires requires {Driver::sleep(); Driver::awake();}
{
    UART_iostream uart;
    uart << "\nSleep/awake (s/a): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 's':
	       case 'S': Driver::sleep();

	break; case 'a':
	       case 'A': Driver::awake();

	break; default: uart << "What?\n";
    }
}

// no reset
// --------
template <typename Driver>
void print_menu_reset()
{}

template <typename Driver>
    requires requires {Driver::engage(); Driver::disengage();}
void print_menu_reset()
{ 
    UART_iostream uart;
    uart << "6. engage/disengage\n";
}

template <typename Driver>
void test_reset()
{ }

template <typename Driver>
void test_reset()
    requires requires {Driver::engage(); Driver::disengage();}
{
    UART_iostream uart;
    uart << "\nEngage/disengage (e/d): ";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': Driver::engage();

	break; case 'd':
	       case 'D': Driver::disengage();

	break; default: uart << "What?\n";
    }
}

// mode
// ----
template <typename Driver>
void print_menu_mode()
{ }

template <typename Driver>
void print_menu_mode()
    requires (Driver::MS123_connected())
{ 
    UART_iostream uart;
    uart << "7. Write mode\n";
}

template <typename Driver>
void test_mode_automatic_check()
{} 

template <typename Driver>
void test_mode_automatic_check()
    requires (Driver::MS123_connected())
{
    UART_iostream uart;
    auto test = Test::interface(uart, "mode()");

    Driver::full_step_mode();
    CHECK_TRUE(test, Driver::mode() == Driver::Mode::full_step, "full_step");

    Driver::half_step_mode();
    CHECK_TRUE(test, Driver::mode() == Driver::Mode::half_step, "half_step");

    Driver::quarter_step_mode();
    CHECK_TRUE(test, Driver::mode() == Driver::Mode::quarter_step
							    , "quarter_step");

    Driver::eighth_step_mode();
    CHECK_TRUE(test, Driver::mode() == Driver::Mode::eighth_step, "eighth_step");

    Driver::sixteenth_step_mode();
    CHECK_TRUE(test, Driver::mode() == Driver::Mode::sixteenth_step
							    , "sixteenth_step");

    Driver::full_step_mode(); // lo dejamos en full step mode
}


template <typename Driver>
void test_mode()
{
    UART_iostream uart;
    uart << "\nMode\n"
	      "----\n"
	      "0. Automatic check\n";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case '0': test_mode_automatic_check<Driver>();
	break; default: uart << "What?\n";
    }
}


template <typename Driver>
void test_mode()
    requires (Driver::MS123_connected())
{
    UART_iostream uart;
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
	break; case '0': test_mode_automatic_check<Driver>();
	break; case '1': Driver::full_step_mode();
	break; case '2': Driver::half_step_mode();
	break; case '3': Driver::quarter_step_mode();
	break; case '4': Driver::eighth_step_mode();
	break; case '5': Driver::sixteenth_step_mode();

	break; default: uart << "What?\n";
    }
}

void test_read_mode()
{
    using Mode = A4988::Mode;

    UART_iostream uart;
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
    UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Step\n"
		  "2. Direction\n"
		  "3. Read mode\n";

	print_menu_enable<A4988>();
	print_menu_sleep<A4988>();
	print_menu_reset<A4988>();
	print_menu_mode<A4988>();

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': test_step();
	    break; case '2': test_direction();
	    break; case '3': test_read_mode();
	    break; case '4': test_enable<A4988>();
	    break; case '5': test_sleep<A4988>();
	    break; case '6': test_reset<A4988>();
	    break; case '7': test_mode<A4988>();

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    A4988_pins::STEP::handle_interrupt();
}
