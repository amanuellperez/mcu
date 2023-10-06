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
#include "../../std_array.h"
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

void test_constructor()
{
    test::interface("constructor");

    mtd::string_view sempty{};
    CHECK_TRUE(sempty.size() == 0, "string_view()");

    // CUIDADO: str.begin()/end() NO son iteradores para mtd!!!
    // No tienen especializaciones de la primary template mtd::iterator_traits!!
    std::string str{"abc"};
//    mtd::string_view sit{str.begin(), str.end()};
//    CHECK_EQUAL_CONTAINERS(str, sit, "string_view(begin, end)");

    mtd::string_view schar{"abc"};
    CHECK_EQUAL_CONTAINERS(str, schar, "string_view(const char*)");

    mtd::string_view schar2{"abc", 2};
    std::string str2{"ab"};
    CHECK_EQUAL_CONTAINERS(str2, schar2, "string_view(const char* size_type)");


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


template <std::contiguous_iterator It>
void test_basic(It p0, It pe)
{
    test::interface("basic");
    std::string_view sview{p0, pe};
    mtd::string_view mview{p0, pe};

    compare_strings_views0(sview, mview);
    compare_strings_views1(sview, mview);
}

void test_empty()
{
    test::interface("empty");
    compare_strings_views0(std::string_view{}, mtd::string_view{});
}

void test_const_char(const char* str)
{   
    test::interface("const char*");
    std::cout << "str = " << str << '\n';

    std::string_view sview{str};
    mtd::string_view mview{str};

    compare_strings_views0(sview, mview);
    compare_strings_views1(sview, mview);


}

void test_remove_prefix(const char* str, size_t n, const std::string& res)
{
    mtd::string_view sview{str};

    sview.remove_prefix(n);
    CHECK_EQUAL_CONTAINERS(sview, res, 
	    alp::as_str() << "remove_prefix(" << str 
			  << ", " << n << ", " << res << ")");
}


void test_remove_prefix()
{
    test::interface("remove_prefix");

    test_remove_prefix("123456789", 3, "456789");
    test_remove_prefix("123", 0, "123");
    test_remove_prefix("123", 1, "23");
    test_remove_prefix("123", 2, "3");
    test_remove_prefix("123", 3, "");
}


void test_remove_suffix(const char* str, size_t n, const std::string& res)
{
    mtd::string_view sview{str};

    sview.remove_suffix(n);
    CHECK_EQUAL_CONTAINERS(sview, res, 
	    alp::as_str() << "remove_suffix(" << str 
			  << ", " << n << ", " << res << ")");
}


void test_remove_suffix()
{
    test::interface("remove_suffix");

    test_remove_suffix("123456789", 3, "123456");
    test_remove_suffix("123", 0, "123");
    test_remove_suffix("123", 1, "12");
    test_remove_suffix("123", 2, "1");
    test_remove_suffix("123", 3, "");
}

template <std::contiguous_iterator It>
void test_swap(It p0, It pe, It q0, It qe)
{   
    mtd::string_view sa{p0, pe};
    mtd::string_view sb{q0, qe};

    sa.swap(sb);

    CHECK_EQUAL_CONTAINERS(sa.begin(), sa.end(), q0, qe, "(sa == b?)");
    CHECK_EQUAL_CONTAINERS(sb.begin(), sb.end(), p0, pe, "(sb == a?)");
}


void test_swap()
{
    test::interface("swap");

    const char str0[] = "abcd";
    const char str1[] = "1234";

    test_swap(mtd::begin(str0), mtd::end(str0),
	      mtd::begin(str1), mtd::end(str1));
}

void test_find()
{
    test::interface("find");

    const char str[] = "abcd";
    mtd::string_view sview{str};

    auto i = sview.find('a');
    CHECK_TRUE(i == 0, "find");

    i = sview.find('b');
    CHECK_TRUE(i == 1, "find");

    i = sview.find('c');
    CHECK_TRUE(i == 2, "find");

    i = sview.find('z');
    CHECK_TRUE(i == mtd::string_view::npos, "find");

    i = sview.find("bc");
    CHECK_TRUE(i == 1, "find");

    i = sview.find("ac");
    CHECK_TRUE(i == mtd::string_view::npos, "find");
}

int main()
{
try{
    test::header("string_view");

    test_types();

    test_constructor();

    test_empty();

    const char str[] = "abcd";
    test_basic(std::begin(str), std::end(str));

    test_const_char("abcd");

    test_remove_prefix();
    test_remove_suffix();
    test_swap();

    test_find();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





