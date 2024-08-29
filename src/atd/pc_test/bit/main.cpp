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


#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <vector>

#include <cstddef>
#include "../../atd_bit.h"

using namespace test;


void test_bit()
{    
    test::interface("bit");
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
    test::interface("concat_bytes");

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


    {
	uint8_t x0 = 0xFF;
	uint8_t x1 = 0xFF;
	CHECK_TRUE(atd::concat_bytes<uint16_t>(x0, x1) == 65535u, "concat_bytes");
	CHECK_TRUE(atd::concat_bytes<int16_t>(x0, x1) == -1, "concat_bytes");

    }
}

void test_bitmask()
{
    test::interface("make_range_bitmask");

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
    test::interface("make_bitmask");
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
    test::interface("Range_bitmask");
    
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
    test::interface("write_bits");

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
    test::interface("write_range_bits");

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
    test::interface("read_bits");

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
    test::interface("zero_with_bits");

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
    test::interface("write_zero");

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
    test::interface("is_one_most_significant_bit_of");

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
    test::interface("byte<>()");

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
    test::interface("nibble<>()");


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

void test_reverse_bits()
{
    test::interface("reverse_bits()");

    {// uint8_t (enumeración exhaustiva)
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000000}) == 0b00000000, "reverse_bits(00000000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000001}) == 0b10000000, "reverse_bits(00000001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000010}) == 0b01000000, "reverse_bits(00000010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000011}) == 0b11000000, "reverse_bits(00000011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000100}) == 0b00100000, "reverse_bits(00000100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000101}) == 0b10100000, "reverse_bits(00000101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000110}) == 0b01100000, "reverse_bits(00000110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00000111}) == 0b11100000, "reverse_bits(00000111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001000}) == 0b00010000, "reverse_bits(00001000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001001}) == 0b10010000, "reverse_bits(00001001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001010}) == 0b01010000, "reverse_bits(00001010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001011}) == 0b11010000, "reverse_bits(00001011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001100}) == 0b00110000, "reverse_bits(00001100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001101}) == 0b10110000, "reverse_bits(00001101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001110}) == 0b01110000, "reverse_bits(00001110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00001111}) == 0b11110000, "reverse_bits(00001111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010000}) == 0b00001000, "reverse_bits(00010000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010001}) == 0b10001000, "reverse_bits(00010001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010010}) == 0b01001000, "reverse_bits(00010010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010011}) == 0b11001000, "reverse_bits(00010011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010100}) == 0b00101000, "reverse_bits(00010100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010101}) == 0b10101000, "reverse_bits(00010101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010110}) == 0b01101000, "reverse_bits(00010110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00010111}) == 0b11101000, "reverse_bits(00010111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011000}) == 0b00011000, "reverse_bits(00011000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011001}) == 0b10011000, "reverse_bits(00011001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011010}) == 0b01011000, "reverse_bits(00011010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011011}) == 0b11011000, "reverse_bits(00011011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011100}) == 0b00111000, "reverse_bits(00011100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011101}) == 0b10111000, "reverse_bits(00011101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011110}) == 0b01111000, "reverse_bits(00011110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00011111}) == 0b11111000, "reverse_bits(00011111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100000}) == 0b00000100, "reverse_bits(00100000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100001}) == 0b10000100, "reverse_bits(00100001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100010}) == 0b01000100, "reverse_bits(00100010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100011}) == 0b11000100, "reverse_bits(00100011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100100}) == 0b00100100, "reverse_bits(00100100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100101}) == 0b10100100, "reverse_bits(00100101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100110}) == 0b01100100, "reverse_bits(00100110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00100111}) == 0b11100100, "reverse_bits(00100111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101000}) == 0b00010100, "reverse_bits(00101000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101001}) == 0b10010100, "reverse_bits(00101001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101010}) == 0b01010100, "reverse_bits(00101010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101011}) == 0b11010100, "reverse_bits(00101011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101100}) == 0b00110100, "reverse_bits(00101100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101101}) == 0b10110100, "reverse_bits(00101101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101110}) == 0b01110100, "reverse_bits(00101110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00101111}) == 0b11110100, "reverse_bits(00101111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110000}) == 0b00001100, "reverse_bits(00110000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110001}) == 0b10001100, "reverse_bits(00110001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110010}) == 0b01001100, "reverse_bits(00110010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110011}) == 0b11001100, "reverse_bits(00110011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110100}) == 0b00101100, "reverse_bits(00110100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110101}) == 0b10101100, "reverse_bits(00110101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110110}) == 0b01101100, "reverse_bits(00110110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00110111}) == 0b11101100, "reverse_bits(00110111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111000}) == 0b00011100, "reverse_bits(00111000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111001}) == 0b10011100, "reverse_bits(00111001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111010}) == 0b01011100, "reverse_bits(00111010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111011}) == 0b11011100, "reverse_bits(00111011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111100}) == 0b00111100, "reverse_bits(00111100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111101}) == 0b10111100, "reverse_bits(00111101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111110}) == 0b01111100, "reverse_bits(00111110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b00111111}) == 0b11111100, "reverse_bits(00111111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000000}) == 0b00000010, "reverse_bits(01000000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000001}) == 0b10000010, "reverse_bits(01000001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000010}) == 0b01000010, "reverse_bits(01000010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000011}) == 0b11000010, "reverse_bits(01000011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000100}) == 0b00100010, "reverse_bits(01000100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000101}) == 0b10100010, "reverse_bits(01000101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000110}) == 0b01100010, "reverse_bits(01000110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01000111}) == 0b11100010, "reverse_bits(01000111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001000}) == 0b00010010, "reverse_bits(01001000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001001}) == 0b10010010, "reverse_bits(01001001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001010}) == 0b01010010, "reverse_bits(01001010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001011}) == 0b11010010, "reverse_bits(01001011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001100}) == 0b00110010, "reverse_bits(01001100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001101}) == 0b10110010, "reverse_bits(01001101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001110}) == 0b01110010, "reverse_bits(01001110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01001111}) == 0b11110010, "reverse_bits(01001111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010000}) == 0b00001010, "reverse_bits(01010000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010001}) == 0b10001010, "reverse_bits(01010001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010010}) == 0b01001010, "reverse_bits(01010010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010011}) == 0b11001010, "reverse_bits(01010011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010100}) == 0b00101010, "reverse_bits(01010100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010101}) == 0b10101010, "reverse_bits(01010101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010110}) == 0b01101010, "reverse_bits(01010110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01010111}) == 0b11101010, "reverse_bits(01010111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011000}) == 0b00011010, "reverse_bits(01011000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011001}) == 0b10011010, "reverse_bits(01011001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011010}) == 0b01011010, "reverse_bits(01011010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011011}) == 0b11011010, "reverse_bits(01011011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011100}) == 0b00111010, "reverse_bits(01011100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011101}) == 0b10111010, "reverse_bits(01011101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011110}) == 0b01111010, "reverse_bits(01011110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01011111}) == 0b11111010, "reverse_bits(01011111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100000}) == 0b00000110, "reverse_bits(01100000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100001}) == 0b10000110, "reverse_bits(01100001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100010}) == 0b01000110, "reverse_bits(01100010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100011}) == 0b11000110, "reverse_bits(01100011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100100}) == 0b00100110, "reverse_bits(01100100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100101}) == 0b10100110, "reverse_bits(01100101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100110}) == 0b01100110, "reverse_bits(01100110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01100111}) == 0b11100110, "reverse_bits(01100111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101000}) == 0b00010110, "reverse_bits(01101000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101001}) == 0b10010110, "reverse_bits(01101001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101010}) == 0b01010110, "reverse_bits(01101010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101011}) == 0b11010110, "reverse_bits(01101011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101100}) == 0b00110110, "reverse_bits(01101100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101101}) == 0b10110110, "reverse_bits(01101101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101110}) == 0b01110110, "reverse_bits(01101110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01101111}) == 0b11110110, "reverse_bits(01101111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110000}) == 0b00001110, "reverse_bits(01110000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110001}) == 0b10001110, "reverse_bits(01110001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110010}) == 0b01001110, "reverse_bits(01110010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110011}) == 0b11001110, "reverse_bits(01110011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110100}) == 0b00101110, "reverse_bits(01110100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110101}) == 0b10101110, "reverse_bits(01110101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110110}) == 0b01101110, "reverse_bits(01110110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01110111}) == 0b11101110, "reverse_bits(01110111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111000}) == 0b00011110, "reverse_bits(01111000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111001}) == 0b10011110, "reverse_bits(01111001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111010}) == 0b01011110, "reverse_bits(01111010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111011}) == 0b11011110, "reverse_bits(01111011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111100}) == 0b00111110, "reverse_bits(01111100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111101}) == 0b10111110, "reverse_bits(01111101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111110}) == 0b01111110, "reverse_bits(01111110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b01111111}) == 0b11111110, "reverse_bits(01111111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000000}) == 0b00000001, "reverse_bits(10000000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000001}) == 0b10000001, "reverse_bits(10000001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000010}) == 0b01000001, "reverse_bits(10000010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000011}) == 0b11000001, "reverse_bits(10000011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000100}) == 0b00100001, "reverse_bits(10000100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000101}) == 0b10100001, "reverse_bits(10000101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000110}) == 0b01100001, "reverse_bits(10000110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10000111}) == 0b11100001, "reverse_bits(10000111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001000}) == 0b00010001, "reverse_bits(10001000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001001}) == 0b10010001, "reverse_bits(10001001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001010}) == 0b01010001, "reverse_bits(10001010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001011}) == 0b11010001, "reverse_bits(10001011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001100}) == 0b00110001, "reverse_bits(10001100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001101}) == 0b10110001, "reverse_bits(10001101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001110}) == 0b01110001, "reverse_bits(10001110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10001111}) == 0b11110001, "reverse_bits(10001111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010000}) == 0b00001001, "reverse_bits(10010000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010001}) == 0b10001001, "reverse_bits(10010001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010010}) == 0b01001001, "reverse_bits(10010010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010011}) == 0b11001001, "reverse_bits(10010011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010100}) == 0b00101001, "reverse_bits(10010100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010101}) == 0b10101001, "reverse_bits(10010101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010110}) == 0b01101001, "reverse_bits(10010110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10010111}) == 0b11101001, "reverse_bits(10010111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011000}) == 0b00011001, "reverse_bits(10011000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011001}) == 0b10011001, "reverse_bits(10011001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011010}) == 0b01011001, "reverse_bits(10011010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011011}) == 0b11011001, "reverse_bits(10011011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011100}) == 0b00111001, "reverse_bits(10011100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011101}) == 0b10111001, "reverse_bits(10011101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011110}) == 0b01111001, "reverse_bits(10011110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10011111}) == 0b11111001, "reverse_bits(10011111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100000}) == 0b00000101, "reverse_bits(10100000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100001}) == 0b10000101, "reverse_bits(10100001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100010}) == 0b01000101, "reverse_bits(10100010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100011}) == 0b11000101, "reverse_bits(10100011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100100}) == 0b00100101, "reverse_bits(10100100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100101}) == 0b10100101, "reverse_bits(10100101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100110}) == 0b01100101, "reverse_bits(10100110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10100111}) == 0b11100101, "reverse_bits(10100111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101000}) == 0b00010101, "reverse_bits(10101000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101001}) == 0b10010101, "reverse_bits(10101001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101010}) == 0b01010101, "reverse_bits(10101010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101011}) == 0b11010101, "reverse_bits(10101011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101100}) == 0b00110101, "reverse_bits(10101100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101101}) == 0b10110101, "reverse_bits(10101101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101110}) == 0b01110101, "reverse_bits(10101110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10101111}) == 0b11110101, "reverse_bits(10101111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110000}) == 0b00001101, "reverse_bits(10110000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110001}) == 0b10001101, "reverse_bits(10110001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110010}) == 0b01001101, "reverse_bits(10110010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110011}) == 0b11001101, "reverse_bits(10110011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110100}) == 0b00101101, "reverse_bits(10110100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110101}) == 0b10101101, "reverse_bits(10110101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110110}) == 0b01101101, "reverse_bits(10110110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10110111}) == 0b11101101, "reverse_bits(10110111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111000}) == 0b00011101, "reverse_bits(10111000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111001}) == 0b10011101, "reverse_bits(10111001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111010}) == 0b01011101, "reverse_bits(10111010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111011}) == 0b11011101, "reverse_bits(10111011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111100}) == 0b00111101, "reverse_bits(10111100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111101}) == 0b10111101, "reverse_bits(10111101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111110}) == 0b01111101, "reverse_bits(10111110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b10111111}) == 0b11111101, "reverse_bits(10111111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000000}) == 0b00000011, "reverse_bits(11000000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000001}) == 0b10000011, "reverse_bits(11000001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000010}) == 0b01000011, "reverse_bits(11000010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000011}) == 0b11000011, "reverse_bits(11000011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000100}) == 0b00100011, "reverse_bits(11000100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000101}) == 0b10100011, "reverse_bits(11000101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000110}) == 0b01100011, "reverse_bits(11000110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11000111}) == 0b11100011, "reverse_bits(11000111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001000}) == 0b00010011, "reverse_bits(11001000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001001}) == 0b10010011, "reverse_bits(11001001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001010}) == 0b01010011, "reverse_bits(11001010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001011}) == 0b11010011, "reverse_bits(11001011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001100}) == 0b00110011, "reverse_bits(11001100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001101}) == 0b10110011, "reverse_bits(11001101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001110}) == 0b01110011, "reverse_bits(11001110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11001111}) == 0b11110011, "reverse_bits(11001111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010000}) == 0b00001011, "reverse_bits(11010000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010001}) == 0b10001011, "reverse_bits(11010001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010010}) == 0b01001011, "reverse_bits(11010010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010011}) == 0b11001011, "reverse_bits(11010011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010100}) == 0b00101011, "reverse_bits(11010100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010101}) == 0b10101011, "reverse_bits(11010101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010110}) == 0b01101011, "reverse_bits(11010110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11010111}) == 0b11101011, "reverse_bits(11010111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011000}) == 0b00011011, "reverse_bits(11011000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011001}) == 0b10011011, "reverse_bits(11011001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011010}) == 0b01011011, "reverse_bits(11011010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011011}) == 0b11011011, "reverse_bits(11011011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011100}) == 0b00111011, "reverse_bits(11011100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011101}) == 0b10111011, "reverse_bits(11011101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011110}) == 0b01111011, "reverse_bits(11011110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11011111}) == 0b11111011, "reverse_bits(11011111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100000}) == 0b00000111, "reverse_bits(11100000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100001}) == 0b10000111, "reverse_bits(11100001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100010}) == 0b01000111, "reverse_bits(11100010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100011}) == 0b11000111, "reverse_bits(11100011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100100}) == 0b00100111, "reverse_bits(11100100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100101}) == 0b10100111, "reverse_bits(11100101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100110}) == 0b01100111, "reverse_bits(11100110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11100111}) == 0b11100111, "reverse_bits(11100111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101000}) == 0b00010111, "reverse_bits(11101000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101001}) == 0b10010111, "reverse_bits(11101001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101010}) == 0b01010111, "reverse_bits(11101010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101011}) == 0b11010111, "reverse_bits(11101011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101100}) == 0b00110111, "reverse_bits(11101100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101101}) == 0b10110111, "reverse_bits(11101101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101110}) == 0b01110111, "reverse_bits(11101110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11101111}) == 0b11110111, "reverse_bits(11101111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110000}) == 0b00001111, "reverse_bits(11110000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110001}) == 0b10001111, "reverse_bits(11110001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110010}) == 0b01001111, "reverse_bits(11110010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110011}) == 0b11001111, "reverse_bits(11110011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110100}) == 0b00101111, "reverse_bits(11110100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110101}) == 0b10101111, "reverse_bits(11110101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110110}) == 0b01101111, "reverse_bits(11110110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11110111}) == 0b11101111, "reverse_bits(11110111)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111000}) == 0b00011111, "reverse_bits(11111000)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111001}) == 0b10011111, "reverse_bits(11111001)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111010}) == 0b01011111, "reverse_bits(11111010)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111011}) == 0b11011111, "reverse_bits(11111011)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111100}) == 0b00111111, "reverse_bits(11111100)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111101}) == 0b10111111, "reverse_bits(11111101)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111110}) == 0b01111111, "reverse_bits(11111110)");
CHECK_TRUE(atd::reverse_bits(uint8_t{0b11111111}) == 0b11111111, "reverse_bits(11111111)");
    }

    {// uint16_t
CHECK_TRUE(atd::reverse_bits(uint16_t{0x0001}) == 0x8000u, "reverse_bits(0x0001)");
CHECK_TRUE(atd::reverse_bits(uint16_t{0x00FF}) == 0xFF00u, "reverse_bits(0x00FF)");
CHECK_TRUE(atd::reverse_bits(uint16_t{0xAAAA}) == 0x5555u, "reverse_bits(0xAAAA)");
CHECK_TRUE(atd::reverse_bits(uint16_t{0xCCCC}) == 0x3333u, "reverse_bits(0xCCCC)");
    }


}

