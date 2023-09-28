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

#include "../../std_char_traits.h" 

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <vector>
#include <cstring>

using namespace test;

void test_basic()
{
    using tr = mtd::char_traits<char>;
    
    test::interface("basic");

    {
	tr::char_type a, b;
	a = 'a';
	b = 'x';

	tr::assign(a, b);
	CHECK_TRUE(a == b, "assign");
	CHECK_TRUE(tr::eq(a, b), "eq");
	a = 'a';
	CHECK_TRUE(!tr::eq(a, b), "!eq");
    }
    {
	tr::char_type s[4] = {'h', 'o', 'l', 'a'};
	tr::char_type d[4];

	auto r = tr::copy(d, s, 4);
	CHECK_TRUE(r == d, "return copy");
	CHECK_EQUAL_CONTAINERS(s, s + 4, d, d + 4, "copy");
    }

    {
	tr::char_type a = 'a';
	tr::int_type i = tr::to_int_type(a);
	tr::char_type r = tr::to_char_type(i);
	CHECK_TRUE(a == r, "to_int_type/to_char_type");
    }

    {
	CHECK_TRUE(tr::eof() == EOF, "eof");
	CHECK_TRUE((tr::eq_int_type(4, 4)), "eq_int_type");
    }
}


void test_length(const char* str)
{
    using mct = mtd::char_traits<char>;
    using sct = std::char_traits<char>;
    CHECK_TRUE(mct::length(str) == sct::length(str), 
		alp::as_str() << "length(" << str << ")");
}

void test_length()
{
    test::interface("length");

    test_length("");
    test_length("abc");
    
}

bool equal_signs(int x, int y)
{
    if (x== 0){
	if (y == 0)	return true;
	else		return false;
    }
    else if (x > 0){
	if (y > 0)	return true;
	else		return false;
    }
    else{ // x < 0
	if (y < 0)	return true;
	else		return false;
    }
// para ver la segunda parte: return (x*y > 0);
}

void test_compare(const char* s1, const char* s2, size_t n)
{
    using mct = mtd::char_traits<char>;
    using sct = std::char_traits<char>;

    CHECK_TRUE(equal_signs(mct::compare(s1, s2, n), sct::compare(s1, s2, n)), 
		alp::as_str() << "compare(" << s1 << ", " << s2 
			      << ", " << n << ")");
}

void test_compare()
{
    test::interface("compare");

    test_compare("", "", 0);

    test_compare("abc", "abc", 0);
    test_compare("abc", "abc", 1);
    test_compare("abc", "abc", 2);
    test_compare("abc", "abc", 3);

    test_compare("abcde", "abcjk", 0);
    test_compare("abcde", "abcjk", 1);
    test_compare("abcde", "abcjk", 2);
    test_compare("abcde", "abcjk", 3);
    test_compare("abcde", "abcjk", 4);
    test_compare("abcde", "abcjk", 5);
}

void test_find(const char* s1, size_t n, char c)
{
    using mct = mtd::char_traits<char>;
    using sct = std::char_traits<char>;

    CHECK_TRUE(mct::find(s1, n, c) == sct::find(s1, n, c), 
		alp::as_str() << "find(" << s1 << ", " << n
			      << ", " << c << ")");
}
void test_find()
{
    test::interface("find");
    
    test_find("", 0, 'd');

    test_find("a", 1, 'a');
    test_find("a", 1, 'b');

    test_find("abcdefg", 0, 'd');
    test_find("abcdefg", 1, 'd');
    test_find("abcdefg", 2, 'd');
    test_find("abcdefg", 3, 'd');
    test_find("abcdefg", 4, 'd');
    test_find("abcdefg", 5, 'd');
    test_find("abcdefg", 6, 'd');
    test_find("abcdefg", 7, 'd');

}
int main()
{
try{
    test::header("char_traits");

    test_basic();

    test_compare();
    test_length();
    test_find();


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





