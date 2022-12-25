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

#ifndef __PRJ_SONGS_H__
#define __PRJ_SONGS_H__

#include <atd_memory.h>
#include "mus_musician.h"

// Himno de la alegría
// ¿Se llama ode to joy en ingles? @_@
//
// Escribir la canción en song.txt y usando un script que se genere esto
// automáticamente.
constexpr const atd::Progmem_array<music::Song_note, 62> PROGMEM
    ode_to_joy =
{
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  24 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  32 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  24 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  32 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::small,  music::Note::Si,  32 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Sol,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Fa,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Mi,  16 },
    music::Song_note{music::Octave::one_line,  music::Note::Re,  24 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  8 },
    music::Song_note{music::Octave::one_line,  music::Note::Do,  32 }
};



#endif



