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

#include "../../std_utility.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace test;


void test_pair()
{
    test::interfaz("pair");

    mtd::pair<int, float> p1;
    CHECK_TRUE(p1.first == 0 and p1.second == 0.0, "pair{}");

    mtd::pair<int, float> p2{-3, 25.4};
    CHECK_TRUE(p2.first == -3 and p2.second == float{25.4}, "pair{x,y}");

    mtd::pair<int, float> p3{p2};
    CHECK_TRUE(p2.first == p3.first and p2.second == p3.second, "pair{pair}");
    CHECK_TRUE(p2 == p3, "operator==");
    CHECK_TRUE(p2 != p1, "operator!=");

}

void test_tuple_element()
{
    test::interfaz("tuple_element");

    using P = mtd::pair<int, float>;
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<0, P>, int>),
               "tuple_element<0>");

    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<1, P>, float>),
               "tuple_element<1>");
}



int main()
{
try{
    test::header("utility");

    test_pair();
    test_tuple_element();
}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





