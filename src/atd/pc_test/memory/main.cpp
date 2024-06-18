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


#include "../../atd_memory.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;

// Queda raro este test en el ordenador... 
// TODO: ¿cómo probar esta clase?
struct Read{
    int operator()(const int& x)
    { return 5; }
};



void test_const()
{
    test::interface("atd::Const");

    atd::ROM<int, Read> x{5};

    CHECK_TRUE(x == 5, "int");
}



int main()
{
try{
    test::header("memory");

    test_const();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





