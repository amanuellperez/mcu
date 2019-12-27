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

#include "../../atd_register.h"

#include <alp_test.h>
#include <iostream>

//using namespace std;
using namespace test;
using namespace atd;

using std::cout;


void tst()
{    
    test::interfaz("Register");

    int x = 0b10101010;

    Register{x}.write_one_bit<0>();
    check_true(x == 0b10101011, "write_one_bit");

    Register{x}.write_one_bit<2>();
    check_true(x == 0b10101111, "write_one_bit");

    Register{x}.write_one_bit<4,6>();
    check_true(x == 0b11111111, "write_one_bit");

    Register{x}.write_zero_bit<0,7>();
    check_true(x == 0b01111110, "write_zero_bit");

    Register{x}.write_zero_bit<2>();
    check_true(x == 0b01111010, "write_zero_bit");

    Register{x}.flip_bit<1,7>();
    check_true(x == 0b11111000, "flip_bit");

    check_true(Register{x}.bit<1>() == 0, "bit");
    check_true(Register{x}.bit<6>() == 1, "bit");

    check_true(Register{x}.is_one_bit<6>() == true , "is_one_bit<6>");
    check_true(Register{x}.is_zero_bit<6>() == false, "is_zero_bit<6>");

    check_true(Register{x}.is_one_bit<0>() == false, "is_one_bit<0>");
    check_true(Register{x}.is_zero_bit<0>() == true, "is_zero_bit<0>");

}



int main()
{
try{
    test::header("atd_register");

    tst();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





