// Copyright (C) 2021 Manuel Perez 
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

#include "../../atd_eng_magnitude.h"
#include "../../atd_decimal.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


bool equal(const double& a, const double& b)
{
    if (a <= b)
	return (b - a) < 0.01;

    else
	return (a - b) < 0.01;

}


void test_constexpr()
{
    test::interface("constexpr");

    using Freq = atd::ENG_frequency<int>;
    
    constexpr Freq f{321, 3};
    CHECK_TRUE(f.internal_value() == 321 and f.exponent() == 3, "constexpr");

}

void test_basic()
{
    test::interface("basic");
    using Rep      = double;
    using Freq     = atd::ENG_frequency<Rep>;
    using Length   = atd::ENG_length<Rep>;
    using Time     = atd::ENG_time<Rep>;
    using Velocity = atd::ENG_velocity<Rep>;

    // construction
    {
	constexpr Freq a{20, 3};
	CHECK_TRUE(a.internal_value() == 20.0 and a.exponent() == 3, "constructor");
	CHECK_TRUE(a == a, "operator==");
	CHECK_TRUE(!(a != a), "operator!=");
    }
    {
	Freq a{20, -3};
	CHECK_TRUE(a.internal_value() == 20.0 and a.exponent() == -3, "constructor");
    }
    {
	Freq a0{1, 0};
	CHECK_TRUE(a0.internal_value() == 1.0 and a0.exponent() == 0, "constructor");

	Freq a1{10, 0};
	CHECK_TRUE(a1.internal_value() == 10.0 and a1.exponent() == 0, "constructor");

	Freq a2{100, 0};
	CHECK_TRUE(a2.internal_value() == 100.0 and a2.exponent() == 0, "constructor");

	Freq a3{1000, 0};
	CHECK_TRUE(a3.internal_value() == 1.0 and a3.exponent() == 3, "constructor");

	Freq a4{10000, 0};
	CHECK_TRUE(a4.internal_value() == 10.0 and a4.exponent() == 3, "constructor");
    }

// a + b
    {
	Freq a{20,3};
	Freq b{30,3};
	Freq c = a + b;
	CHECK_TRUE(c.internal_value() == 50 and c.exponent() == 3, "operator+");
    }

    {
	Freq a{20,0};
	Freq b{30,3};
	Freq c = a + b;
	CHECK_TRUE(c.internal_value() == static_cast<Rep>(30.02) and c.exponent() == 3, "operator+");
    }
    {
	Freq a{20,0};
	Freq b{30,6};
	Freq c = a + b;
	CHECK_TRUE(c.internal_value() == (Rep) 30.00002 and c.exponent() == 6, "operator+");
    }
    {// extremo
	Freq a{1,0};
	Freq b{999,0};
	Freq c = a + b;
	CHECK_TRUE(c.internal_value() == 1.0 and c.exponent() == 3, "operator+");
    }
    {// extremo
	Freq a{1,0};
	Freq b{0,0};
	Freq c = a + b;
	CHECK_TRUE(c.internal_value() == 1.0 and c.exponent() == 0, "operator+");
    }



// a - b
    {
	Freq a{30,3};
	Freq b{20,3};
	Freq c = a - b;
	CHECK_TRUE(c.internal_value() == 10 and c.exponent() == 3, "operator-");
    }

    {
	Freq a{20,3};
	Freq b{30,3};
	Freq c = a - b;
	CHECK_TRUE(c.internal_value() == -10 and c.exponent() == 3, "operator-");
    }

    {
	Freq a{20,0};
	Freq b{30,3};
	Freq c = b - a;
	CHECK_TRUE(c.internal_value() == (Rep) 29.980 and c.exponent() == 3, "operator-");
    }
    {
	Freq a{20,0};
	Freq b{30,6};
	Freq c = b - a;
	CHECK_TRUE(c.internal_value() == (Rep) 29.999980 and c.exponent() == 6, "operator-");
    }
    {// extremo
	Freq a{1,0};
	Freq b{1,3};
	Freq c = b - a;
	CHECK_TRUE(c.internal_value() == 999.0 and c.exponent() == 0, "operator-");
    }
    {// extremo
	Freq a{0,0};
	Freq b{1,3};
	Freq c = b - a;
	CHECK_TRUE(c.internal_value() == 1.0 and c.exponent() == 3, "operator-");
    }

// a*x
    {
	Freq a{20,0};
	Freq c = 2*a;
	CHECK_TRUE(c.internal_value() == 40.0 and c.exponent() == 0, "operator*");
    }
    {
	Freq a{20,0};
	Freq c = 1000*a;
	CHECK_TRUE(c.internal_value() == 20.0 and c.exponent() == 3, "operator*");
    }
    {
	Freq a{900,3};
	Freq c = a*250;
	CHECK_TRUE(c.internal_value() == 225.0 and c.exponent() == 6, "operator*");
    }
    {// extremo
	Freq a{20,3};
	Freq c = a*1;
	CHECK_TRUE(c.internal_value() == 20.0 and c.exponent() == 3, "operator*");
    }

// a/x
    {
	Freq a{20,0};
        Freq c = a / 2;
        CHECK_TRUE(c.internal_value() == 10.0 and c.exponent() == 0, "operator/");
    }
    {
	Freq a{20,0};
	Freq c = a / 10;
	CHECK_TRUE(c.internal_value() == 2.0 and c.exponent() == 0, "operator/");
    }

    {
	Freq a{20,0};
	Freq c = a / 100;
	CHECK_TRUE(c.internal_value() == 200.0 and c.exponent() == -3, "operator/");
    }
    {
        Freq a{20,0};
	Freq c = a / 1000;
	CHECK_TRUE(c.internal_value() == 20.0 and c.exponent() == -3, "operator/");
    }
    {
        Freq a{20,0};
	Freq c = a / 10000;
	CHECK_TRUE(c.internal_value() == 2.0 and c.exponent() == -3, "operator/");
    }

    {
	Freq a{20,0};
	Freq c = a / 0.1;
	CHECK_TRUE(c.internal_value() == 200.0 and c.exponent() == 0, "operator/");
    }
    {
	Freq a{20,0};
	Freq c = a / 0.01;
	CHECK_TRUE(c.internal_value() == 2.0 and c.exponent() == 3, "operator/");
    }
    {
	Freq a{20,0};
	Freq c = a / 0.001;
	CHECK_TRUE(c.internal_value() == 20.0 and c.exponent() == 3, "operator/");
    }

    {
	Freq a{2,3};
	Freq c = a / 10000000;
	CHECK_TRUE(c.internal_value() == 200.0 and c.exponent() == -6, "operator/");
    }

    {
	Freq a{900,3};
        Freq c = a / 250;
        CHECK_TRUE(c.internal_value() == (Rep) 3.6 and c.exponent() == 3, "operator/");
    }
    {// extremo
	Freq a{20,3};
        Freq c = a / 1;
        CHECK_TRUE(c.internal_value() == 20.0 and c.exponent() == 3, "operator/");
    }

// a*b
    {
	Velocity v{30,3};
	Time t{20,3};
	Length s = v * t;
	CHECK_TRUE(s.internal_value() == 600 and s.exponent() == 6, "operator*");
    }
    {
	Velocity v{30,0};
	Time t{20,3};
	Length s = v * t;
	CHECK_TRUE(s.internal_value() == 600 and s.exponent() == 3, "operator*");
    }
    {
	Velocity v{30,0};
	Time t{20,-3};
	Length s = v * t;
	CHECK_TRUE(s.internal_value() == 600 and s.exponent() == -3, "operator*");
    }
    {// overflow?
	Velocity v{999,0};
	Time t{999,0};
	Length s = v * t;
	CHECK_TRUE(s.internal_value() == (Rep) 998.001 and s.exponent() == 3, "operator*");
    }
    {// overflow?
	Velocity v{-999,0};
	Time t{-999,0};
	Length s = v * t;
	CHECK_TRUE(s.internal_value() == (Rep) 998.001 and s.exponent() == 3, "operator*");
    }

// a/b
    {
	Length s{60,0};
	Time t{20,0};
        Velocity v = s / t;
        CHECK_TRUE(v.internal_value() == 3 and v.exponent() == 0, "operator/");
    }
    {// speed of light
	Length s{300,-3};
	Time t{1,-9};
        Velocity v = s / t;
        CHECK_TRUE(v.internal_value() == 300 and v.exponent() == 6, "operator/");
    }
    {// overflow?
	Length s{1,3};
	Time t{50,3};
        Velocity v = s / t;
        CHECK_TRUE(v.internal_value() == 20 and v.exponent() == -3, "operator/");
    }
    {// overflow?
	Length s{9,6};
	Time t{900,3};
        Velocity v = s / t;
        CHECK_TRUE(v.internal_value() == 10 and v.exponent() == 0, "operator/");
    }

// number / a
    {
	Freq f{1, 6};
	Time t = 1 / f;
        CHECK_TRUE(t.internal_value() == 1 and t.exponent() == -6, "operator/");
    }
    {
	Freq f{1, 6};
	Time t = 35 / f;
        CHECK_TRUE(t.internal_value() == 35 and t.exponent() == -6, "operator/");
    }
    {
	Freq f{900, 6};
	Time t = 9 / f;
        CHECK_TRUE(t.internal_value() == 10 and t.exponent() == -9, "operator/");
    }

}

