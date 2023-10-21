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

#include <iostream>
#include "../../atd_sci_number.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;

template <Type::Integer Rep>
void print(const atd::Sci_number<Rep>& x, bool print_return = true)
{ 
    std::cout << x;
    if (print_return)
	std::cout << "\n"; 
}

template <typename Rep, typename Int>
void test_constructor(const Int& x, const Rep& res, int exp)
{
    atd::Sci_number<Rep> s{x};
    CHECK_TRUE(s.significand() == res and s.exponent() == exp, "constructor");
}

template <typename Rep, typename Int>
void test_constructor_decimal(const Int& integer_part, const Int& decimal_part, 
				    const Rep& res, int exp)
{
    atd::Sci_number<Rep> s{integer_part, decimal_part};
    CHECK_TRUE(s.significand() == res and s.exponent() == exp, 
	    alp::as_str() << "constructor_decimal(" << (int) integer_part
			<< "." << (int) decimal_part << ")");
}

void test_constructor()
{
    test::interface("constructor");
    
    test_constructor<uint8_t>(0, 0, 0);
    test_constructor<uint8_t>(120, 120, 0);
    test_constructor<uint8_t>(300, 30, 1);
    test_constructor<uint8_t>(210'000, 210, 3);

    test_constructor<int8_t>(-120, -120, 0);
    test_constructor<int8_t>(-300, -30, 1);
    test_constructor<int8_t>(-210'000, -21, 4);

    test_constructor<uint16_t>(-123, 0, 0);

    test_constructor_decimal<uint8_t>(0,0, 0, 0);
    test_constructor_decimal<uint8_t>(0,23, 23, -2);
    test_constructor_decimal<uint8_t>(0,1, 1, -1);
    test_constructor_decimal<uint8_t>(1,23, 123, -2);
    test_constructor_decimal<uint8_t>(12,3, 123, -1);
    test_constructor_decimal<uint8_t>(20,3, 203, -1);
    test_constructor_decimal<uint8_t>(123,45, 123, 0);
    test_constructor_decimal<uint8_t>(1,3, 13, -1);
    // overflow:
    test_constructor_decimal<uint8_t>(0,25567, 255, -3);
    test_constructor_decimal<uint8_t>(0,25667, 25, -2);

    test_constructor_decimal<int8_t>(1,23, 123, -2);
    test_constructor_decimal<int8_t>(12,3, 123, -1);
    test_constructor_decimal<int8_t>(20,3, 20, 0);
    test_constructor_decimal<int8_t>(123,45, 123, 0);

    test_constructor_decimal<int8_t>(-1,23, -123, -2);
    test_constructor_decimal<int8_t>(-12,3, -123, -1);
    test_constructor_decimal<int8_t>(-20,3, -20, 0);
    test_constructor_decimal<int8_t>(-123,45, -123, 0);

    test_constructor_decimal<uint16_t>(12345,6789, 12345, 0);
    test_constructor_decimal<uint16_t>(1234,56789, 12345, -1);
    test_constructor_decimal<uint16_t>(123,456789, 12345, -2);
    test_constructor_decimal<uint16_t>(12,3456789, 12345, -3);
    test_constructor_decimal<uint16_t>(1,23456789, 12345, -4);
    test_constructor_decimal<uint16_t>(0,123456789, 12345, -5);
    test_constructor_decimal<uint16_t>(0ul,6987654321ul, 6987, -4);

    {
    auto x = atd::Sci_number<uint8_t>(3).E(4);
    CHECK_TRUE(x.significand() == 3 and x.exponent() == 4, "E");
    }
    {
    auto x = atd::Sci_number<uint8_t>(3).E(-4);
    CHECK_TRUE(x.significand() == 3 and x.exponent() == -4, "E");
    }
    {// not explicit
    atd::Sci_number<uint8_t> x = 1'000'000u;
    CHECK_TRUE(x == 1'000'000u, "not explicit");
    }
    {// explicit
    atd::Sci_number<int8_t> x{uint8_t{250}};
    CHECK_TRUE(x == 250, "not explicit");
    }
}

void test_comparison()
{
    test::interface("operator==");

    // Observar que, como solo trabajamos con 2 cifras significativas (o
    // menores que 256), se redondean los números:
    CHECK_TRUE(atd::Sci_number<uint8_t>{314} != 314, "operator!=");
    CHECK_TRUE(atd::Sci_number<uint8_t>{314} == 310, "operator==");
    CHECK_TRUE(atd::Sci_number<uint8_t>{314} == atd::Sci_number<uint8_t>{310}, "operator==");

    CHECK_TRUE(atd::Sci_number<uint8_t>{300'000} == 300'000, "operator==");

    CHECK_TRUE(atd::Sci_number<int8_t>{-300'000} == -300'000, "operator==");

    CHECK_TRUE(atd::Sci_number<int8_t>(20).E(-4) == 
	       atd::Sci_number<int8_t>(2).E(-3), "operator==");
    CHECK_TRUE(atd::Sci_number<int8_t>(2).E(-3) == 
	       atd::Sci_number<int8_t>(20).E(-4), "operator==");
}

template <typename Rep, typename Int>
void test_less_than(const Int& a, const Int& b)
{
    std::cout << a << " < " << b << "?\n";

    CHECK_TRUE(atd::Sci_number<Rep>{a} < atd::Sci_number<Rep>{b}, "\t<");
    CHECK_TRUE(atd::Sci_number<Rep>{a} <= atd::Sci_number<Rep>{b}, "\t<=");
    CHECK_TRUE(!(atd::Sci_number<Rep>{a} > atd::Sci_number<Rep>{b}), "\t>");
    CHECK_TRUE(!(atd::Sci_number<Rep>{a} >= atd::Sci_number<Rep>{b}), "\t>=");
}

void test_order()
{
    test::interface("order");

    test_less_than<uint8_t>(0, 20);
    test_less_than<uint8_t>(10, 20);
    test_less_than<uint8_t>(10, 100);
    test_less_than<uint8_t>(300, 1200);

    test_less_than<int8_t>(0, 20);
    test_less_than<int8_t>(-10, 0);
    test_less_than<int8_t>(-10, 20);
    test_less_than<int8_t>(-10, 100);
    test_less_than<int8_t>(-300, 1200);

    test_less_than<uint16_t>(123, 897689);
    test_less_than<uint16_t>(-123, 897689);

}

template <typename Rep, typename Int>
void test_addition(const Int& a0, const Int& b0, const Int& c)
{
    atd::Sci_number<Rep> a{a0};
    atd::Sci_number<Rep> b{b0};
    CHECK_TRUE(a + b == c, 
	    alp::as_str() << (int) a0 << " + " << (int) b0);

}

template <typename Rep, typename Int>
void test_addition_decimal(const Int& i0, const Int& f0,
		   const Int& i1, const Int& f1,
		   const Int& sig, int exp)
{
    atd::Sci_number<Rep> a{i0, f0};
    atd::Sci_number<Rep> b{i1, f1};
    auto c = a + b;
print(c);
    CHECK_TRUE(c.significand() ==  sig and
	    c.exponent() == exp, 
	    alp::as_str() << (int) i0 << "." << (int) f0 
		<< " + " <<  (int) i1 << "." << (int) f1);

}
void test_addition()
{
    test::interface("operator+");

    test_addition<uint8_t>(0, 0, 0);
    test_addition<uint8_t>(0, 250, 250);
    test_addition<uint8_t>(0, 300, 300);

    test_addition<uint8_t>(25, 30, 55);
    test_addition<uint8_t>(300, 130, 430);
    test_addition<uint8_t>(1300, 132, 1430);
    test_addition<uint8_t>(12300, 243, 12500);
    test_addition<uint8_t>(123000, 243, 123000);

    test_addition_decimal<uint8_t>(12,3, 1,43, 137, -1);
    test_addition_decimal<uint8_t>(0,3, 10,0, 103, -1);
    test_addition_decimal<uint8_t>(1,3, 0,12, 142, -2);

    test_addition_decimal<int8_t>(-1,3, 0,12, -118, -2);
    test_addition_decimal<int8_t>(0,123, 0,123, 24, -2);

    test_addition_decimal<int16_t>(10,123, 234,1, 24422 , -2);
    test_addition_decimal<int16_t>(-1,256, -45,1, -4635, -2);

}


template <typename Rep, typename Int>
void test_substraction(const Int& a0, const Int& b0, const Int& c)
{
    atd::Sci_number<Rep> a{a0};
    atd::Sci_number<Rep> b{b0};
    CHECK_TRUE(a - b == c, 
	    alp::as_str() << (int) a0 << " + " << (int) b0);

}
void test_substraction()
{
    test::interface("operator+");

    test_substraction<uint8_t>(0, 0, 0);
    test_substraction<uint8_t>(250, 0, 250);
    test_substraction<uint8_t>(300, 0, 300);

    test_substraction<uint8_t>(30, 25, 5);
    test_substraction<uint8_t>(300, 130, 170);
    test_substraction<uint8_t>(1300, 132, 1170);
    test_substraction<uint8_t>(12300, 243, 12100);
    test_substraction<uint8_t>(123000, 243, 123000);
}



template <typename Rep, typename Int>
void test_operator_minus(const Int& x0)
{
    atd::Sci_number<Rep> x{x0};
    atd::Sci_number<Rep> y = -x;
    CHECK_TRUE(y == -x0, alp::as_str() << "-" << x0);
}

void test_operator_minus()
{
    test::interface("operator-(a)");
    
    test_operator_minus<int8_t>(0);
    test_operator_minus<int8_t>(12);
    test_operator_minus<int8_t>(123);
    test_operator_minus<int8_t>(1230);
    test_operator_minus<int8_t>(123000);

    test_operator_minus<int8_t>(-12);
    test_operator_minus<int8_t>(-123);
    test_operator_minus<int8_t>(-1230);
    test_operator_minus<int8_t>(-123000);

    test_operator_minus<int16_t>(0);
    test_operator_minus<int16_t>(12);
    test_operator_minus<int16_t>(123);
    test_operator_minus<int16_t>(1234);
    test_operator_minus<int16_t>(123450);

    test_operator_minus<int16_t>(-12);
    test_operator_minus<int16_t>(-123);
    test_operator_minus<int16_t>(-1234);
    test_operator_minus<int16_t>(-123450);

    test_operator_minus<int32_t>(1234567890);
    test_operator_minus<int32_t>(-1234567890);
}


template <typename Rep, typename Int>
void test_multiplication(const Int& a0, const Int& b0, const Int& res)
{
    atd::Sci_number<Rep> a{a0};
    atd::Sci_number<Rep> b{b0};
    auto c = a * b;
    CHECK_TRUE(c == res, 
	    alp::as_str() << (int) a0 << " * " << (int) b0);

}


template <typename Rep, typename Int>
void test_multiplication_decimal(const Int& a0, const Int& f0, 
				 const Int& a1, const Int& f1,
				 const Int& sig, int exp)
{
    atd::Sci_number<Rep> a{a0, f0};
    atd::Sci_number<Rep> b{a1, f1};
    auto c = a * b;
print(c);
    CHECK_TRUE(c.significand() == sig and c.exponent() == exp,
	    alp::as_str() << (int) a0 << '.' << (int) f0 << " * " 
	                  << (int) a1 << '.' << (int) f1);

}

void test_multiplication()
{
    test::interface("multiplication");

    test_multiplication<uint8_t>(12, 5, 60);
    test_multiplication<uint8_t>(12, 34, 400); // 408
    test_multiplication<uint8_t>(123'000ul, 25'000ul, 3'000'000'000ul);
									
    test_multiplication<int8_t>(12, -5, -60);
    test_multiplication<int8_t>(-12, -34, 400); // 408

    test_multiplication_decimal<uint8_t>(2,3, 4,5, 103, -1);

    // Observar que mantiene el número de cifras signficativas:
    test_multiplication_decimal<uint8_t>(0,12, 10,0, 120, -2);
    test_multiplication_decimal<uint8_t>(0,12, 100,0, 120, -1);
}


template <typename Rep, typename Int>
void test_division(const Int& a0, const Int& b0, const Int& res)
{
    atd::Sci_number<Rep> a{a0};
    atd::Sci_number<Rep> b{b0};
    auto c = a / b;
    CHECK_TRUE(c == res, 
	    alp::as_str() << (int) a0 << " / " << (int) b0);

}


template <typename Rep, typename Int>
void test_division_decimal(const Int& a0, const Int& f0, 
				 const Int& a1, const Int& f1,
				 const Int& sig, int exp)
{
    atd::Sci_number<Rep> a{a0, f0};
    atd::Sci_number<Rep> b{a1, f1};
    auto c = a / b;
    CHECK_TRUE(c.significand() == sig and c.exponent() == exp,
	    alp::as_str() << (int) a0 << '.' << (int) f0 << " / " 
	                  << (int) a1 << '.' << (int) f1);

}

void test_division()
{
    test::interface("division");

    test_division<uint8_t>(0, 5, 0);
    test_division<uint8_t>(60, 5, 12);
    test_division<uint8_t>(60'000, 5, 12'000);

    test_division<int8_t>(-60'000, 5, -12'000);


    // Observar que el número de cifras significativas que devuelve es
    // un poco a voleo (aunque el resultado es correcto, que de momento es lo
    // que importa)
    test_division_decimal<uint8_t>(1,0, 2,0, 50, -2);
    test_division_decimal<uint8_t>(1,0, 5,0, 200, -3);
}

int main()
{
try{
    test::header("atd_Sci_number");

    test_constructor();
    test_comparison();
    test_order();
    test_operator_minus();
    test_addition();
    test_substraction();
    test_multiplication();
    test_division();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





