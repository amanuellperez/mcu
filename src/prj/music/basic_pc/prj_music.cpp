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

#include "prj_main.h"


void Main::musical_scale_one_line()
{
    constexpr uint16_t t = 800;

    UART uart;
    uart << "\nOne line octave\n";
    musical_scale(Octave::one_line, t);

    uart << "Do\n"; 
    Musician::play(Octave::two_line, Note::Do, t);
}


void Main::musical_scale_all()
{
    constexpr uint16_t t = 800;

    UART uart;
    uart << "\nGreat octave\n";
    musical_scale(Octave::great, t);

    uart << "\nSmall octave\n";
    musical_scale(Octave::small, t);

    uart << "\nOne line octave\n";
    musical_scale(Octave::one_line, t);

    uart << "\nTwo line octave\n";
    musical_scale(Octave::two_line, t);

    uart << "\nThree line octave\n";
    musical_scale(Octave::three_line, t);
}


void Main::musical_scale(Octave octave, uint16_t t)
{
    UART uart;
    uart << "\tDo "; 
    Musician::play(octave, Note::Do, t);

    uart << "Re "; 
    Musician::play(octave, Note::Re, t);

    uart << "Mi "; 
    Musician::play(octave, Note::Mi, t);

    uart << "Fa "; 
    Musician::play(octave, Note::Fa, t);

    uart << "Sol "; 
    Musician::play(octave, Note::Sol, t);

    uart << "La "; 
    Musician::play(octave, Note::La, t);

    uart << "Si "; 
    Musician::play(octave, Note::Si, t);
}



