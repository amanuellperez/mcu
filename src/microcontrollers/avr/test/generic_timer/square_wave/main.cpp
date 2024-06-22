// Copyright (C) 2022 Manuel Perez 
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_UART_iostream.h"
#include "../../../avr_timer0_generic.h"
#include "../../../avr_timer1_generic.h"

namespace my_mcu = avr_;

using SWG0 = my_mcu::Square_wave_generator0_g;
using SWG1 = my_mcu::Square_wave_generator1_g;

enum class Use_timer{zero, one };

static_assert(SWG0::is_pin(11));
static_assert(SWG0::is_pin(12));
//DONT_COMPILE(static_assert(SWG0::is_pin(13)));
static_assert(SWG1::is_pin(15));
static_assert(SWG1::is_pin(16));
//DONT_COMPILE(static_assert(SWG1::is_pin(17)));

int main()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\nSquare wave test\n"
	        "----------------\n";

    uint8_t npin = 0;
    auto use_timer = Use_timer::zero;


    while(1){
    // select_pin();
	uart << "\nChoose pin to generate wave:\n"
		"[0] in pin " << (int) SWG0::pin[0] << " (Timer0)\n"
		"[1] in pin " << (int) SWG0::pin[1] << " (Timer0)\n"
		"[2] in pin " << (int) SWG1::pin[0] << " (Timer1)\n"
		"[3] in pin " << (int) SWG1::pin[1] << " (Timer1)\n";

	char c{};
	uart >> c;
	switch (c){
	    break; case '0': npin = 0; use_timer = Use_timer::zero;
	    break; case '1': npin = 1; use_timer = Use_timer::zero;
	    break; case '2': npin = 0; use_timer = Use_timer::one;
	    break; case '3': npin = 1; use_timer = Use_timer::one;
	}

    // select_frequency();
	uart << "\nFrequency in Hz to generate: ";
	uint32_t freq;
	uart >> freq;
	if (freq > 0){
	    SWG0::stop();
	    SWG1::stop();

	    switch(use_timer){
		break; case Use_timer::zero: SWG0::generate(freq, SWG0::pin[npin]);
		break; case Use_timer::one : SWG1::generate(freq, SWG1::pin[npin]);
	    }

	    uart << "\nGenerating " << freq << " Hz in pin ";
	    if (use_timer == Use_timer::zero)
		uart << (int) SWG0::pin[npin];
	    else 
		uart << (int) SWG1::pin[npin];

	    uart << '\n';

	}
	else {
	    uart << "Invalid frequency\n";
	}

    }
}



