// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include <avr_timer1_basic.h>
#include <avr_time.h>
#include <avr_UART_iostream.h>


using Timer = avr::Timer1;
using namespace avr::literals;


// fg = ft / (1 + top); 
// fg = frecuencia generada
// ft = frecuencia del timer = f_clock / divisor
// top= valor que ponemos en el ICR1
void select_frequency(const uint16_t freq)
{
    static_assert(MCU_CLOCK_FREQUENCY_IN_HZ == 1000000u);

    if (freq <= 15){
	uint16_t top  = 15625u / freq - 1; // 15625 = 1000000/64;
	Timer::off();
	Timer::input_capture_register(top);
	Timer::output_compare_register_A(8); // 500 us
	Timer::PWM_pin_A_non_inverting_mode();
	Timer::on<64>();
    }
    else{
        uint16_t top =
            static_cast<uint16_t>(uint32_t{1000000u} / uint32_t{freq}) - 1;
        Timer::off();
	Timer::input_capture_register(top);
	Timer::output_compare_register_A(500); // 500 us
	Timer::PWM_pin_A_non_inverting_mode();
	Timer::on<1>();
    }


}


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

// data
    uint16_t freq = 500;

    // configuración inicial, para que se vea algo en el osciloscopio
    Timer::mode_fast_PWM_top_ICR1();

    select_frequency(freq);

    uart << "\n\nStroboscope test\n"
	        "----------------\n"
            "Connect oscilloscope to pin "
         << uint16_t{Timer::OCA_pin()} << '\n';

    while(1){
	uart << "\nMenu\n"
	          "----\n"
                "1. off\n"
		"2. on\n"
		"3. frequency\n";

        char c{};
	uart >> c;
	switch(c){
	    case '1':
		Timer::off();
		break;

	    case '2':
		select_frequency(freq);
		break;

	    case '3':
		uart << "\nFrequency (in Hz): ";
		uart >> freq;
		uart << freq;
		select_frequency(freq);
		break;

	    default:
		uart << "I don't understand.\n";
		break;

	}
    }
}



