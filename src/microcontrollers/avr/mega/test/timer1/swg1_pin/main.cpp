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

#include "../../../mega_cfg.h"
#include "../../../mega_timer1.h"
#include <avr_time.h>
#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART>;

					 
// Hwd devices
// -----------
using Timer    = myu::Timer1;
// DONT_COMPILE(using SWG1_pin1 = myu::SWG1_pin<18>;)
using SWG_pin1 = myu::SWG1_pin<15>;
using SWG_pin2 = myu::SWG1_pin<16>;


// Cfg
// ---
using namespace myu::literals;


// FUNCTIONS
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}


void hello()
{
    UART_iostream uart;
    uart << "\n\nSWG1_pin test\n"
	        "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{SWG_pin1::number} 
	 << " and " << uint16_t{SWG_pin2::number}
         << '\n';
}



void generate_sw_signal_pin_A()
{
    UART_iostream uart;
    uart << "\nFrequency: ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    uart << "\nPulses number to generate (0 = don't stop): ";
    uint16_t npulses;
    uart >> npulses;

    myu::SW_signal sw{freq};

    if (npulses == 0){
	uart << "Generating " << sw.frequency() << '\n';
	SWG_pin1::generate(sw);
    }
    else{
	uart << "Generating " << npulses << " at " << sw.frequency() << '\n';
	SWG_pin1::generate(sw, npulses);
    }
}


void test_pin_A()
{
    UART_iostream uart;
    uart << "\nTest pin A\n"
	    "-------------\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate SWG signal\n"
	    "3. Stop timer\n"
	    "4. Connect pin to timer\n"
	    "5. Disconnect pin from timer\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': SWG_pin1::write_zero();
	break; case '1': SWG_pin1::write_one();
	break; case '2': generate_sw_signal_pin_A();
	break; case '3': SWG_pin1::stop();
	break; case '4': SWG_pin1::connect();
	break; case '5': SWG_pin1::disconnect();
	break; default: uart << "What?\n";
    }


}



void generate_sw_signal_pin_B()
{
    UART_iostream uart;
    uart << "\nFrequency: ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    myu::SW_signal sw{freq};
    SWG_pin2::generate(sw);
}


void test_pin_B()
{
    UART_iostream uart;
    uart << "\nTest pin B\n"
	    "-------------\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate SWG signal\n"
	    "3. Stop timer\n"
	    "4. Connect pin to timer\n"
	    "5. Disconnect pin from timer\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': SWG_pin2::write_zero();
	break; case '1': SWG_pin2::write_one();
	break; case '2': generate_sw_signal_pin_B();
	break; case '3': SWG_pin2::stop();
	break; case '4': SWG_pin2::connect();
	break; case '5': SWG_pin2::disconnect();
	break; default: uart << "What?\n";
    }


}


int main()
{
    init_uart();
    
    hello();
    
    myu::enable_interrupts();

    UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "A. SWG_pin A\n"
		  "B. SWG_pin B\n";



        char c{};
	uart >> c;
	switch(c){
	    break; case 'a':
		   case 'A': test_pin_A();

	    break; case 'b':
		   case 'B': test_pin_B();

	    break; default:
		uart << "What?\n";
	}
    }
}


ISR_TIMER1_CAPT{
    SWG_pin1::handle_interrupt();
}

