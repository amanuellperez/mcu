// Copyright (C) 2021 Manuel Perez 
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


#include "instcodes.h"

void Sci_code::print(std::ostream& out, uint8_t code)
{
    switch(code){
	break; case Sci_code::null: out << "null";
	break; case Sci_code::ac: out << "ac";
	break; case Sci_code::ans: out << "ans";
	break; case Sci_code::del: out << "del";
	break; case Sci_code::up: out << "up";
	break; case Sci_code::down: out << "down";
	break; case Sci_code::right: out << "right";
	break; case Sci_code::left: out << "left";
	break; case Sci_code::ten_x: out << "ten_x";
	break; case Sci_code::m_plus: out << "m_plus";
	break; case Sci_code::s2d: out << "s2d";
	break; case Sci_code::eng: out << "eng";
	break; case Sci_code::rcl: out << "rcl";
	break; case Sci_code::sin: out << "sin";
	break; case Sci_code::cos: out << "cos";
	break; case Sci_code::tan: out << "tan";
	break; case Sci_code::hyp: out << "hyp";
	break; case Sci_code::sexa: out << "sexa";
	break; case Sci_code::minus: out << "minus";
	break; case Sci_code::ln: out << "ln";
	break; case Sci_code::log: out << "log";
	break; case Sci_code::pow: out << "pow";
	break; case Sci_code::square: out << "square";
	break; case Sci_code::frac: out << "frac";
	break; case Sci_code::inverse: out << "inverse";
	break; case Sci_code::calc: out << "calc";
	break; case Sci_code::shift: out << "shift";
	break; case Sci_code::alpha: out << "alpha";
	break; case Sci_code::mode: out << "mode";
	break; case Sci_code::sqrt: out << "sqrt";
	break; case Sci_code::unknown: out << "unknown";
	break; default: out << code;
    }

}

//const char* abb2str[] = {
//    "Ans",
//    "x10",
//    "sin(",
//    "cos(",
//    "tan(",
//    "º",
//    "-",
//    "ln(",
//    "log(",
//    "^",
//};
