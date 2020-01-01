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

#include "../../atd_type_traits.h"

#include <alp_test.h>
#include <iostream>

using namespace atd;
using namespace test;




template <typename Int, Int v>
struct Wrapper{
    constexpr static Int value = v;
};



void test_less_than()
{
    test::interfaz("Less_than");

    CHECK_TRUE((Wrapper<bool, Less_than<int, 3, 4>::value>::value == true),
               "3 < 4?");
    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 4>::value>::value == false),
               "4 < 4?");
    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 3>::value>::value == false),
               "4 < 3?");
}


void test_pertenece_al_intervalo_cerrado()
{
    test::interfaz("test_pertenece_al_intervalo_cerrado");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 5, 2, 8>::value>::value 
		== true),
        "5 pertenece a [2,8]?");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 1, 2, 8>::value>::value 
		== false),
        "1 pertenece a [2,8]?");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 10, 2, 8>::value>::value 
		== false),
        "10 pertenece a [2,8]?");
}



int main()
{
try{
    test::header("atd_type_traits");

    test_less_than();
    test_pertenece_al_intervalo_cerrado();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





