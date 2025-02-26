// Copyright (C) 2025 Manuel Perez 
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

// Esta variable definirla en el makefile (hay que definirla antes de
// atd_trace.h!!! El mejor sitio: el makefile
#define TRACE_LEVEL 90
#include "../../atd_contracts.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


void test_pre()
{
    test::interface("precondition");
    
    std::cerr << "NO AUTOMATIC TEST!!!\n";

    atd::ctrace_bind(std::cout);

    atd::precondition<9>(true, "This is an error, only for TRACE_LEVEL >= 9");

}



int main()
{
try{
    test::header("atd_contracts");

    test_pre();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





