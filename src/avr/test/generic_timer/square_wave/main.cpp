// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
#include "../../../avr_UART.h"
#include "../../../avr_timer0_generic.h"
//#include "../../../avr_timer1_generic.h"
#include "../../../avr_time.h"

using Timer = avr_::Timer0_generic;


void print_msg(const uint32_t& freq, Timer::Connect connect)
{
    avr_::UART_iostream uart;

    uart << "\nGenerating " << freq << " Hz in pin ";
    switch (connect){
	break; case Timer::Connect::only_channel1:
		uart << (int) Timer::pin_channel1;

	break; case Timer::Connect::only_channel2:
		uart << (int) Timer::pin_channel2;

	break; case Timer::Connect::channel1_and_2:
		uart << (int) Timer::pin_channel1 
		     << " and " << (int) Timer::pin_channel2;
    }

    uart << '\n';
}


int main()
{
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.on();

    uart << "\n\nSquare wave test\n"
	        "----------------\n";

    Timer::Connect connect = Timer::Connect::only_channel1;

    while(1){
	uart << "Frequency in Hz to generate (0 to choose channel): ";
	uint32_t freq;
	uart >> freq;
	if (freq > 0){
	    Timer::off();
	    Timer::square_wave_generate(freq, connect);
	    print_msg(freq, connect);
	}
	
	else{
	    uart << "\nChoose channel to generate wave:\n"
		    "1. channel one\n"
		    "2. channel two\n"
		    "b. both channels\n";

	    char c{};
	    uart >> c;
	    switch (c){
		break; case '1': connect = Timer::Connect::only_channel1;
		break; case '2': connect = Timer::Connect::only_channel2;
		break; default : connect = Timer::Connect::channel1_and_2;
	    }
	}

    }
}



