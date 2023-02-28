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

#include "../../atd_bit.h"

#include <alp_test.h>
#include <alp_string.h>
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
    test::interfaz("make_range_bitmask");

    CHECK_TRUE((atd::make_range_bitmask<1,2, uint8_t>() == 0x06), "make_range_bitmask");
    CHECK_TRUE((atd::make_range_bitmask<1,1, uint8_t>() == 0x02), "make_range_bitmask");

    CHECK_TRUE((atd::make_range_bitmask<1,2, int16_t>() == 0x06), "make_range_bitmask");
    CHECK_TRUE((atd::make_range_bitmask<10,12, int16_t>() == 0x1C00), "make_range_bitmask");
    CHECK_TRUE((atd::make_range_bitmask<0,12, int16_t>() == 0x1FFF), "make_range_bitmask");

    CHECK_TRUE((atd::make_range_bitmask<31,31, uint32_t>() == 0x80000000), "make_range_bitmask");

    {
    constexpr uint8_t x = atd::make_range_bitmask<1,2,uint8_t>();
    CHECK_TRUE(x == 0x06, "make_range_bitmask in compile time");
    }
}

void test_make_bitmask()
{
    test::interfaz("make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 0>, uint8_t>() ==
                0b00000001),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 1>, uint8_t>() ==
                0b00000010),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 2>, uint8_t>() ==
                0b00000100),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 3>, uint8_t>() ==
                0b00001000),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 4>, uint8_t>() ==
                0b00010000),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 5>, uint8_t>() ==
                0b00100000),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 6>, uint8_t>() ==
                0b01000000),
               "make_bitmask");
    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 7>, uint8_t>() ==
                0b10000000),
               "make_bitmask");

    CHECK_TRUE((atd::make_bitmask<atd::static_array<uint8_t, 1, 3, 7>, uint8_t>() ==
                0b10001010),
               "make_bitmask");
}

template <typename Int>
void test_mask()
{
    {
    // TODO: more tests!!! but how??? random? systematic?
    constexpr atd::Range_bitmask<2,6, Int> mask;

    Int x = 0x35;
    Int res = mask(x);
    CHECK_TRUE(res == Int{0x0D}, "Range_bitmask::get");

    mask(x) = Int{0x16};
    CHECK_TRUE(x == Int{0x59}, "Range_bitmask::set");

    {// const
    const Int cx = 0x35;
    Int cres = mask(cx);
    CHECK_TRUE(cres == Int{0x0D}, "Range_bitmask::get");
    }
    // ASSERT_NOT_COMPILE(constexpr atd::Range_bitmask<6,5, Int> mask4;)
}

    {// to_bool
    constexpr atd::Range_bitmask<3,3, Int> mask;

    Int x = 0x39;
    CHECK_TRUE(atd::to_bool(mask(x)), "to_bool(Range_bitmask)");

    x = Int{0x11};
    CHECK_TRUE(!atd::to_bool(mask(x)), "!to_bool(Range_bitmask)");

    }
}

