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
    uint16_t abort_time = 400;

    myu::Enable_change_level_interrupt<speed_sensor_pin> inter{};

    Miniclock_ms::reset();

    if (wait_till([]{ return Miniclock_ms::is_off(); })
		._at_most_ms(abort_time)) {
	    uart << "Miniclock_ms doesn't start! Aborting\n";
	    return;
    }


    if (wait_till([]{ return Miniclock_ms::is_on(); })
		._at_most_ms(abort_time)) {
	    uart << "Miniclock_ms doesn't end! Aborting\n";
	    return;
    }

    uart << "Time: " << Miniclock_ms::time() << " ms\n";

}

