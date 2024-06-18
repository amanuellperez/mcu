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

#include "../../std_sizeof_txt.h"

#include <alp_test.h>
#include <iostream>

using namespace test;

void tst()
{    
    CHECK_TRUE(mtd::__Num_caracteres_max<uint8_t> == 3, "Num_caracteres_max<uint8_t>");
    CHECK_TRUE(mtd::__Num_caracteres_max<int8_t> == 4, "Num_caracteres_max<int8_t>");
    CHECK_TRUE(mtd::__Num_caracteres_max<uint16_t> == 5, "Num_caracteres_max<uint16_t>");
    CHECK_TRUE(mtd::__Num_caracteres_max<int16_t> == 6, "Num_caracteres_max<int16_t>");
    CHECK_TRUE(mtd::__Num_caracteres_max<uint32_t> == 10, "Num_caracteres_max<uint32_t>");
    CHECK_TRUE(mtd::__Num_caracteres_max<int32_t> == 11, "Num_caracteres_max<int32_t>");
}



int main()
{
try{
    test::header("__Num_caracteres_max");

    tst();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





