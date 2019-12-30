// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "../../std_array.h"

#include <alp_test.h>
#include <iostream>


using namespace test;

namespace avr
{
using size_t = uint8_t;
}


void test_array()
{
    std::cout << "Probando mtd::array\n";
    std::cout << "------------------\n";

    mtd::array<int, 5> a = {4,5,6,7,8};

    check_true(a.size() == 5, "size()");

    a[0] = 1;
    check_true(a[0] == 1, "operator[]");

    for (size_t i = 0; i < a.size(); ++i)
	a[i] = i + 1;


    check_true(a[0] == a.front(), "front()");
    check_true(a[a.size()-1] == a.back(), "back()");

    // El estandar dice que esto debería de ser válido
//    array<int,0> b;
//    check_true(b.size() == 0, "size() vacio");
//    check_true(b.empty() == true, "empty()");

    check_true(a.begin() == &a[0], "begin()");
    check_true(a.end() == &a[a.size()], "end()");

//    a.fill(10);
    

}



int main()
{
    test_array();
}





