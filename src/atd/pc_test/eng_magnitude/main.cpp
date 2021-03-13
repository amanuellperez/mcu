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

#include "../../atd_eng_magnitude.h"
#include "../../atd_decimal.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


template <typename Rep>
void test_basic()
{
    test::interfaz("basic");

    using Meter = atd::Meter<Rep>;
    using Kilometer = atd::Kilometer<Rep>;

    {
    Meter m0{20000};
    atd::ENG_Magnitude m{m0};

    CHECK_TRUE(m.value() == Rep{20}, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    ++m;
    CHECK_TRUE(m.value() == Rep{21}, "operator++");
    CHECK_TRUE(m.exponent() == 3, "operator++");
    std::cout << m << '\n';

    --m;
    CHECK_TRUE(m.value() == Rep{20}, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    Meter m1b{30000};
    atd::ENG_Magnitude m1{m1b};
    CHECK_TRUE(m1.value() == Rep{30}, "constructor");
    CHECK_TRUE(m1.exponent() == 3, "constructor");
    m += m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.value() == Rep{50}, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");
    m -= m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.value() == Rep{20}, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");

    }
    {
    Kilometer x0{999};
    atd::ENG_Magnitude x{x0};
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    ++x;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == Rep{1}, "operator++");
    CHECK_TRUE(x.exponent() == 6, "operator++");

    --x;
    std::cout << x << '\n';
    std::cout << x.value() << '\n';
    CHECK_TRUE(x.value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    Kilometer x1b{1};
    atd::ENG_Magnitude x1{x1b};
    CHECK_TRUE(x1.value() == Rep{1}, "constructor");
    CHECK_TRUE(x1.exponent() == 3, "constructor");

    x += x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == Rep{1}, "constructor");
    CHECK_TRUE(x.exponent() == 6, "constructor");

    x -= x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    }
}

void test_double()
{
    test::interfaz("double");

    using Kilometer = atd::Kilometer<double>;

    {// caso extremo
    Kilometer x0{999.99};
    atd::ENG_Magnitude x{x0};
    std::cout << ":: " << x << '\n';
    CHECK_TRUE(x.value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    ++x;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 1.00099, "operator++");
    CHECK_TRUE(x.exponent() == 6, "operator++");

    std::cout << "antes= " << x.value() << '\n';
    --x;
    std::cout << x << '\n';
    std::cout << x.value() << '\n';
    CHECK_TRUE(x.value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    Kilometer x1b{1};
    atd::ENG_Magnitude x1{x1b};
    CHECK_TRUE(x1.value() == 1, "constructor");
    CHECK_TRUE(x1.exponent() == 3, "constructor");

    x += x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 1.00099, "constructor");
    CHECK_TRUE(x.exponent() == 6, "constructor");

    x -= x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");
    }
//    { // Esto no tiene que compilar:
//    using Not_decimal = atd::Magnitude<atd::Units_length, int, std::ratio<1, 2>>;
//    Not_decimal bad{20};
//    atd::ENG_Magnitude eng1{bad};
//    }
}

void test_symbol()
{
    using Kilometer = atd::Kilometer<double>;
    using MegaHertz = atd::MegaHertz<double>;
    using Microsecond = atd::Microsecond<double>;
    using Kelvin = atd::Kelvin<double>;
    using Pascal = atd::Pascal<double>;

    {
    atd::ENG_Magnitude x{Kilometer{100}};
    CHECK_STDOUT(x, "100 km");
    }
    {
    atd::ENG_Magnitude x{MegaHertz{100}};
    CHECK_STDOUT(x, "100 MHz");
    }
    {
    atd::ENG_Magnitude x{Microsecond{100}};
    CHECK_STDOUT(x, "100 us");
    }
    {
    atd::ENG_Magnitude x{Kelvin{100}};
    CHECK_STDOUT(x, "100 K");
    }
    {
    atd::ENG_Magnitude x{Pascal{100}};
    CHECK_STDOUT(x, "100 Pa");
    }
}

template <typename Rep>
void test_order()
{
    using Hertz = atd::Hertz<Rep>;
    using KiloHertz = atd::KiloHertz<Rep>;

    {
    atd::ENG_Magnitude a{Hertz{100}};
    atd::ENG_Magnitude b{Hertz{200}};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    atd::ENG_Magnitude a{Hertz{100}};
    atd::ENG_Magnitude b{KiloHertz{200}};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    atd::ENG_Magnitude a{Hertz{1000}};
    atd::ENG_Magnitude b{KiloHertz{1}};
    CHECK_TRUE(!(a < b), "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(!(b > a), "operator>");
    }
}

void test_to_eng_notation()
{
    using Hertz0 = atd::Hertz<atd::Decimal<uint16_t, 2>>;
    using Hertz1 = atd::Hertz<double>;

    using ENG_frequency = atd::ENG_frequency<uint8_t>;

    Hertz0 f0{Hertz0::Rep{120, 13}};
    ENG_frequency e0 = atd::to_eng_notation<uint8_t>(f0);
    CHECK_TRUE(e0.value() == 120, "to_eng_notation");

    Hertz1 f1{234.67};
    ENG_frequency e1 = atd::to_eng_notation<uint8_t>(f1);
    CHECK_TRUE(e1.value() == 234, "to_eng_notation");

    auto e2 = atd::to_eng_notation(f0);
    CHECK_TRUE((e2.value() == Hertz0::Rep{120,13}), "to_eng_notation");


}


int main()
{
try{
    test::header("atd_eng_notation");

    test_basic<double>();
    test_double();
    test_basic<atd::Decimal<int,2>>();
    test_order<double>();
    test_order<atd::Decimal<int,2>>();
    test_to_eng_notation();
    test_symbol();


}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





