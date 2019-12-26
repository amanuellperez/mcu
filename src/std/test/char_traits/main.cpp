// Copyright (C) 2019-2020 A.Manuel L.Perez
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
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
using namespace test;



void test_char_traits()
{
    using tr = char_traits<char>;
    
    test::interfaz("char_traits");
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

int main()
{
try{
    test::header("char_traits");

    test_char_traits();

}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





