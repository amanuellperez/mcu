// Copyright (C) 2022 Manuel Perez 
////           mail: <manuel2perez@proton.me>
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

#include <avr_atmega.h>
#include "../../../dev_square_wave.h"
#include <mcu_miniclock.h>

// DUDA: ¿cómo medir bien esto? No tengo claro como medir la señal generada.

// Devices
// -------
namespace myu = atmega;
using Micro = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

constexpr uint8_t test_pin = 12;

using Pin = myu::Output_pin<test_pin>;
using Miniclock1_us = mcu::Miniclock_us<myu::Micro, myu::Time_counter1>;
using Miniclock1_ms = mcu::Miniclock_ms<myu::Micro, myu::Time_counter1>;

using SWG_us = dev::Square_wave_generator< myu::Square_wave_generator0_g
					  , Pin
					  , Miniclock1_us
					  , false>;

using SWG_ms = dev::Square_wave_generator< myu::Square_wave_generator0_g
					  , Pin
					  , Miniclock1_ms
					  , false>;
					  

				       

// Functions
// ---------
enum class Cfg{ in_ms, in_us };

void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}


void ask_cfg(Cfg& cfg)
{
    UART_iostream uart;

    uart << "\nTime in:\n"
	  "1. microseconds\n"
	  "2. milliseconds\n";

    char ans{};
    uart >> ans;

    switch (ans){
	break; case '1': cfg = Cfg::in_us;
	break; case '2': cfg = Cfg::in_ms;
    }
}

template <typename SWG>
void generate(const uint32_t& freq, const uint16_t& t)
{
    constexpr uint16_t nburst = 400;

    UART_iostream uart;
    uart << "Generating " << (int) nburst << " burst of " 
	 << freq << " Hz of " << t;

    if constexpr (std::is_same_v<SWG, SWG_us>)
	uart << " us\n";

    else
	uart << " ms\n";

    SWG::cfg();

    Micro::Disable_interrupts lock;	
    for (uint16_t i = 0; i < nburst; ++i){
	SWG::generate(freq, t);
	SWG::wait(t);
    }

}


void generate(const Cfg& cfg, const uint32_t& freq, const uint16_t& t)
{
    switch(cfg){
	break; case Cfg::in_us: generate<SWG_us>(freq, t);
	break; case Cfg::in_ms: generate<SWG_ms>(freq, t);
    }
}

int main()
{
    init_uart();

    UART_iostream uart;

    uart << "\n\nSquare wave test\n"
	        "----------------\n"
		"Connect oscilloscope to pin " << (int) test_pin << '\n';


    uint32_t freq = 0;
    uint16_t t = 320;
    Cfg cfg = Cfg::in_ms;

    while(1){
	uart << "Frequency of the burst (0 to change unit of time):\n";
	uart >> freq;
	if (freq > 0){
	    uart << "Time: ";
	    uart >> t;
	    if (t > 0){
		generate(cfg, freq, t);
	    }
	}

	else
	    ask_cfg(cfg);
    }


}




