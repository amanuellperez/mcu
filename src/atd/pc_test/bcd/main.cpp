// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_bcd.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>

using namespace test;

void test_bcd(int en_hex, int en_dec)
{
    for (int x = 0; x < 10; ++x){
        int res = atd::BCD2int(en_hex + x);
        CHECK_TRUE(res == en_dec + x,
                   alp::as_str() << "BCD2int(" << en_dec + x << ")");

	CHECK_TRUE(atd::int2BCD(res) == en_hex + x, 
                   alp::as_str() << "int2BCD(" << en_dec + x << ")");
    }
}

void test_bcd()
{
    test::interfaz("BCD2int");

    test_bcd(0x00, 0);
    test_bcd(0x10, 10);
    test_bcd(0x20, 20);
    test_bcd(0x30, 30);
    test_bcd(0x40, 40);
    test_bcd(0x50, 50);
    test_bcd(0x60, 60);
    test_bcd(0x70, 70);
    test_bcd(0x80, 80);
    test_bcd(0x90, 90);

}




int main()
{
try{
    test::header("atd_bcd");

    test_bcd();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





