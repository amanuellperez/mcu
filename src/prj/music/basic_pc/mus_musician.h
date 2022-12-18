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

#ifndef __MUS_MUSICIAN_H__
#define __MUS_MUSICIAN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases responsables de tocar un instrumento
 *
 * HISTORIA
 *    A.Manuel L.Perez
 *    18/12/2022 Escrito
 *
 ****************************************************************************/
#include "mus_scale.h"

namespace music{


// Un músico es el responsable de tocar un instrumento musical
// El instrumento musical realmente será un SWG (square wave generator) o algo
// parecido (sinus wave generator???)
template <typename Instrument0>
class Musician{
public:
    using Instrument = Instrument0;

    // static interface
    Musician() = delete;

    /// configuramos el musician
    static void cfg();

    /// Toca la nota indicada
    static void play(Octave, Note, uint16_t duration_in_ms);
    // TODO: static void play(Sheet_music);

private:
    static void play(uint32_t freq, uint16_t duration_in_ms);
    
};

template <typename I>
inline void Musician<I>::play(Octave octave, Note note, uint16_t duration_in_ms)
{ 
    play(note_to_frequency(octave, note), duration_in_ms); 
}


template <typename I>
inline void Musician<I>::play(uint32_t freq, uint16_t duration_in_ms)
{
    Instrument::burst(freq, duration_in_ms);
}

template <typename I>
inline void Musician<I>::cfg()
{ 
    Instrument::cfg();
}

} // namespace music
#endif
