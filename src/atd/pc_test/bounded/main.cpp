// Copyright (C) 2021-2022 Manuel Perez 
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

#include "../../atd_bounded.h"

#include <alp_test.h>
#include <iostream>



using namespace test;

void test_bounded()
{
    test::interface("Bounded");

// contructor
    {
    atd::Bounded<int, -5, 10> a;
    a = 2;
    CHECK_TRUE(a == 2, "constructor");
    CHECK_TRUE(a.min() == -5, "min()");
    CHECK_TRUE(a.max() == 10, "max()");

    atd::Bounded<int, 0, 10> b{3};
    CHECK_TRUE(b == 3, "constructor");

    atd::Bounded<int, -5, 10> c{10};
    CHECK_TRUE(c == 10, "constructor");

    atd::Bounded<int, -5, 10> d{20};
    CHECK_TRUE(d == 10, "constructor");

    atd::Bounded<int, -5, 10> e{-5};
    CHECK_TRUE(e == -5, "constructor");

    atd::Bounded<int, -5, 10> f{-50};
    CHECK_TRUE(f == -5, "constructor");
    }
    
    
// algebra
{
// +=
    atd::Bounded<int, -5, 10> a;
    a = 1;
    a += 4;
    CHECK_TRUE(a == 5, "+=");
    a += 5;
    CHECK_TRUE(a == 10, "+=");
    a += 5;
    CHECK_TRUE(a == 10, "+=");

    a = 1;
    a += 30;
    CHECK_TRUE(a == 10, "+=");

// -=
    a = 5;
    a -= 2;
    CHECK_TRUE(a == 3, "-=");

    a -= 15;
    CHECK_TRUE(a == -5, "-=");

    a = -3;
    a -= 2;
    CHECK_TRUE(a == -5, "-=");

// *=
    a = 2;
    a *= 2;
    CHECK_TRUE(a == 4, "*=");

    a *= 3;
    CHECK_TRUE(a == 10, "*=");

    a = 2;
    a *= 5;
    CHECK_TRUE(a == 10, "*=");

    a = -2;
    a *= 2;
    CHECK_TRUE(a == -4, "*=");

    a *= 3;
    CHECK_TRUE(a == -5, "*=");

    a = -1;
    a *= 5;
    CHECK_TRUE(a == -5, "*=");

// ++
    a = 8;
    ++a;
    CHECK_TRUE(a == 9, "++");

    ++a;
    CHECK_TRUE(a == 10, "++");

    ++a;
    CHECK_TRUE(a == 10, "++");

    {
    a = 8;
    auto res = a++;
    CHECK_TRUE(res == 8, "++");
    CHECK_TRUE(a == 9, "++");
    }


// --
    a = -3;
    --a;
    CHECK_TRUE(a == -4, "--");

    --a;
    CHECK_TRUE(a == -5, "--");

    --a;
    CHECK_TRUE(a == -5, "--");

    {
    a = -3;
    auto res = a--;
    CHECK_TRUE(res == -3, "--");
    CHECK_TRUE(a == -4, "--");
    }

{// bucle básico
    atd::Bounded<int, 0, 4> a = 4;
    for (int i = 4; i > 0; --i, --a){
	CHECK_TRUE(a == i, "basic loop decreasing");
    }
    CHECK_TRUE(a == 0, "basic loop decreasing");
    --a;
    CHECK_TRUE(a == 0, "basic loop decreasing");


    a = 0;
    for (int i = 0;  i < 4; ++i, ++a){
	CHECK_TRUE(a == i, "basic loop increasing");
    }
    CHECK_TRUE(a == 4, "basic loop increasing");
    ++a;
    CHECK_TRUE(a == 4, "basic loop increasing");

}
}


}




int main()
{
try{
    test::header("atd_types");

    test_bounded();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