template <typename Rep>
void test_basic2()
{
    test::interface("basic");

    using Freq     = atd::ENG_frequency<Rep>;
    using Length   = atd::ENG_length<Rep>;
    using Time     = atd::ENG_time<Rep>;
    using Velocity = atd::ENG_velocity<Rep>;

    {
    Length m{20, 3};

    CHECK_TRUE(m.internal_value() == Rep{20}, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    ++m;
    CHECK_TRUE(m.internal_value() == Rep{21}, "operator++");
    CHECK_TRUE(m.exponent() == 3, "operator++");
    std::cout << m << '\n';

    --m;
    CHECK_TRUE(m.internal_value() == Rep{20}, "constructor");
    CHECK_TRUE(m.exponent() == 3, "constructor");
    std::cout << m << '\n';

    Length m1{30, 3};
    CHECK_TRUE(m1.internal_value() == Rep{30}, "constructor");
    CHECK_TRUE(m1.exponent() == 3, "constructor");
    m += m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.internal_value() == Rep{50}, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");
    m -= m1;
    std::cout << m << '\n';
    CHECK_TRUE(m.internal_value() == Rep{20}, "operator+=");
    CHECK_TRUE(m.exponent() == 3, "operator+=");

    }
    {
    Length x{999, 3};
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    ++x;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == Rep{1}, "operator++");
    CHECK_TRUE(x.exponent() == 6, "operator++");

    --x;
    std::cout << x << '\n';
    std::cout << x.internal_value() << '\n';
    CHECK_TRUE(x.internal_value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    Length x1{1,3};
    CHECK_TRUE(x1.internal_value() == Rep{1}, "constructor");
    CHECK_TRUE(x1.exponent() == 3, "constructor");

    x += x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == Rep{1}, "constructor");
    CHECK_TRUE(x.exponent() == 6, "constructor");

    x -= x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == Rep{999}, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    }
    {
	Velocity v{20,0};
	Time t{900, 3};
        Length s = v * t;
	CHECK_TRUE(s.internal_value() == Rep{18} and s.exponent() == 6, "operator*");
    }
    {
	Freq f{1,6};
	Time t = 1 / f;
	CHECK_TRUE(t.internal_value() == Rep{1} and t.exponent() == -6, "operator/");
    }
    {
	Time t{30,0};
	Length s{60, 0};
        Velocity v = s / t;
	CHECK_TRUE(v.internal_value() == Rep{2} and v.exponent() == 0, "operator/");
    }
    {// operator freq/freq 
	Freq f1{1, 6};
	Freq f2{200, 3};
	Rep res = f1 / f2 + 1;
	CHECK_TRUE(res == Rep{6}, "operator/");
    }

// operaciones con números
    {// 1 / freq
	Freq f{1,6};
	Time t = 1 / f;
	CHECK_TRUE(t.internal_value() == Rep{1} and t.exponent() == -6, "operator/");
    }
    {// construimos como en avr
	constexpr unsigned long long int freq = 1000000ul;
	constexpr Freq f{freq, 0};
	CHECK_TRUE(f.internal_value() == Rep{1} and f.exponent() == 6, "constructor");
    }
    {// construimos como en avr
	unsigned long long int freq = 8000000ul;
	Freq f{freq, 0};
	CHECK_TRUE(f.internal_value() == Rep{8} and f.exponent() == 6, "constructor");
    }
}