void test_mask()
{
    test::interfaz("Range_bitmask");
    
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

template <int pos>
void test_write_bits2(int x, int res)
{
    atd::write_bit<pos>::template to<1>::in(x);
    CHECK_TRUE(x == res, "write_bit");
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

    test_write_bits2<0>(x, 0x01);
}

template <int i0, int i1, int res>
void test_write_range_bits(int x0, int x1)
{
    atd::write_range_bits<i0, i1>::template to<res>::in(x0);
    CHECK_TRUE(x0 == x1, alp::as_str() << "write_range_bits("<< x0 << ")");
}

void test_write_range_bits()
{
    test::interfaz("write_range_bits");

    test_write_range_bits<0,3,0x05>(0, 0x05);
    test_write_range_bits<1,4,0x05>(0, 0x05 << 1);
    test_write_range_bits<2,5,0x05>(0, 0x05 << 2);
    test_write_range_bits<3,6,0x05>(0, 0x05 << 3);
    test_write_range_bits<4,7,0x05>(0, 0x05 << 4);

    test_write_range_bits<0,3,0x05>(0xFF, 0xF5);
    test_write_range_bits<1,4,0x05>(0xFF, 0xEB);
    test_write_range_bits<2,5,0x05>(0xFF, 0xD7);
}

void test_read_bits()
{
    test::interfaz("read_bits");

    uint8_t data = 0b01011010;

// 1 bit
    CHECK_TRUE((atd::read_bits<0>::of(data) == 0b00000000), "read_bits");
    CHECK_TRUE((atd::read_bits<1>::of(data) == 0b00000010), "read_bits");
    CHECK_TRUE((atd::read_bits<2>::of(data) == 0b00000000), "read_bits");
    CHECK_TRUE((atd::read_bits<3>::of(data) == 0b00001000), "read_bits");
    CHECK_TRUE((atd::read_bits<4>::of(data) == 0b00010000), "read_bits");
    CHECK_TRUE((atd::read_bits<5>::of(data) == 0b00000000), "read_bits");
    CHECK_TRUE((atd::read_bits<6>::of(data) == 0b01000000), "read_bits");
    CHECK_TRUE((atd::read_bits<7>::of(data) == 0b00000000), "read_bits");

// 2 bit
    CHECK_TRUE((atd::read_bits<0,1>::of(data) == 0b00000010), "read_bits");
    CHECK_TRUE((atd::read_bits<1,0>::of(data) == 0b00000010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,6>::of(data) == 0b01000000), "read_bits");
    CHECK_TRUE((atd::read_bits<0,7>::of(data) == 0b00000000), "read_bits");
    CHECK_TRUE((atd::read_bits<1,6>::of(data) == 0b01000010), "read_bits");
    CHECK_TRUE((atd::read_bits<1,7>::of(data) == 0b00000010), "read_bits");

// more
    CHECK_TRUE((atd::read_bit<0>::of(data)	== 0b00000000), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1>::of(data)	== 0b00000010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2>::of(data) == 0b00000010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2,3>::of(data)   == 0b00001010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2,3,4>::of(data) == 0b00011010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2,3,4,5>::of(data) == 0b00011010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2,3,4,5,6>::of(data) == 0b01011010), "read_bits");
    CHECK_TRUE((atd::read_bits<0,1,2,3,4,5,6,7>::of(data) == 0b01011010), "read_bits");

}

void test_zero_with_bits()
{
    test::interfaz("zero_with_bits");

// 1 bit
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<0>::to<1>() == 0b00000001), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<1>::to<1>() == 0b00000010), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<2>::to<1>() == 0b00000100), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<3>::to<1>() == 0b00001000), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<4>::to<1>() == 0b00010000), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<5>::to<1>() == 0b00100000), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<6>::to<1>() == 0b01000000), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<7>::to<1>() == 0b10000000), "zero_with_bits");

// more bits
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<0,2,4,6>::to<1,1,1,1>() == 0b01010101), "zero_with_bits");
    CHECK_TRUE((atd::zero<uint8_t>::with_bits<0,1,2,3>::to<1,1,1,1>() == 0b00001111), "zero_with_bits");

}


void test_dynamic_write()
{
    test::interfaz("write_zero");

    uint8_t x = 0xFF;

    atd::write_zero(x, 0);
    CHECK_TRUE(x == 0b11111110, "write_zero");

    atd::write_zero(x, 1);
    CHECK_TRUE(x == 0b11111100, "write_zero");

    atd::write_zero(x, 2);
    CHECK_TRUE(x == 0b11111000, "write_zero");

    atd::write_zero(x, 3);
    CHECK_TRUE(x == 0b11110000, "write_zero");

    atd::write_zero(x, 4);
    CHECK_TRUE(x == 0b11100000, "write_zero");

    atd::write_zero(x, 5);
    CHECK_TRUE(x == 0b11000000, "write_zero");

    atd::write_zero(x, 6);
    CHECK_TRUE(x == 0b10000000, "write_zero");

    atd::write_zero(x, 7);
    CHECK_TRUE(x == 0b00000000, "write_zero");

// write_one
    atd::write_one(x, 0);
    CHECK_TRUE(x == 0b00000001, "write_one");

    atd::write_one(x, 1);
    CHECK_TRUE(x == 0b00000011, "write_one");

    atd::write_one(x, 2);
    CHECK_TRUE(x == 0b00000111, "write_one");

    atd::write_one(x, 3);
    CHECK_TRUE(x == 0b00001111, "write_one");

    atd::write_one(x, 4);
    CHECK_TRUE(x == 0b00011111, "write_one");

    atd::write_one(x, 5);
    CHECK_TRUE(x == 0b00111111, "write_one");

    atd::write_one(x, 6);
    CHECK_TRUE(x == 0b01111111, "write_one");

    atd::write_one(x, 7);
    CHECK_TRUE(x == 0b11111111, "write_one");

}

