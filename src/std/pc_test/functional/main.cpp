// Copyright (C) 2022 Manuel Perez 
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

#include "../../std_functional.h"
#include <functional>

#include <alp_test.h>
#include <iostream>
#include <alp_string.h>


using namespace test;

template <typename T, typename Cmp>
bool compare(const T& a, const T& b, Cmp cmp)
{ 
    return cmp(a,b); 
}

template <typename Std_cmp, typename Mtd_cmp>
void test_comparisons(Mtd_cmp cmp1, Std_cmp cmp2, const std::string& name)
{
    CHECK_TRUE((compare(2, 3, cmp1) == compare(2, 3, cmp2)), name);
    CHECK_TRUE((compare(2, 2, cmp1) == compare(2, 2, cmp2)), name);
    CHECK_TRUE((compare(3, 2, cmp1) == compare(3, 2, cmp2)), name);
}

void test_comparisons()
{
    test::interface("comparisons");

    test_comparisons(mtd::less<int>{}, std::less<int>{}, "less");
    test_comparisons(mtd::less_equal<int>{}, std::less_equal<int>{}, "less_equal");

    test_comparisons(mtd::greater<int>{}, std::greater<int>{}, "greater");
    test_comparisons(mtd::greater_equal<int>{}, std::greater_equal<int>{}, "greater_equal");

    test_comparisons(mtd::equal_to<int>{}, std::equal_to<int>{}, "equal_to");
    test_comparisons(mtd::not_equal_to<int>{}, std::not_equal_to<int>{}, "not_equal_to");


}

void test_identity()
{
    test::interface("identity");

    int x = 3;
    CHECK_TRUE(mtd::identity{}(x) == std::identity{}(x), "identity");
}

int main()
{
try{
    test::header("functional");

    test_comparisons();
    test_identity();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
