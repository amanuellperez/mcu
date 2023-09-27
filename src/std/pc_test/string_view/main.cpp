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

#include "../../std_string_view.h"
#include <string_view>

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <string>

using namespace test;

void test_types()
{    
    using S = std::string_view;
    using M = mtd::string_view;
    CHECK_TRUE(std::is_same_v<S::value_type, M::value_type>, "value_type");
    CHECK_TRUE(std::is_same_v<S::pointer, M::pointer>, "pointer");
    CHECK_TRUE(std::is_same_v<S::const_pointer, M::const_pointer>, "const_pointer");
    CHECK_TRUE(std::is_same_v<S::reference, M::reference>, "reference");
    CHECK_TRUE(std::is_same_v<S::const_reference, M::const_reference>, "const_reference");
    CHECK_TRUE(std::is_same_v<S::const_iterator, M::const_iterator>, "const_iterator");
    CHECK_TRUE(std::is_same_v<S::size_type, M::size_type>, "size_type");
    CHECK_TRUE(std::is_same_v<S::difference_type, M::difference_type>, "difference_type");
}

void compare_strings_views0(const std::string_view& sview, const mtd::string_view& mview)
{
    CHECK_TRUE(sview.size() == mview.size(), "size");
    CHECK_TRUE(sview.length() == mview.length(), "length");
    CHECK_TRUE(sview.empty() == mview.empty(), "empty");

    CHECK_EQUAL_CONTAINERS(sview, mview, "equal?");
    CHECK_EQUAL_CONTAINERS(sview.cbegin(), sview.cend(),
			   mview.cbegin(), mview.cend(), "cbegin/cend");

    CHECK_EQUAL_CONTAINERS(sview.rbegin(), sview.rend(),
			   mview.rbegin(), mview.rend(), "rbegin/rend");

    CHECK_EQUAL_CONTAINERS(sview.crbegin(), sview.crend(),
			   mview.crbegin(), mview.crend(), "crbegin/crend");


    for (size_t i = 0; i < mview.size(); ++i)
	CHECK_TRUE(sview[i] == mview[i], 
		    alp::as_str() << "operator[" << i << "]");
}

void compare_strings_views1(const std::string_view& sview, const mtd::string_view& mview)
{
    // front da core tanto con std como con mtd (normal, accede a data[0])
    CHECK_TRUE(sview.front() == mview.front(), "front");
    CHECK_TRUE(sview.back() == mview.back(), "back");
    CHECK_TRUE(sview.data() == mview.data(), "data");

}


template <typename Str>
void test_basic(const Str& str)
{
    test::interface("basic");
    std::string_view sview{str.begin(), str.end()};
    mtd::string_view mview{str.begin(), str.end()};

    compare_strings_views0(sview, mview);
    compare_strings_views1(sview, mview);
}

void test_empty()
{
    test::interface("empty");
    compare_strings_views0(std::string_view{}, mtd::string_view{});
}

void test_const_char()
{   
    test::interface("const char*");

    const char str[] = "abcd";
    std::string_view sview{str};
    mtd::string_view mview{str};


}

int main()
{
try{
    test::header("string_view");

    test_types();
    test_empty();
    test_basic(std::string{"abcd"});
    // TODO: no compila.
//    test_basic(std::array<char, 5>("abcd"));
    test_const_char("abcd");
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





