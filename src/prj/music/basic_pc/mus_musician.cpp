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

#include "mus_musician.h"
#include <atd_memory.h>
#include <avr_memory.h>	// macro pgm_read_byte!!! 

namespace music{

// TODO: No funciona leer directamente la enum. Tengo que usar la macro
// pgm_read_byte para que funcione. ¿por qué? 
//	Sospecha: atd::progmem_read está definida para (uint8_t/uint16_t)
//	pero no para la enum Octave/Note. El compilador debería de generar un
//	error indicando que no la encuentra, pero no lo genera. ???
Song_note progmem_read(const Song_note& x)
{
    Song_note res;
//    res.octave = Octave{atd::progmem_read(x.octave)};
//    res.note   = Note{atd::progmem_read(x.note)};
    res.octave = Octave{pgm_read_byte(&x.octave)};
    res.note   = Note{pgm_read_byte(&x.note)};
    res.ticks  = atd::progmem_read(x.ticks);

    return res;
}

}// namespace
 
