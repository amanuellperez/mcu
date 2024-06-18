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

#include "../../atd_static.h"

#include <alp_test.h>
#include <iostream>
#include <algorithm>

using namespace test;

struct Opt{
    int id;
    int next;
};

std::ostream& operator<<(std::ostream& out, const Opt& opt)
{
    return out<< '(' << opt.id << ", " << opt.next << ')';
}

bool operator==(const Opt& a, const Opt& b)
{
    return a.id == b.id and a.next == b.next;
}


void test_static_array()
{
    test::interface("static_array");

    using Pin = atd::static_array<int, 10, 20, 30>;
    CHECK_TRUE(Pin::at<0> == 10, "static_array::at<>");
    CHECK_TRUE(Pin::at<1> == 20, "static_array::at<>");
    CHECK_TRUE(Pin::at<2> == 30, "static_array::at<>");

    CHECK_TRUE(Pin::data[0] == 10, "static_array::data[]");
    CHECK_TRUE(Pin::data[1] == 20, "static_array::data[]");
    CHECK_TRUE(Pin::data[2] == 30, "static_array::data[]");
    CHECK_TRUE(Pin::size == 3, "static_array::size");

    Pin pin;
    CHECK_TRUE(pin[0] == 10, "static_array.operator[]");
    CHECK_TRUE(pin[1] == 20, "static_array.operator[]");
    CHECK_TRUE(pin[2] == 30, "static_array.operator[]");
    CHECK_TRUE(pin.size == 3, "static_array.size");

    // caso degenerado
    using Null = atd::static_array<int>;
    CHECK_TRUE(Null::size == 0, "static_array.size");

// static_array_push_back
    using Pin2 = atd::static_array_push_back<int, Pin, 40>;
    CHECK_TRUE((std::is_same_v<Pin2, atd::static_array<int, 10, 20, 30, 40>>),
		"static_array_push_back");

// static_array_push_front
    {
    using Pin2 = atd::static_array_push_front<int, Pin, 40>;
    CHECK_TRUE((std::is_same_v<Pin2, atd::static_array<int, 40, 10, 20, 30>>),
		"static_array_push_front");
    }


// Funciona con algoritmos std?
    {
	using Array = atd::static_array<int, 0,1,2,0,1,0,3,3,4,0>;
	
	int n0 = std::count(Array::begin(), Array::end(), 0);
	CHECK_TRUE(n0 == 4, "count");

	int n1 = std::count(Array::begin(), Array::end(), 1);
	CHECK_TRUE(n1 == 2, "count");

	int n2 = std::count(Array::begin(), Array::end(), 2);
	CHECK_TRUE(n2 == 1, "count");

	int n3 = std::count(Array::begin(), Array::end(), 3);
	CHECK_TRUE(n3 == 2, "count");

	int n4 = std::count(Array::begin(), Array::end(), 4);
	CHECK_TRUE(n4 == 1, "count");

	int n5 = std::count(Array::begin(), Array::end(), 5);
	CHECK_TRUE(n5 == 0, "count");
    }
}


template <int opt0>
struct is_opt{
    constexpr bool operator()(const Opt& opt) const { return (opt.id == opt0); }

};


void test_static_find_if()
{
    using Array_opt = atd::static_array<Opt, Opt{0, 1}, Opt{2, 1}, Opt{0, 2},
	  Opt{1,1}, Opt{2,2}, Opt{0, 3}, Opt{1,2}>;

    using Subset0 = atd::static_extract_subset<is_opt<0>, Array_opt>;
    using Subset1 = atd::static_extract_subset<is_opt<1>, Array_opt>;
    using Subset2 = atd::static_extract_subset<is_opt<2>, Array_opt>;
    using Subset3 = atd::static_extract_subset<is_opt<3>, Array_opt>;

    CHECK_TRUE(Subset0::size == 3, "static_extract_subset");
    CHECK_TRUE(Subset1::size == 2, "static_extract_subset");
    CHECK_TRUE(Subset2::size == 2, "static_extract_subset");
    CHECK_TRUE(Subset3::size == 0, "static_extract_subset");

    CHECK_TRUE((Subset0::at<0> == Opt{0,1}), "static_extract_subset");
    CHECK_TRUE((Subset0::at<1> == Opt{0,2}), "static_extract_subset");
    CHECK_TRUE((Subset0::at<2> == Opt{0,3}), "static_extract_subset");

    CHECK_TRUE((Subset1::at<0> == Opt{1,1}), "static_extract_subset");
    CHECK_TRUE((Subset1::at<1> == Opt{1,2}), "static_extract_subset");

    CHECK_TRUE((Subset2::at<0> == Opt{2,1}), "static_extract_subset");
    CHECK_TRUE((Subset2::at<1> == Opt{2,2}), "static_extract_subset");
}


void test_variadic_element()
{
    test::interface("static_variadic_element");

    CHECK_TRUE((atd::static_variadic_element<0, int, 10, 20, 30> == 10),
	"static_variadic_element");
    CHECK_TRUE((atd::static_variadic_element<1, int, 10, 20, 30> == 20),
	"static_variadic_element");
    CHECK_TRUE((atd::static_variadic_element<2, int, 10, 20, 30> == 30),
	"static_variadic_element");

}

int main()
{
try{
    test::header("atd_static");

    test_variadic_element();
    test_static_array();
    test_static_find_if();


}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





