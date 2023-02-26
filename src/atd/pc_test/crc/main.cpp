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

int main()
{
try{
    test::header("CRC");
    
    test_crc7();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





