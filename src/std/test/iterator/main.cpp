// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../std_iterator.h"
#include "../../std_algorithm.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

using namespace test;

void test_reverse_iterator()
{
    test::interfaz("reverse_iterator");
    
    std::vector v = {1,2,3,4,5,6};

    auto p0 = mtd::reverse_iterator(v.end());
    auto pe = mtd::reverse_iterator(v.begin());

    auto q0 = v.rbegin();
    auto qe = v.rend();

    while (q0 != qe and p0 != pe){
	CHECK_TRUE(*p0 == *q0, "reverse_iterator");
	++p0; ++q0;
    }

    CHECK_TRUE(q0 == qe and p0 == pe, "reverse_iterator(size)");
 

}


int main()
{
try{
    test::header("std_iterator.h");

    test_reverse_iterator();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





