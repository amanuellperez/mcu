// Copyright (C) 2021 A.Manuel L.Perez 
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


#include "keycodes.h"

void Code::print(std::ostream& out, uint8_t code)
{
    switch(code){
	break; case Code::ac: out << "ac";
	break; case Code::ans: out << "ans";
	break; case Code::del: out << "del";
	break; case Code::up: out << "up";
	break; case Code::down: out << "down";
	break; case Code::right: out << "right";
	break; case Code::left: out << "left";
	break; case Code::ten_x: out << "ten_x";
	break; case Code::m_plus: out << "m_plus";
	break; case Code::s2d: out << "s2d";
	break; case Code::eng: out << "eng";
	break; case Code::rcl: out << "rcl";
	break; case Code::sin: out << "sin";
	break; case Code::cos: out << "cos";
	break; case Code::tan: out << "tan";
	break; case Code::hyp: out << "hyp";
	break; case Code::sexa: out << "sexa";
	break; case Code::minus: out << "minus";
	break; case Code::ln: out << "ln";
	break; case Code::log: out << "log";
	break; case Code::pow: out << "pow";
	break; case Code::square: out << "square";
	break; case Code::root: out << "root";
	break; case Code::frac: out << "frac";
	break; case Code::inverse: out << "inverse";
	break; case Code::calc: out << "calc";
	break; case Code::shift: out << "shift";
	break; case Code::alpha: out << "alpha";
	break; case Code::mode: out << "mode";
	break; case Code::sqrt: out << "sqrt";
	break; case Code::unknown: out << "unknown";
	break; default: out << code;
    }

}

