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

template <int n1, int n2>
void test_decimal_construct_convert(int integer_part, int frac_part, int res)
{
    atd::Decimal<int, n1> d1{integer_part, frac_part};

    atd::Decimal<int, n2> d2{d1};
    CHECK_TRUE(d2.internal_value() == res, "constructor conversion");
}

void test_decimal_construct()
{
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

// constexpr
    {
	constexpr atd::Decimal<int, 2> c{3,14};
    }

// Conversiones
    test_decimal_construct_convert<1,1>(31,4, 314);
    test_decimal_construct_convert<1,2>(31,4, 3140);
    test_decimal_construct_convert<1,3>(31,4, 31400);


    test_decimal_construct_convert<3,1>(31,415, 314);
    test_decimal_construct_convert<3,2>(31,415, 3141);
    test_decimal_construct_convert<3,3>(31,415, 31415);
    test_decimal_construct_convert<3,4>(31,415, 314150);

}



void test_decimal_common_type()
{
    {
    using D1 = atd::Decimal<int, 2>;
    using D2 = atd::Decimal<long int, 4>;

    using CD = std::common_type_t<D1, D2>;
    CHECK_TRUE((std::is_same_v<CD::Rep, long int>), "common_type::Rep");
    CHECK_TRUE(CD::num_decimals == 4, "common_type::num_decimals");
    }

    {// caso degenerado: D1 == D2
    using D1 = atd::Decimal<int, 2>;

    using CD = std::common_type_t<D1, D1>;
    CHECK_TRUE((std::is_same_v<CD::Rep, D1::Rep>), "common_type::Rep");
    CHECK_TRUE(CD::num_decimals == D1::num_decimals, "common_type::num_decimals");
    }
}

template <typename D1, typename D2>
void test_decimal_order(const D1& a, const D2& b,
			bool res_eq, bool res_noteq,
			bool res_lt, bool res_le,
			bool res_gt, bool res_ge)
{
    CHECK_TRUE((a == b) == res_eq, "operator==");
    CHECK_TRUE((a != b) == res_noteq, "operator!=");
    CHECK_TRUE((a < b) == res_lt, "operator<");
    CHECK_TRUE((a <= b) == res_le, "operator<=");
    CHECK_TRUE((a > b) == res_gt, "operator>");
    CHECK_TRUE((a >= b) == res_ge, "operator>=");
}


void test_decimal_order()
{
// básicos
    test_decimal_order(atd::Decimal<int, 2>{3,14},
		       atd::Decimal<int, 2>{5,20},
		       false, true, 
		       true, true, false, false);

    test_decimal_order(atd::Decimal<int, 2>{3,14},
		       atd::Decimal<int, 2>{3,14},
		       true, false, 
		       false, true, false, true);

    test_decimal_order(atd::Decimal<int, 2>{13,14},
		       atd::Decimal<int, 2>{3,14},
		       false, true, 
		       false, false, true, true);

// con tipos diferentes
    test_decimal_order(atd::Decimal<int, 1>{3,1},
		       atd::Decimal<int, 3>{3,147},
		       false, true, 
		       true, true, false, false);


}


// ip = integer part
// fp = fractional part
template <int n1, int n2>
void test_decimal_multiplication(int ip1, int fp1, int ip2, int fp2, 
				 int ipr, int fpr)
{
    atd::Decimal<int, n1> x{ip1,fp1};
    atd::Decimal<int, n2> y{ip2,fp2};
    auto p = x*y;
    CHECK_TRUE(p.num_decimals == x.num_decimals + y.num_decimals, "operator*");
    auto [ip, fp] = p.value();
    CHECK_TRUE(ip == ipr and fp == fpr, "operator*");
}


// ip = integer part
// fp = fractional part
template <int n1, int n2>
void test_decimal_division(int ip1, int fp1, int ip2, int fp2, 
				 int ipr, int fpr)
{
    atd::Decimal<int, n1> x{ip1,fp1};
    atd::Decimal<int, n2> y{ip2,fp2};
    auto p = x / y;
    auto [ip, fp] = p.value();
    CHECK_TRUE(ip == ipr and fp == fpr, "operator*");
}


void test_decimal_arithmetic()
{
// arithmetic
    atd::Decimal<int, 2> a{3,14};
    atd::Decimal<int, 2> b{2,45};

    a += b;
    CHECK_TRUE(a.internal_value() == 314 + 245, "a += b");
    
    a -= b;
    CHECK_TRUE(a.internal_value() == 314, "a -= b");

    atd::Decimal<int, 2> c = a;
    CHECK_TRUE(c == a, "operator=");

// Suma
    c = a + b;
    CHECK_TRUE(c.internal_value() == a.internal_value() + b.internal_value(),
               "a + b");

    c = a - b;
    CHECK_TRUE(c.internal_value() == a.internal_value() - b.internal_value(),
               "a - b");

// Producto por escalares
    c = 2*a;
    CHECK_TRUE(c.internal_value() == 2*a.internal_value(), "2*a");

    c = a*2;
    CHECK_TRUE(c.internal_value() == 2*a.internal_value(), "a*2");

    c = c/2;
    CHECK_TRUE(c == a, "a/2");

// Producto
    test_decimal_multiplication<2,2>(3,14   , 2,75  , 8,6350);
    test_decimal_multiplication<1,5>(10,2   , 7,12345,	72,659190);

    test_decimal_division<1,5>(10,2   , 7,12345,    1,4);
    test_decimal_division<0,0>(10,0   , 2,0,	    5,0);
    test_decimal_division<2,1>(3,14   , 2, 3,	    1,36);
}



void test_decimal()
{
    test::interfaz("Decimal");

    test_decimal_construct();
    test_decimal_common_type();
    test_decimal_order();
    test_decimal_arithmetic();


// operator<<
    {
    atd::Decimal<int, 2> a{922,06};
    CHECK_STDOUT(a, "922.06");

    atd::Decimal<int, 0> b{876};
    CHECK_STDOUT(b, "876");
    }
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





