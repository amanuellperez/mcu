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

#include "../../atd_float.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>



using namespace test;

template <int ndec>
void test_integer_and_decimal_part(int i0, int d0, int dres)
{
    using D = atd::Decimal<int, ndec>;

    D res;
    atd::Integer_and_decimal_part{i0,d0}.as(res);
    auto [i, d] = res.value();
    CHECK_TRUE(i == i0 and d == dres, 
		alp::as_str() << "as_decimal(" << i0 << ", " << d0 << ")");
}


void test_integer_and_decimal_part()
{
    test::interface("integer_and_decimal_part");
    test_integer_and_decimal_part<2>(0, 0, 0);

    test_integer_and_decimal_part<1>(2, 3, 3);
    test_integer_and_decimal_part<2>(2, 3, 30);
    test_integer_and_decimal_part<3>(2, 3, 300);

    test_integer_and_decimal_part<1>(-2, 3, 3);
    test_integer_and_decimal_part<2>(-2, 3, 30);
    test_integer_and_decimal_part<3>(-2, 3, 300);

}

int main()
{
try{
    test::header("atd_names");
    
    test_integer_and_decimal_part();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





