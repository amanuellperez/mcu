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
#define TRACE_LEVEL 7
#include "../../atd_trace.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


void test_trace()
{
    test::interface("ctrace");
    
    std::cerr << "NO AUTOMATIC TEST!!!\n";

    atd::ctrace_bind(std::cout);
    atd::ctrace<4>() << "ctrace(4): " << 1 << '\n';
    atd::ctrace<9>() << "ctrace(9): " << 1 << '\n';

}



int main()
{
try{
    test::header("atd_trace");

    test_trace();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





