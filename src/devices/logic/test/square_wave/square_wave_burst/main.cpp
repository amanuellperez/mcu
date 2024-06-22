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
#include <dev_miniclock.h>


// DUDA: ¿cómo medir bien esto? No tengo claro como medir la señal generada.

// Devices
// -------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;

constexpr uint8_t test_pin = 12;

using Pin = my_mcu::Output_pin<test_pin>;
using Miniclock1_us = dev::Miniclock_us<my_mcu::Micro, my_mcu::Time_counter1_g>;

using SWG_us = 
	dev::Square_wave_burst_generator< my_mcu::Square_wave_burst_generator0_g
					  , Pin
					  , Miniclock1_us>;

					  

				       

// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}




void generate(const uint32_t& freq, const uint16_t& t)
{
    constexpr uint16_t nburst = 1000;

    my_mcu::UART_iostream uart;
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

    my_mcu::UART_iostream uart;

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




