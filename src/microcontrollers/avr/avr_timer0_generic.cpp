// Copyright (C) 2022 Manuel Perez 
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

#include "avr_timer0_generic.h"

namespace avr_{

// ¿es genérico o depende del Timer0?
// Por culpa de frequency_in_Hz_to_prescaler_top que conoce la implementación
// del Timer esta función no es genérica.
void Square_wave_generator0_g::generate(uint32_t freq_in_Hz, uint8_t npin)
{
    auto [d, t] = frequency_in_Hz_to_prescaler_top(freq_in_Hz);

    init();

    disconnect_all_pins();
    connect_pin(npin);

    top(t); 
    Timer::clock_frequency(d); // esto enciende el Timer
}


Square_wave_burst_generator0_g::
		    Square_wave_burst_generator0_g(uint32_t freq_in_Hz)
{
    std::tie(prescaler_factor_, top_) = 
				frequency_in_Hz_to_prescaler_top(freq_in_Hz);
    init();
    disconnect_all_pins();
}

}// namespace
