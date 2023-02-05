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

#include "../../std_bit.h"

#include <alp_test.h>
#include <iostream>

#include <bit>


using namespace test;


void test_bit()
{
    CHECK_TRUE(static_cast<int>(mtd::endian::little) ==
					static_cast<int>(std::endian::little),
               "endian::little");

    CHECK_TRUE(static_cast<int>(mtd::endian::big) ==
					    static_cast<int>(std::endian::big),
               "endian::big");

    CHECK_TRUE(static_cast<int>(mtd::endian::native) ==
					static_cast<int>(std::endian::native),
               "endian::native");

    if (mtd::endian::native == mtd::endian::little)
	std::cout << "es little endian\n";
    else if (mtd::endian::native == mtd::endian::big)
	std::cout << "es big endian\n";
    else
	std::cout << "DESCONOCIDO\n";

}

int main()
{
    test_bit();
}





