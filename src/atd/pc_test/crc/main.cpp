// Copyright (C) 2023 Manuel Perez 
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


#include "../../atd_crc.h"

#include <iostream> 
#include <span>

#include <alp_test.h>
#include <alp_string.h>

using namespace test;

void test_crc7()
{
    test::interface("CRC7");

    uint8_t msg[] = {0xAB, 0xCD, 0xEF};
    CHECK_TRUE(atd::CRC7({msg, 1}) == 0x20u, "atd::CRC7");
    CHECK_TRUE(atd::CRC7({msg, 2}) == 0x24u, "atd::CRC7");
    CHECK_TRUE(atd::CRC7(msg) == 0x4Cu, "atd::CRC7");
    
}

void test_crc8_Maxim(std::span<const uint8_t> msg, uint8_t res)
{
    CHECK_TRUE(atd::CRC8_Maxim(msg) == res, "atd::CRC8_Maxim");
}

void test_crc8_Maxim(const uint8_t* msg, size_t N, uint8_t res)
{
    CHECK_TRUE(atd::CRC8_Maxim(msg, N) == res, "atd::CRC8_Maxim");
}

void test_crc8_Maxim()
{
    test::interface("CRC8");

    {
	uint8_t msg[] = {0x01}; 
	test_crc8_Maxim(msg, 0x5E);
	test_crc8_Maxim(msg, 1, 0x5E);
    }
    {
	uint8_t msg[] = {0x02}; 
	test_crc8_Maxim(msg, 0xBC);
	test_crc8_Maxim(msg, 1, 0xBC);
    }
    {
	uint8_t msg[] = {0x0F}; 
	test_crc8_Maxim(msg, 0x41);
	test_crc8_Maxim(msg, 1, 0x41);
    }
    {
	// uint8_t msg[] = {0x00, 0x00, 0x00, 0x01, 0xB8, 0x1C, 0x02};
	uint8_t msg[] = {0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00};
	test_crc8_Maxim(msg, 0xA2);
	test_crc8_Maxim(msg, 7, 0xA2);
    }
    {
	// uint8_t msg[] = {0x05, 0x17, 0x02, 0x6E, 0x6E, 0xFF, 0x28};
	uint8_t msg[] = {0x28, 0xFF, 0x6E, 0x6E, 0x02, 0x17, 0x05};
	test_crc8_Maxim(msg, 0x8C);
	test_crc8_Maxim(msg, 7, 0x8C);
    }
    
}


int main()
{
try{
    test::header("CRC");
    
    test_crc7();
    test_crc8_Maxim();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