void test_is_one_most_significant_bit_of()
{
    test::interfaz("is_one_most_significant_bit_of");

// Hago un test exhaustivo
// uint8_t
    std::cout << "uint8_t ... ";
    for (uint8_t n = 0; n < 0x80; ++n)
	if (atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << (int) n << ") is true!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}

    for (uint8_t n = 0x80; n < 0xFF; ++n)
	if (!atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << (int) n << ") is false!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}
    std::cout << "OK\n";

// uint16_t
    std::cout << "uint16_t ... ";
    for (uint16_t n = 0; n < 0x8000; ++n)
	if (atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is true!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}

    for (uint16_t n = 0x8000; n < 0xFFFF; ++n)
	if (!atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is false!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}
    std::cout << "OK\n";

// uint32_t
    std::cout << "uint32_t ... ";
    for (uint32_t n = 0; n < 0x80000000; ++n)
	if (atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is true!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}

    for (uint32_t n = 0x80000000; n < 0xFFFFFFFF; ++n)
	if (!atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is false!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}
    std::cout << "OK\n";



// uint64_t
    std::cout << "uint64_t ... ";
    for (uint64_t n = 0; n < 0x8000000000000000; ++n)
	if (atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is true!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}

    for (uint64_t n = 0x8000000000000000; n < 0xFFFFFFFFFFFFFFFF; ++n)
	if (!atd::is_one_most_significant_bit_of(n)){
	    std::cerr << "ERROR: is_one_most_significant_bit_of(0x" 
				    << std::hex << n << ") is false!\n";
	    throw std::logic_error{"is_one_most_significant_bit_of"};
	}
    std::cout << "OK\n";

}


void test_byte()
{
    test::interfaz("byte<>()");

    std::cout << "uint8_t ... ";
    for (uint8_t n = 0; n < 0xFF; ++n)
	if (atd::byte<0>(n) != n){
	    std::cerr << "ERROR: byte<0>(" << n << ") == " << atd::byte<0>(n) << "???";
	    throw std::logic_error{"byte<>()"};
	}
    std::cout << "OK\n";

// DONT_HAVE_TO_COMPILE(uint8_t y = atd::byte<1>(uint8_t{0xAB}));
    
    {
    uint16_t x = 0xABCD;
    CHECK_TRUE(atd::byte<0>(x) == 0xCD, "byte<0>(uint16_t)");
    CHECK_TRUE(atd::byte<1>(x) == 0xAB, "byte<1>(uint16_t)");
    }

    {
    uint32_t x = 0xABCDEF12;
    CHECK_TRUE(atd::byte<0>(x) == 0x12, "byte<0>(uint32_t)");
    CHECK_TRUE(atd::byte<1>(x) == 0xEF, "byte<1>(uint32_t)");
    CHECK_TRUE(atd::byte<2>(x) == 0xCD, "byte<2>(uint32_t)");
    CHECK_TRUE(atd::byte<3>(x) == 0xAB, "byte<3>(uint32_t)");
    }

    {
    uint64_t x = 0xABCDEF1234567890;
    CHECK_TRUE(atd::byte<0>(x) == 0x90, "byte<0>(uint64_t)");
    CHECK_TRUE(atd::byte<1>(x) == 0x78, "byte<1>(uint64_t)");
    CHECK_TRUE(atd::byte<2>(x) == 0x56, "byte<2>(uint64_t)");
    CHECK_TRUE(atd::byte<3>(x) == 0x34, "byte<3>(uint64_t)");
    CHECK_TRUE(atd::byte<4>(x) == 0x12, "byte<4>(uint64_t)");
    CHECK_TRUE(atd::byte<5>(x) == 0xEF, "byte<5>(uint64_t)");
    CHECK_TRUE(atd::byte<6>(x) == 0xCD, "byte<6>(uint64_t)");
    CHECK_TRUE(atd::byte<7>(x) == 0xAB, "byte<7>(uint64_t)");
    }

}

void test_nibble()
{
    test::interfaz("nibble<>()");


    {
    uint8_t x = 0xEF;
    CHECK_TRUE(atd::nibble<0>(x) == 0x0F, "nibble<0>(uint8_t)");
    CHECK_TRUE(atd::nibble<1>(x) == 0x0E, "nibble<1>(uint8_t)");
    }

//    DONT_HAVE_TO_COMPILE(uint8_t y = atd::nibble<2>(uint8_t{0xAB}));
    
    {
    uint16_t x = 0xABCD;
    CHECK_TRUE(atd::nibble<0>(x) == 0x0D, "nibble<0>(uint16_t)");
    CHECK_TRUE(atd::nibble<1>(x) == 0x0C, "nibble<1>(uint16_t)");
    CHECK_TRUE(atd::nibble<2>(x) == 0x0B, "nibble<2>(uint16_t)");
    CHECK_TRUE(atd::nibble<3>(x) == 0x0A, "nibble<3>(uint16_t)");
    }

    {
    uint32_t x = 0xABCDEF12;
    CHECK_TRUE(atd::nibble<0>(x) == 0x02, "nibble<0>(uint32_t)");
    CHECK_TRUE(atd::nibble<1>(x) == 0x01, "nibble<1>(uint32_t)");
    CHECK_TRUE(atd::nibble<2>(x) == 0x0F, "nibble<2>(uint32_t)");
    CHECK_TRUE(atd::nibble<3>(x) == 0x0E, "nibble<3>(uint32_t)");
    CHECK_TRUE(atd::nibble<4>(x) == 0x0D, "nibble<4>(uint32_t)");
    CHECK_TRUE(atd::nibble<5>(x) == 0x0C, "nibble<5>(uint32_t)");
    CHECK_TRUE(atd::nibble<6>(x) == 0x0B, "nibble<6>(uint32_t)");
    CHECK_TRUE(atd::nibble<7>(x) == 0x0A, "nibble<7>(uint32_t)");
    }

    {
    uint64_t x = 0xABCDEF1234567890;
    CHECK_TRUE(atd::nibble<0>(x) == 0x00, "nibble<0>(uint64_t)");
    CHECK_TRUE(atd::nibble<1>(x) == 0x09, "nibble<1>(uint64_t)");
    CHECK_TRUE(atd::nibble<2>(x) == 0x08, "nibble<2>(uint64_t)");
    CHECK_TRUE(atd::nibble<3>(x) == 0x07, "nibble<3>(uint64_t)");
    CHECK_TRUE(atd::nibble<4>(x) == 0x06, "nibble<4>(uint64_t)");
    CHECK_TRUE(atd::nibble<5>(x) == 0x05, "nibble<5>(uint64_t)");
    CHECK_TRUE(atd::nibble<6>(x) == 0x04, "nibble<6>(uint64_t)");
    CHECK_TRUE(atd::nibble<7>(x) == 0x03, "nibble<7>(uint64_t)");
    CHECK_TRUE(atd::nibble<8>(x) == 0x02, "nibble<8>(uint64_t)");
    CHECK_TRUE(atd::nibble<9>(x) == 0x01, "nibble<9>(uint64_t)");
    CHECK_TRUE(atd::nibble<10>(x) == 0x0F, "nibble<10>(uint64_t)");
    CHECK_TRUE(atd::nibble<11>(x) == 0x0E, "nibble<11>(uint64_t)");
    CHECK_TRUE(atd::nibble<12>(x) == 0x0D, "nibble<12>(uint64_t)");
    CHECK_TRUE(atd::nibble<13>(x) == 0x0C, "nibble<13>(uint64_t)");
    CHECK_TRUE(atd::nibble<14>(x) == 0x0B, "nibble<14>(uint64_t)");
    CHECK_TRUE(atd::nibble<15>(x) == 0x0A, "nibble<15>(uint64_t)");
    }

}

int main()
{
try{
    test::header("atd_bit");

    test_bitmask();
    test_make_bitmask();
    test_mask();
    test_bit();
    test_concat_bytes();
    test_write_bits();
    test_write_range_bits();
    test_read_bits();
    test_zero_with_bits();
    test_dynamic_write();
    test_is_one_most_significant_bit_of();
    test_byte();
    test_nibble();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





