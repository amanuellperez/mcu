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

#include "../../std_span.h"
#include "../../std_array.h"
#include <span>
#include <array>

#include <alp_test.h>
#include <iostream>

using namespace test;


void check_equal0(std::span<int> sp, mtd::span<int> mp)
{
    CHECK_TRUE(sp.size() == mp.size(), "size");

    for (size_t i = 0; i < sp.size(); ++i)
	CHECK_TRUE(sp[i] == mp[i], "operator[]");
}

void check_span0(std::span<int> sp, mtd::span<int> mp)
{
    check_equal0(sp, mp);

    {// begin/end
	auto si = sp.begin();
	auto mi = mp.begin();
	
	while (si != sp.end()){
	    CHECK_TRUE(*si == *mi, "iterator");
	    ++si;
	    ++mi;
	}
	CHECK_TRUE(mi == mp.end(), "end");
    }
    {// rbegin/rend
	auto si = sp.rbegin();
	auto mi = mp.rbegin();
	
	while (si != sp.rend()){
	    CHECK_TRUE(*si == *mi, "reverse_iterator");
	    ++si;
	    ++mi;
	}
	CHECK_TRUE(mi == mp.rend(), "end");

    }

//    {// first
//	//auto si = sp.first<2>();
//	auto mi = mp.first<2>();
//
////	CHECK_TRUE(si.size() == mi.size(), "first(size)");
////
////	for (size_t i = 0; i < si.size(); ++i)
////	    CHECK_TRUE(sp[i] == mp[i], "first");
//    }
////
////    {// last
////	auto si = sp.last<2>();
////	auto mi = mp.last<2>();
////
////	CHECK_TRUE(si.size() == mi.size(), "last(size)");
////
////	for (size_t i = 0; i < si.size(); ++i)
////	    CHECK_TRUE(sp[i] == mp[i], "last");
////    }
}

template <size_t sz>
void check_equal(std::span<int, sz> sp, mtd::span<int, sz> mp)
{
    CHECK_TRUE(sp.size() == mp.size(), "size");

    for (size_t i = 0; i < sp.size(); ++i)
	CHECK_TRUE(sp[i] == mp[i], "operator[]");
}


template <size_t sz>
void check_span(std::span<int, sz> sp, mtd::span<int, sz> mp)
{
    check_equal(sp, mp);

    {// begin/end
	auto si = sp.begin();
	auto mi = mp.begin();
	
	while (si != sp.end()){
	    CHECK_TRUE(*si == *mi, "iterator");
	    ++si;
	    ++mi;
	}
	CHECK_TRUE(mi == mp.end(), "end");
    }
    {// rbegin/rend
	auto si = sp.rbegin();
	auto mi = mp.rbegin();
	
	while (si != sp.rend()){
	    CHECK_TRUE(*si == *mi, "reverse_iterator");
	    ++si;
	    ++mi;
	}
	CHECK_TRUE(mi == mp.rend(), "end");

    }

}


void test_span()
{
    int a[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

    check_equal0(a, a);

    std::array sa = {3, 6, 9};
    mtd::array ma = {3, 6, 9};

    check_span0(sa, ma);

    {// constructor(it, n)
	std::span<int, 2> sb{a, 2};
	mtd::span<int, 2> mb{a, 2};
	check_equal(sb, mb);
    }
    {// first
	std::span<int, 4> sb{a, 4};
	mtd::span<int, 4> mb{a, 4};

	auto sc = sb.first<2>();
	auto mc = mb.first<2>();
	check_equal(sc, mc);

    }
    {// last
	std::span<int, 4> sb{a, 4};
	mtd::span<int, 4> mb{a, 4};

	auto sc = sb.last<2>();
	auto mc = mb.last<2>();
	check_equal(sc, mc);

    }

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
