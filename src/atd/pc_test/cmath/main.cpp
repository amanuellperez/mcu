// Copyright (C) 2023 Manuel Perez 
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

#include "../../atd_cmath.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;


struct Int{
    int x;
    constexpr Int abs() const {return Int{atd::abs(x)};}
};
bool operator==(const Int& a, const Int& b)
{return a.x == b.x; }

struct X {};

void test_abs()
{
    test::interface("test_abs()");

    for (int n = 0; n < 100; ++n){
	if (atd::abs(n) != n)
	    CHECK_TRUE(true, alp::as_str() << "test_abs(" << n << ")");
    }

    for (int n = -100; n < 0; ++n){
	if (atd::abs(n) != -n)
	    CHECK_TRUE(true, alp::as_str() << "test_abs(" << n << ")");
    }

    for (unsigned int n = 0; n < 100; ++n){
	if (atd::abs(n) != n)
	    CHECK_TRUE(true, alp::as_str() << "test_abs(" << n << ")");
    }

    Int x{-2};
    CHECK_TRUE(atd::abs(x) == Int{2}, "Int::abs");

    std::cout << "OK\n";



}



template <typename Int>
constexpr std::pair<Int, Int> test_div(Int a, Int b)
{
    auto [aq, ar] = atd::div(a, b);
    auto [sq, sr] = atd::div(a, b);
    CHECK_TRUE(aq == sq and ar == sr,
               alp::as_str() << "div(" << a << ", " << b << ")");

    return {aq, ar};
}

constexpr int const_test_div()
{
    auto [q, r] = atd::div(10, 3);
    return q + r;
}

void test_div()
{
    for (int a = 1; a < 1000; ++a)
	for (int b = 1; b < 100; ++b)
	    test_div(a, b);

    constexpr int x = const_test_div();
    CHECK_TRUE(x == const_test_div(), "constexpr div");
}



int main()
{
try{
    test::header("atd_cmath");
    
    test_abs();
    test_div();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





