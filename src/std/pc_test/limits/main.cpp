// Copyright (C) 2021 Manuel Perez 
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

#include "../../std_limits.h"
#include <limits>

#include <alp_test.h>
#include <iostream>
#include <alp_string.h>


using namespace test;

template<typename T>
void test_limits(const std::string& name)
{
    std::cout << "\ntest_limits<" << name << ">\n"
	      <<   "---------------------\n";

    CHECK_TRUE(mtd::numeric_limits<T>::is_specialized ==
                   std::numeric_limits<T>::is_specialized,
               "is_specialized");

    CHECK_TRUE(mtd::numeric_limits<T>::is_signed ==
                   std::numeric_limits<T>::is_signed,
               "is_signed");

    CHECK_TRUE(mtd::numeric_limits<T>::is_integer ==
                   std::numeric_limits<T>::is_integer,
               "is_integer");

    CHECK_TRUE(mtd::numeric_limits<T>::is_exact ==
                   std::numeric_limits<T>::is_exact,
               "is_exact");

    CHECK_TRUE(mtd::numeric_limits<T>::digits == std::numeric_limits<T>::digits,
               "digits");

    CHECK_TRUE(mtd::numeric_limits<T>::digits10 ==
                   std::numeric_limits<T>::digits10,
               "digits10");

    CHECK_TRUE(mtd::numeric_limits<T>::radix == std::numeric_limits<T>::radix,
               "radix");

    CHECK_TRUE(mtd::numeric_limits<T>::min() == std::numeric_limits<T>::min(),
               "min()");

    CHECK_TRUE(mtd::numeric_limits<T>::max() == std::numeric_limits<T>::max(),
               "max()");

    CHECK_TRUE(mtd::numeric_limits<T>::lowest() ==
                   std::numeric_limits<T>::lowest(),
               "lowest()");
}


void test_limits()
{
    test_limits<int8_t>("int8_t");
    test_limits<const int8_t>("const int8_t");

    test_limits<uint8_t>("uint8_t");
    test_limits<const uint8_t>("const uint8_t");

    test_limits<int16_t>("int16_t");
    test_limits<const int16_t>("const int16_t");

    test_limits<uint16_t>("uint16_t");
    test_limits<const uint16_t>("const uint16_t");

    test_limits<int32_t>("int32_t");
    test_limits<const int32_t>("const int32_t");

    test_limits<uint32_t>("uint32_t");
    test_limits<const uint32_t>("const uint32_t");

    test_limits<int64_t>("int64_t");
    test_limits<const int64_t>("const int64_t");

    test_limits<uint64_t>("uint64_t");
    test_limits<const uint64_t>("const uint64_t");

}


int main()
{
try{
    test::header("limits");

    test_limits();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
