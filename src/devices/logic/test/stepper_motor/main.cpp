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



#include <atd_magnitude.h>
#include <avr_atmega.h>
#include <dev_A4988.h>

#include <atd_test.h>


constexpr const char* msg_hello = "\n\n"
	    "*************************************************************\n"
	    "                    STEPPER MOTOR TEST\n"
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

#include "../../dev_stepper_motor.h"
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

static constexpr uint8_t MS1= 10;
static constexpr uint8_t MS2= 11;
static constexpr uint8_t MS3= 12;
//static constexpr uint8_t MS1= mcu::Pin_connection_type::floating;
//static constexpr uint8_t MS2= mcu::Pin_connection_type::floating;
//static constexpr uint8_t MS3= mcu::Pin_connection_type::floating;

// En lugar de dejarlos floating conectar no_reset con no_sleep
//static constexpr uint8_t NO_RESET= mcu::Pin_connection_type::floating;
//static constexpr uint8_t NO_SLEEP= mcu::Pin_connection_type::floating;
static constexpr uint8_t NO_RESET= 13;
static constexpr uint8_t NO_SLEEP= 14;

static constexpr uint8_t DIR     = 15;
static constexpr uint8_t STEP_pin= 16;

using STEP = myu::SWG1_pin<STEP_pin>;
};

// Devices
// -------
using A4988 = dev::A4988_basic<Micro, A4988_pins>;

using Motor = dev::NEMA17<A4988>;
//static_assert(Motor::step_angle == Motor::Millidegree{1'800});

// Types
using Degree = Motor::Degree;
using Degrees_per_second = Motor::Degrees_per_second;
using Speed  = Degrees_per_second;

using Hertz  = Motor::Hertz;
using Frequency= Hertz;
using NSteps_t= Motor::NSteps_t;


// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg<baud_rate>();
    uart.turn_on();
}

void press_key_to_continue()
{
    UART_iostream uart;
    uart << "Press a key to continue\n";

    char c{};
    uart >> c;
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
    uart << msg_hello;
    uart << "Connections:\n"
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
    uart << "Motor: \n";
    uart << "\tstep angle = " << Motor::step_angle() << '\n';
}

void test_turn()
{
    UART_iostream uart;
    uart << "\nNumber of degrees to turn: ";
    int16_t degree{};
    uart >> degree;

    if (degree == 0)
	return;

    uart << "Angular speed (in degrees/second): ";
    int16_t speed{};
    uart >> speed;
    if (speed == 0)
	return;

    Motor::turn(Degree{degree}, Degrees_per_second(speed));
}

void test_angle2freq(Test& test, const Degree& degree, const Speed& speed,
		const NSteps_t& res_nsteps, const Frequency& res_freq)
{
    auto [freq, nsteps] = Motor::angle2freq(degree, speed);
    CHECK_TRUE(test,
		nsteps == res_nsteps and freq == res_freq, "angle2freq");

//    UART_iostream uart;
//    uart << "nsteps = " << nsteps << " ?= " << res_nsteps << '\n';
//    uart << "freq   = " << freq << " ?= " << res_freq << '\n';
}

void test_angle2direction(Test& test, const Degree& degree, const Speed& speed,
			Motor::Direction res)
{
    CHECK_TRUE(test,
		Motor::angle2direction(degree, speed) == res,
		"angle2direction");
}