void test_Bit()
{
    test::interface("bit (dynamic version)");

    {
    uint8_t x = 0xF0;
    CHECK_TRUE(atd::bit(0).of(x) == 0, "bit(0).of");
    CHECK_TRUE(atd::bit(1).of(x) == 0, "bit(1).of");
    CHECK_TRUE(atd::bit(2).of(x) == 0, "bit(2).of");
    CHECK_TRUE(atd::bit(3).of(x) == 0, "bit(3).of");
    CHECK_TRUE(atd::bit(4).of(x) == 1, "bit(4).of");
    CHECK_TRUE(atd::bit(5).of(x) == 1, "bit(5).of");
    CHECK_TRUE(atd::bit(6).of(x) == 1, "bit(6).of");
    CHECK_TRUE(atd::bit(7).of(x) == 1, "bit(7).of");

    atd::bit(0).of(x) = 1;
    CHECK_TRUE(atd::bit(0).of(x) == 1, "bit(0).of");
    
    // DONT_COMPILE(const uint8_t y = 0; atd::bit(0).of(y) = 1;)

    }

    {
    uint16_t x = 0xF0F0;
    CHECK_TRUE(atd::bit(0).of(x) == 0, "bit(0).of");
    CHECK_TRUE(atd::bit(1).of(x) == 0, "bit(1).of");
    CHECK_TRUE(atd::bit(2).of(x) == 0, "bit(2).of");
    CHECK_TRUE(atd::bit(3).of(x) == 0, "bit(3).of");

    CHECK_TRUE(atd::bit(4).of(x) == 1, "bit(4).of");
    CHECK_TRUE(atd::bit(5).of(x) == 1, "bit(5).of");
    CHECK_TRUE(atd::bit(6).of(x) == 1, "bit(6).of");
    CHECK_TRUE(atd::bit(7).of(x) == 1, "bit(7).of");

    CHECK_TRUE(atd::bit(8).of(x) == 0, "bit(8).of");
    CHECK_TRUE(atd::bit(9).of(x) == 0, "bit(9).of");
    CHECK_TRUE(atd::bit(10).of(x) == 0, "bit(10).of");
    CHECK_TRUE(atd::bit(11).of(x) == 0, "bit(11).of");

    CHECK_TRUE(atd::bit(12).of(x) == 1, "bit(12).of");
    CHECK_TRUE(atd::bit(13).of(x) == 1, "bit(13).of");
    CHECK_TRUE(atd::bit(14).of(x) == 1, "bit(14).of");
    CHECK_TRUE(atd::bit(15).of(x) == 1, "bit(15).of");
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
    test_reverse_bits();
    test_Bit();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





