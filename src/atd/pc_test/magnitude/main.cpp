// Copyright (C) 2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../atd_magnitude.h"
#include "../../atd_decimal.h"
#include "../../atd_cast.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;



// Pruebas básicas con un único tipo de multiplier (EJ: todo en metros)
void test_magnitude_basic()
{
    using Meter = atd::Meter<int>;
    using Kilometer = atd::Kilometer<int>;
   // using Meter = atd::Magnitude<atd::Units_length, int, std::ratio<1>>;
    

// constructor
    Meter m1{10};
    CHECK_TRUE(m1.value() == 10, "constructor");

    {
    Meter m2;
    m2 = m1;
    CHECK_TRUE(m2.value() == m1.value(), "operator=");
    }

    Meter m2{25};
    

{// relación de orden
    Meter a{10};
    Meter b{20};
    CHECK_TRUE(Meter{10} == Meter{10}, "operator==");
    CHECK_TRUE(a != b, "operator!=");
    CHECK_TRUE(a < b, "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(b > a, "operator>");
    CHECK_TRUE(b >= a, "operator>=");
}

{// relación de orden(2)
    Meter a{10};
    Meter b{10};
    CHECK_TRUE(a == b, "operator!=");
    CHECK_TRUE(!(a < b), "operator<");
    CHECK_TRUE(a <= b, "operator<=");
    CHECK_TRUE(!(b > a), "operator>");
    CHECK_TRUE(b >= a, "operator>=");
}

{// relación de orden(3)
    CHECK_TRUE(Meter{1000} == Kilometer{1}, "operator!=");
    CHECK_TRUE(Meter{2000} < Kilometer{5}, "operator<");
    CHECK_TRUE(Meter{2000} > Kilometer{1}, "operator>");
}


// estructura de espacio vectorial
    m1 += m2;
    CHECK_TRUE(m1.value() == 35, "operator+=");

    m1 -= m2;
    CHECK_TRUE(m1.value() == 10, "operator-=");

    m1 *= 2;
    CHECK_TRUE(m1.value() == 20, "operator*=");

    m1 /= 2;
    CHECK_TRUE(m1.value() == 10, "operator/=");

    {
	auto res = m1 / 2;
	CHECK_TRUE(res.value() == 5, "operator/=");

    }
    using CM = std::common_type_t<Meter, Meter>;
    CHECK_TRUE((std::is_same_v<Meter::Unit, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<Meter::Rep, CM::Rep>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<Meter::Multiplier, CM::Multiplier>),
               "common_type::Multiplier");
    CHECK_TRUE((std::is_same_v<Meter, CM>), "common_type");

    Meter m3 = m1 + m2;
    CHECK_TRUE(m3.value() == m1.value() + m2.value(), "operator+");
    
    Meter m4 = m1 - m2;
    CHECK_TRUE(m4.value() == m1.value() - m2.value(), "operator-");

    m3 = 2*m1;
    CHECK_TRUE(m3.value() == 2*m1.value(), "operator*");

    m4 = m1*2;
    CHECK_TRUE(m3 == m4, "operator/");

// operator/ (mismas magnitudes)
    {
	Meter m1{1};
	Kilometer m2{1};
	int r = m2 / m1;
	CHECK_TRUE(r == 1000, "Kilometer / Meter");

	r = m1 / m2; // como int no tiene resolución, devuelve 0.
	CHECK_TRUE(r == 0, "Meter / Kilometer");

	m1 = Meter{50000};
	m2 = Kilometer{25};
	r = m1 / m2;
	CHECK_TRUE(r == 2, "Kilometer / Meter");
	CHECK_TRUE(m1 / m1 == 1, "Meter / Meter");

    }
}


//
void test_common_type()
{
    {// caso degenerado
    using Kilometer = atd::Magnitude<atd::Units_length, int, std::ratio<1000>>;

    using CM = std::common_type_t<Kilometer, Kilometer>;
    CHECK_TRUE((std::is_same_v<CM::Unit, Kilometer::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, Kilometer::Rep>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, Kilometer::Multiplier>),
               "common_type::Multiplier");
    }
    {
    using Kilometer = atd::Magnitude<atd::Units_length, int, std::ratio<1000>>;
    using Millimeter = atd::Magnitude<atd::Units_length, long int, std::ratio<1,1000>>;

    using CM = std::common_type_t<Kilometer, Millimeter>;
    CHECK_TRUE((std::is_same_v<atd::Units_length, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, long int>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, std::ratio<1,1000>>),
               "common_type::Multiplier");
    }

    {
    using Kilometer = atd::Magnitude<atd::Units_length, double, std::kilo>;
    using Foot = atd::Magnitude<atd::Units_length, float, std::ratio<3048,10000>>;

    using CM = std::common_type_t<Kilometer, Foot>;
    CHECK_TRUE((std::is_same_v<atd::Units_length, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, double>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, std::ratio<1,1250>>),
               "common_type::Multiplier");
    }

}

void test_magnitude_multiplier()
{// mezclamos diferentes multipliers
    using Kilometer = atd::Magnitude<atd::Units_length, int, std::kilo>;
    using Meter = atd::Magnitude<atd::Units_length, int, std::ratio<1>>;
    using Centimeter = atd::Magnitude<atd::Units_length, int, std::centi>;
    using Millimeter = atd::Magnitude<atd::Units_length, int, std::milli>;

    {
    auto m1 = Meter{3} + Centimeter{25} + Millimeter{7};
    CHECK_TRUE(m1.value() == 3257, "Meter + Centimeter + Millimeter");

    Centimeter m2 = Meter{3} + Centimeter{25} + Millimeter{7};
    CHECK_TRUE(m2 == Centimeter{325}, "Meter + Centimeter + Millimeter");
    }

    {
    auto m = Kilometer{2} + Meter{27};
    CHECK_TRUE(m.value() == 2027, "Kilometer + Meter");
    }

}

// Igual que test_magnitude_multiplier pero con tipo double
void test_magnitude_multiplier2()
{// mezclamos diferentes multipliers
    using Kilometer = atd::Magnitude<atd::Units_length, double, std::kilo>;
    using Meter = atd::Magnitude<atd::Units_length, double, std::ratio<1>>;
    using Centimeter = atd::Magnitude<atd::Units_length, double, std::centi>;
    using Millimeter = atd::Magnitude<atd::Units_length, double, std::milli>;

    {
    auto m1 = Meter{3} + Centimeter{25} + Millimeter{7};
    CHECK_TRUE(m1.value() == 3257, "Meter + Centimeter + Millimeter");

    Centimeter m2 = Meter{3} + Centimeter{25} + Millimeter{7};
    CHECK_TRUE(m2 == Centimeter{325.7}, "Meter + Centimeter + Millimeter");

    Meter m3 = Meter{3} + Centimeter{25} + Millimeter{7};
    CHECK_TRUE(m3 == Meter{3.257}, "Meter + Centimeter + Millimeter");
    }

    {
    auto m = Kilometer{2} + Meter{27};
    CHECK_TRUE(m.value() == 2027, "Kilometer + Meter");
    }

}


void test_magnitud_conversiones()
{
//    using Kilometer = atd::Magnitude<atd::Units_length, double, std::kilo>;
//    using Meter = atd::Magnitude<atd::Units_length, double, std::ratio<1>>;
//    using Centimeter = atd::Magnitude<atd::Units_length, double, std::centi>;
//    using Millimeter = atd::Magnitude<atd::Units_length, double, std::milli>;

    using Kilometer = atd::Kilometer<double>;
    using Meter = atd::Meter<double>;
    using Centimeter = atd::Centimeter<double>;
    using Millimeter = atd::Millimeter<double>;


    {// Conversión básica
    Centimeter cm = Meter{3};
    CHECK_TRUE(cm == Centimeter{300}, "meter -> cm");
    CHECK_TRUE(cm.value() == 300, "meter -> cm");

    CHECK_TRUE(Kilometer{2.345} == Millimeter{2345000}, "km <-> mm");
    CHECK_TRUE(Meter{Millimeter{1234}} == Meter{1.234}, "m <-> mm");
    CHECK_TRUE(Meter{Millimeter{1234}}.value() == 1.234, "m <-> mm");

    CHECK_TRUE(Millimeter{Centimeter{3}} == Millimeter{30}, "cm -> mm");
    CHECK_TRUE(Millimeter{Centimeter{3}}.value() == 30, "cm -> mm");

    CHECK_TRUE(Centimeter{Millimeter{3}} == Centimeter{0.3}, "mm -> cm");
    CHECK_TRUE(Centimeter{Millimeter{3}}.value() == 0.3, "mm -> cm");
    }

    {// Hertz
    using KiloHertz = atd::KiloHertz<double>;
    using MegaHertz = atd::MegaHertz<double>;
    using GigaHertz = atd::GigaHertz<double>;
    using TeraHertz = atd::TeraHertz<double>;

    KiloHertz f = MegaHertz{4};
    CHECK_TRUE(f == KiloHertz{4000}, "MHz -> kHz");
    CHECK_TRUE(MegaHertz{GigaHertz{2}} == MegaHertz{2000}, "GHz <-> MHz");
    CHECK_TRUE(TeraHertz{GigaHertz{2}} == TeraHertz{0.002}, "THz <-> GHz");
    }
}


void test_magnitude_and_decimal()
{
//    using Pascal =
//        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 1>, std::ratio<1>>;
//
//    using Hectopascal =
//        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 3>, std::hecto>;

    using Pascal = atd::Pascal<atd::Decimal<int, 3>>;
    using Hectopascal = atd::Hectopascal<atd::Decimal<int, 3>>;

    Pascal p1{Pascal::Rep{92045,3}};
    Hectopascal p2{Hectopascal::Rep{10,423}};

    CHECK_STDOUT(p2, "10.423");
    std::cout << ">>> Convierto a pascal\n";
    Pascal p2_to_pascal{p2};

    CHECK_STDOUT(p2_to_pascal, "1042.300");
    Pascal rp = p1 + p2;
    auto [n, f] = rp.value().value(); 
    CHECK_TRUE(n == 93087 and f == 600, "pascal + hectopascal");

    // pruebo que compile operator<<
    CHECK_STDOUT(rp, "93087.6");


}

// En temperaturas solo voy a manejar 2 cifras decimales
bool equal(double x, double y)
{
    return (x-y) < 0.01;
}

void test_magnitude_temperature()
{
//    using Kelvin = atd::
//        Magnitude<atd::Units_kelvin, double, std::ratio<1>>;
//
//    using Celsius = atd::
//        Magnitude<atd::Units_kelvin, double, std::ratio<1>, std::ratio<27315, 100>>;
//
//    using Fahrenheit = atd::Magnitude<atd::Units_kelvin,
//                                      double,
//                                      std::ratio<5, 9>,
//                                      std::ratio<45967, 180>>;
//
    using Kelvin = atd::Kelvin<double>;
    using Celsius = atd::Celsius<double>;
    using Fahrenheit = atd::Fahrenheit<double>;
    CHECK_TRUE((equal(Kelvin{Celsius{0}}.value(), Kelvin{273.15}.value())),
               "ºC -> ºK");
    CHECK_TRUE(Kelvin{Celsius{20}} == Kelvin{293.15}, "ºC -> ºK");

    CHECK_TRUE((equal(Celsius{Kelvin{10}}.value(), Celsius{-263.15}.value())),
               "ºK -> ºC");
    CHECK_TRUE((equal(Celsius{Kelvin{300}}.value(), Celsius{26.85}.value())),
               "ºK -> ºC");

    CHECK_TRUE((equal(Fahrenheit{Celsius{0}}.value(), Fahrenheit{32}.value())),
               "ºC -> ºF");
    CHECK_TRUE((equal(Fahrenheit{Celsius{35}}.value(), Fahrenheit{95}.value())),
               "ºC -> ºF");

    CHECK_TRUE((equal(Celsius{Fahrenheit{0}}.value(), 
						Celsius{-17.7777}.value())),
               "ºC -> ºF");

    CHECK_TRUE((equal(Celsius{Fahrenheit{40}}.value(), 
						Celsius{4.44444}.value())),
               "ºF -> ºC");
    CHECK_TRUE((equal(Celsius{Fahrenheit{40}}.value(), 
						Celsius{4.44444}.value())),
               "ºF -> ºC");
    CHECK_TRUE((equal(Celsius{Fahrenheit{-20}}.value(), 
						Celsius{-28.8888}.value())),
               "ºF -> ºC");

    CHECK_TRUE((equal(Kelvin{Fahrenheit{0}}.value(), 
						Kelvin{255.372}.value())),
               "ºF -> ºK");
    CHECK_TRUE((equal(Kelvin{Fahrenheit{20}}.value(), 
						Kelvin{266.483}.value())),
               "ºF -> ºK");

// Una prueba mezclando medidas (cosa que no habría que hacer)
    Kelvin k1 = Fahrenheit{10} + Celsius{20} + Kelvin{34};
    CHECK_TRUE((equal(k1.value(), 362.9278)), "Sumando");
    // Observar que la resta en diferenets unidades no tiene mucho sentido:
    //	Si resto 0ºC - 10ºC obtienes -10ºC, pero ¿y si resto 0ºC - 0ºK?
    //	Si paso K a C:
    //	    0 C - 0 K = 0 C - (-273.15) C = +273.15 C !!! <-- ABSURDO, 
    //	ya que la "idea" de restar 0 K no es restar un valor absoluto sino un
    //	incremento. Lo que uno esperaría es que 0C - 0K = 0C.
    // Kelvin k2 = Celsius{0} - Kelvin{0};
}

void test_magnitude_temperature_decimal()
{
    using Rep = atd::Decimal<int, 2>;

    using Kelvin = atd::Kelvin<Rep>;
    using Celsius = atd::Celsius<Rep>;
    using Fahrenheit = atd::Fahrenheit<Rep>;

//    using Kelvin = atd::
//        Magnitude<atd::Units_kelvin, atd::Decimal<int,2>, std::ratio<1>>;

//    using Celsius = atd::
//        Magnitude<atd::Units_kelvin, atd::Decimal<int,2>, std::ratio<1>, std::ratio<27315, 100>>;

//    using Fahrenheit = atd::Magnitude<atd::Units_kelvin,
//                                      atd::Decimal<int,2>,
//                                      std::ratio<5, 9>,
//                                      std::ratio<45967, 180>>;

    CHECK_TRUE((Kelvin{Celsius{0}} == Kelvin{Rep{273,15}}), "ºC -> ºK");
    CHECK_TRUE((Kelvin{Celsius{20}}== Kelvin{Rep{293,15}}), "ºC -> ºK");

    CHECK_TRUE((Celsius{Kelvin{10}} == Celsius{Rep{-263, 15}}), "ºK -> ºC");
    CHECK_TRUE((Celsius{Kelvin{300}} == Celsius{Rep{26, 85}}), "ºK -> ºC");

    CHECK_TRUE((Fahrenheit{Celsius{0} } == Fahrenheit{32}), "ºC -> ºF");
    CHECK_TRUE((Fahrenheit{Celsius{35}} == Fahrenheit{95}), "ºC -> ºF");

    CHECK_TRUE((Celsius{Fahrenheit{0} } == Celsius{Rep{-17, 77}}), "ºC -> ºF");
    // Lo esta rodeando a 4.45 en lugar de dejarlo en 4.44 ???
    CHECK_TRUE((Celsius{Fahrenheit{40}} == Celsius{Rep{4, 45}}), "ºF -> ºC");


    CHECK_TRUE((Celsius{Fahrenheit{-20}} == Celsius{Rep{-28, 88}}), "ºF -> ºC");

    CHECK_TRUE((Kelvin{Fahrenheit{0}} == Kelvin{Rep{255, 37}}), "ºF -> ºK");
    CHECK_TRUE((Kelvin{Fahrenheit{20}} == Kelvin{Rep{266, 48}}), "ºF -> ºK");
}

void test_magnitude_frequency()
{
    using Int = atd::Decimal<uint32_t, 3>;
    using Hertz = atd::Hertz<Int>;
    using KiloHertz = atd::KiloHertz<Int>;
    using MegaHertz = atd::MegaHertz<Int>;
    using Millisecond = atd::Millisecond<Int>;
    using Microsecond = atd::Microsecond<Int>;

    // Types
    CHECK_TRUE((std::is_same_v<Hertz::Displacement, std::ratio<0>>),
	    "Hertz::Displacement == 0");

    // Range
    {
	Hertz f{1000000}; // = 1MHz
	CHECK_TRUE(f.value() == Int{1000000}, "Hertz{1000000}");
    }

    // common_type
    {
    using CM = std::common_type_t<Hertz, MegaHertz>;
    CHECK_TRUE((std::is_same_v<Hertz::Unit, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<Hertz::Rep, CM::Rep>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<Hertz::Multiplier, CM::Multiplier>),
               "common_type::Multiplier");
    CHECK_TRUE((std::is_same_v<Hertz, CM>), "common_type");
    }
    {
    using CR = std::common_type_t<Hertz::Rep, MegaHertz::Rep>;
    CHECK_TRUE((std::is_same_v<CR, Int>), "common_type::Rep");
    }


    // Microsecond T = atd::inverse(res);
    using Mag_inverse = atd::Magnitude_inverse_t<MegaHertz>;
    CHECK_TRUE((std::is_same_v<Mag_inverse::Unit, Microsecond::Unit>),
               "Mag_inverse::Unit");
    CHECK_TRUE((std::is_same_v<Mag_inverse::Rep, Microsecond::Rep>),
               "Mag_inverse::Rep");
    CHECK_TRUE((std::is_same_v<Mag_inverse::Multiplier, Microsecond::Multiplier>),
               "Mag_inverse::Multiplier");
    CHECK_TRUE((std::is_same_v<Mag_inverse::Displacement, Microsecond::Displacement>),
               "Mag_inverse::Displacement");

    {
    auto f = MegaHertz{2};
    Microsecond T1 = atd::inverse(f);
//    std::cout << "T1 = " << T1.value() << " us\n";
    CHECK_TRUE((T1.value() == Int{0,5}), "atd::inverse");
    }

    {
    auto f = KiloHertz{2};
    Microsecond T1 = atd::inverse(f);
    CHECK_TRUE((T1.value() == Int{500,0}), "atd::inverse");
    Millisecond T2 = atd::inverse(f);
    CHECK_TRUE((T2.value() == Int{0,5}), "atd::inverse");
//    // observar cómo se calcula correctamente con las unidades
//    std::cout << "T1 = " << T1.value() << " us\n";
//    std::cout << "T2 = " << T2.value() << " ms\n";
    }

    {// 1 / 1_ms
	Millisecond T{1};
	auto f = Int{1} / T;
	CHECK_TRUE((std::is_same_v<decltype(f), KiloHertz>), "1/ms = kHz");
	CHECK_TRUE(f == KiloHertz{1}, "1/ms = kHz");
    }
    {// 1 / 2_ms
	Millisecond T{2};
	auto f = Int{1} / T;
	CHECK_TRUE((std::is_same_v<decltype(f), KiloHertz>), "1/ms = kHz");
	CHECK_TRUE((f == KiloHertz{Int{0,5}}), "1/ms = kHz");
    }
    {// 5 / 2_ms
	Millisecond T{2};
	auto f = Int{5} / T;
	CHECK_TRUE((std::is_same_v<decltype(f), KiloHertz>), "1/ms = kHz");
	CHECK_TRUE((f == KiloHertz{Int{2,5}}), "1/ms = kHz");
    }
    {// 1 / 1_us
	Microsecond T{1};
	auto f = Int{1} / T;
	CHECK_TRUE((std::is_same_v<decltype(f), MegaHertz>), "1/us = MHz");
	CHECK_TRUE((f == MegaHertz{Int{1,0}}), "1/us = MHz");
    }


    {// magnitud_cast
	MegaHertz f0{1};
	Hertz f1 = atd::magnitude_cast<Hertz>(f0);
//	std::cout << "f1 en hertz = " << f1.value() << '\n';
	CHECK_TRUE(f1.value() == Int{1000000}, "magnitude_cast");
    }


    {// Constructor
	MegaHertz f0{1};
	Hertz f1{f0};
//	std::cout << "f1 en hertz = " << f1.value() << '\n';
//	std::cout << "Int{1000000}= " << Int{1000000} << '\n';
	CHECK_TRUE(f1.value() == Int{1000000}, "Hertz{MegaHertz{1}}");
    }

    {// comparando Hertz == MegaHertz
	Hertz f0{1000000};
	MegaHertz f1{1};
	
//	std::cout << "f0 = " << f0.value() << '\n';
//	std::cout << "f1 = " << f1.value() << '\n';
//	using CT = std::common_type_t<Hertz, MegaHertz>;
//	std::cout << "CT{f0} = " << CT{f0}.value() << '\n';
//	std::cout << "CT{f1} = " << CT{f1}.value() << '\n';
//        std::cout << "CT::M = " << CT::Multiplier::num << "/"
//                  << CT::Multiplier::den << '\n';
//        std::cout << "Hertz::M = " << Hertz::Multiplier::num << "/"
//                  << Hertz::Multiplier::den << '\n';
//        std::cout << "MegaHertz::M = " << MegaHertz::Multiplier::num << "/"
//                  << MegaHertz::Multiplier::den << '\n';

        CHECK_TRUE(f0 == f1, "hertz == megahertz???");
    }


    {
	uint32_t period_in_us = 1;
	Hertz freq{5000};
	Microsecond T0 = atd::inverse(freq);
	std::cout << "freq = " << freq << " Hz\n";
	std::cout << "T0 = " << T0 << " us\n";
	uint32_t T = atd::to_integer<uint32_t>(T0.value());
	std::cout << "T = " << T << " us\n";
        uint32_t top = T / (static_cast<uint32_t>(period_in_us) * uint32_t{2});
	std::cout << "top = " << top << "\n";
    }

    {// bug: Magnitude * Decimal ¿qué funcion llamar?
	Hertz f{10};
	Int a = 20;
	Hertz res = f * a;
	std::cout << res << '\n';
    }

}

void test_arithmetic()
{
    using Kilometer = atd::Kilometer<double>;
    using Meter = atd::Meter<double>;

// operaciones con escalares
    {
	Kilometer a{3};
	auto b = 2.0*a;
	CHECK_TRUE((std::is_same_v<decltype(b), Kilometer>), "int * kilometer");
	CHECK_TRUE(b == Kilometer{6}, "int * kilometer");
    }
    {
	Kilometer a{3};
	auto b = a / 2;
	CHECK_TRUE((std::is_same_v<decltype(b), Kilometer>), "kilometer / int");
	CHECK_TRUE(b == Kilometer{1.5}, "kilometer / int");
    }

// operaciones con magnitudes
    {
	Kilometer a{3};
	Meter b{2};
	auto c = a + b;
	CHECK_TRUE((std::is_same_v<decltype(c), Meter>), "Meter + Kilometer");
	CHECK_TRUE(c == Meter{3002}, "Meter + Kilometer");

	auto d = a - b;
	CHECK_TRUE((std::is_same_v<decltype(d), Meter>), "Kilometer - Meter");
	CHECK_TRUE(d == Meter{2998}, "Kilometer - Meter");
    }

}


void test_magnitude()
{
    test::interfaz("Magnitude");

    test_magnitude_basic();
    test_common_type();
    test_magnitud_conversiones();
    test_magnitude_multiplier();
    test_magnitude_multiplier2();
    test_arithmetic();
    test_magnitude_and_decimal();
    test_magnitude_temperature();
    test_magnitude_temperature_decimal();
    test_magnitude_frequency();
}


template <typename Multiplier>
using Frequency = atd::Frequency<int, Multiplier>;

using Hertz = atd::Hertz<int>;
using MegaHertz = atd::MegaHertz<int>;

Hertz operator"" _Hz(unsigned long long int x) { return Hertz{x}; }
MegaHertz operator"" _MHz(unsigned long long int x) 
{ return MegaHertz{x}; }


template<typename Multiplier>
void f(const Frequency<Multiplier>& f)
{
    std::cout << "frequency = " << f;

    if constexpr (std::is_same_v<Multiplier, std::ratio<1,1>>)
	std::cout << " Hz\n";
    else if constexpr (std::is_same_v<Multiplier, std::mega>)
	std::cout << " MHz\n";
    else
	std::cout << " unknown!!!\n";

    std::cout << "en Hz = " << Hertz{f} << " Hz\n";

}

void test_usability()
{
    f(Hertz{20});
    f(30_Hz);
    f(MegaHertz{2});
    f(5_MHz);
}



int main()
{
try{
    test::header("atd_magnitude");
    
    test_magnitude();
    test_usability();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}