void test_automatic()
{
    // Los overflows y los casting dan dolor de cabeza. Hagamos algunas
    // comprobaciones básicas:
    
    UART_iostream uart;

    auto test = Test::interface(uart, "angle2direction");

    test_angle2direction(test, Degree{+100}, Degrees_per_second{+100} , 
						    Motor::Direction::positive);
    test_angle2direction(test, Degree{+100}, Degrees_per_second{-100} , 
						    Motor::Direction::negative);
    test_angle2direction(test, Degree{-100}, Degrees_per_second{100} , 
						    Motor::Direction::negative);
    test_angle2direction(test, Degree{-100}, Degrees_per_second{-100} , 
						    Motor::Direction::positive);

    test.interface("angle2freq");


// +angle, +speed
    test_angle2freq(test, Degree{360}, Speed{360}, NSteps_t{200}, Hertz{200});
    test_angle2freq(test, Degree{2*360}, Speed{2*360}, NSteps_t{400}, Hertz{400});
    test_angle2freq(test, Degree{3*360}, Speed{3*360}, NSteps_t{600}, Hertz{600});

    test_angle2freq(test, Degree{360}, Speed{720}, NSteps_t{200}, Hertz{400});
    test_angle2freq(test, Degree{360}, Speed{1080}, NSteps_t{200}, Hertz{600});

    // 10 vueltas
    test_angle2freq(test, Degree{3'600}, Speed{1'080}, NSteps_t{2'000}, Hertz{600});

// +angle, -speed
    test_angle2freq(test, Degree{+360}, Speed{-360}, NSteps_t{200}, Hertz{200});

// -angle, +speed
    test_angle2freq(test, Degree{-360}, Speed{360}, NSteps_t{200}, Hertz{200});

// -angle, -speed
    test_angle2freq(test, Degree{-360}, Speed{-360}, NSteps_t{200}, Hertz{200});


// step_angle
    if constexpr (requires {Motor::Driver::full_step_mode();}){
	press_key_to_continue();

	test.interface("step_angle");
	int16_t nsteps = 200;
	Degree angle{Degree::Rep{1,8}}; // if constexpr Rep == Float16 ...

	Motor::full_step_mode();
	CHECK_TRUE(test, Motor::nsteps_per_turn() == nsteps, "full_step_mode");
	CHECK_TRUE(test, Motor::step_angle() == angle, "full_step_mode");

	Motor::half_step_mode();
	CHECK_TRUE(test, Motor::nsteps_per_turn() == 2*nsteps, "half_step_mode");
	CHECK_TRUE(test, Motor::step_angle() == angle/2, "half_step_mode");

	Motor::quarter_step_mode();
	CHECK_TRUE(test, Motor::nsteps_per_turn() == 4*nsteps, "quarter_step_mode");
	CHECK_TRUE(test, Motor::step_angle() == angle/4, "quarter_step_mode");

	Motor::eighth_step_mode();
	CHECK_TRUE(test, Motor::nsteps_per_turn() == 8*nsteps, "eighth_step_mode");
	CHECK_TRUE(test, Motor::step_angle() == angle / 8, "eighth_step_mode");

	Motor::sixteenth_step_mode();
	CHECK_TRUE(test, Motor::nsteps_per_turn() == 16*nsteps, "sixteenth_step_mode");
	CHECK_TRUE(test, Motor::step_angle() == angle / 16, "sixteenth_step_mode");


	Motor::full_step_mode(); // lo dejamos en full step mode
    }

}


void print_mode()
{
    using Mode = Motor::Mode;

    UART_iostream uart;
    uart << "\n\n";
    switch (Motor::mode()){
	break; case Mode::full_step: uart << "Mode full_step\n";
	break; case Mode::half_step: uart << "Mode half_step\n";
	break; case Mode::quarter_step: uart << "Mode quarter_step\n";
	break; case Mode::eighth_step: uart << "Mode eighth_step\n";
	break; case Mode::sixteenth_step: uart << "Mode sixteenth_step\n";
	break; default: uart << "Unknown mode\n";
    }
}
template <typename Motor>
void test_mode()
{
    UART_iostream uart;
    uart << "\nMode\n"
	      "----\n"
	      "0. Read mode\n";

    if constexpr (requires {Motor::Driver::full_step_mode();}){
	uart << "1. Full mode\n"
	      "2. Half mode\n"
	      "3. Quarter mode\n"
	      "4. Eighth mode\n"
	      "5. Sixteenth mode\n";
    }
	      
    char opt{};
    uart >> opt;

    if (opt == '0'){
	print_mode();
	return;
    }

    if constexpr (requires {Motor::Driver::full_step_mode();}){
	switch (opt){
	    break; case '1': Motor::full_step_mode();
	    break; case '2': Motor::half_step_mode();
	    break; case '3': Motor::quarter_step_mode();
	    break; case '4': Motor::eighth_step_mode();
	    break; case '5': Motor::sixteenth_step_mode();

	    break; default: uart << "What?\n";
	}
    }
}



template <typename Motor>
void test_enable()
{
    UART_iostream uart;
    uart << "\nTo enable/disable you have to connect pin NO_ENABLE\n";
}

template <typename Motor>
    requires requires {Motor::Driver::enable();}
void test_enable()
{
    UART_iostream uart;
    uart << "\nenable/disable (e/d): ";
    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': Motor::enable();

	break; case 'd':
	       case 'D': Motor::disable();

	break; default: uart << "What?\n";
    }
}

template <typename Motor>
void test_sleep()
{
    UART_iostream uart;
    uart << "\nTo sleep/awake you have to connect pin NO_SLEEP\n";
}

template <typename Motor>
    requires requires {Motor::Driver::sleep();}
void test_sleep()
{
    UART_iostream uart;
    uart << "\nsleep/awake (s/a): ";
    char opt{};
    uart >> opt;

    switch (opt){
	break; case 's':
	       case 'S': Motor::sleep();

	break; case 'a':
	       case 'A': Motor::awake();

	break; default: uart << "What?\n";
    }
}

template <typename Motor>
void test_engage()
{
    UART_iostream uart;
    uart << "\nTo engage/disengage you have to connect pin NO_RESET\n";
}

template <typename Motor>
    requires requires {Motor::Driver::engage();}
void test_engage()
{
    UART_iostream uart;
    uart << "\nengage/disengage (e/d): ";
    char opt{};
    uart >> opt;

    switch (opt){
	break; case 'e':
	       case 'E': Motor::engage();

	break; case 'd':
	       case 'D': Motor::disengage();

	break; default: uart << "What?\n";
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
		  "0. Automatic test\n"
		  "1. Turn\n"
		  "2. Mode\n"
		  "3. Enable/disable\n"
		  "4. Sleep/awake\n"
		  "5. Engage/disengage\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '0': test_automatic();
	    break; case '1': test_turn();
	    break; case '2': test_mode<Motor>();
	    break; case '3': test_enable<Motor>();
	    break; case '4': test_sleep<Motor>();
	    break; case '5': test_engage<Motor>();
		    

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    A4988_pins::STEP::handle_interrupt();
}
