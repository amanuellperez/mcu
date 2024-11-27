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

#include <mega.h>
#include "../../../dev_square_wave.h"
#include <mcu_miniclock.h>


// DUDA: ¿cómo medir bien esto? No tengo claro como medir la señal generada.

// Devices
// -------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

constexpr uint8_t test_pin = 12;

using Pin = myu::hwd::Output_pin<test_pin>;
using Miniclock1_us = mcu::Miniclock_us<myu::Micro, myu::Time_counter1>;

using SWG_us = 
	dev::Square_wave_burst_generator< myu::Square_wave_burst_generator0_g
					  , Pin
					  , Miniclock1_us>;

					  

				       

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}




void generate(const uint32_t& freq, const uint16_t& t)
{
    constexpr uint16_t nburst = 1000;

    UART_iostream uart;
    uart << "Generating " << nburst << " pulses of " << freq << " Hz"
	 << " of " << t << " us\n";

    SWG_us swg_us{freq};

    for (uint16_t i = 0; i < nburst; ++i){
	swg_us.generate_burst(t);
	swg_us.wait(t);
    }
}

int main()
{
    init_uart();

    UART_iostream uart;

    uart << "\n\nSquare wave burst test\n"
	        "----------------------\n"
		"Connect oscilloscope to pin " << (int) test_pin << '\n';


    uint32_t freq    = 38000;
    uint16_t t_in_us = 500;

    while(1){
	uart << "Frequency of the burst: ";
	uart >> freq;
	uart << freq << '\n';
	uart << "Time of pulses in us: ";
	uart >> t_in_us;
	uart << t_in_us << '\n';
	if (freq > 0 and t_in_us > 0)
	    generate(freq, t_in_us);

    }


}




