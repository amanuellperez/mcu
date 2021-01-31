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

#include "../../std_cmath.h"

#include <alp_test.h>
#include <iostream>

#include <bit>


using namespace test;

template <typename Int>
void test_abs(Int x)
{
    CHECK_TRUE(mtd::abs(x) == std::abs(x), "abs");
}

void test_abs()
{
    test::interfaz("abs");

    test_abs<int>(0);
    test_abs<int>(20);
    test_abs<int>(-20);

    test_abs<long>(0);
    test_abs<long>(20l);
    test_abs<long>(-20l);

    test_abs<long long>(0);
    test_abs<long long>(20ll);
    test_abs<long long>(-20ll);

}

int main()
{
try{
    test::header("cmath");

    test_abs();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
