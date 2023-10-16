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

#include "prj_main.h"


void Main::run()
{
    UART uart;
    uart << "\n\nTemperature datalogger\n"
	        "----------------------\n"
	    "Connect DS18B20 to pin " << (int) one_wire_pin 
					<< " with a 4.7k pull-up resistor\n";

    Watchdog::interrupt_mode<Watchdog::Period::T1_s>();
    Micro::enable_interrupts();

    Micro::sleep_mode(mcu::Sleep::mode::idle);

    while (1){
	Micro::sleep();

	auto T = sensor_.read_temperature(sensor_timeout_ms);
	if (sensor_.last_operation_is_ok()){
	    uart << nseconds << " s: " << T << " ºC\n";
	}

	else 
	    print_error(uart, sensor_.result_last_operation());


    }
}


