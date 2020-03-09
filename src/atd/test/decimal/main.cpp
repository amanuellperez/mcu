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

#include "../../atd_decimal.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <vector>

#include <cstddef>

using namespace test;


template <typename Int, int n>
void test_ten_to_the(Int res)
{
    constexpr Int x = atd::ten_to_the<n, Int>();
    CHECK_TRUE(x == res, alp::as_str() << "ten_to_the(" << n << ")");
}


void test_ten_to_the()
{
    test::interfaz("ten_to_the");
 
    test_ten_to_the<int, 0>(1);
    test_ten_to_the<int, 1>(10);
    test_ten_to_the<int, 2>(100);
    test_ten_to_the<int, 3>(1000);
    test_ten_to_the<int, 4>(10000);
}

template <typename Int, int ndigits>
void test_decimal_construct(Int n, Int f, Int res)
{
    atd::Decimal<Int, ndigits> dec{n,f};
    CHECK_TRUE(dec.internal_value() == res, alp::as_str() << "dec{" << n << ", " << f << "}");

}

template <typename Int, int ndigits>
void test_decimal_value(Int n, Int f, Int fres)
{
    atd::Decimal<Int, ndigits> dec{n,f};
    auto [integer_part, fractional_part] = dec.value();

    CHECK_TRUE(integer_part == n and
	       fractional_part == fres, alp::as_str() << "value{" << n << ", " << f << "}");

}


template <typename Int, int ndigits>
void test_decimal_from_internal_value(Int x, Int n, Int f)
{
    auto dec = atd::Decimal<int, ndigits>::from_internal_value(x);
    auto [integer_part, fractional_part] = dec.value();

    CHECK_TRUE(integer_part == n and
	       fractional_part == f, alp::as_str() << "from_internal_value(" << n << ", " << f << ")");
}


void test_decimal()
{
    test::interfaz("Decimal");

// normal +
    test_decimal_construct<int, 2>(3,  0, 300);
    test_decimal_construct<int, 2>(3,  1, 301);
    test_decimal_construct<int, 2>(3, 01, 301);
    test_decimal_construct<int, 2>(3, 10, 310);
    test_decimal_construct<int, 2>(3, 14, 314);
    test_decimal_construct<int, 2>(3, 141, 314);
    test_decimal_construct<int, 2>(3, 1415, 314);

    test_decimal_construct<int, 1>(3, 14, 31);
    test_decimal_construct<int, 2>(3, 14, 314);
    test_decimal_construct<int, 3>(3, 14, 3014);
    test_decimal_construct<int, 4>(3, 14, 30014);
    test_decimal_construct<int, 5>(3, 14, 300014);


// normal -
    test_decimal_construct<int, 2>(-3, 01, -301);
    test_decimal_construct<int, 2>(-3, 10, -310);
    test_decimal_construct<int, 2>(-3, 14, -314);
    test_decimal_construct<int, 2>(-3, 141, -314);
    test_decimal_construct<int, 2>(-3, 1415, -314);

// degenerado
    test_decimal_construct<int, 0>(3, 1415, 3);
    test_decimal_construct<int, 0>(-3, 1415, -3);


    test_decimal_value<int, 2>(3,  1, 1);
    test_decimal_value<int, 2>(3, 10, 10);
    test_decimal_value<int, 2>(3, 141, 14);
    test_decimal_value<int, 2>(3, 1415, 14);

// normal
    test_decimal_from_internal_value<int, 1>(314, 31, 4);
    test_decimal_from_internal_value<int, 2>(314, 3, 14);
    test_decimal_from_internal_value<int, 3>(3014, 3, 14);
    test_decimal_from_internal_value<int, 4>(30014, 3, 14);
    test_decimal_from_internal_value<int, 5>(300014, 3, 14);

// arithmetic
    atd::Decimal<int, 2> a{3,14};
    atd::Decimal<int, 2> b{2,45};

    atd::Decimal<int, 2> c = a + b;

    a += b;
    CHECK_TRUE(a.internal_value() == 314 + 245, "a += b");
    CHECK_TRUE(a.internal_value() == c.internal_value(), "a + b");
    

    a -= b;
    CHECK_TRUE(a.internal_value() == 314, "a -= b");
    c = a - b;
    CHECK_TRUE(c.internal_value() == 314 - 245, "a - b");

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

}


int main()
{
try{
    test::header("atd_decimal");

    test_ten_to_the();
    test_most_significant_digits();
    test_decimal();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





