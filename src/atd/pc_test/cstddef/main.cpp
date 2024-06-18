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

#include "../../atd_cstddef.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <vector>


using namespace test;

void test_write_read(std::byte b)
{
    std::stringstream str;
    atd::write(str, b);
    std::byte res;
    atd::read(str, res);
    CHECK_TRUE(b == res, alp::as_str() << "read/write(" << static_cast<int>(b) << ")");
}

void test_print()
{    
    test::interface("print");

    for (int i = 0; i < 256; ++i)
	test_write_read(std::byte{static_cast<unsigned char>(i)});

    std::cout << "RESTO NO AUTOMÁTICAS\n";

    std::cout << "No sé cómo probarlo rápidamente. Lo pruebo usando UART\n";
//    for (unsigned int i = 0; i < 255; ++i){
//	std::cout << i  << " = ";
//	atd::print_in_binary(std::cout , std::byte{i});
//    }
}



int main()
{
try{
    test::header("atd_cstddef");

    test_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





