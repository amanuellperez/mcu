// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_math.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;


void test_number_of_digits()
{
    test::interfaz("number_of_digits");

    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 1, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 10; i < 100; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 2, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 100; i < 1000; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 3, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 1000; i < 10000; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 4, alp::as_str() << "number_of_digits(" << i << ")");

}

void test_ten_to_the()
{
    test::interfaz("ten_to_the");

    constexpr int i0 = atd::ten_to_the<int>(0);
    CHECK_TRUE(i0 == 1, "ten_to_the<0>");

    constexpr int i1 = atd::ten_to_the<int>(1);
    CHECK_TRUE(i1 == 10, "ten_to_the<1>");

    constexpr int i2 = atd::ten_to_the<int>(2);
    CHECK_TRUE(i2 == 100, "ten_to_the<2>");

    constexpr int i3 = atd::ten_to_the<int>(3);
    CHECK_TRUE(i3 == 1000, "ten_to_the<3>");
}

template <typename Int, int n>
void test_most_significant_digits(Int x0, Int res)
{
    Int x = atd::most_significant_digits<Int, n>(x0);
    CHECK_TRUE(x == res,
               alp::as_str() << "most_significant_digits(" << x0 << ")");
}

void test_most_significant_digits()
{
    test::interfaz("most_significant_digits");

    test_most_significant_digits<int, 1>(1, 1);
    test_most_significant_digits<int, 1>(12, 1);
    test_most_significant_digits<int, 1>(123, 1);
    test_most_significant_digits<int, 1>(1234, 1);

    test_most_significant_digits<int, 2>(1, 1);
    test_most_significant_digits<int, 2>(12, 12);
    test_most_significant_digits<int, 2>(123, 12);
    test_most_significant_digits<int, 2>(1234, 12);

// bug:
    test_most_significant_digits<int, 1>(10, 1);
    test_most_significant_digits<int, 2>(100, 10);
    
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

void test_abs()
{
    test::interfaz("test_abs()");
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

}

inline void test_is_power_of_ten(int x, bool res)
{
    std::cout << x << "; " << atd::is_power_of_ten(x) << '\n';
    CHECK_TRUE(atd::is_power_of_ten(x) == res, "is_power_of_ten");
}


void test_is_power_of_ten()
{
    test::interfaz("is_power_of_ten");
    
    test_is_power_of_ten(1, true);
    test_is_power_of_ten(10, true);
    test_is_power_of_ten(100, true);
    test_is_power_of_ten(1000, true);
    test_is_power_of_ten(10000, true);

    for (int i = 2; i < 9; ++i)
	test_is_power_of_ten(i, false);

    for (int i = 11; i < 99; ++i)
	test_is_power_of_ten(i, false);

    constexpr bool res = atd::is_power_of_ten(10);
    bool x = std::bool_constant<res>::value;
    CHECK_TRUE(x == true, "is_power_of_ten(static)");

}


void test_power_of_ten()
{
    test::interfaz("power_of_ten");
    
    CHECK_TRUE((atd::exponent_of_power_of_ten(1) == 0), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(10) == 1), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(100) == 2), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(1000) == 3), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(10000) == 4), "power_of_ten");

}


int main()
{
try{
    test::header("atd_math");
    
    test_is_power_of_ten();
    test_power_of_ten();
    test_number_of_digits();
    test_ten_to_the();
    test_most_significant_digits();
    test_div();
    test_abs();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





