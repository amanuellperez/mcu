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
#include <mega.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

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

using PWM_pinA = myu::driver::PWM1_pin<ENA_pin>;
using PWM_pinB = myu::driver::PWM1_pin<ENB_pin>;

// Devices
// -------
struct L298N_cfg : dev::default_cfg::L298N {
    using Micro    = ::Micro;
    using PWM_pinA = ::PWM_pinA;
    static constexpr uint8_t IN1 = IN1_pin;
    static constexpr uint8_t IN2 = IN2_pin;

    using PWM_pinB = ::PWM_pinB;
    static constexpr uint8_t IN3 = IN3_pin;
    static constexpr uint8_t IN4 = IN4_pin;
};


using L298N = dev::hwd::L298N<L298N_cfg>;

template <uint8_t n>
using L298N_H_bridge = dev::L298N_H_bridge<L298N, n>;

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void hello()
{
    UART_iostream uart;
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

atd::Sign ask_sign(std::iostream& out)
{
    out << "sign (+/-): ";
    char sign{'+'};
    out >> sign;

    out << '\n';

    if (sign == '-')
	return atd::Sign::negative;

    return atd::Sign::positive;
}


void port1_voltage()
{
    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;

    auto sign = ask_sign(uart);

    auto p = atd::Percentage{static_cast<uint8_t>(per)};

    L298N::voltage1(sign, p);

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage1_percentage()),
						    "voltage1_percentage()");
    CHECK_TRUE(test, L298N::voltage1_sign() == sign, "voltage1_sign()");
}


void port1_percentage()
{
    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;

    auto p = atd::Percentage{static_cast<uint8_t>(per)};
    L298N::voltage1_percentage(p);


    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage1_percentage()),
						    "voltage1_percentage()");
}



void port1_sign()
{
    UART_iostream uart;
    auto sign = ask_sign(uart);

    L298N::voltage1_sign(sign);

    Micro::wait_ms(1); // para que de tiempo a los pins a cambiar
    Test test{uart};
    CHECK_TRUE(test, L298N::voltage1_sign() == sign, "voltage1_sign()");
}


void test_port1()
{
    UART_iostream uart;
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
    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;
    auto p = atd::Percentage{static_cast<uint8_t>(per)};

    auto sign = ask_sign(uart);

    L298N::voltage2(sign, p);

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage2_percentage()),
						    "voltage2_percentage()");
    CHECK_TRUE(test, L298N::voltage2_sign() == sign, "voltage2_sign()");
}


void port2_percentage()
{
    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;
    auto p = atd::Percentage{static_cast<uint8_t>(per)};

    L298N::voltage2_percentage(p);

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,L298N::voltage2_percentage()),
						    "voltage2_percentage()");
}



void port2_sign()
{
    UART_iostream uart;
    atd::Sign sign = ask_sign(uart);

    L298N::voltage2_sign(sign);

    Micro::wait_ms(1); // para que de tiempo a los pins a cambiar
    Test test{uart};
    CHECK_TRUE(test, L298N::voltage2_sign() == sign, "voltage2_sign()");
}


void test_port2()
{
    UART_iostream uart;
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


template <uint8_t n>
void port_voltage()
{
    using Bridge = L298N_H_bridge<n>;

    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;
    auto p = atd::Percentage{static_cast<uint8_t>(per)};

    auto sign = ask_sign(uart);

    Bridge::voltage(sign, p);

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,Bridge::voltage_percentage()),
						    "voltage_percentage()");
    CHECK_TRUE(test, Bridge::voltage_sign() == sign, "voltage_sign()");
}


template <uint8_t n>
void port_percentage()
{
    using Bridge = L298N_H_bridge<n>;

    UART_iostream uart;
    uart << "\nPercentage: ";
    uint16_t per{0};
    uart >> per;

    if (per == 0) return;
    auto p = atd::Percentage{static_cast<uint8_t>(per)};

    Bridge::voltage_percentage(p);

    Test test{uart};
    CHECK_TRUE(test, equal_percentage(p ,Bridge::voltage_percentage()),
						    "voltage_percentage()");
}



template <uint8_t n>
void port_sign()
{
    using Bridge = L298N_H_bridge<n>;

    UART_iostream uart;
    atd::Sign sign = ask_sign(uart);

    Bridge::voltage_sign(sign);

    Micro::wait_ms(1); // para que de tiempo a los pins a cambiar
    Test test{uart};
    CHECK_TRUE(test, Bridge::voltage_sign() == sign, "voltage_sign()");
}


template <uint8_t n>
void test_port()
{
    using Bridge = L298N_H_bridge<n>;

    UART_iostream uart;
    uart << "\n\nPort " << Bridge::nport << " change:\n"
	    "\t0. stop\n"
	    "\t1. Voltage\n"
	    "\t2. Percentage\n"
	    "\t3. sign\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': Bridge::stop();
	break; case '1': port_voltage<n>();
	break; case '2': port_percentage<n>();
	break; case '3': port_sign<n>();

	break; default: uart << "What???\n";
    }

}


// Main
// ----
int main() 
{
    init_uart();

    hello();

    UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Port A (out1-out2)\n"
		  "2. Port B (out3-out4)\n"
		  "3. Test L298N_H_bridge<1>\n"
		  "4. Test L298N_H_bridge<2>\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': test_port1();
	    break; case '2': test_port2();
	    break; case '3': test_port<1>();
	    break; case '4': test_port<2>();

	    break; default: uart << "What???\n";

	}
    }
}


