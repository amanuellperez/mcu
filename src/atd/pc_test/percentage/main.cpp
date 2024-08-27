// Copyright (C) 2024 Manuel Perez 
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


#include "../../atd_percentage.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;


template <typename Int>
void test_as_ratio(const Int& n, const Int& d, uint8_t res)
{
	auto p = atd::Percentage::as_ratio(n, d);
	CHECK_TRUE(p.as_uint() == res, 
		    alp::as_str() << "as_ratio(" << (int)n << ", " << (int)d << ")");
}

void test_percentage_constructor()
{
    test::interface("atd::Percentage()");

// constructor
    {// normal
    auto p = atd::Percentage{30};
    CHECK_TRUE(p.as_uint() == 30 , "as_uint");
    }
    {// normal
    auto p = atd::Percentage{30};
    CHECK_TRUE(p.as_uint() == 30 , "as_uint");
    CHECK_PRINT(std::cout << p, "30 %");
    }
    {// overflow 
    auto p = atd::Percentage{130};
    CHECK_TRUE(p.as_uint() == 0 , "overflow");
    }

// ratio
    test_as_ratio<uint8_t>(0, 200, 0);
    test_as_ratio<uint8_t>(20, 200, 10);
    test_as_ratio<uint8_t>(40, 200, 20);
    test_as_ratio<uint8_t>(60, 200, 30);
    test_as_ratio<uint8_t>(80, 200, 40);
    test_as_ratio<uint8_t>(100, 200, 50);
    test_as_ratio<uint8_t>(120, 200, 60);
    test_as_ratio<uint8_t>(140, 200, 70);
    test_as_ratio<uint8_t>(160, 200, 80);
    test_as_ratio<uint8_t>(180, 200, 90);
    test_as_ratio<uint8_t>(200, 200, 100);

    test_as_ratio<int8_t>(0, 100, 0);
    test_as_ratio<int8_t>(20, 100, 20);
    test_as_ratio<int8_t>(40, 100, 40);
    test_as_ratio<int8_t>(60, 100, 60);
    test_as_ratio<int8_t>(80, 100, 80);
    test_as_ratio<int8_t>(100, 100, 100);
}


template <typename Int>
void test_percentage_of(uint8_t p0, const Int& x, const Int& res)
{
    atd::Percentage p{p0};
    CHECK_TRUE(p.of(x) == res, "of");
}

void test_percentage_of()
{
    test::interface("of");

    test_percentage_of(0, 35, 0);
    test_percentage_of(20, 100, 20);
    test_percentage_of(100, 43, 43);
    test_percentage_of(50, 80, 40);
    test_percentage_of(25, -200, -50);
    test_percentage_of(10, -1200, -120);
    test_percentage_of<uint16_t>(50, 50'000, 25'000);

    
}

void test_order()
{
    using P = atd::Percentage;
    CHECK_TRUE(P{20} > P{10}, ">");

    CHECK_TRUE(P{20} >= P{10}, ">=");
    CHECK_TRUE(P{20} >= P{20}, ">=");

    CHECK_TRUE(P{20} <= P{20}, "<=");
    CHECK_TRUE(P{10} <= P{20}, "<=");

    CHECK_TRUE(P{10} < P{20}, "<=");
}


void test_percentage()
{
 
    test::interface("Percentage");
    test_percentage_constructor();
    test_percentage_of();
    test_order();
}


template <typename Int>
void test_parts_per_1024_of(uint16_t p0, const Int& x, const Int& res)
{
    atd::Parts_per<1024> p{p0};
//    std::cout << p << " of " << x << " = " << p.of(x) << '\n';
    CHECK_TRUE(p.of(x) == res, "of");
}


void test_parts_per_1024()
{
    using Per = atd::Parts_per<1024>;
    CHECK_TRUE(std::is_same_v<Per::Rep, uint16_t>, "Rep");

    {// normal
    Per p{30};
    CHECK_TRUE(p.as_uint() == 30 , "as_uint");
    }
    {// normal
    Per p = 300;
    CHECK_TRUE(p.as_uint() == 300 , "as_uint");
    CHECK_PRINT(std::cout << p, "300 out of 1024");
    }
    {// overflow 
    Per p = 2'000;
    CHECK_TRUE(p.as_uint() == 0 , "overflow");
    }

// of()
    test_parts_per_1024_of(0, 35, 0);
    test_parts_per_1024_of(20, 1024, 20);
    test_parts_per_1024_of(1024, 43, 43);
    test_parts_per_1024_of(512, 80, 40);
    test_parts_per_1024_of(256, -200, -50);
    test_parts_per_1024_of(10, -1200, -11);
    test_parts_per_1024_of<uint16_t>(512, 50'000, 25'000);
}

void test_parts_per()
{
    test::interface("Parts_per");

    test_parts_per_1024();
}

int main()
{
try{
    test::header("percentage");

    test_percentage();
    test_parts_per();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





