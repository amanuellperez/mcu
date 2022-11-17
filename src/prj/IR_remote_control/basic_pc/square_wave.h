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
#pragma once

#ifndef __SQUARE_WAVE_H__
#define __SQUARE_WAVE_H__

#include <avr_pin.h>

// TODO: que admita Frequency directamente.
template <typename Timer>
inline constexpr typename Timer::counter_type 
    freq_in_kHz_to_top(const typename Timer::counter_type& freq)
{ return 1000 / (freq * 2); }


template <typename Timer, uint8_t npin>
void generate_38kHz_on()
{
    static_assert(npin == Timer::pin_channel1);

    // constexpr Timer::counter_type T = 1000 / (38 * 2); // freq = 38kHz
    constexpr typename Timer::counter_type T = freq_in_kHz_to_top<Timer>(38);

    Timer::mode_square_wave();
    Timer::square_wave_top(T); 
    Timer::square_wave_connect_ch1(); 
    Timer::template on<1>();		
}

template <typename Timer, uint8_t npin>
void generate_38kHz_off()
{
    static_assert(npin == Timer::pin_channel1);

    Timer::off();
    Timer::square_wave_disconnect_ch1();

    // Garantizamos que acabe en cero
    avr::Pin<Timer::pin_channel1>::as_output();
    avr::Pin<Timer::pin_channel1>::write_zero();

}


#endif
