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

// TODO: que admita Frequency directamente.
template <typename Square_wave>
inline constexpr typename Square_wave::counter_type 
    freq_in_kHz_to_top(const typename Square_wave::counter_type& freq)
{ return 1000 / (freq * 2); }


template <typename Square_wave>
void generate_38kHz_on()
{
    // constexpr Square_wave::counter_type T = 1000 / (38 * 2); // freq = 38kHz
    constexpr typename Square_wave::counter_type T = freq_in_kHz_to_top<Square_wave>(38);

    Square_wave::mode_square_wave();
    Square_wave::square_wave_top(T); 
    Square_wave::square_wave_connect_ch1(); 
    Square_wave::template on<1>();		
}

template <typename Square_wave>
void generate_38kHz_off()
{
    Square_wave::off();
    Square_wave::square_wave_disconnect_ch1();

    // Garantizamos que acabe en cero
    avr::Pin<Square_wave::pin_channel1>::write_zero();

}


#endif
