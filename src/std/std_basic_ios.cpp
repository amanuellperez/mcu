// Copyright (C) 2019-2020 Manuel Perez 
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

#include "std_basic_ios.h"
#include "std_ostream.h"


namespace STD{


void basic_ios::init(streambuf* sb)
{
    ios_base::init(sb);

    streambuf_ = sb;

    fill_ = ' ';
    ostream_tie_ = 0;

}

streambuf* basic_ios::rdbuf(streambuf* sb)
{
    streambuf* old = streambuf_;

    streambuf_ = sb;
    clear();

    return old;
}

void basic_ios::clear(iostate state)
{
    if (rdbuf())
	streambuf_state_ = state;
    else
	streambuf_state_ = state | badbit;
}
ostream* basic_ios::tie() const { return ostream_tie_;}

ostream* basic_ios::tie(ostream* os)
{
    ostream* old = ostream_tie_;
    ostream_tie_ = os;

    return old;
}


basic_ios::char_type basic_ios::fill(char_type c)
{
    char_type old = fill_;
    fill_ = c;

    return old;
}


}// namespace


