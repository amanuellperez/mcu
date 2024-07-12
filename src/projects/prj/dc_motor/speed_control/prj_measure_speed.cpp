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

#include "prj_main.h"
#include <pli_iostream.h>

void Main::measure_speed()
{
    auto t = Speedmeter::measure_time_in_ms();

    if (t == 0) {
	uart << "Can't read anything. Is motor stop?\n";
	return;
    }

    uart << "Time: " << t << " ms "
	    "-> speed: " << Speedmeter::measure_speed_dps() <<
	    " = " << Speedmeter::measure_speed_rpm() << " rpm\n";
}

void Main::table_speed()
{
    uart << "\n\nAutomatic test\n"
	        "--------------\n";

    uint8_t nmes = pli::ask<uint8_t>(uart, "Number of measurements: ");
    if (nmes == 0) return;

    bool verbose = !(pli::ask_yesno(uart, "Silent mode"));
    uart << '\n';

    table_speed_impl(nmes, verbose);
}

void Main::table_speed_impl(uint8_t nmes, bool verbose)
{
    for (uint8_t p = 0; p <= 100; p += 5){
	uart << atd::Percentage{p} << '\t';
	Motor::turn(Direction::positive, p);
	Micro::wait_ms(500); // Darle tiempo a la inercia del motor
		
	atd::Float16 mean_rpm = 0;

	for (uint8_t i = 0; i < nmes; ++i){
	    auto t = Speedmeter::measure_time_in_ms();
	    if (t == 0) {
		uart << "---\n";
		break;
	    }

	    auto rpm = Speedmeter::measure_speed_rpm();
	    mean_rpm += rpm;

	    if (verbose)
		uart << rpm << " rpm\t";
	}

	if (mean_rpm != 0){
	    uart << "mean = " << (mean_rpm / atd::Float16{nmes}) << '\n';
	}
    }

    Motor::stop();

}

