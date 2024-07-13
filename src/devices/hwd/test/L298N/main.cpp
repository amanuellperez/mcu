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

#include "../../dev_L298N.h"
#include <avr_atmega.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
static constexpr uint8_t IN1_pin = 11;
static constexpr uint8_t IN2_pin = 12;
static constexpr uint8_t IN3_pin = 13;
static constexpr uint8_t IN4_pin = 14;

// Timer1
static constexpr uint8_t ENA_pin = 15;
static constexpr uint8_t ENB_pin = 16;

using PWM_pinA = my_mcu::PWM1_pin<ENA_pin>;
using PWM_pinB = my_mcu::PWM1_pin<ENB_pin>;

// Devices
// -------
using L298_pinA = dev::L298N_pin_cfg<PWM_pinA, IN1_pin, IN2_pin>;
using L298_pinB = dev::L298N_pin_cfg<PWM_pinB, IN3_pin, IN4_pin>;

using L298N = dev::L298N_basic<Micro, L298_pinA, L298_pinB>;


// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void hello()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nL298N test\n"
	        "----------\n"
		"Connections:\n"
		"\tIN1 = " << (int) IN1_pin << 
		"; IN2 = " << (int) IN2_pin << 
		"; IN3 = " << (int) IN3_pin << 
		"; IN4 = " << (int) IN4_pin << 
		"\n\tENA = " << (int) ENA_pin <<
		"; ENB = " << (int) ENB_pin 
		<< "\n\n";
}

// Admito un error del 2 por cien
constexpr uint8_t percentage_error = 2;

inline 
bool equal_percentage(const atd::Percentage& a, 
		      const atd::Percentage& b)
{
    if (a >= b)
	return (a.as_uint() - b.as_uint()) < percentage_error;

    else
	return (b.as_uint() - a.as_uint()) < percentage_error;
}
inline atd::Sign to_Sign(char c)
{
    if (c == '-') return atd::Sign::negative;
    return atd::Sign::positive;
}


void port1_voltage()
{
    my_mcu::UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t p{0};
    uart >> p;

    if (p == 0) return;

    uart << "sign (+/-): ";
    char sign{'+'};
    uart >> sign;
    if (sign != '-')
	sign = '+';

    L298N::voltage1(to_Sign(sign), static_cast<uint8_t>(p));

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage1_percentage()),
						    "voltage1_percentage()");
}


void port1_percentage()
{
    my_mcu::UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t p{0};
    uart >> p;

    if (p == 0) return;

    L298N::voltage1_percentage(static_cast<uint8_t>(p));

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage1_percentage()),
						    "voltage1_percentage()");
}



void port1_sign()
{
    my_mcu::UART_iostream uart;
    uart << "sign (+/-): ";
    char sign{'+'};
    uart >> sign;
    if (sign != '+' and sign != '-')
	return;

    L298N::voltage1_sign(to_Sign(sign));
}


void test_port1()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nPort 1 change:\n"
	    "\t0. stop\n"
	    "\t1. Voltage\n"
	    "\t2. Percentage\n"
	    "\t3. sign\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': L298N::stop1();
	break; case '1': port1_voltage();
	break; case '2': port1_percentage();
	break; case '3': port1_sign();

	break; default: uart << "What???\n";
    }

}


void port2_voltage()
{
    my_mcu::UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t p{0};
    uart >> p;

    if (p == 0) return;

    uart << "sign (+/-): ";
    char sign{'+'};
    uart >> sign;

    if (sign != '-')
	sign = '+';

    L298N::voltage2(to_Sign(sign), static_cast<uint8_t>(p));

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage2_percentage()),
						    "voltage2_percentage()");
}


void port2_percentage()
{
    my_mcu::UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t p{0};
    uart >> p;

    if (p == 0) return;

    L298N::voltage2_percentage(static_cast<uint8_t>(p));

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage2_percentage()),
						    "voltage2_percentage()");
}



void port2_sign()
{
    my_mcu::UART_iostream uart;
    uart << "sign (+/-): ";
    char sign{'+'};
    uart >> sign;
    if (sign != '+' and sign != '-')
	return;

    L298N::voltage2_sign(to_Sign(sign));
}


void test_port2()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nPort 2 change:\n"
	    "\t0. stop\n"
	    "\t1. Voltage\n"
	    "\t2. Percentage\n"
	    "\t3. sign\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': L298N::stop2();
	break; case '1': port2_voltage();
	break; case '2': port2_percentage();
	break; case '3': port2_sign();

	break; default: uart << "What???\n";
    }

}
// Main
// ----
int main() 
{
    init_uart();

    hello();

    my_mcu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Port A (out1-out2)\n"
		  "2. Port B (out3-out4)\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': test_port1();
	    break; case '2': test_port2();

	    break; default: uart << "What???\n";

	}
    }
}


