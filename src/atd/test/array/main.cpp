// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "../../atd_array.h"
#include "../../atd_ostream.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

using namespace test;

template <typename T, uint8_t num_elementos>
void print(const atd::Circular_array<T, num_elementos>& v)
{
    std::cout	<< "size = [" << atd::write_as_uint8_t(v.size()) << "]: ";
    for (uint8_t i = 0; i < v.size(); ++i)
	std::cout << v[i] << ' ';
    std::cout << '\n';
		
}

void test_circular_array()
{
    atd::Circular_array<int, 2> x;
    x.clear();
    print(x);
}


int main()
{
try{
    test::header("atd_array");

    test_circular_array();
}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





