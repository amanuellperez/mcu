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

#include "../../dev_square_wave.h"
#include "../../dev_miniclocks.h"
#include <avr_atmega.h>

// DUDA: ¿cómo medir bien esto? No tengo claro como medir la señal generada.

// Devices
// -------
namespace mcu = atmega;
using Micro = mcu::Micro;

constexpr uint8_t test_pin = 12;

using Pin = mcu::Output_pin<test_pin>;
using Miniclock1_us = dev::Miniclock_us<mcu::Micro, mcu::Time_counter1_generic>;
using Miniclock1_ms = dev::Miniclock_ms<mcu::Micro, mcu::Time_counter1_generic>;

using SWG1_us = dev::Square_wave_generator< mcu::Square_wave_generator0_g
					  , Pin
					  , Miniclock1_us
					  , false>;

using SWG1_ms = dev::Square_wave_generator< mcu::Square_wave_generator0_g
					  , Pin
					  , Miniclock1_ms
					  , false>;
					  

				       

// Functions
// ---------
enum class Cfg{ in_ms, in_us };

void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}


void ask_cfg(Cfg& cfg)
{
    mcu::UART_iostream uart;

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

void generate(const Cfg& cfg, const uint32_t& freq, const uint16_t& t)
{
    constexpr uint8_t nburst = 10;

    mcu::UART_iostream uart;
    uart << "Generating " << (int) nburst << " burst of " 
	 << freq << " of " << t;
    switch(cfg){
	    break; case Cfg::in_us: uart << " us\n";
	    break; case Cfg::in_ms: uart << " ms\n";
    }

    for (uint8_t i = 0; i < nburst; ++i){
	switch(cfg){
	    break; case Cfg::in_us: SWG1_us::burst(freq, t);
	    break; case Cfg::in_ms: SWG1_ms::burst(freq, t);
	}

	Miniclock1_ms::wait(t);// para que se vea mejor en el osciloscopio
    }
}


int main()
{
    init_uart();

    mcu::UART_iostream uart;

    uart << "\n\nSquare wave test\n"
	        "----------------\n"
		"Connect oscilloscope to pin " << (int) test_pin << '\n';

    SWG1_ms::cfg();

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




