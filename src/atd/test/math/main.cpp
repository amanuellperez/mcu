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

#include "../../atd_math.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <vector>

#include <cstddef>

using namespace test;


void test_div()
{
    test::interfaz("div");

    auto [q, r] = atd::div(20, 3);
    CHECK_TRUE(q == 6 and r == 2, "div(20/3)");
}


int main()
{
try{
    test::header("atd_math");

    test_div();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





