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

struct bounded_cfg {
    using index_type = int;
    
// Dimensiones del background
    static constexpr index_type bg_width  = 16;
    static constexpr index_type bg_height = 4;
    
// Dimensiones del rectángulo
    static constexpr index_type width = 6;
    static constexpr index_type height= 2;

};

void test_fix_bounded_rectangle_ij()
{
    test::interface("fix_Bounded_rectangle_ij");
    
    using BR = atd::fix_Bounded_rectangle_ij<bounded_cfg>;
    using Coord = BR::Coord_ij;

    BR br;

    CHECK_TRUE(br.width() == bounded_cfg::width, "width()");
    CHECK_TRUE(br.height() == bounded_cfg::height, "height()");

    CHECK_TRUE(br.p0() == Coord{0,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,5}, "p1()");
    CHECK_TRUE(br.pe() == Coord{2,6}, "pe()");

}

int main()
{
try{
    test::header("atd_coordinates");

    test_pagecol();
    test_fix_bounded_rectangle_ij();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





