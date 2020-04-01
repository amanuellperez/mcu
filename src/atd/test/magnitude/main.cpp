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
    
    using U1 = atd::Unit<1,2,3>;
    using U2 = atd::Unit<4,5,6>;

    using Up = atd::Unit_plus<U1, U2>;
    CHECK_TRUE(Up::m == U1::m + U2::m and
	       Up::kg == U1::kg + U2::kg and
	       Up::s == U1::s + U2::s, "Unit_plus");

    using Um = atd::Unit_minus<U1, U2>;
    CHECK_TRUE(Um::m == U1::m - U2::m and
	       Um::kg == U1::kg - U2::kg and
	       Um::s == U1::s - U2::s, "Unit_minus");
}

// Pruebas básicas con un único tipo de multiplier (EJ: todo en metros)
void test_magnitude_basic()
{
    using Meter = atd::Magnitude<atd::Units_meter, int, std::ratio<1>>;
    

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
    using Milimeter = atd::Magnitude<atd::Units_meter, long int, std::ratio<1,1000>>;

    using CM = std::common_type_t<Kilometer, Milimeter>;
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
    using Milimeter = atd::Magnitude<atd::Units_meter, int, std::milli>;

    {
    auto m1 = Meter{3} + Centimeter{25} + Milimeter{7};
    CHECK_TRUE(m1.value() == 3257, "Meter + Centimeter + Milimeter");
    }

    {
    auto m = Kilometer{2} + Meter{27};
    CHECK_TRUE(m.value() == 2027, "Kilometer + Meter");
    }

}

void test_magnitud_and_decimal()
{
    using Pascal =
        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 1>, std::ratio<1>>;

    using Hecto_pascal =
        atd::Magnitude<atd::Units_pascal, atd::Decimal<int, 3>, std::hecto>;

    Pascal p1{Pascal::Rep{92045,3}};
    Hecto_pascal p2{Hecto_pascal::Rep{10,423}};
    auto res = p1 + p2;
    std::cout << "res = " << res << '\n';

    Pascal rp = p1 + p2;
    std::cout << "res en pascal = " << rp << '\n';
}

void test_magnitude()
{
    test::interfaz("Magnitude");

    test_magnitude_basic();
    test_common_type();
    test_magnitude_multiplier();
    test_magnitud_and_decimal();
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





