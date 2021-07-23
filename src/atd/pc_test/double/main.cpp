// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_double.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;

inline bool equal(double a, double b)
{
    if (a > b)
	return (a-b < 0.01);

    return (b-a < 0.01);
}

template<typename Int>
void test_mantissa_to_integer(const double& x, const Int& res)
{
    CHECK_TRUE(atd::mantissa_to_integer<Int>(x) == res, "mantissa_to_integer");
}


void test_mantissa_to_integer()
{
// degenerado
    test_mantissa_to_integer<uint8_t>(0, 0);
    test_mantissa_to_integer<uint16_t>(0, 0);

    test_mantissa_to_integer<int8_t>(0, 0);
    test_mantissa_to_integer<int16_t>(0, 0);

// normal
    test_mantissa_to_integer<uint8_t>(0.123, 12);
    test_mantissa_to_integer<uint16_t>(0.123, 123);
    test_mantissa_to_integer<uint32_t>(0.123, 123);
    test_mantissa_to_integer<uint64_t>(0.123, 123);

    test_mantissa_to_integer<int8_t>(0.123, 12);
    test_mantissa_to_integer<int16_t>(0.123, 123);
    test_mantissa_to_integer<int32_t>(0.123, 123);
    test_mantissa_to_integer<int64_t>(0.123, 123);
}


template <typename Int>
void test_integer_to_mantissa(Int x, double res)
{
    CHECK_TRUE(equal(atd::integer_to_mantissa<double>(x), res),
               "integer_to_mantissa");
}

void test_integer_to_mantissa()
{
// degenerado
    test_integer_to_mantissa<uint8_t>(0, 0);
    test_integer_to_mantissa<uint16_t>(0, 0);

    test_integer_to_mantissa<int8_t>(0, 0);
    test_integer_to_mantissa<int16_t>(0, 0);
    
// normal
    test_integer_to_mantissa<uint8_t>(23, 0.23);
    test_integer_to_mantissa<uint16_t>(23, 0.23);
    test_integer_to_mantissa<uint32_t>(23, 0.23);
    test_integer_to_mantissa<uint64_t>(23, 0.23);

    test_integer_to_mantissa<int8_t>(23, 0.23);
    test_integer_to_mantissa<int16_t>(23, 0.23);
    test_integer_to_mantissa<int32_t>(23, 0.23);
    test_integer_to_mantissa<int64_t>(23, 0.23);
}



void test_modf(double x, double i1, double f1)
{
    auto [i, f] = atd::modf(x);
    CHECK_TRUE(equal(i, i1) and equal(f, f1), "modf");
}

void test_modf()
{
// normal
    test_modf(2.354, 2, 0.35);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);

    test_modf(2.354, 2, 0.35);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);
}


template <typename Int>
void test_modf_as_int(double x, Int i1, Int f1)
{
    auto [i, f] = atd::modf_as_int<Int>(x);
    CHECK_TRUE(i == i1 and f == f1, "modf_as_int");
}


void test_modf_as_int()
{
    // normal
    test_modf_as_int<uint8_t>(2.354, 2, 35);
    test_modf_as_int<uint16_t>(2.354, 2, 354);
    test_modf_as_int<uint32_t>(2.354, 2, 354);
    // NO FUNCIONA con 64 bits. El motivo es que 2.354 lo guarda como
    // 2.3540000000000001024!!! 
//   test_modf_as_int<uint64_t>(2.354, 2, 354);

    test_modf_as_int<int8_t>(2.354, 2, 35);
    test_modf_as_int<int16_t>(2.354, 2, 354);
    test_modf_as_int<int32_t>(2.354, 2, 354);
//    test_modf_as_int<int64_t>(2.354, 2, 354);

}

void test_print()
{
    CHECK_PRINT((atd::print(std::cout, 12.45)), "12.45");
}


int main()
{
try{
    test::header("double");
    
    test_integer_to_mantissa();
    test_mantissa_to_integer();
    test_modf();
    test_modf_as_int();
    test_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





