// Copyright (C) 2023 Manuel Perez 
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


#include "../../atd_rom.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;

struct Read{
    int operator()(const int& x)
    { return x; }
};

constexpr int data[] = 
    {1, 2, 3, 4,
     5, 6, 7, 8,
     9,10,11,12};


void test_rom()
{
    test::interface("atd::Const");

    atd::ROM<int, Read> x{5};

    CHECK_TRUE(x == 5, "int");
}


void test_rom_biarray()
{
    test::interface("ROM_biarray");

    atd::ROM_biarray<int, 3, 4, Read> 
	    x = {1, 2, 3, 4,
		 5, 6, 7, 8,
		 9,10,11,12};

    CHECK_TRUE(x.rows() == 3, "rows");
    CHECK_TRUE(x.cols() == 4, "cols");

    bool ok = true;
    for (size_t i = 0; i < x.size(); ++i)
	if (x[i] != (int) i + 1) ok = false;

    CHECK_TRUE(ok, "operator[]");

    std::cout << "\nBidimensional\n"
	           "-------------\n";
    for (size_t i = 0; i < x.rows(); ++i){
	for (size_t j = 0; j < x.cols(); ++j)
	    std::cout << x(i, j) << ' ';

	std::cout << '\n';
    }
    std::cout << "\n-------------\n"
		 "Now as rows\n"
		 "-----------\n";

    for (size_t i = 0; i < x.rows(); ++i){
	auto  row = x.row(i);
	for (auto y: row)
	    std::cout << y << ' ';
	std::cout << '\n';
    }
    std::cout << '\n';

    // automatic
    ok = true;
    for (size_t i = 0; i < x.rows(); ++i){
	auto  row = x.row(i);
	for (size_t j = 0; j < x.cols(); ++j)
	    if (x(i, j) != row[j]) ok = false;
    }

    CHECK_TRUE(ok, "row()");
}


int main()
{
try{
    test::header("memory");

    test_rom();
    test_rom_biarray();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





