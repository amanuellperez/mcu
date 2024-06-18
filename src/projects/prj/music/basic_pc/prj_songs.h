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

#ifndef __PRJ_SONGS_H__
#define __PRJ_SONGS_H__

#include <atd_memory.h>
#include "mus_musician.h"

// Himno de la alegría
// ¿Se llama ode to joy en ingles? @_@
//
// Escribir la canción en song.txt y usando un script que se genere esto
// automáticamente.
constexpr const atd::ROM_array<music::Note, 62, music::ROM_read_Note> PROGMEM
    ode_to_joy =
{
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  24 },
    music::Note{music::Octave::one_line,  music::Step::Re,  8 },
    music::Note{music::Octave::one_line,  music::Step::Re,  32 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  24 },
    music::Note{music::Octave::one_line,  music::Step::Do,  8 },
    music::Note{music::Octave::one_line,  music::Step::Do,  32 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  8 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  8 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  8 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  8 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::small   ,  music::Step::Si,  32 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Sol,  16 },
    music::Note{music::Octave::one_line,  music::Step::Fa,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Do,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  16 },
    music::Note{music::Octave::one_line,  music::Step::Mi,  16 },
    music::Note{music::Octave::one_line,  music::Step::Re,  24 },
    music::Note{music::Octave::one_line,  music::Step::Do,  8 },
    music::Note{music::Octave::one_line,  music::Step::Do,  32 }
};



#endif



