// Copyright (C) 2021 Manuel Perez 
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

#include "../../atd_ratio.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>


using namespace test;


void test_ratio()
{
    test::interface("inverse");

    {
	using X = std::ratio<2,3>;
	using Y = std::ratio<3,2>;
    CHECK_TRUE((std::is_same_v<atd::ratio_inverse<X>,Y>), "ratio_inverse");
    }
}

template <int n, int d>
void test_is_power_of_ten(bool res)
{
    using Q = std::ratio<n, d>;
    CHECK_TRUE(atd::ratio_is_power_of_ten<Q> == res, "ratio_is_power_of_ten");
}

void test_is_power_of_ten()
{
    test::interface("is_power_of_ten");

    test_is_power_of_ten<10, 1>(true);
    test_is_power_of_ten<100, 1>(true);
    test_is_power_of_ten<1000, 1>(true);
    test_is_power_of_ten<10000, 1>(true);
    test_is_power_of_ten<100000, 1>(true);
    test_is_power_of_ten<1000000, 1>(true);

    test_is_power_of_ten<1, 1>(true);
    test_is_power_of_ten<1, 10>(true);
    test_is_power_of_ten<1, 100>(true);
    test_is_power_of_ten<1, 1000>(true);
    test_is_power_of_ten<1, 10000>(true);
    test_is_power_of_ten<1, 100000>(true);
    test_is_power_of_ten<1, 1000000>(true);

    test_is_power_of_ten<1, 2>(false);
    test_is_power_of_ten<2, 3>(false);

}

template <int n, int d>
void test_exponent(int res)
{
    using Q = std::ratio<n, d>;
    CHECK_TRUE(atd::ratio_exponent_of_power_of_ten<Q> == res, 
	    alp::as_str() << "ratio_exponent_of_power_of_ten(" << n << "/" << d <<")");
}


void test_exponent()
{
    test::interface("ratio_exponent_of_power_of_ten");

    test_exponent<10, 1>(1);
    test_exponent<100, 1>(2);
    test_exponent<1000, 1>(3);
    test_exponent<10000, 1>(4);
    test_exponent<100000, 1>(5);
    test_exponent<1000000, 1>(6);

    test_exponent<1, 1>(0);
    test_exponent<1, 10>(-1);
    test_exponent<1, 100>(-2);
    test_exponent<1, 1000>(-3);
    test_exponent<1, 10000>(-4);
    test_exponent<1, 100000>(-5);
    test_exponent<1, 1000000>(-6);

}

int main()
{
try{
    test::header("atd_ratio");

    test_ratio();
    test_is_power_of_ten();
    test_exponent();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





