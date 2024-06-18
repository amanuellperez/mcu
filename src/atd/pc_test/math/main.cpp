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

#include "../../atd_math.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <cmath>
#include <array>


using namespace test;


void test_number_of_digits()
{
    test::interface("number_of_digits");

    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 1, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 10; i < 100; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 2, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 100; i < 1000; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 3, alp::as_str() << "number_of_digits(" << i << ")");

    for (int i = 1000; i < 10000; ++i)
	CHECK_TRUE(atd::number_of_digits(i) == 4, alp::as_str() << "number_of_digits(" << i << ")");

}

template <typename Int>
void test_ten_to_the(int n, const Int& res)
{
    Int i = atd::ten_to_the<Int>(n);
    CHECK_TRUE(i == res, alp::as_str() << "ten_to_the<" << n << ">");
}

void test_ten_to_the()
{
    test::interface("ten_to_the");

    test_ten_to_the<int>(0, 1);
    test_ten_to_the<int>(1, 10);
    test_ten_to_the<int>(2, 100);
    test_ten_to_the<int>(3, 1000);

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
    test::interface("most_significant_digits");

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
    test_most_significant_digits<int8_t, 3>(123, 123);
    
}



inline void test_is_power_of_ten(int x, bool res)
{
    std::cout << x << "; " << atd::is_power_of_ten(x) << '\n';
    CHECK_TRUE(atd::is_power_of_ten(x) == res, "is_power_of_ten");
}


void test_is_power_of_ten()
{
    test::interface("is_power_of_ten");
    
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
    test::interface("power_of_ten");
    
    CHECK_TRUE((atd::exponent_of_power_of_ten(1) == 0), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(10) == 1), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(100) == 2), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(1000) == 3), "power_of_ten");
    CHECK_TRUE((atd::exponent_of_power_of_ten(10000) == 4), "power_of_ten");

}


void test_remove_trailing_zeros()
{
    test::interface("remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(0) == 0),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(1) == 1),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(10) == 1),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(-10) == -1),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(12300) == 123),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(123) == 123),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<int>(1020300) == 10203),
               "remove_trailing_zeros");

    CHECK_TRUE((atd::remove_trailing_zeros<uint8_t>(100) == 1),
               "remove_trailing_zeros");
}


template <size_t N>
void test_digits_of(int x, const std::array<int, N>& res)
{
    atd::Digits_of d{x};
    
    CHECK_TRUE(d.begin() != d.end(), "Digits_of");

    int i = 0;
    for (auto p = d.begin(); p != d.end(); ++p, ++i){
        CHECK_TRUE(*p == res[i],
                   alp::as_str() << "Digits_of[" << i << "]");
    }
    CHECK_TRUE(N == i, "Digits_of::size");
}

void test_digits_of()
{
    test::interface("Digits_of");
    {std::array<int, 1> res = {0}; test_digits_of(0, res);}
    {std::array<int, 1> res = {2}; test_digits_of(2, res);}
    {std::array<int, 2> res = {7, 3}; test_digits_of(37, res);}
    {std::array<int, 3> res = {0, 9, 8}; test_digits_of(890, res);}
    {std::array<int, 4> res = {7, 4, 9, 2}; test_digits_of(2947, res);}
}


template <size_t N>
void test_digits_from_left_to_right(int x, const std::array<int, N>& res, int ndigits = 0)
{
    atd::Digits_from_left_to_right d{x, ndigits};
    
    CHECK_TRUE(d.begin() != d.end(), "Digits_from_left_to_right");

    int i = 0;
    for (auto p = d.begin(); p != d.end(); ++p, ++i){
        CHECK_TRUE(*p == res[i],
                   alp::as_str() << "Digits_from_left_to_right[" << i << "]");
    }
    CHECK_TRUE(N == i, "Digits_from_left_to_right::size");
}

