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


#include "../../atd_geometry_2d.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;



void test_point()
{
    test::interface("Poing");

    using Point = atd::geo2d::Point<int>;

    {
	Point p{3, 9};

	CHECK_TRUE(p.x == 3, "x");
	CHECK_TRUE(p.y == 9, "y");
	CHECK_PRINT(std::cout << p, "(3, 9)");
    }
}



int main()
{
try{
    test::header("Geometry 2D");

    test_point();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





