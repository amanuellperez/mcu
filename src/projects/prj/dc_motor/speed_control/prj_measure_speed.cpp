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

#include "mcu_time.h"


void Main::measure_speed()
{
    uint8_t max_abort_time = 250;

    Speed_sensor_pin::enable_change_level_interrupt(); 

    Miniclock_ms::reset();

//    if (no_response_after_ms_while(max_abort_time,
//			    []{ return Miniclock_ms::is_off(); },
//			    []{ Micro::wait_ms(1);}
//			    )
    if (no_response_after(max_abort_time).ms_while(
			    []{ return Miniclock_ms::is_off(); },
			    []{ Micro::wait_ms(1);}
			    )
	){
	    uart << "Miniclock_ms doesn't start! Aborting\n";
	    Speed_sensor_pin::disable_change_level_interrupt();
	    return;
    }


    while (Miniclock_ms::is_on()) 
    { ; } // wait

    uart << "Time: " << Miniclock_ms::time() << " ms\n";

}

