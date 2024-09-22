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

struct bounded_plane_type_cfg {
    using index_type = int;
    
// Dimensiones del background
    static constexpr index_type bg_width  = 16;
    static constexpr index_type bg_height = 4;
    
// Dimensiones del rectángulo
    static constexpr index_type width = 6;
    static constexpr index_type height= 2;

};

void test_fix_bounded_rectangle_ij_plane_type()
{
    test::interface("Bounded_rectangle_ij_plane_type");
    
    using BR = atd::Bounded_rectangle_ij<bounded_plane_type_cfg>;
    using Coord = BR::Coord_ij;

    BR br;

    CHECK_TRUE(br.width() == bounded_plane_type_cfg::width, "width()");
    CHECK_TRUE(br.height() == bounded_plane_type_cfg::height, "height()");

    CHECK_TRUE(br.p0() == Coord{0,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,5}, "p1()");
    CHECK_TRUE(br.pe() == Coord{2,6}, "pe()");

    CHECK_TRUE(br.scroll_left(1) == nm::fail, "scroll_left");
    CHECK_TRUE(br.scroll_up(1) == nm::fail, "scroll_up");

    CHECK_TRUE(br.scroll_right(1) == nm::ok, "scroll_right");

    CHECK_TRUE(br.p0() == Coord{0,1}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,6}, "p1()");
    CHECK_TRUE(br.pe() == Coord{2,7}, "pe()");

    CHECK_TRUE(br.scroll_down(1) == nm::ok, "scroll_down");

    CHECK_TRUE(br.p0() == Coord{1,1}, "p0()");
    CHECK_TRUE(br.p1() == Coord{2,6}, "p1()");
    CHECK_TRUE(br.pe() == Coord{3,7}, "pe()");

// moviendo fuera del bg
    CHECK_TRUE(br.move_to({0, 10}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{0,10}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,15}, "p1()");
    CHECK_TRUE(br.pe() == Coord{2,16}, "pe()");

    CHECK_TRUE(br.move_to({0, 11}) == nm::fail, "move_to");

    CHECK_TRUE(br.move_to({2, 0}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{2,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{3,5}, "p1()");
    CHECK_TRUE(br.pe() == Coord{4,6}, "pe()");

    CHECK_TRUE(br.move_to({3, 0}) == nm::fail, "move_to");

// Cambio de coordenadas
    br.move_to({1,3});
    CHECK_TRUE(br.local_to_background({1,2}) == Coord{2, 5}, "local_to_background");
    CHECK_TRUE(br.background_to_local({2,5}) == Coord{1, 2}, "background_to_local");
    // observar que funciona con numeros negativos si index_type es signed
    CHECK_TRUE(br.background_to_local({0,0}) == Coord{-1, -3}, "background_to_local");
}


struct bounded_cylinder_type_cfg {
// Type
    static constexpr bool cylinder_type{};

    using index_type = int;
    
// Dimensiones del background
    static constexpr index_type bg_width  = 16;
    static constexpr index_type bg_height = 4;
    
// Dimensiones del rectángulo
    static constexpr index_type width = 6;
    static constexpr index_type height= 2;

};



void test_fix_bounded_rectangle_ij_cylinder_type()
{
    test::interface("Bounded_rectangle_ij_cylinder_type");
    
    using BR = atd::Bounded_rectangle_ij<bounded_cylinder_type_cfg>;
    using Coord = BR::Coord_ij;

    BR br;

    CHECK_TRUE(br.width() == bounded_cylinder_type_cfg::width, "width()");
    CHECK_TRUE(br.height() == bounded_cylinder_type_cfg::height, "height()");

    CHECK_TRUE(br.p0() == Coord{0,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,5}, "p1()");
    CHECK_TRUE(br.pm() == br.pe()   , "pm()");
    CHECK_TRUE(br.pe() == Coord{2,6}, "pe()");


    CHECK_TRUE(br.scroll_right(1) == nm::ok, "scroll_right");

    CHECK_TRUE(br.p0() == Coord{0,1}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,6}, "p1()");
    CHECK_TRUE(br.pm() == br.pe()   , "pm()");
    CHECK_TRUE(br.pe() == Coord{2,7}, "pe()");

    CHECK_TRUE(br.scroll_down(1) == nm::ok, "scroll_down");

    CHECK_TRUE(br.p0() == Coord{1,1}, "p0()");
    CHECK_TRUE(br.p1() == Coord{2,6}, "p1()");
    CHECK_TRUE(br.pm() == br.pe()   , "pm()");
    CHECK_TRUE(br.pe() == Coord{3,7}, "pe()");

    CHECK_TRUE(br.move_to({0, 10}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{0,10}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,15}, "p1()");
    CHECK_TRUE(br.pm() == br.pe()   , "pm()");
    CHECK_TRUE(br.pe() == Coord{2,16}, "pe()");

    CHECK_TRUE(br.move_to({0, 11}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{0,11}, "p0()");
    CHECK_TRUE(br.p1() == Coord{1,0}, "p1()");
    CHECK_TRUE(br.pm() == Coord{2,16}, "pm()");
    CHECK_TRUE(br.pe() == Coord{2,1}, "pe()");

    CHECK_TRUE(br.move_to({2, 0}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{2,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{3,5}, "p1()");
    CHECK_TRUE(br.pm() == br.pe()   , "pm()");
    CHECK_TRUE(br.pe() == Coord{4,6}, "pe()");

    CHECK_TRUE(br.move_to({3, 0}) == nm::ok, "move_to");
    CHECK_TRUE(br.p0() == Coord{3,0}, "p0()");
    CHECK_TRUE(br.p1() == Coord{0,5}, "p1()");
    CHECK_TRUE(br.pm() == Coord{4,6}, "pm()");
    CHECK_TRUE(br.pe() == Coord{1,6}, "pe()");

// cambio de coordenadas
    br.move_to({1,3});
    CHECK_TRUE(br.local_to_background({1,2}) == Coord{2, 5}, "local_to_background");
    CHECK_TRUE(br.background_to_local({2,5}) == Coord{1, 2}, "background_to_local");
    // observar que funciona con numeros negativos si index_type es signed
    CHECK_TRUE(br.background_to_local({0,0}) == Coord{-1, -3}, "background_to_local");
}


int main()
{
try{
    test::header("atd_coordinates");

    test_pagecol();
    test_fix_bounded_rectangle_ij_plane_type();
    test_fix_bounded_rectangle_ij_cylinder_type();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





