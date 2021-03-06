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

#include "../../atd_eng_notation.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


void test_basic()
{
    test::interfaz("basic");

    using Meter = atd::Meter<double>;
    using Kilometer = atd::Kilometer<double>;

    {
    Meter m0{20000};
    atd::Magnitude_ENG_notation m{m0};

    CHECK_TRUE(m.value() == 20.0, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    ++m;
    CHECK_TRUE(m.value() == 21.0, "operator++");
    CHECK_TRUE(m.exponent() == 3, "operator++");
    std::cout << m << '\n';

    --m;
    CHECK_TRUE(m.value() == 20.0, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    Meter m1b{30000};
    atd::Magnitude_ENG_notation m1{m1b};
    CHECK_TRUE(m1.value() == 30.0, "constructor");
    CHECK_TRUE(m1.exponent() == 3, "constructor");
    m += m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.value() == 50.0, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");
    m -= m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.value() == 20.0, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");

    }
    {
    Kilometer x0{999};
    atd::Magnitude_ENG_notation x{x0};
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 999, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    ++x;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 1, "operator++");
    CHECK_TRUE(x.exponent() == 6, "operator++");

    --x;
    std::cout << x << '\n';
    std::cout << x.value() << '\n';
    CHECK_TRUE(x.value() == 999, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    Kilometer x1b{1};
    atd::Magnitude_ENG_notation x1{x1b};
    CHECK_TRUE(x1.value() == 1, "constructor");
    CHECK_TRUE(x1.exponent() == 3, "constructor");

    x += x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 1.0, "constructor");
    CHECK_TRUE(x.exponent() == 6, "constructor");

    x -= x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.value() == 999.0, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    }
    {
    Kilometer x0{999.99};
    atd::Magnitude_ENG_notation x{x0};
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
    atd::Magnitude_ENG_notation x1{x1b};
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
//    atd::Magnitude_ENG_notation eng1{bad};
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
    atd::Magnitude_ENG_notation x{Kilometer{100}};
    CHECK_STDOUT(x, "100 km");
    }
    {
    atd::Magnitude_ENG_notation x{MegaHertz{100}};
    CHECK_STDOUT(x, "100 MHz");
    }
    {
    atd::Magnitude_ENG_notation x{Microsecond{100}};
    CHECK_STDOUT(x, "100 us");
    }
    {
    atd::Magnitude_ENG_notation x{Kelvin{100}};
    CHECK_STDOUT(x, "100 K");
    }
    {
    atd::Magnitude_ENG_notation x{Pascal{100}};
    CHECK_STDOUT(x, "100 Pa");
    }
}

void test_order()
{
    using Hertz = atd::Hertz<double>;
    using KiloHertz = atd::KiloHertz<double>;

    {
    atd::Magnitude_ENG_notation a{Hertz{100}};
    atd::Magnitude_ENG_notation b{Hertz{200}};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    atd::Magnitude_ENG_notation a{Hertz{100}};
    atd::Magnitude_ENG_notation b{KiloHertz{200}};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    atd::Magnitude_ENG_notation a{Hertz{1000}};
    atd::Magnitude_ENG_notation b{KiloHertz{1}};
    CHECK_TRUE(!(a < b), "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(!(b > a), "operator>");
    }
}


int main()
{
try{
    test::header("atd_eng_notation");


    test_basic();
    test_order();
    test_symbol();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





