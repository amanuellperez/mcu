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
    CHECK_TRUE(atd::is_one_bit<0>::of(x) == false, "bit<0>::of");
    CHECK_TRUE(atd::is_one_bit<1>::of(x) == false, "bit<1>::of");
    CHECK_TRUE(atd::is_one_bit<2>::of(x) == false, "bit<2>::of");
    CHECK_TRUE(atd::is_one_bit<3>::of(x) == false, "bit<3>::of");
    CHECK_TRUE(atd::is_one_bit<4>::of(x) == true, "bit<4>::of");
    CHECK_TRUE(atd::is_one_bit<5>::of(x) == true, "bit<5>::of");
    CHECK_TRUE(atd::is_one_bit<6>::of(x) == true, "bit<6>::of");
    CHECK_TRUE(atd::is_one_bit<7>::of(x) == true, "bit<7>::of");

    CHECK_TRUE(atd::is_zero_bit<0>::of(x) == true, "bit<0>::of");
    CHECK_TRUE(atd::is_zero_bit<1>::of(x) != false, "bit<1>::of");
    CHECK_TRUE(atd::is_zero_bit<2>::of(x) != false, "bit<2>::of");
    CHECK_TRUE(atd::is_zero_bit<3>::of(x) != false, "bit<3>::of");
    CHECK_TRUE(atd::is_zero_bit<4>::of(x) != true, "bit<4>::of");
    CHECK_TRUE(atd::is_zero_bit<5>::of(x) != true, "bit<5>::of");
    CHECK_TRUE(atd::is_zero_bit<6>::of(x) != true, "bit<6>::of");
    CHECK_TRUE(atd::is_zero_bit<7>::of(x) != true, "bit<7>::of");
    }

    {
    uint16_t x = 0xF0F0;
    CHECK_TRUE(atd::is_one_bit<0>::of(x) == false, "bit<0>::of");
    CHECK_TRUE(atd::is_one_bit<1>::of(x) == false, "bit<1>::of");
    CHECK_TRUE(atd::is_one_bit<2>::of(x) == false, "bit<2>::of");
    CHECK_TRUE(atd::is_one_bit<3>::of(x) == false, "bit<3>::of");
    CHECK_TRUE(atd::is_one_bit<4>::of(x) == true, "bit<4>::of");
    CHECK_TRUE(atd::is_one_bit<5>::of(x) == true, "bit<5>::of");
    CHECK_TRUE(atd::is_one_bit<6>::of(x) == true, "bit<6>::of");
    CHECK_TRUE(atd::is_one_bit<7>::of(x) == true, "bit<7>::of");
    CHECK_TRUE(atd::is_one_bit<8>::of(x) == false, "bit<8>::of");
    CHECK_TRUE(atd::is_one_bit<9>::of(x) == false, "bit<9>::of");
    CHECK_TRUE(atd::is_one_bit<10>::of(x) == false, "bit<10>::of");
    CHECK_TRUE(atd::is_one_bit<11>::of(x) == false, "bit<11>::of");

    CHECK_TRUE(atd::is_zero_bit<0>::of(x) != false, "bit<0>::of");
    CHECK_TRUE(atd::is_zero_bit<1>::of(x) != false, "bit<1>::of");
    CHECK_TRUE(atd::is_zero_bit<2>::of(x) != false, "bit<2>::of");
    CHECK_TRUE(atd::is_zero_bit<3>::of(x) != false, "bit<3>::of");
    CHECK_TRUE(atd::is_zero_bit<4>::of(x) != true, "bit<4>::of");
    CHECK_TRUE(atd::is_zero_bit<5>::of(x) != true, "bit<5>::of");
    CHECK_TRUE(atd::is_zero_bit<6>::of(x) != true, "bit<6>::of");
    CHECK_TRUE(atd::is_zero_bit<7>::of(x) != true, "bit<7>::of");
    CHECK_TRUE(atd::is_zero_bit<8>::of(x) != false, "bit<8>::of");
    CHECK_TRUE(atd::is_zero_bit<9>::of(x) != false, "bit<9>::of");
    CHECK_TRUE(atd::is_zero_bit<10>::of(x) != false, "bit<10>::of");
    CHECK_TRUE(atd::is_zero_bit<11>::of(x) != false, "bit<11>::of");
    }


}