void test_observers()
{
    test::interface("observers");

    using Potential = atd::ENG_electric_potential<double>;
    {
	Potential V{1};
	CHECK_TRUE(V.millivalue() == 1000, "millivalue");
    }
    {
	Potential V{1.234};
	CHECK_TRUE(V.millivalue() == 1234, "millivalue");
    }
    {
	Potential V{1.23456};
	CHECK_TRUE((equal(V.millivalue(), 1234.56)), "millivalue");
    }
    {
	Potential V{0.00456};
	CHECK_TRUE((equal(V.millivalue(), 4.56)), "millivalue");
    }
    {
	Potential V{123,6};
	CHECK_TRUE((equal(V.millivalue(), 123000000000)), "millivalue");
    }

    {
	using Rep = atd::Decimal<uint32_t, 3>;
	using DPotential = atd::ENG_electric_potential<Rep>;
	DPotential V{Rep{1,234}};
	CHECK_TRUE(V.millivalue() == Rep{1234}, "millivalue");

    }

}

void test_double()
{
    test::interface("double");

    using Length   = atd::ENG_length<double>;

    {// caso extremo
    Length x{999.99, 3};
    
    std::cout << ":: " << x << '\n';
    CHECK_TRUE(x.internal_value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    ++x;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == 1.00099, "operator++");
    CHECK_TRUE(x.exponent() == 6, "operator++");

    std::cout << "antes= " << x.internal_value() << '\n';
    --x;
    std::cout << x << '\n';
    std::cout << x.internal_value() << '\n';
    CHECK_TRUE(x.internal_value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");

    Length x1{1,3};
    CHECK_TRUE(x1.internal_value() == 1, "constructor");
    CHECK_TRUE(x1.exponent() == 3, "constructor");

    x += x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == 1.00099, "constructor");
    CHECK_TRUE(x.exponent() == 6, "constructor");

    x -= x1;
    std::cout << x << '\n';
    CHECK_TRUE(x.internal_value() == 999.99, "constructor");
    CHECK_TRUE(x.exponent() == 3, "constructor");
    }
//    { // Esto no tiene que compilar:
//    using Not_decimal = atd::Magnitude<atd::Units_length, int, std::ratio<1, 2>>;
//    Not_decimal bad{20};
//    atd::ENG_Magnitude eng1{bad};
//    }
}


