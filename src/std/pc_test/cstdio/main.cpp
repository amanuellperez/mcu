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

#include "../../std_cstdio.h"

#include <alp_test.h>
#include <iostream>
#include <cstring>
#include <limits>

using namespace test;

void print(char* p0, char* pe)
{
    for (; p0 != pe; ++p0)
	std::cout << *p0;
}

template <typename Int>
void test_int_to_cstring(Int i, const std::string& msg)
{
    char buffer[100];
    char* pc = mtd::__int_to_cstring(buffer, buffer + 100, i);
    std::cout << msg << ": [";
    print(pc, buffer + 100);
    std::cout << "] ?= [" << i << "]\n";
}

template <typename Int>
void test_int_to_cstring(Int x0, Int x1, const std::string& msg)
{
    test_int_to_cstring(x0, msg);
    test_int_to_cstring(x1, msg);
    Int x = std::numeric_limits<Int>::min() + Int{1};

    test_int_to_cstring(x, msg);
    x = std::numeric_limits<Int>::max();
    test_int_to_cstring(x, msg);
}


void test_int_to_cstring()
{
    test::interfaz("int_to_cstring");

    int i0 = 234, i1 = -45;
    test_int_to_cstring(i0, i1, "int");

    long l0 = 234, l1 = -45;
    test_int_to_cstring(l0, l1, "long");

    long long ll0 = 234, ll1 = -45;
    test_int_to_cstring(ll0, ll1, "long long");


    unsigned int ui0 = 234, ui1 = 0;
    test_int_to_cstring(ui0, ui1, "unsigned int");

    unsigned long ul0 = 234, ul1 = 0;
    test_int_to_cstring(ul0, ul1, "unsigned long");

    unsigned long long ull0 = 234, ull1 = 0;
    test_int_to_cstring(ull0, ull1, "unsigned long long");

}


int main()
{
try{
    test::header("std_cstdio");

    std::cerr << "NO SON AUTOMÁTICAS ESTAS PRUEBAS!!!\n";
    test_int_to_cstring();

}catch(alp::Excepcion& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





