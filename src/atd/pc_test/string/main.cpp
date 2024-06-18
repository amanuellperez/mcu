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

#include "../../atd_string.h"

#include <alp_test.h>
#include <iostream>
#include <cstdint>


using namespace test;

void test_digit_to_int(char x, int res)
{
    CHECK_TRUE(atd::digit_to_<int>(x) == res, "digit_to_int");
}

void test_digit_to_int()
{
    test_digit_to_int('0', 0);
    test_digit_to_int('1', 1);
    test_digit_to_int('2', 2);
    test_digit_to_int('3', 3);
    test_digit_to_int('4', 4);
    test_digit_to_int('5', 5);
    test_digit_to_int('6', 6);
    test_digit_to_int('7', 7);
    test_digit_to_int('8', 8);
    test_digit_to_int('9', 9);
}

void test_digit_to_char(int x, char res)
{
    CHECK_TRUE(atd::digit_to_char(x) == res, "digit_to_char");
}

void test_digit_to_char()
{
    test_digit_to_char(0, '0');
    test_digit_to_char(1, '1');
    test_digit_to_char(2, '2');
    test_digit_to_char(3, '3');
    test_digit_to_char(4, '4');
    test_digit_to_char(5, '5');
    test_digit_to_char(6, '6');
    test_digit_to_char(7, '7');
    test_digit_to_char(8, '8');
    test_digit_to_char(9, '9');
}


void test_binary_char_to()
{
    test::interface("binary_char_to");

    {
	CHECK_TRUE(0b10101010 == atd::binary_char_to<uint8_t>("10101010"), 
							"binary_char_to");

	CHECK_TRUE(0b00000001 == atd::binary_char_to<uint8_t>("00000001"), 
							"binary_char_to");

	CHECK_TRUE(0b10010001 == atd::binary_char_to<uint8_t>("10010001"), 
							"binary_char_to");

	CHECK_TRUE(0b00000000 == atd::binary_char_to<uint8_t>("00000000"), 
							"binary_char_to");
	CHECK_TRUE(0b11111111 == atd::binary_char_to<uint8_t>("11111111"), 
							"binary_char_to");
    }
}

int main()
{
try{
    test::header("atd_string");

    test_digit_to_int();
    test_digit_to_char();
    test_binary_char_to();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