void test_concat_bytes()
{
    test::interfaz("concat_bytes");

    CHECK_TRUE(atd::concat_bytes<uint16_t>(0x52, 0xF4) == 0x52F4, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint16_t>(0x00, 0x00) == 0, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint16_t>(0xFF, 0x00) == 0xFF00, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint16_t>(0x00, 0xFF) == 0x00FF, "concat_bytes");

    CHECK_TRUE(atd::concat_bytes<uint32_t>(0x12, 0x34) == 0x1234, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint32_t>(0x12, 0x34, 0x56) == 0x123456, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int32_t>(0x12, 0x34, 0x56) == 0x123456, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint32_t>(0x12, 0x34, 0x56, 0x78) == 0x12345678, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int32_t>(0x12, 0x34, 0x56, 0x78) == 0x12345678, "concat_bytes");

    CHECK_TRUE(atd::concat_bytes<uint64_t>(0x12, 0x34) == 0x1234, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint64_t>(0x12, 0x34, 0x56) == 0x123456, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int64_t>(0x12, 0x34, 0x56) == 0x123456, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<uint64_t>(0x12, 0x34, 0x56, 0x78) == 0x12345678, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int64_t>(0x12, 0x34, 0x56, 0x78) == 0x12345678, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int64_t>(0x12, 0x34, 0x56, 0x78, 0x9A) == 0x123456789A, "concat_bytes");
    CHECK_TRUE(atd::concat_bytes<int64_t>
		(0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC) == 0x123456789ABC, 
		"concat_bytes");

    CHECK_TRUE(atd::concat_bytes<int64_t>
		(0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE) == 0x123456789ABCDE, 
		"concat_bytes");
}

void test_bitmask()
{
    test::interfaz("bitmask");

    CHECK_TRUE((atd::bitmask<1,2, uint8_t>() == 0x06), "bitmask");
    CHECK_TRUE((atd::bitmask<1,1, uint8_t>() == 0x02), "bitmask");

    CHECK_TRUE((atd::bitmask<1,2, int16_t>() == 0x06), "bitmask");
    CHECK_TRUE((atd::bitmask<10,12, int16_t>() == 0x1C00), "bitmask");
    CHECK_TRUE((atd::bitmask<0,12, int16_t>() == 0x1FFF), "bitmask");

    CHECK_TRUE((atd::bitmask<31,31, uint32_t>() == 0x80000000), "bitmask");

    {
    constexpr uint8_t x = atd::bitmask<1,2,uint8_t>();
    CHECK_TRUE(x == 0x06, "bitmask in compile time");
    }
}


template <typename Int>
void test_mask()
{
    // TODO: more tests!!! but how??? random? systematic?
    constexpr atd::Mask_of_bits<2,6, Int> mask;

    Int x = 0x35;
    Int res = mask(x);
    CHECK_TRUE(res == Int{0x0D}, "Mask_of_bits::get");

    mask(x) = Int{0x16};
    CHECK_TRUE(x == Int{0x59}, "Mask_of_bits::set");
}

void test_mask()
{
    test::interfaz("Mask_of_bits");
    
    test_mask<uint8_t>();
    test_mask<int8_t>();
    test_mask<uint16_t>();
    test_mask<int16_t>();
    test_mask<uint32_t>();
    test_mask<int32_t>();
    test_mask<uint64_t>();
    test_mask<int64_t>();
}






template <typename Int>
void test_write_bits(int x, int res)
{
    atd::write_bits<0,1>::to<1,1>::in(x);
    CHECK_TRUE(x == res, "write_bits");

}

void test_write_bits()
{
    test::interfaz("write_bits");

    test_write_bits<uint8_t>(0x00, 0x03);
    test_write_bits<int8_t>(0x00, 0x03);
    test_write_bits<uint16_t>(0x00, 0x03);
    test_write_bits<int16_t>(0x00, 0x03);
    test_write_bits<uint32_t>(0x00, 0x03);
    test_write_bits<int32_t>(0x00, 0x03);
    test_write_bits<uint64_t>(0x00, 0x03);
    test_write_bits<int64_t>(0x00, 0x03);

    test_write_bits<uint8_t>(0xF0, 0xF3);
    test_write_bits<int8_t>(0xF0, 0xF3);
    test_write_bits<uint16_t>(0xF0, 0xF3);
    test_write_bits<int16_t>(0xF0, 0xF3);
    test_write_bits<uint32_t>(0xF0, 0xF3);
    test_write_bits<int32_t>(0xF0, 0xF3);
    test_write_bits<uint64_t>(0xF0, 0xF3);
    test_write_bits<int64_t>(0xF0, 0xF3);

    int x = 0;
    atd::write_bit<0>::to<1>::in(x);
    CHECK_TRUE(x == 0x01, "write_bit");
}

int main()
{
try{
    test::header("atd_bit");

    test_bitmask();
    test_mask();
    test_bit();
    test_concat_bytes();
    test_write_bits();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





