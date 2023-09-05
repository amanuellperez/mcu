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

#include "avr_sleep.h"

namespace avr_{

static void sleep_mode(Sleep::Mode mode)
{
    switch (mode){
	break; case Sleep::Mode::idle: 
			    Sleep::idle_mode();

	break; case Sleep::Mode::ADC_noise_reduction: 
			    Sleep::ADC_noise_reduction_mode();

	break; case Sleep::Mode::power_down: 
			    Sleep::power_down_mode();

	break; case Sleep::Mode::power_save: 
			    Sleep::power_save_mode();

	break; case Sleep::Mode::standby: 
			    Sleep::standby_mode();

	break; case Sleep::Mode::extended_standby: 
			    Sleep::extended_standby_mode();
    }
    
}

void sleep(Sleep::Mode mode)
{
    sleep_mode(mode);

    Sleep::enable();
    Sleep::instruction();
    Sleep::disable();

}

}// namespace
 
