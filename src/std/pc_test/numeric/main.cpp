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

#include "../../std_numeric.h"
#include <numeric>

#include <array>

#include <alp_test.h>
#include <iostream>
#include <alp_string.h>

using namespace test;

template <typename Int>
void test_gcd(Int a, Int b)
{
    CHECK_TRUE(mtd::gcd(a, b) == std::gcd(a, b),
               alp::as_str() << "gcd(" << a << ", " << b << ")");
}

void test_gcd()
{
    test::interfaz("gcd");
    
    test_gcd<int>(0, 0);

    for (int a = -500; a < 500; ++a)
	for (int b = -500; b < 500; ++b)
	    test_gcd<int>(a, b);

}


template <typename Int>
void test_lcm(Int a, Int b)
{
    CHECK_TRUE(mtd::lcm(a, b) == std::lcm(a, b),
               alp::as_str() << "lcm(" << a << ", " << b << ")");
}

void test_lcm()
{
    test::interfaz("lcm");
    
    test_lcm<int>(0, 0);

    for (int a = 1; a < 1000; ++a)
	for (int b = 1; b < 1000; ++b)
	    test_lcm<int>(a, b);

}


void test_iota()
{
    test::interfaz("iota");
 
    std::array<int, 10> x;
    std::array<int, 10> y;

    std::iota(x.begin(), x.end(), 10);
    mtd::iota(y.begin(), y.end(), 10);
    CHECK_EQUAL_CONTAINERS_C(x, y, "iota");

}

int main()
{
try{
    test::header("numeric");

    test_gcd();
    test_lcm();
    test_iota();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
