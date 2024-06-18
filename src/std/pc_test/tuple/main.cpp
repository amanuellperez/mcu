// Copyright (C) 2020 Manuel Perez 
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

#include "../../std_tuple.h"

#include <alp_test.h>
#include <iostream>
#include <string>

using namespace test;


void test_tuple()
{
    test::interfaz("tuple");

    mtd::tuple<int> ti{2};
    mtd::tuple<int, char, float> t3{2, 'c', 3.14};
    const mtd::tuple<int, char, float> ct3{7, 'a', 2.71};

    CHECK_TRUE(mtd::get<0>(ti) == 2, "get<0>");

    CHECK_TRUE(mtd::get<0>(t3) == 2, "get<0>");
    CHECK_TRUE(mtd::get<1>(t3) == 'c', "get<1>");
    CHECK_TRUE(mtd::get<2>(t3) == 3.14f, "get<2>");

    CHECK_TRUE(mtd::get<0>(ct3) == 7, "get<0>");
    CHECK_TRUE(mtd::get<1>(ct3) == 'a', "get<1>");
    CHECK_TRUE(mtd::get<2>(ct3) == 2.71f, "get<2>");

}

void test_tuple_element()
{
    {
    using T = mtd::tuple<int>;
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<0, T>, int>),
               "tuple_element");
    }
    {
    using T = mtd::tuple<int, std::string>;
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<0, T>, int>),
               "tuple_element");
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<1, T>, std::string>),
               "tuple_element");
    }
    {
    using T = mtd::tuple<float, int, std::string>;
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<0, T>, float>),
               "tuple_element");
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<1, T>, int>),
               "tuple_element");
    CHECK_TRUE((std::is_same_v<mtd::tuple_element_t<2, T>, std::string>),
               "tuple_element");
    }
}

mtd::pair<int, char> f(int x, char c) {return {x, c};}


void test_tie()
{
    int i;
    char c;

//    mtd::pair<int, char> p{1, 'a'};
    mtd::tuple<int&, char&>  t = mtd::tie(i, c);
    mtd::get<0>(t) = 54;
    mtd::get<1>(t) = 'b';

// >>> probamos que compile
    mtd::pair<int, char> p{3, 'x'};
    t = p;
    t = mtd::pair<int, char>{4, 'a'};
// <<< probamos que compile

    mtd::tie(i, c) = f(3, 'x');
    CHECK_TRUE(i == 3 and c == 'x', "tie");
    mtd::tie(i, c) = f(5, 'a');
    CHECK_TRUE(i == 5 and c == 'a', "tie");
}

int main()
{
try{
    test::header("utility");

    test_tuple();
    test_tuple_element();
    test_tie();

}catch(std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