template <typename Rep>
void test_order()
{
    test::interface("order");

    using Freq     = atd::ENG_frequency<Rep>;

    {
    constexpr Freq a{100,0};
    constexpr Freq b{200,0};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    Freq a{100,0};
    Freq b{200,3};
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(b > a, "operator>");
    }

    {
    Freq a{1,3};
    Freq b{1,3};
    CHECK_TRUE(!(a < b), "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b >= a, "operator>=");
    CHECK_TRUE(!(b > a), "operator>");
    }

//    {// bug
//    constexpr Freq f {1,6};
//    Freq f2 = f / typename Freq::Scalar{2};
//    std::cout << " -----> " << f << " / 2 = " << f2 << '\n';
//    }
}

void test_to_eng_magnitude()
{
    using Hertz0 = atd::Hertz<atd::Decimal<uint16_t, 2>>;
    using Hertz1 = atd::Hertz<double>;

    using ENG_frequency = atd::ENG_frequency<uint16_t>;

    Hertz0 f0{Hertz0::Rep{120, 13}};
    ENG_frequency e0 = atd::to_eng_magnitude<uint16_t>(f0);
    CHECK_TRUE(e0.internal_value() == 120, "to_eng_magnitude");

    Hertz1 f1{234.67};
    ENG_frequency e1 = atd::to_eng_magnitude<uint16_t>(f1);
    CHECK_TRUE(e1.internal_value() == 234, "to_eng_magnitude");

    auto e2 = atd::to_eng_magnitude(f0);
    CHECK_TRUE((e2.internal_value() == Hertz0::Rep{120,13}), "to_eng_magnitude");
}

void test_to_magnitude()
{
    using ENG_frequency = atd::ENG_frequency<double>;
    ENG_frequency f0{324.23, 3};

    using Hertz0 = atd::Hertz<atd::Decimal<uint32_t, 2>>;
    Hertz0 h0 = atd::to_magnitude<Hertz0::Rep>(f0);
    CHECK_TRUE(h0.value() == Hertz0::Rep{324230}, "to_magnitude");

    using Hertz1 = atd::Hertz<uint32_t>;
    Hertz1 h1 = atd::to_magnitude<Hertz1::Rep>(f0);
    CHECK_TRUE(h1.value() == Hertz1::Rep{324230}, "to_magnitude");

    using Hertz2 = atd::Hertz<int>;
    Hertz2 h2 = atd::to_magnitude<Hertz2::Rep>(f0);
    CHECK_TRUE(h2.value() == Hertz2::Rep{324230}, "to_magnitude");

}


void test_bugs()
{

    test::interface("bugs");

{
    using Rep = atd::Decimal<uint32_t, 3>;
    using Freq = atd::ENG_frequency<Rep>;
    using Time = atd::ENG_time<Rep>;

    {
	Freq f{125, 3};
	Freq f2 = f / 131072u;
	CHECK_TRUE((f2 == Freq{Rep{953, 674}, -3}), "b1");

	Time t = atd::time_in_us<Rep>(1024);
	CHECK_TRUE(t.internal_value() == Rep::significand(1024)
		    and t.exponent() == -3, "b2");
    }
    {
	Freq f{125, 3};
	uint16_t top = std::numeric_limits<uint16_t>::max();
	std::cout << "MAX = " << top << '\n';
	Freq f2 = f / (2 * (1u + top));
	std::cout << f2 << '\n';
	CHECK_TRUE((f2 == Freq{Rep{953, 674}, -3}), "b1");


	Time t = atd::time_in_us<Rep>(1024);
	CHECK_TRUE(t.internal_value() == Rep::significand(1024)
		    and t.exponent() == -3, "b2");
    }
    
    {
	Freq f1 = atd::frequency_in_Hz<Rep>(400);
	Freq f2 = atd::frequency_in_Hz<Rep>(20);

	Freq::Rep res = f1 / (2*f2) - Freq::Rep{1};
	CHECK_TRUE(res == Freq::Rep{9}, "b3");
    }
    
    {
	Freq f1{1, 3};
	Freq f2{29, -3};
	CHECK_TRUE(f2 < f1, "b4");
    }
}

    {
	using Potential = atd::ENG_electric_potential<atd::Decimal<uint32_t, 3>>;
	Potential::Rep res_val = Potential::Rep::significand(1090);
	std::cout << "res_val = " << res_val << '\n';
	std::cout << "res_val (internal) = " << res_val.significand() << '\n';
	Potential res_pot {res_val, 0};
	std::cout << "res_pot = " << res_pot << '\n';
	std::cout << "res_pot (internal) = " << res_pot.internal_value().significand() << '\n';

	Potential::Exponent exp = 0;
	write_as_eng(res_val, exp);
	std::cout << "res_val (despues) = " << res_val << '\n';
	std::cout << "exp = " << exp << '\n';

	auto xx = atd::to_integer<Potential::Rep>(res_val);
	std::cout << "xx = " << xx << '\n';

    }


{
// Time
using avr_Time      = atd::ENG_time<atd::Decimal<uint32_t, 3>>;
using avr_Frequency = atd::ENG_frequency<atd::Decimal<uint32_t, 3>>;
uint32_t n = 100;
avr_Frequency x{};
x = avr_Time::Rep{1} / avr_Time{n, -6};
std::cout << "x = " << x << '\n';

}
}

template <typename U, typename R>
void test_print(const atd::ENG_Magnitude<U,R>& m, const std::string& res)
{
    std::stringstream out;
    print_unit(out, m);
    CHECK_TRUE(out.str() == res, "print_unit");
}


void test_print()
{
    test::interface("print");

    test_print(atd::ENG_frequency<int>{100,0}, " Hz");
    test_print(atd::ENG_frequency<int>{100,3}, "kHz");
    test_print(atd::ENG_frequency<int>{100,6}, "MHz");
    test_print(atd::ENG_frequency<int>{100,9}, "GHz");
    test_print(atd::ENG_frequency<int>{100,12}, "THz");

    test_print(atd::ENG_time<int>{100,-3}, "ms");
    test_print(atd::ENG_time<int>{100,-6}, "us");
    test_print(atd::ENG_time<int>{100,-9}, "ns");
    test_print(atd::ENG_time<int>{100,-12}, "ps");

// unidades
    test_print(atd::ENG_length<int>{100, 3}, "km");
    test_print(atd::ENG_temperature<int>{100, 3}, "kK");
    test_print(atd::ENG_pressure<int>{100, 3}, "kPa");
}



int main()
{
try{
    test::header("atd_eng_magnitude");

    test_constexpr();
    test_basic();

    test_basic2<double>();
    test_double();
    test_basic2<atd::Decimal<int,2>>();
    test_observers();
    test_order<double>();
    test_order<atd::Decimal<int,2>>();
    test_to_eng_magnitude();
    test_to_magnitude();
    test_print();

    test_bugs();


}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





