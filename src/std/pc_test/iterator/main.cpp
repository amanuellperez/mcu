// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../std_iterator.h"
#include "../../std_algorithm.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <vector>
#include <array>

using namespace test;

void test_reverse_iterator()
{
    test::interface("reverse_iterator");
    
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

template <typename It>
void test_advance(It it, int n)
{
    It p = it;
    It q = it;
    std::advance(p, n);
    mtd::advance(q, n);
    std::cout << *p << "; " << *q << '\n';
    CHECK_TRUE(*p == *q, "advance");

}

void test_advance()
{
    test::interface("advance");

    { // random iterator
    int x[10];
    for (int i = 0; i < 10; ++i)
	x[i] = i;
    test_advance(x, 2);
    }
    // TODO: probar bidirectional e input iterator!!!
    // No puedo usar std::list ya que su tipo no es un mtd::bidirectional_it.
}

void test_begin_end_size()
{
    test::interface("begin/end/size");

    int a[3] = {10, 20, 30};
    CHECK_TRUE(mtd::begin(a) == std::begin(a), "begin");
    CHECK_TRUE(mtd::end(a) == std::end(a), "end");
    CHECK_TRUE(mtd::size(a) == std::size(a), "size");

    std::array<int, 3> b{5, 6, 7};
    CHECK_TRUE(mtd::begin(b) == std::begin(b), "begin");
    CHECK_TRUE(mtd::end(b) == std::end(b), "end");
    CHECK_TRUE(mtd::size(b) == std::size(b), "size");
}

template <typename T>
void test_iter_value(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::iter_value_t<T>, std::iter_value_t<T>>
		    , alp::as_str() << "iter_value(" << name_type << ")");
}

void test_iter_value()
{
    test::interface("iter_value");

    test_iter_value<std::vector<int>>("vector<int>");
    test_iter_value<std::array<int,4>>("array<int>");
    test_iter_value<std::string>("string");
		
}

int main()
{
try{
    test::header("std_iterator.h");

    test_reverse_iterator();
    test_advance();
    test_begin_end_size();

    test_iter_value();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





