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


#include "../prj_main.h"
#include "hwd_dht.h"

void Main::hello()
{
    uart << "\n\nTemperature and humidity datalogger\n"
	        "-----------------------------------\n"
	    "Connect " << sensor_name << " to pin " << (int) sensor_pin
					<< " with a 4.7k pull-up resistor\n"
	    "and a cristal of 32kHz in pins TOSC1 and TOSC2\n\n";
}

void print_result(std::ostream& out, Sensor::Result result)
{
    using Result = Sensor::Result;

    switch(result){
	break; case Result::ok:	out << "ok\n";
	break; case Result::train_of_pulses_bad_polarity:
			out << "Bad polarity\n";

	break; case Result::train_of_pulses_bad_size:
			out << "Bad size\n";
	break; case Result::bad_check_sum:
			out << "Wrong check sum\n";
    }	    

}

void Main::print_data(std::ostream& out)
{
    // auto [T, H] = sensor_.read(timeout_ms);
    auto [T, H] = sensor_.read();

    if (sensor_.last_operation_fail())
	print_result(out, sensor_.result_last_operation());

    else{
	print_time(out, Clock::now());
	out << ": " << T << '\t' << H << " %\n";
    }


}

// No tiene configuración
void Main::print_sensor_options()
{ return;}



