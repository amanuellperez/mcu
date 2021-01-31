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

#include "../../atd_magnitude.h"
#include "../../atd_decimal.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;


void test_unit()
{
    test::interfaz("Unit");
    
    using U1 = atd::Unit<1,2,3,4>;
    using U2 = atd::Unit<5,6,7,8>;

    using Up = atd::Unit_plus<U1, U2>;
    CHECK_TRUE(Up::m == U1::m + U2::m and
	       Up::kg == U1::kg + U2::kg and
	       Up::s == U1::s + U2::s and
	       Up::K == U1::K + U2::K, "Unit_plus");

    using Um = atd::Unit_minus<U1, U2>;
    CHECK_TRUE(Um::m == U1::m - U2::m and
	       Um::kg == U1::kg - U2::kg and
	       Um::s == U1::s - U2::s and
	       Um::K == U1::K - U2::K, "Unit_minus");
}

// Pruebas básicas con un único tipo de multiplier (EJ: todo en metros)
void test_magnitude_basic()
{
    using Meter = atd::Meter<int>;
   // using Meter = atd::Magnitude<atd::Units_meter, int, std::ratio<1>>;
    

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

// estructura de espacio vectorial
    m1 += m2;
    CHECK_TRUE(m1.value() == 35, "operator+=");

    m1 -= m2;
    CHECK_TRUE(m1.value() == 10, "operator-=");

    m1 *= 2;
    CHECK_TRUE(m1.value() == 20, "operator*=");

    m1 /= 2;
    CHECK_TRUE(m1.value() == 10, "operator/=");


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
}


//
void test_common_type()
{
    {// caso degenerado
    using Kilometer = atd::Magnitude<atd::Units_meter, int, std::ratio<1000>>;

    using CM = std::common_type_t<Kilometer, Kilometer>;
    CHECK_TRUE((std::is_same_v<CM::Unit, Kilometer::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, Kilometer::Rep>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, Kilometer::Multiplier>),
               "common_type::Multiplier");
    }
    {
    using Kilometer = atd::Magnitude<atd::Units_meter, int, std::ratio<1000>>;
    using Millimeter = atd::Magnitude<atd::Units_meter, long int, std::ratio<1,1000>>;

    using CM = std::common_type_t<Kilometer, Millimeter>;
    CHECK_TRUE((std::is_same_v<atd::Units_meter, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, long int>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, std::ratio<1,1000>>),
               "common_type::Multiplier");
    }

    {
    using Kilometer = atd::Magnitude<atd::Units_meter, double, std::kilo>;
    using Foot = atd::Magnitude<atd::Units_meter, float, std::ratio<3048,10000>>;

    using CM = std::common_type_t<Kilometer, Foot>;
    CHECK_TRUE((std::is_same_v<atd::Units_meter, CM::Unit>), "common_type::Unit");
    CHECK_TRUE((std::is_same_v<CM::Rep, double>), "common_type::Rep");
    CHECK_TRUE((std::is_same_v<CM::Multiplier, std::ratio<1,1250>>),
               "common_type::Multiplier");
    }

}

void test_magnitude_multiplier()
{// mezclamos diferentes multipliers
    using Kilometer = atd::Magnitude<atd::Units_meter, int, std::kilo>;
    using Meter = atd::Magnitude<atd::Units_meter, int, std::ratio<1>>;
    using Centimeter = atd::Magnitude<atd::Units_meter, int, std::centi>;
    using Millimeter = atd::Magnitude<atd::Units_meter, int, std::milli>;

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
    using Kilometer = atd::Magnitude<atd::Units_meter, double, std::kilo>;
    using Meter = atd::Magnitude<atd::Units_meter, double, std::ratio<1>>;
    using Centimeter = atd::Magnitude<atd::Units_meter, double, std::centi>;
    using Millimeter = atd::Magnitude<atd::Units_meter, double, std::milli>;

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
//    using Kilometer = atd::Magnitude<atd::Units_meter, double, std::kilo>;
//    using Meter = atd::Magnitude<atd::Units_meter, double, std::ratio<1>>;
//    using Centimeter = atd::Magnitude<atd::Units_meter, double, std::centi>;
//    using Millimeter = atd::Magnitude<atd::Units_meter, double, std::milli>;

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

    {// hertz
    using Kilohertz = atd::Kilohertz<double>;
    using Megahertz = atd::Megahertz<double>;
    using Gigahertz = atd::Gigahertz<double>;
    using Terahertz = atd::Terahertz<double>;

    Kilohertz f = Megahertz{4};
    CHECK_TRUE(f == Kilohertz{4000}, "MHz -> kHz");
    CHECK_TRUE(Megahertz{Gigahertz{2}} == Megahertz{2000}, "GHz <-> MHz");
    CHECK_TRUE(Terahertz{Gigahertz{2}} == Terahertz{0.002}, "THz <-> GHz");
    }
}


void test_magnitude_and_decimal()
{
//    using Pascal =
//        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 1>, std::ratio<1>>;
//
//    using Hectopascal =
//        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 3>, std::hecto>;

    using Pascal = atd::Pascal<atd::Decimal<int, 1>>;
    using Hectopascal = atd::Hectopascal<atd::Decimal<int, 3>>;

    Pascal p1{Pascal::Rep{92045,3}};
    Hectopascal p2{Hectopascal::Rep{10,423}};

    CHECK_STDOUT(p2, "10.423");
    std::cout << ">>> Convierto a pascal\n";
    Pascal p2_to_pascal{p2};

    CHECK_STDOUT(p2_to_pascal, "1042.3");
    Pascal rp = p1 + p2;
    auto [n, f] = rp.value().value(); 
    CHECK_TRUE(n == 93087 and f == 6, "pascal + hectopascal");

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

void test_magnitude()
{
    test::interfaz("Magnitude");

    test_magnitude_basic();
    test_common_type();
    test_magnitud_conversiones();
    test_magnitude_multiplier();
    test_magnitude_multiplier2();
    test_magnitude_and_decimal();
    test_magnitude_temperature();
    test_magnitude_temperature_decimal();
}



int main()
{
try{
    test::header("atd_magnitude");
    
    test_unit();
    test_magnitude();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}

