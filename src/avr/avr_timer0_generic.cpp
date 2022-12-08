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

#include "avr_timer0_generic.h"

namespace avr_{

constexpr void Timer0_generic::
square_wave_connect_to(Connect connection)
{
    switch (connection){
	break; case Connect::only_channel1:
	    square_wave_connect_ch1();
	    square_wave_disconnect_ch2();

	break; case Connect::only_channel2:
	    square_wave_disconnect_ch1();
	    square_wave_connect_ch2();

	break; case Connect::channel1_and_2:
	    square_wave_connect_ch1();
	    square_wave_connect_ch2();
    }
}


// ¿es genérico o depende del Timer0?
// Por culpa de frequency_in_Hz_to_prescaler_top que conoce la implementación
// del Timer esta función no es genérica.
void Timer0_generic::
square_wave_generate(uint32_t freq_in_Hz, Connect connection)
{
    auto [d, t] = frequency_in_Hz_to_prescaler_top(freq_in_Hz);

    mode_square_wave();
    square_wave_top(t); 
    square_wave_connect_to(connection);
    Timer::clock_frequency(d); // esto enciende el Timer
}

}// namespace
