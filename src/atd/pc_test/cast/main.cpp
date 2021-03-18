// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_cast.h"

#include <alp_test.h>
#include <iostream>



using namespace test;


void test_bounded_cast()
{    
    test::interfaz("bounded_cast");

    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{0}) == 0, "(0) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{255}) == 255, "(255) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{1000}) == 255, "(1000) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(int16_t{-20}) == 0, "(-20) uint16_t -> uint8_t");


}


template <typename Int, Int v>
struct Wrapper{
    constexpr static Int value = v;
};

void test_safe_static_cast()
{ 
    test::interfaz("safe_static_cast");

    // Lo meto dentro del Wraper para que se vea que se genera en tiempo de
    // compilación.
    std::cout << "static_cast: " << (int) 
	Wrapper<uint8_t, atd::safe_static_cast<uint8_t, uint16_t, 10u>()>::value
	<< " OK\n";

    // Este no compila
//    std::cout << "static_cast: " << (int) 
//	Wrapper<uint8_t, safe_static_cast<uint8_t, uint16_t, 256u>()>::value
//	<< " OK\n";
}

void test_to_integer()
{
    using Dec8_1 = atd::Decimal<uint8_t, 1>;

    Dec8_1 d{10,3};
    CHECK_TRUE(atd::to_integer<uint8_t>(d) == 10, "to_integer");
    CHECK_TRUE(atd::to_integer<long>(d) == 10UL, "to_integer");

{// bug
    using Dec1 = atd::Decimal<uint64_t, 3>;
    using Dec2 = atd::Decimal<uint32_t, 3>;
    Dec1 from{953, 67};
    Dec2 to = atd::to_integer<Dec2>(from);
    std::cout << from << " --> " << to << '\n';
}

}


int main()
{
try{
    test::header("atd_cast");

    test_bounded_cast();
    test_safe_static_cast();
    test_to_integer();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





