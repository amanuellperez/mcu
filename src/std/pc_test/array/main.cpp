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

#include "../../std_array.h"


#include <alp_test.h>
#include <iostream>
#include <cstdint>


using namespace test;

namespace avr
{
using size_t = uint8_t;
}


void test_array()
{
    test::interfaz("mtd::array");

    mtd::array<int, 5> a = {4,5,6,7,8};

    CHECK_TRUE(a.size() == 5, "size()");

    a[0] = 1;
    CHECK_TRUE(a[0] == 1, "operator[]");

    for (size_t i = 0; i < a.size(); ++i)
	a[i] = i + 1;


    CHECK_TRUE(a[0] == a.front(), "front()");
    CHECK_TRUE(a[a.size()-1] == a.back(), "back()");

    // El estandar dice que esto debería de ser válido
//    array<int,0> b;
//    CHECK_TRUE(b.size() == 0, "size() vacio");
//    CHECK_TRUE(b.empty() == true, "empty()");

    CHECK_TRUE(a.begin() == &a[0], "begin()");
    CHECK_TRUE(a.end() == &a[a.size()], "end()");

//    a.fill(10);
    
    // Probamos deducción automática
    mtd::array b = {2,3,4};
    CHECK_TRUE(b[0] == 2, "deducción automática");

{// reverse iterator
    int i = b.size() - 1;
    for (auto p = b.rbegin(); p != b.rend(); ++p)
    {
	CHECK_TRUE(*p == b[i], "rbegin/rend");
	--i;
    }
}
}


int main()
{
try{
    test::header("array");

    test_array();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




