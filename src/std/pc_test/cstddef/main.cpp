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

#include "../../std_cstddef.h"

#include <alp_test.h>
#include <iostream>

using namespace test;

void test_byte()
{
    std::cout << "Probando mtd::byte\n";
    std::cout << "------------------\n";

    mtd::byte a{0b00001111};
    mtd::byte b{0b10101010};

    CHECK_TRUE(b == mtd::byte{0b10101010}, "operator==");

    CHECK_TRUE((a & b) == mtd::byte{0b00001010}, "operator&");
    CHECK_TRUE((a | b) == mtd::byte{0b10101111}, "operator|");
    CHECK_TRUE((a ^ b) == mtd::byte{0b10100101}, "operator^");

    auto c = b;
    c &= a;
    CHECK_TRUE(c == mtd::byte{0b00001010}, "operator&=");

    c = b;
    c |= a;
    CHECK_TRUE(c == mtd::byte{0b10101111}, "operator|=");

    c = b;
    c ^= a;
    CHECK_TRUE(c == mtd::byte{0b10100101}, "operator^=");

    CHECK_TRUE((~b) == mtd::byte{0b01010101}, "operator~");

    mtd::byte d{1};
    CHECK_TRUE((d << 0) == mtd::byte{0b00000001}, "operator<<0");
    CHECK_TRUE((d << 1) == mtd::byte{0b00000010}, "operator<<1");
    CHECK_TRUE((d << 2) == mtd::byte{0b00000100}, "operator<<2");
    CHECK_TRUE((d << 3) == mtd::byte{0b00001000}, "operator<<3");
    CHECK_TRUE((d << 4) == mtd::byte{0b00010000}, "operator<<4");
    CHECK_TRUE((d << 5) == mtd::byte{0b00100000}, "operator<<5");
    CHECK_TRUE((d << 6) == mtd::byte{0b01000000}, "operator<<6");
    CHECK_TRUE((d << 7) == mtd::byte{0b10000000}, "operator<<7");

    d = mtd::byte{0b10000000};
    CHECK_TRUE((d >> 0) == mtd::byte{0b10000000}, "operator>>0");
    CHECK_TRUE((d >> 1) == mtd::byte{0b01000000}, "operator>>1");
    CHECK_TRUE((d >> 2) == mtd::byte{0b00100000}, "operator>>2");
    CHECK_TRUE((d >> 3) == mtd::byte{0b00010000}, "operator>>3");
    CHECK_TRUE((d >> 4) == mtd::byte{0b00001000}, "operator>>4");
    CHECK_TRUE((d >> 5) == mtd::byte{0b00000100}, "operator>>5");
    CHECK_TRUE((d >> 6) == mtd::byte{0b00000010}, "operator>>6");
    CHECK_TRUE((d >> 7) == mtd::byte{0b00000001}, "operator>>7");

    d >>= 4;
    CHECK_TRUE(d == mtd::byte{0b00001000}, "operator=>>4");

    d <<= 4;
    CHECK_TRUE(d == mtd::byte{0b10000000}, "operator=>>4");

    std::cout << mtd::to_integer<int>(b) << '\n';
    
}



int main()
{
    test_byte();
}





