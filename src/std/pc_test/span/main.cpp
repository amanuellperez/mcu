// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../std_span.h"
#include "../../std_array.h"
#include <span>
#include <array>

#include <alp_test.h>
#include <iostream>

using namespace test;


void check_span(std::span<int> sp, mtd::span<int> mp)
{
    CHECK_TRUE(sp.size() == mp.size(), "size");

    for (size_t i = 0; i < sp.size(); ++i)
	CHECK_TRUE(sp[i] == mp[i], "operator[]");

    {
	auto si = sp.begin();
	auto mi = mp.begin();
	
	while (si != sp.end()){
	    CHECK_TRUE(*si == *mi, "iterator");
	    ++si;
	    ++mi;
	}
	CHECK_TRUE(mi == mp.end(), "end");

    }
}


void test_span()
{
    int a[3] = {10, 20, 30};

    check_span(a, a);

    std::array sa = {3, 6, 9};
    mtd::array ma = {3, 6, 9};

    check_span(sa, ma);

}





int main()
{
try{
    test::header("span");

    test_span();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
