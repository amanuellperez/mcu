// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_bit.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

#include <cstddef>

using namespace test;


void test_bit()
{    
    test::interfaz("bit");
    {
    uint8_t x = 0xF0;
    CHECK_TRUE(atd::bit<0>::of(x) == false, "bit<0>::of");
    CHECK_TRUE(atd::bit<1>::of(x) == false, "bit<1>::of");
    CHECK_TRUE(atd::bit<2>::of(x) == false, "bit<2>::of");
    CHECK_TRUE(atd::bit<3>::of(x) == false, "bit<3>::of");
    CHECK_TRUE(atd::bit<4>::of(x) == true, "bit<4>::of");
    CHECK_TRUE(atd::bit<5>::of(x) == true, "bit<5>::of");
    CHECK_TRUE(atd::bit<6>::of(x) == true, "bit<6>::of");
    CHECK_TRUE(atd::bit<7>::of(x) == true, "bit<7>::of");
    }

    {
    uint16_t x = 0xF0F0;
    CHECK_TRUE(atd::bit<0>::of(x) == false, "bit<0>::of");
    CHECK_TRUE(atd::bit<1>::of(x) == false, "bit<1>::of");
    CHECK_TRUE(atd::bit<2>::of(x) == false, "bit<2>::of");
    CHECK_TRUE(atd::bit<3>::of(x) == false, "bit<3>::of");
    CHECK_TRUE(atd::bit<4>::of(x) == true, "bit<4>::of");
    CHECK_TRUE(atd::bit<5>::of(x) == true, "bit<5>::of");
    CHECK_TRUE(atd::bit<6>::of(x) == true, "bit<6>::of");
    CHECK_TRUE(atd::bit<7>::of(x) == true, "bit<7>::of");
    CHECK_TRUE(atd::bit<8>::of(x) == false, "bit<8>::of");
    CHECK_TRUE(atd::bit<9>::of(x) == false, "bit<9>::of");
    CHECK_TRUE(atd::bit<10>::of(x) == false, "bit<10>::of");
    CHECK_TRUE(atd::bit<11>::of(x) == false, "bit<11>::of");
    }



    uint8_t x = 0x0A;
    atd::write_one_bit<7>(x);
    CHECK_TRUE(x == 0x8A, "write_one");

    atd::write_zero_bit<7>(x);
    CHECK_TRUE(x == 0x0A, "write_zero");

    atd::write_one_bit<4,5,6,7>(x);
    CHECK_TRUE(x == 0xFA, "write_one");

    atd::write_zero_bit<4,5,6,7>(x);
    CHECK_TRUE(x == 0x0A, "write_zero");
}

void test_concat_bits()
{
    test::interfaz("concat_bits");

    CHECK_TRUE(atd::concat_bits(0x52, 0xF4) == 0x52F4, "concat_bits");
    CHECK_TRUE(atd::concat_bits(0x00, 0x00) == 0, "concat_bits");
    CHECK_TRUE(atd::concat_bits(0xFF, 0x00) == 0xFF00, "concat_bits");
    CHECK_TRUE(atd::concat_bits(0x00, 0xFF) == 0x00FF, "concat_bits");
}

void test_bitmask()
{
    test::interfaz("bitmask");

    uint8_t x = 0b01010101;
    CHECK_TRUE(atd::bitmask0<0>(x) == 0b00000001, "bitmask0");
    CHECK_TRUE(atd::bitmask0<1>(x) == 0b00000000, "bitmask0");
    CHECK_TRUE(atd::bitmask0<2>(x) == 0b00000100, "bitmask0");
    CHECK_TRUE(atd::bitmask0<3>(x) == 0b00000000, "bitmask0");
    CHECK_TRUE(atd::bitmask0<4>(x) == 0b00010000, "bitmask0");
    CHECK_TRUE(atd::bitmask0<5>(x) == 0b00000000, "bitmask0");
    CHECK_TRUE(atd::bitmask0<6>(x) == 0b01000000, "bitmask0");
    CHECK_TRUE(atd::bitmask0<7>(x) == 0b00000000, "bitmask0");

    CHECK_TRUE((atd::bitmask0<0,uint8_t>() == 0b00000001u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<1,uint8_t>() == 0b00000010u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<2,uint8_t>() == 0b00000100u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<3,uint8_t>() == 0b00001000u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<4,uint8_t>() == 0b00010000u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<5,uint8_t>() == 0b00100000u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<6,uint8_t>() == 0b01000000u), "bitmask0");
    CHECK_TRUE((atd::bitmask0<7,uint8_t>() == 0b10000000u), "bitmask0");

    CHECK_TRUE((atd::bitmask1<0,uint8_t>() == 0b11111110u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<1,uint8_t>() == 0b11111101u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<2,uint8_t>() == 0b11111011u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<3,uint8_t>() == 0b11110111u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<4,uint8_t>() == 0b11101111u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<5,uint8_t>() == 0b11011111u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<6,uint8_t>() == 0b10111111u), "bitmask1");
    CHECK_TRUE((atd::bitmask1<7,uint8_t>() == 0b01111111u), "bitmask1");


}

template <typename Int>
void test_Bit()
{
    test::interfaz("Bit_of");

    Int x{0xF0};
    atd::Bit_of<0, Int> b{x};
    b.set();
    CHECK_TRUE(x == Int{0xF1}, "set");
    CHECK_TRUE(b == 1, "operator bool");
    b.clear();
    CHECK_TRUE(x == Int{0xF0}, "clear");
    CHECK_TRUE(b == 0, "operator bool");

    b = 1;
    CHECK_TRUE(x == Int{0xF1}, "operator=");

    b = 0;
    CHECK_TRUE(x == Int{0xF0}, "operator=");

    CHECK_TRUE(atd::bit<0>::of(x) == 0, "of");

    b = 1;
    CHECK_TRUE(atd::bit<0>::of(x) == 1, "of");

    atd::bit<0>::of(x) = 0;
    CHECK_TRUE(b == 0, "of = 0");

    atd::bit<0>::of(x) = 1;
    CHECK_TRUE(b == 1, "of = 1");

    atd::bit<0>::of(x) = false;
    CHECK_TRUE(b == 0, "of = false");

    atd::bit<0>::of(x) = true;
    CHECK_TRUE(b == 1, "of = true");
}

std::byte result() { return std::byte{0xFF}; }

bool is_ok() {return atd::bit<3>::of(result());}

void test_Bit()
{
    test_Bit<uint8_t>();
    test_Bit<std::byte>();

    if (!is_ok())
	std::cout << "Por aqui nunca llega, es para probar que compile is_ok\n";
}


int main()
{
try{
    test::header("atd_bit");

    test_bitmask();
    test_Bit();
    test_bit();
    test_concat_bits();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





