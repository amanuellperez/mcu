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
//static constexpr uint8_t NO_ENABLE= mcu::Pin_connection::floating;
static constexpr uint8_t NO_ENABLE= 9;

//static constexpr uint8_t MS1= 10;
//static constexpr uint8_t MS2= 11;
//static constexpr uint8_t MS3= 12;
static constexpr uint8_t MS1= mcu::Pin_connection::floating;
static constexpr uint8_t MS2= mcu::Pin_connection::floating;
static constexpr uint8_t MS3= mcu::Pin_connection::floating;

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
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void press_key_to_continue()
{
    myu::UART_iostream uart;
    uart << "Press a key to continue\n";

    char c{};
    uart >> c;
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
    uart << "\tstep angle = " << Motor::step_angle << '\n';
}

void test_turn()
{
    myu::UART_iostream uart;
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

//    myu::UART_iostream uart;
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
void test_angle2freq()
{
    // Los overflows y los casting dan dolor de cabeza. Hagamos algunas
    // comprobaciones básicas:
    
    myu::UART_iostream uart;
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

//    press_key_to_continue();

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
		  "0. angle2freq test\n"
		  "1. turn\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '0': test_angle2freq();
	    break; case '1': test_turn();
		    

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    A4988_pins::STEP::handle_interrupt();
}
