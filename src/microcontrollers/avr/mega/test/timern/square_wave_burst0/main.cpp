// Copyright (C) 2023 Manuel Perez 
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
#include "../../../mega_UART_iostream.h"
#include "../../../mega_timer0.h"
#include "../../../mega_timer1.h"

namespace my_mcu = mega_;
using Micro   = my_mcu::Micro;

static constexpr uint8_t test_pin = 12;
using SWG  = my_mcu::Square_wave_burst_generator0_g;
using Timer = my_mcu::Time_counter1;

static constexpr uint32_t frequency = 38000;
static constexpr Timer::counter_type T = 500; 

int main()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\nSquare burst wave test\n"
	        "----------------------\n";


    Timer::init();
    Timer::turn_on_with_clock_period_of<1>::us(); // 1 us
    Timer::turn_on_with_clock_period_of<1>::us();
		
    SWG swg{frequency}; // <--- FUNDAMENTAL QUE ESTÉ FUERA DEL BUCLE!!!
    while(1){
	swg.generate_burst<test_pin>();
    
	Timer::reset();
	while (Timer::unsafe_value() < T) { ; }

	swg.stop();

	Timer::reset();
	while (Timer::unsafe_value() < T) { ; }

    }
}



