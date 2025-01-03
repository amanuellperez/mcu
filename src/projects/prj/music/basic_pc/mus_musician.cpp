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

#include "mus_musician.h"
#include <atd_memory.h>
#include <mega.h>	// macro pgm_read_byte!!! 

namespace music{
// TODO: progmem_read ya no está en atd!!!
Note progmem_read(const Note& x)
{
    uint8_t midi_number = atd::progmem_read(x.midi_number);
    uint8_t ticks       = atd::progmem_read(x.ticks);

    return Note{midi_number, ticks};
}

}// namespace
 
