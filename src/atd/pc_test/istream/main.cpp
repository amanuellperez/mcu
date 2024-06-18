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

#include "../../atd_istream.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>



using namespace test;

void test_read_int_as_hex(int x0, bool with_0x)
{
    std::stringstream s;
    if (with_0x)
	s << "0x";

    s << std::hex << x0;

    int x;
    atd::read_int_as_hex(s, x, with_0x);
//    CHECK_TRUE(s.good() or s.eof(), "good");
    CHECK_TRUE(x == x0, alp::as_str() << x);
}

void test_read_int_as_hex_space(int x0, bool with_0x, char space)
{
    std::stringstream s;
    if (with_0x)
	s << "0x";

    s << std::hex << x0 << space << "jklmn";

    int x;
    int res = atd::read_int_as_hex(s, x, with_0x);
    CHECK_TRUE(x == x0 and res != 0, alp::as_str() << x);
}


void test_read_int_as_hex_fail()
{
    std::stringstream s;
    s << "kk";
    int x;
    atd::read_int_as_hex(s, x);
    CHECK_TRUE(s.fail(), "fail");
}

void test_read_int_as_hex()
{
    test::interface("read_int_as_hex");

    for (int i = 0; i < 100000; ++i)
	test_read_int_as_hex(i, true);

    test_read_int_as_hex_fail();

    for (int i = 0; i < 100000; ++i)
	test_read_int_as_hex(i, false);

    test_read_int_as_hex_space(23, false, '\n');
    test_read_int_as_hex_space(23, false, '\t');
    test_read_int_as_hex_space(23, false, '\r');
    test_read_int_as_hex_space(23, false, ' ');
//     test_read_int_as_hex_space(23, false, 'r'); <-- esta tiene que fallar
}


int main()
{
try{
    test::header("atd_istream");

    test_read_int_as_hex();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





