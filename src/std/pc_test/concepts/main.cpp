// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../std_concepts.h"
#include <concepts>

#include <alp_test.h>
#include <iostream>


using namespace test;

struct A{};

void test_concepts()
{
    test::interfaz("mtd::concepts");

    CHECK_TRUE(mtd::integral<char> == std::integral<char>, "integral");
    CHECK_TRUE(mtd::integral<short> == std::integral<short>, "integral");
    CHECK_TRUE(mtd::integral<int> == std::integral<int>, "integral");
    CHECK_TRUE(mtd::integral<long> == std::integral<long>, "integral");
    CHECK_TRUE(mtd::integral<float> == std::integral<float>, "integral");
    CHECK_TRUE(mtd::integral<double> == std::integral<double>, "integral");

    CHECK_TRUE(mtd::integral<unsigned char> == std::integral<unsigned char>, "integral");
    CHECK_TRUE(mtd::integral<unsigned short> == std::integral<unsigned short>, "integral");
    CHECK_TRUE(mtd::integral<unsigned int> == std::integral<unsigned int>, "integral");
    CHECK_TRUE(mtd::integral<unsigned long> == std::integral<unsigned long>, "integral");

    CHECK_TRUE(mtd::integral<A> == std::integral<A>, "integral");
}


int main()
{
try{
    test::header("concepts");

    test_concepts();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




