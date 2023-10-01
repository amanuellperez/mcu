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

#pragma once

#ifndef __MUS_SOUNDS_H__
#define __MUS_SOUNDS_H__
namespace music{

// Ejemplos de sirenas
// TODO: darle nombre adecuados: ambulancia? policia? bomberos?
template <typename Musician>
void siren01(uint16_t freq1, uint16_t freq2, uint16_t duration_in_ms)
{
    for (auto f = freq1; f <= freq2; f += 10)
	Musician::play_freq(f, duration_in_ms);

    for (auto f = freq2; f >= freq1; f -= 10)
	Musician::play_freq(f, duration_in_ms);
}

template <typename Musician>
void siren02(uint16_t freq1, uint16_t freq2, uint16_t duration_in_ms)
{
    Musician::play_freq(freq1, duration_in_ms);
    Musician::play_freq(freq2, duration_in_ms);
}


template <typename Musician>
void alarm01(uint16_t freq, uint16_t duration_in_ms)
{
    Musician::play_freq(freq, duration_in_ms);
    Musician::silence(duration_in_ms);
}



}// namespace

#endif


