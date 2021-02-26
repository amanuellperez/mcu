// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_static.h"

#include <alp_test.h>
#include <iostream>

using namespace test;


void test_static_array()
{
    test::interfaz("static_array");

    using Pin = atd::static_array<int, 10, 20, 30>;
    CHECK_TRUE(Pin::data[0] == 10, "static_array::data[]");
    CHECK_TRUE(Pin::data[1] == 20, "static_array::data[]");
    CHECK_TRUE(Pin::data[2] == 30, "static_array::data[]");
    CHECK_TRUE(Pin::size == 3, "static_array::size");

    Pin pin;
    CHECK_TRUE(pin[0] == 10, "static_array.operator[]");
    CHECK_TRUE(pin[1] == 20, "static_array.operator[]");
    CHECK_TRUE(pin[2] == 30, "static_array.operator[]");
    CHECK_TRUE(pin.size == 3, "static_array.size");

    using Pin2 = atd::static_array_add<int, 40, Pin>;
    CHECK_TRUE((std::is_same_v<Pin2, atd::static_array<int, 10, 20, 30, 40>>),
		"static_array_add");
}

int main()
{
try{
    test::header("atd_static");

    test_static_array();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





