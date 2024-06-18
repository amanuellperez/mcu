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



void test_percentage_constructor()
{
    test::interface("atd::Percentage()");

    {// normal
    atd::Percentage p{30};
    CHECK_TRUE(p.as_uint() == 30 , "as_uint");
    }
    {// normal
    atd::Percentage p = 30;
    CHECK_TRUE(p.as_uint() == 30 , "as_uint");
    CHECK_PRINT(std::cout << p, "30 %");
    }
    {// overflow 
    atd::Percentage p = 130;
    CHECK_TRUE(p.as_uint() == 0 , "overflow");
    }

    
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



void test_percentage()
{
 
    test::interface("Percentage");
    test_percentage_constructor();
    test_percentage_of();
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





