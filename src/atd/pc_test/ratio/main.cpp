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

#include "../../atd_ratio.h"

#include <alp_test.h>
#include <iostream>


using namespace test;


void test_ratio()
{
    test::interfaz("inverse");

    {
	using X = std::ratio<2,3>;
	using Y = std::ratio<3,2>;
    CHECK_TRUE((std::is_same_v<atd::ratio_inverse<X>,Y>), "ratio_inverse");
    }
}

int main()
{
try{
    test::header("atd_ratio");

    test_ratio();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





