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

struct Cfg_motor{
    using Driver = A4988;
};

using Motor = dev::NEMA17<Cfg_motor>;
//static_assert(Motor::step_angle == Motor::Millidegree{1'800});

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
    uart << "\n\nStepper motor test\n"
	        "------------------\n"
		"*************************************************************\n"
		"                        IMPORTANT\n"
		" Don't forget to connect an electrolytic capacitor of 100 uF\n"
		" (minimum 47 uF) between VMOT and GND!!!\n"
		"*************************************************************\n"
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

void turn()
{
    Motor::turn(Motor::Degree{3*360}, 
				Motor::Degrees_per_second(3*360));
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
    uart << "step angle = " << Motor::step_angle << '\n';

uint32_t f = 200;
Motor::Driver::Hertz freq_gen{f};
uart << freq_gen << " (?= " << f << ")\n";

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. step\n"
		  "2. direction\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': turn();
		    

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    A4988_pins::STEP::handle_interrupt();
}
