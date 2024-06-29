// Copyright (C) 2024 Manuel Perez 
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

#include <ostream>
#include "atd_test.h"

namespace test{


Test Test::interface(std::ostream& out, const char* name)
{
    Test test{out};
    test.interface(name);
    return test;
}

void Test::interface(const char* name)
{ 
    out() << "\n\n";
    line();
    out() << "Interface: " << name << '\n';
    line();
}


void Test::check_true( bool condition, 
			const char* fname, int line, 
			const char* test_name)
{
    out() << test_name << " ... ";

    if (condition)
	out() << " OK\n";

    else{
	out() << " ERROR in " << fname << '[' << line << "]\n";
	// TODO: necesito un gestor de excepciones, que en el PC lance
	// excepciones pero en el avr anote un código de error:
	// atd::Throw<Error>(cod_error); 
	// Pero ¿cómo implementarlo? 
	// De tenerlo se puede fusionar esta clase con la de alp.
    }
}


}// namespace test



