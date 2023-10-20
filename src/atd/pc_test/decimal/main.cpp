// Copyright (C) 2020-2021 Manuel Perez 
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

#include "../../atd_decimal.h"
#include <iostream>
#include "../../atd_type_traits.h"
#include "../../atd_cast.h"
#include "../../atd_concepts.h"

#include <alp_test.h>
#include <alp_string.h>

#include <cstddef>

using namespace test;



template <typename Int, int ndigits>
void test_decimal_construct(Int n, Int f, Int rep, Int res_n, Int res_f)
{
//    std::cout << "test_decimal_construct(" << n << ", " << f << ", " << rep
//              << ", " << res_n << ", " << res_f << ")\n";

    atd::Decimal<Int, ndigits> dec{n,f};
    CHECK_TRUE(dec.significand() == rep, 
	    alp::as_str() << "dec{" << n << ", " << f << "}");

    auto [n1, f1] = dec.value();

//    std::cout << "res = [" << res_n << "." << res_f << "] =? [" << n1 << "."
//              << f1 << '\n';
    CHECK_TRUE(n1 == res_n and f1 == res_f,
               alp::as_str() << "dec<" << ndigits << ">{" << n << ", " << f << "} genera [" << n1
                             << "." << f1 << ']');
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
void test_decimal_significand(Int x, Int n, Int f)
{
    auto dec = atd::Decimal<int, ndigits>::significand(x);
    auto [integer_part, fractional_part] = dec.value();

    CHECK_TRUE(integer_part == n and
	       fractional_part == f, alp::as_str() << "significand(" << n << ", " << f << ")");
}

template <int n1, int n2>
void test_decimal_construct_convert(int integer_part, int frac_part, int res)
{
    atd::Decimal<int, n1> d1{integer_part, frac_part};

    atd::Decimal<int, n2> d2 = atd::decimal_cast<atd::Decimal<int, n2>>(d1);
    CHECK_TRUE(d2.significand() == res, "constructor conversion");
}

void test_decimal_construct()
{
// normal +
    std::cout << "1ª tanda\n";
    test_decimal_construct<int, 2>(3,0,	    300, 3,0); 
    test_decimal_construct<int, 2>(3,1,	    310, 3,10);
    test_decimal_construct<int, 2>(3,1,	    310, 3,10);
    test_decimal_construct<int, 2>(3,10,    310, 3,10);
    test_decimal_construct<int, 2>(3,14,    314, 3,14);
    test_decimal_construct<int, 2>(3,141,   314, 3,14);
    test_decimal_construct<int, 2>(3,1415,  314, 3,14);

    std::cout << "2ª tanda\n";
    test_decimal_construct<int, 1>(3, 1415, 31	, 3,1);
    test_decimal_construct<int, 2>(3, 14, 314	, 3,14);
    test_decimal_construct<int, 3>(3, 14, 3140	, 3,140);
    test_decimal_construct<int, 4>(3, 14, 31400	, 3,1400);
    test_decimal_construct<int, 5>(3, 14, 314000, 3,14000);

// bug: no funcionaba bien con 0.xxx
    test_decimal_construct<int, 1>(0,10, 1, 0,1); 
    test_decimal_construct<int, 2>(0,10, 10, 0,10); 
    test_decimal_construct<int, 3>(0,10, 100, 0,100); 

// normal -
    test_decimal_construct<int, 2>(-3,1,    -310,   -3,10);
    test_decimal_construct<int, 2>(-3,10,   -310,   -3,10);
    test_decimal_construct<int, 2>(-3,14,   -314,   -3,14);
    test_decimal_construct<int, 2>(-3,141,  -314,   -3,14);
    test_decimal_construct<int, 2>(-3,1415, -314,   -3,14);

// degenerado
    test_decimal_construct<int, 0>( 3,1415,  3,  3,0);
    test_decimal_construct<int, 0>(-3,1415, -3, -3,0);


    test_decimal_value<int, 2>(3,1,	10);
    test_decimal_value<int, 2>(3,10,	10);
    test_decimal_value<int, 2>(3,141,	14);
    test_decimal_value<int, 2>(3,1415,	14);

// normal
    test_decimal_significand<int, 1>(314, 31, 4);
    test_decimal_significand<int, 2>(314, 3, 14);
    test_decimal_significand<int, 3>(3014, 3, 14);
    test_decimal_significand<int, 4>(30014, 3, 14);
    test_decimal_significand<int, 5>(300014, 3, 14);

// constexpr
    {
	constexpr atd::Decimal<int, 2> c{3,14};
    }

// syntax sugar
    {
	using Int = atd::Decimal<int, 3>;

	// Al escribir Int{10,23} se espera escribir el decimal "10,230"
	Int a{10,23};
	CHECK_TRUE(a.significand() == 10230, "constructor");
    }

// Conversiones
    test_decimal_construct_convert<1,1>(31,4, 314);
    test_decimal_construct_convert<1,2>(31,4, 3140);
    test_decimal_construct_convert<1,3>(31,4, 31400);


    test_decimal_construct_convert<3,1>(31,415, 314);
    test_decimal_construct_convert<3,2>(31,415, 3141);
    test_decimal_construct_convert<3,3>(31,415, 31415);
    test_decimal_construct_convert<3,4>(31,415, 314150);

{// bug
    atd::Decimal<int, 2> d{-28, 89};
    CHECK_STDOUT(d, "-28.89");
}

{// construcción con tipo diferentes
//    uint16_t x16 = 300;
//    atd::Decimal<uint8_t, 1> d0{x16};
//    std::cout << "---------------------" << d0 << '\n';

}

{// conversiones de Decimal8 -> Decimal16
    atd::Decimal<uint8_t, 1> d0{2};
    atd::Decimal<uint16_t, 1> d1{d0};
    CHECK_TRUE((d1 == atd::Decimal<uint16_t,1>{2}), "uint8_t -> uint16_t");

    CHECK_DONT_COMPILE((atd::Decimal<uint8_t, 1> d2{atd::Decimal<uint16_t,1>{300}};), 
	        "conversión uint16_t -> uint8_t")
}
}



void test_decimal_common_type()
{
    {
    using D1 = atd::Decimal<int, 2>;
    using D2 = atd::Decimal<long int, 4>;

    using CD = std::common_type_t<D1, D2>;
    CHECK_TRUE((std::is_same_v<CD::Rep, long int>), "common_type::Rep");
    CHECK_TRUE(CD::ndecimals == 4, "common_type::ndecimals");
    }

    {// caso degenerado: D1 == D2
    using D1 = atd::Decimal<int, 2>;

    using CD = std::common_type_t<D1, D1>;
    CHECK_TRUE((std::is_same_v<CD::Rep, D1::Rep>), "common_type::Rep");
    CHECK_TRUE(CD::ndecimals == D1::ndecimals, "common_type::ndecimals");
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
//    std::cerr << "test_decimal_multiplication(" << ip1 << ", " << fp1 << ")\n";
    atd::Decimal<int, n1> x{ip1,fp1};
    atd::Decimal<int, n2> y{ip2,fp2};
    auto p = x*y;
//   std::cerr << x << " * " << y << " = " << p << '\n';

    CHECK_TRUE(p.ndecimals == std::max(x.ndecimals, y.ndecimals),
               "operator*");
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
    CHECK_TRUE(ip == ipr and fp == fpr, "operator/");
}



void test_decimal_arithmetic_decimal()
{
    using Dec = atd::Decimal<int, 2>;

// arithmetic
    {
    Dec x{3,14};
    ++x;
    CHECK_TRUE((x == Dec{4,14}), "++x");

    --x;
    CHECK_TRUE((x == Dec{3,14}), "--x");

    auto y = x++;
    CHECK_TRUE((x == Dec{4,14}), "x++");
    CHECK_TRUE((y == Dec{3,14}), "x++");

    auto z = x--;
    CHECK_TRUE((x == Dec{3,14}), "x--");
    CHECK_TRUE((z == Dec{4,14}), "x--");
    
    
    }

    Dec a{3,14};
    Dec b{2,45};

    a += b;
    CHECK_TRUE((a == Dec{5,59}), "a += b");
    
    a -= b;
    CHECK_TRUE((a == Dec{3,14}), "a -= b");

    a *= b;
    CHECK_TRUE((a == Dec{7,69}), "a *= b");

    a = Dec{4,82};
    b = Dec{2,1};
    a /= b;
    CHECK_TRUE((a == Dec{2,29}), "a /= b");

    {// casos que podrían dar overflow (con una naive implementation)
	using Int = atd::Decimal<uint8_t, 1>;
	Int a{5}; // interno = 50
	Int b{5}; // interno = 50
	b *= a;   // interno = 50*50 = 2500 , overflow
		  // pero el resultado es 25.0, que entra en uint8_t.
	CHECK_TRUE(b == Int{25}, "a *= b");

	a = Int{4,3};
	b = Int{2};
	a /= b;
	CHECK_TRUE((a == Int{2,1}), "a /= b");

    }

    
// Operaciones binarias
    a = Dec{3,14};
    b = Dec{2,45};
    Dec c = a + b;
    CHECK_TRUE((c == Dec{5,59}), "a + b");

    c = a - b;
    CHECK_TRUE((c == Dec{0,69}), "a - b");



// Producto
    test_decimal_multiplication<2,2>(3,14   , 2,75  , 8,63);
    test_decimal_multiplication<1,5>(10,2   , 7,12345,	72,65919);
    
    {// bug
	using Int = atd::Decimal<int, 3>;
	Int a{1};
	Int b{1000000};
	Int res{1000000};
	// std::cout << a << " * " << b << " = " << a*b << '\n';
	CHECK_TRUE(a*b == res, "operator*");
    }
    test_decimal_multiplication<3,3>(1,0, 1000000,0,	1000000,0);

// División
    test_decimal_division<1,5>(10,2   , 7,12345,    1,43189);
    test_decimal_division<0,0>(10,0   , 2,0,	    5,0);
    test_decimal_division<2,1>(3,14   , 2, 3,	    1,36);

    {// bug: overflow!!!
	using Int = atd::Decimal<int, 3>;
	Int a {12345};
	Int b {100};

	CHECK_TRUE(((a / b) == Int{123,450}), "operator/");
    }

    {// inverse
	using Int = atd::Decimal<int, 3>;
	CHECK_TRUE(((1 / Int{100}) == Int::significand(10)), "operator/");
	CHECK_TRUE(((1 / Int{2,34}) == Int{0,427}), "operator/");
	// f = 2 MHz ==> T = 0.5 ms:
	CHECK_TRUE(((1 / Int{2}) == Int{0,5}), "operator/");
    }

}

void test_decimal_arithmetic_escalar()
{
    using Dec = atd::Decimal<int, 2>;

// operator ?=
// -----------
    Dec a{2,81};
    a += 3;
    CHECK_TRUE(a.significand() == 581, "a += 3");

    a -= 3;
    CHECK_TRUE(a.significand() == 281, "a -= 3");

    a *=2;
    CHECK_TRUE(a.significand() == 281*2, "a *=2");

    a /= 2;
    CHECK_TRUE(a.significand() == 281, "a /= 2");


// decimal ? rep
// -------------
    a = Dec{5,4};

    Dec b = a + 3;
    CHECK_TRUE((b == Dec{8,4}), "Decimal + int");

    b = 3 + a;
    CHECK_TRUE((b == Dec{8,4}), "int + Decimal");

    b = a + 0;
    CHECK_TRUE((b == a), "Decimal + int");

    b = a - 3;
    CHECK_TRUE((b == Dec{2,4}), "Decimal - int");

    b = 3 - a;
    CHECK_TRUE((b == Dec{-2,4}), "int - Decimal");

    b = a - 0;
    CHECK_TRUE((b == a), "Decimal - int");

    b = 2*a;
    CHECK_TRUE((b == Dec{10,8}), "Decimal*int");

    b = a*2;
    CHECK_TRUE((b == Dec{10,8}), "int*Decimal");

    b = a / 2;
    CHECK_TRUE((b == Dec{2,7}), "Decimal / int");

    b = 2 / a;
    CHECK_TRUE((b == Dec{0,37}), "int / Decimal");


    {// mezcla
	Dec a{10};
	Dec b{2};
	auto c = a / b - 1;
	int d = atd::to_integer<Dec::Rep>(c);
	CHECK_TRUE(d == 4, "a / b - 1");

    }
}

void test_decimal_arithmetic()
{
    test_decimal_arithmetic_decimal();
    test_decimal_arithmetic_escalar();
}

// ¿Qué castings puedo hacer?
// 1. De signo: (explicitos)
//	unsigned -> int
//	int      -> unsigned
//
// 2. De núm. bits:
//      8 -> 16 (implicito)
//      16-> 8  (explícito)
//
// 3. De núm. decimals: (explitico)
//      2 -> 3 
//      3 -> 2
void test_decimal_cast()
{
    using uDec8_1 = atd::Decimal<uint8_t, 1>;
    using uDec16_1 = atd::Decimal<uint16_t, 1>;
    using uDec16_2 = atd::Decimal<uint16_t, 2>;
    using Dec16_2 = atd::Decimal<int16_t, 2>;

// De signo:
{
    uDec16_2 x1{2,34};
    // 1. unsigned --> signed
    CHECK_DONT_COMPILE(Dec16_2 x2 = x1, "cast: uint -> int");
    Dec16_2 x3 = atd::decimal_cast<Dec16_2>(x1);
    CHECK_TRUE((x3 == Dec16_2{2,34}), "decimal_cast");

    // 2. signed --> unsigned
    CHECK_DONT_COMPILE(uDec16_2 x4 = x3, "cast: int -> uint");
    uDec16_2 x5 = atd::decimal_cast<uDec16_2>(x3);
    CHECK_TRUE((x5 == uDec16_2{2,34}), "decimal_cast");

}

// De núm bits:
{
    // 8 --> 16
    uDec8_1 x1{2,3};
    uDec16_1 x2 = x1;
    CHECK_TRUE((x2 == uDec16_1{2,3}), "implicit cast");

    // 16 --> 8
    CHECK_DONT_COMPILE(uDec8_1 x3 = x2, "cast: 16 --> 8");
    uDec8_1 x4 = atd::decimal_cast<uDec8_1>(x2);
    CHECK_TRUE((x4 == uDec8_1{2,3}), "decimal_cast");
}

// de núm. decimals:
{
    uDec16_1 x1{2,3};
    CHECK_DONT_COMPILE(uDec16_2 x2 = x1, "cast: ndecimals 1 -> 2");
    uDec16_2 x3 = atd::decimal_cast<uDec16_2>(x1);
    CHECK_TRUE((x3 == uDec16_2{2,3}), "decimal_cast");
}

}



//void test_to_integer()
//{
//    using Dec8_1 = atd::Decimal<uint8_t, 1>;
//
//    Dec8_1 d{10,3};
//    CHECK_TRUE(atd::to_integer<uint8_t>(d) == 10, "to_integer");
//    CHECK_TRUE(atd::to_integer<long>(d) == 10UL, "to_integer");
//
//{// bug
//    using Dec1 = atd::Decimal<uint64_t, 3>;
//    using Dec2 = atd::Decimal<uint32_t, 3>;
//    Dec1 from{953, 67};
//    Dec2 to = atd::to_integer<Dec2>(from);
//    std::cout << from << " --> " << to << '\n';
//}
//
//}

void test_numerics()
{
    {
	using Int = atd::Decimal<uint8_t, 2>;
	CHECK_TRUE(Int::min() == Int{0}, "min");
	CHECK_TRUE((Int::max() == Int{2,55}), "max");
    }
    {
	using Int = atd::Decimal<int8_t, 2>;
	CHECK_TRUE((Int::min() == Int{-1,28}), "min");
	CHECK_TRUE((Int::max() == Int{1,27}), "max");
    }
    {
	using Int = atd::Decimal<uint16_t, 2>;
	CHECK_TRUE(Int::min() == Int{0}, "min");
	CHECK_TRUE((Int::max() == Int{655,35}), "max");
    }
    {
	using Int = atd::Decimal<int16_t, 2>;
	CHECK_TRUE((Int::min() == Int{-327,68}), "min");
	CHECK_TRUE((Int::max() == Int{327,67}), "max");
    }

}

template <typename Rep>
void test_traits(bool res)
{
    CHECK_TRUE((std::is_signed_v<atd::Decimal<Rep, 2>> == res), "is_signed_v");
}
void test_traits()
{
    test_traits<int8_t>(true);
    test_traits<int16_t>(true);
    test_traits<int32_t>(true);
    test_traits<int64_t>(true);

    test_traits<uint8_t>(false);
    test_traits<uint16_t>(false);
    test_traits<uint32_t>(false);
    test_traits<uint64_t>(false);

    CHECK_TRUE(atd::is_decimal_v<atd::Decimal<int, 3>>, "is_decimal_v");
    CHECK_TRUE(Type::Decimal<atd::Decimal<int, 3>>, "Decimal");
}

template <typename T, int n, typename Res>
void test_multiply_decimal_type_by_the_to_the(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<atd::multiply_decimal_type_by_ten_to_the_t<T, n>,
			    Res>,
		    alp::as_str() << "multiply_decimal_type_by_ten_to_the_t("
						<< name_type << ")");

}
void test_multiply_decimal_type_by_the_to_the()
{
    test::interface("multiply_decimal_type_by_ten_to_the");
    test_multiply_decimal_type_by_the_to_the<float, 3, float>("float");
    test_multiply_decimal_type_by_the_to_the<atd::Decimal<uint8_t, 2>, 0, 
					     atd::Decimal<uint8_t, 2>>
							("Decimal<uint8_t, 2>");
    test_multiply_decimal_type_by_the_to_the<atd::Decimal<uint8_t, 2>, 1, 
					     atd::Decimal<uint8_t, 1>>
							("Decimal<uint8_t, 2>");
    test_multiply_decimal_type_by_the_to_the<atd::Decimal<uint8_t, 2>, 2, 
					     atd::Decimal<uint8_t, 0>>
							("Decimal<uint8_t, 2>");
    test_multiply_decimal_type_by_the_to_the<atd::Decimal<uint8_t, 2>, 3, 
					     atd::Decimal<uint16_t, 0>>
							("Decimal<uint8_t, 2>");

    
}


void test_decimal()
{
    test::interface("Decimal");

    test_decimal_construct();
    test_decimal_common_type();
    test_decimal_order();
    test_decimal_arithmetic();
    test_decimal_cast();
//    test_to_integer();
    test_numerics();
    test_traits();
    test_multiply_decimal_type_by_the_to_the();

// operator<<
    {
    atd::Decimal<int, 2> a{922,6}; // <--- ESTO ES CONFUSO!!!
    CHECK_STDOUT(a, "922.06");

    atd::Decimal<int, 0> b{876};
    CHECK_STDOUT(b, "876");
    }

// BUGS
// ----
    {
	// CHECK_DONT_COMPILE:
//	atd::Decimal<uint8_t, 1> a{12, 3};
//	uint16_t x = 100;
//	auto b = a* x;
//	std::cout << b << '\n';

	atd::Decimal<uint32_t, 1> a{12, 3};
	uint16_t x = 100;
	auto b = a* x;
	std::cout << b << '\n';
    }
}




int main()
{
try{
    test::header("atd_decimal");

    test_decimal();


}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





