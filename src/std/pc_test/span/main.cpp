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

void test_conversion_from_array(std::span<int> s, mtd::span<int> m, size_t sz)
{
    CHECK_TRUE(s.size() == m.size(), "size");
    CHECK_TRUE(s.extent == m.extent, "extent");
    CHECK_TRUE(m.size() == sz, "size");

    // Observar que como pasamos std::span<int> el extent es dinámico.
    // ¿por qué? No debería de ser static? A fin de cuentas estoy pasando un
    // array de C por debajo (en el caso de a[9])
    std::cout << "std.extent =\t\t\t" << s.extent
	      << "\nmtd.extent =\t\t\t" << m.extent 
	      << "\nstatic_cast<size_t>(-1) =\t" << static_cast<size_t>(-1) << '\n';

    std::cout << "std.size () = " << s.size()
	      << "\nmtd.size() = " << m.size() << '\n';
}




void test_span()
{
    int a[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

    check_equal0(a, a);

    std::array sa = {3, 6, 9};
    mtd::array ma = {3, 6, 9};

    test_conversion_from_array(a, a, 9);
    test_conversion_from_array(sa, ma, 3);

    check_span0(sa, ma);

    {// constructor(it, n)
	std::span<int, 2> sb{a, 2};
	mtd::span<int, 2> mb{a, 2};
	check_equal(sb, mb);
    }
    {// first
	std::span<int, 4> sb{a, 4};
	mtd::span<int, 4> mb{a, 4};

	auto sc1 = sb.first<2>();
	auto mc1 = mb.first<2>();
	check_equal(sc1, mc1);

	auto sc2 = sb.first(2);
	auto mc2 = mb.first(2);
	check_equal(sc2, mc2);
    }

    {// last
	std::span<int, 4> sb{a, 4};
	mtd::span<int, 4> mb{a, 4};

	auto sc1 = sb.last<2>();
	auto mc1 = mb.last<2>();
	check_equal(sc1, mc1);

	auto sc2 = sb.last(2);
	auto mc2 = mb.last(2);
	check_equal(sc2, mc2);

    }

    {// Deduction guides
    std::array<int, 10> sa;
    mtd::array<int, 10> ma;

    std::span sb{sa};
    mtd::span mb{ma};
    CHECK_TRUE(sb.size() == mb.size(), "Deduction guides");
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