void test_digits_from_left_to_right()
{
    test::interface("Digits_from_left_to_right");

// sin ceros a la izda
    {std::array<int,1> res = {0}; test_digits_from_left_to_right(0, res);}
    {std::array<int,2> res = {1, 0}; test_digits_from_left_to_right(10, res);}
    {std::array<int,3> res = {1, 0, 0}; test_digits_from_left_to_right(100, res);}
    {std::array<int,4> res = {1, 0, 0, 0}; test_digits_from_left_to_right(1000, res);}
    {std::array<int,1> res = {2}; test_digits_from_left_to_right(2, res);}
    {std::array<int,2> res = {3, 7}; test_digits_from_left_to_right(37, res);}
    {std::array<int,3> res = {8, 9, 0}; test_digits_from_left_to_right(890, res);}
    {std::array<int,4> res = {2, 9, 4, 7}; test_digits_from_left_to_right(2947, res);}

// con ceros a la izda
    {std::array<int,3> res = {0, 0, 0}; test_digits_from_left_to_right(0, res, 3);}
    {std::array<int,3> res = {0, 1, 0}; test_digits_from_left_to_right(10, res, 3);}
    {std::array<int,3> res = {1, 0, 0}; test_digits_from_left_to_right(100, res, 3);}
    {std::array<int,4> res = {1, 0, 0, 0}; test_digits_from_left_to_right(1000, res, 3);}
    {std::array<int,4> res = {0, 0, 0, 2}; test_digits_from_left_to_right(2, res, 4);}
    {std::array<int,4> res = {0, 0, 3, 7}; test_digits_from_left_to_right(37, res, 4);}
    {std::array<int,3> res = {8, 9, 0}; test_digits_from_left_to_right(890, res, 3);}
    {std::array<int,4> res = {2, 9, 4, 7}; test_digits_from_left_to_right(2947, res, 2);}

}

void test_is_even()
{
    test::interface("is_even/is_odd");
    for (uint8_t i = 0; i < 10; i += 2){
	CHECK_TRUE(atd::is_even(i), "is_even");
	CHECK_TRUE(!atd::is_odd(i), "is_odd");
    }

    for (uint8_t i = 1; i < 10; i += 2){
	CHECK_TRUE(!atd::is_even(i), "is_even");
	CHECK_TRUE(atd::is_odd(i), "is_odd");
    }
}

template <typename Int>
void test_sign_of(const Int& x, int res)
{
    CHECK_TRUE(atd::sign_of(x) == res, 
		    alp::as_str() << "sign_of(" << (int) x << ")");
}

void test_sign_of()
{
    test::interface("sign_of");

    test_sign_of<uint8_t>(uint8_t{3}, 1);
    test_sign_of<uint8_t>(uint8_t{0}, 0);

    test_sign_of<int8_t>(int8_t{3}, 1);
    test_sign_of<int8_t>(int8_t{0}, 0);
    test_sign_of<int8_t>(int8_t{-3}, -1);
}

template <std::integral Int>
inline
void test_divide_rounding(const Int& D, const Int& d, const Int& res)
{
    CHECK_TRUE(atd::divide_rounding(D, d) == res,
		alp::as_str() << "divide_rounding(" << D << ", " << d << ')');
}

template <std::integral Int>
void test_divide_rounding(const Int& D, const Int& d)
{
    float res = (float) D /(float)  d;
    CHECK_TRUE(atd::divide_rounding(D, d) == std::lround(res),
		alp::as_str() << "divide_rounding(" << D << ", " << d << ')');
}

template <std::integral Int>
void test_divide_rounding(const Int& D)
{
    for (Int d = 1; d <= D; ++d)
	test_divide_rounding(D, d);
}

void test_divide_rounding()
{
    test::interface("divide_rounding");

    test_divide_rounding(10);
    test_divide_rounding(100);

}

void test_sign()
{
    test::interface("Sign");
    
    atd::Sign sign = atd::Sign::positive;
    CHECK_PRINT(std::cout << sign, '+');
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
    test_remove_trailing_zeros();
    test_digits_of();
    test_digits_from_left_to_right();
    test_is_even();
    test_sign_of();
    test_divide_rounding();
    test_sign();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





