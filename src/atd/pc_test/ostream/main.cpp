// Copyright (C) 2022 Manuel Perez 
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

#include "../../atd_ostream.h"

#include <alp_test.h>
#include <iostream>



using namespace test;


#define CHECK_WIDTH(number, w, res_ok)	\
    {std::cout << "check[" << __FILE__ << '-' << __LINE__ <<"]: ";  \
	std::cout << '[';   \
	 atd::print(std::cout, number, nm::Width{w});	    \
	std::cout << "] =? [" << res_ok << "]\n";}

void test_print()
{
    test::interface("print");

    CHECK_WIDTH(2, 3, "  2");
    CHECK_WIDTH(34, 5, "   34");
}

void test_print_int_as_hex()
{
    test::interface("print_int_as_hex");

    std::cout << "TODO: hacerlas automáticas\n";

    for (uint8_t i = 0; i < 250; i += 10){
	std::cout << std::hex << (int) i << " -> ";
	atd::print_int_as_hex(std::cout, i);
	std::cout << '\n';
    }

    for (uint16_t i = 10000; i < 30000; i += 1000){
	std::cout << std::hex << i << " -> ";
	atd::print_int_as_hex(std::cout, i);
	std::cout << '\n';
    }


    for (int16_t i = -30000; i < -10000; i += 1000){
	std::cout << std::hex << i << " -> ";
	atd::print_int_as_hex(std::cout, i);
	std::cout << '\n';
    }

    CHECK_PRINT(atd::print_int_as_hex(std::cout, uint8_t{254}, 0), "FE");
    CHECK_PRINT(atd::print_int_as_hex(std::cout, uint8_t{254}, 'x'), "0xFE");
    CHECK_PRINT(atd::print_int_as_hex(std::cout, uint8_t{254}, '\\'), "\\xFE");
}

template <typename Int>
void test_write_as_int(const Int& x, const std::string& res)
{
    CHECK_PRINT(std::cout << atd::write_as_int(x), res);
}

void test_write_as_int()
{
    test::interface("write_as_int");
    
    std::cout << std::dec;
    uint8_t u8 = 65;
    CHECK_PRINT(std::cout << u8, "A");
    CHECK_PRINT(std::cout << atd::write_as_int(u8), "65");
    int8_t i8 = 65;
    CHECK_PRINT(std::cout << atd::write_as_int(i8), "65");

    test_write_as_int<uint16_t>(65, "65");
    test_write_as_int<int16_t>(65, "65");
    test_write_as_int<uint32_t>(65, "65");
    test_write_as_int<int32_t>(65, "65");
    test_write_as_int<uint64_t>(65, "65");
    test_write_as_int<int64_t>(65, "65");
}

int main()
{
try{
    test::header("atd_ostream");

    test_print();
    test_print_int_as_hex();
    test_write_as_int();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





