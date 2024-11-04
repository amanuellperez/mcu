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

#include <mega.h>

//#include <atd_test.h>
//using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

#include "../../../mcu_PWM.h"

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------

// Timer
// -----
struct Pulse_wave_cfg {
    using PWM_pin = myu::PWM1_pin<15>;
    using Micro   = myu::Micro;

    // Para 100 Hz funciona bien, pero a F_CPU = 1MHz no se puede generar
    // 800kHz.
    // ¿Funcionará a 20MHz? Con 20 MHz son 25 ticks de reloj la frecuencia de
    // 800kHz. Muy justo... :?
    //static constexpr uint32_t frequency_in_hz = 800'000; // F_CPU = ???
    static constexpr uint32_t frequency_in_hz = 100;
    static constexpr uint8_t duty_cycle_bit0 = 30;
    static constexpr uint8_t duty_cycle_bit1 = 60;
};

using PW_pin = mcu::Pulse_wave_01_pin_polling<Pulse_wave_cfg>;


// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nPulse wave test\n"
	        "---------------\n"
		"Connections:\n"
		"\tConnect oscilloscope to pin " << (int) PW_pin::number
		<< "\n\n";
}

void print_cfg(std::ostream& out)
{
    out << "\nCfg\n"
	      "---\n";
    PW_pin::print_cfg(out);
    out << '\n';
}

// Main
// ----
int main()
{
    init_uart();

    hello();

    myu::UART_iostream uart;
    if (PW_pin::init() == nm::fail){
	uart << "**** ERROR ****\n"
	        "PW_pin::init()\n"
	        "Change frequency to generate or F_CPU\n";
    }
    
    print_cfg(uart);

    while(1){
	uart << "\n\nWrite uint8_t to send: ";
	uint16_t x{};
	uart >> x;

	uint8_t data[1] = {(uint8_t) x};
	PW_pin::send(data, 1);
    }

}




