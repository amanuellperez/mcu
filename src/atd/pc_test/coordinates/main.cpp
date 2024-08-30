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

#include "../../atd_coordinates.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>

#include <cstddef>

using namespace test;

void test_pagecol()
{
    test::interface("PageCol");

    using PageCol = atd::PageCol<int>;

    PageCol a{2, 3};
    CHECK_TRUE(a.page == 2, "PageCol{}");
    CHECK_TRUE(a.col  == 3, "PageCol{}");

    PageCol b{4, 7};
    auto c = a + b;
    CHECK_TRUE(c.page == 6, "operator+");
    CHECK_TRUE(c.col  == 10, "operator+");

    c = a - b;
    CHECK_TRUE(c.page == -2, "operator-");
    CHECK_TRUE(c.col  == -4, "operator-");

}


int main()
{
try{
    test::header("atd_coordinates");

    test_pagecol();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





