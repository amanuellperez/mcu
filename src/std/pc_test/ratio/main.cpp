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

#include "../../std_ratio.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <numeric>
#include <ratio>

using namespace test;

namespace mtd{
template <intmax_t a, intmax_t b>
inline std::ostream& operator<<(std::ostream& out,
                                          const mtd::ratio<a, b>& q)
{
    return out << q.num << '/' << q.den;
}
}

template <intmax_t a, intmax_t b>
constexpr void test_static_gcd()
{
    std::integral_constant<intmax_t, mtd::__static_gcd(a, b)> x;
    CHECK_TRUE(x.value == std::gcd(a, b),
               alp::as_str() << "__static_gcd(" << a << ", " << b << ")");
}

void test_static_gcd()
{
    test::interfaz("__static_gcd");
    
    test_static_gcd<1, 2>();
    test_static_gcd<2, 3>();
    test_static_gcd<10, 10>();
    test_static_gcd<20, 30>();

}   

template <intmax_t N, intmax_t D>
void test_ratio()
{
    mtd::ratio<N, D> r;
    std::ratio<N, D> sr;
    CHECK_TRUE(r.num == sr.num and r.den == sr.den,
               alp::as_str() << "ratio<" << N << ", " << D << ">");
}

void test_ratio()
{
    test::interfaz("ratio");

    test_ratio<2,3>();
    test_ratio<10,20>();
    test_ratio<7,7>();
//     test_ratio<7,0>(); // no debe de compilar
}

template <typename q1, typename q2>
void test_add()
{
    mtd::ratio_add<q1, q2> pru;
    using sq1 = std::ratio<q1::num, q1::den>;
    using sq2 = std::ratio<q2::num, q2::den>;
    std::ratio_add<sq1, sq2> res;

    CHECK_TRUE(pru.num == res.num and pru.den == res.den,
               alp::as_str() << "ratio_add<" << q1{} << ", " << q2{} << ">");
}


void test_add()
{
    test::interfaz("add");
    test_add<mtd::ratio<1, 2>, mtd::ratio<1,3>>();
    test_add<mtd::ratio<2, 2>, mtd::ratio<2,2>>();
    test_add<mtd::ratio<11, 2>, mtd::ratio<20,7>>();
}


template <typename q1, typename q2>
void test_subtract()
{
    mtd::ratio_subtract<q1, q2> pru;
    using sq1 = std::ratio<q1::num, q1::den>;
    using sq2 = std::ratio<q2::num, q2::den>;
    std::ratio_subtract<sq1, sq2> res;

    CHECK_TRUE(pru.num == res.num and pru.den == res.den,
               alp::as_str() << "ratio_subtract<" << q1{} << ", " << q2{} << ">");
}


void test_subtract()
{
    test::interfaz("subtract");
    test_subtract<mtd::ratio<1, 2>, mtd::ratio<1,3>>();
    test_subtract<mtd::ratio<2, 2>, mtd::ratio<2,2>>();
    test_subtract<mtd::ratio<11, 2>, mtd::ratio<20,7>>();
}


template <typename q1, typename q2>
void test_multiply()
{
    mtd::ratio_multiply<q1, q2> pru;
    using sq1 = std::ratio<q1::num, q1::den>;
    using sq2 = std::ratio<q2::num, q2::den>;
    std::ratio_multiply<sq1, sq2> res;

    CHECK_TRUE(pru.num == res.num and pru.den == res.den,
               alp::as_str() << "ratio_multiply<" << q1{} << ", " << q2{} << ">");
}


void test_multiply()
{
    test::interfaz("multiply");
    test_multiply<mtd::ratio<1, 2>, mtd::ratio<1,3>>();
    test_multiply<mtd::ratio<2, 2>, mtd::ratio<2,2>>();
    test_multiply<mtd::ratio<11, 2>, mtd::ratio<20,7>>();
}


template <typename q1, typename q2>
void test_divide()
{
    mtd::ratio_divide<q1, q2> pru;
    using sq1 = std::ratio<q1::num, q1::den>;
    using sq2 = std::ratio<q2::num, q2::den>;
    std::ratio_divide<sq1, sq2> res;

    CHECK_TRUE(pru.num == res.num and pru.den == res.den,
               alp::as_str() << "ratio_divide<" << q1{} << ", " << q2{} << ">");
}


void test_divide()
{
    test::interfaz("divide");
    test_divide<mtd::ratio<1, 2>, mtd::ratio<1,3>>();
    test_divide<mtd::ratio<2, 2>, mtd::ratio<2,2>>();
    test_divide<mtd::ratio<11, 2>, mtd::ratio<20,7>>();
}


template <typename q1, typename q2>
void test_equal(bool res)
{
    CHECK_TRUE((mtd::ratio_equal_v<q1, q2> == res),
               alp::as_str() << "ratio_equal(" << q1{} << ")");
}

void test_equal()
{
    test::interfaz("equal");

    test_equal<mtd::ratio<1,2>, mtd::ratio<1,2>>(true);
    test_equal<mtd::ratio<2,4>, mtd::ratio<1,2>>(true);
    test_equal<mtd::ratio<2,2>, mtd::ratio<1,1>>(true);
    test_equal<mtd::ratio<1,2>, mtd::ratio<1,1>>(false);
}

template <typename q1, typename q2>
void test_not_equal(bool res)
{
    CHECK_TRUE((mtd::ratio_not_equal_v<q1, q2> == res),
               alp::as_str() << "ratio_not_equal(" << q1{} << ")");
}

void test_not_equal()
{
    test::interfaz("not_equal");

    test_not_equal<mtd::ratio<1,2>, mtd::ratio<1,2>>(false);
    test_not_equal<mtd::ratio<2,4>, mtd::ratio<1,2>>(false);
    test_not_equal<mtd::ratio<2,2>, mtd::ratio<1,1>>(false);
    test_not_equal<mtd::ratio<1,2>, mtd::ratio<1,1>>(true);
}



int main()
{
try{
    test::header("ratio");

    test_static_gcd();
    test_ratio();
    test_add();
    test_subtract();
    test_multiply();
    test_divide();
    test_equal();
    test_not_equal();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





