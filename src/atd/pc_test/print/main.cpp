// Copyright (C) 2025 Manuel Perez 
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

#include "../../atd_print.h"


#include <iostream>

#include <alp_test.h>
#include <alp_string.h>


using namespace test;


// Hay que comprobar que los `const std::array` se conviertan adecuadamente a
// span
void print(std::ostream& out, const std::array<uint8_t, 36>& s)
{ atd::xxd_print(out, s); }

void test_xxd_print()
{
    test::interface("xxd_print");

    uint8_t x[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	           16,17,18,19,20,21,22,23,24,25,26,27,28,29,30, 31, 
		   'a','b','c','d'};

    atd::xxd_print(std::cout, x);
    
    std::cout << "\n\n";
    std::array<uint8_t, 36> a{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	           16,17,18,19,20,21,22,23,24,25,26,27,28,29,30, 31, 
		   'a','b','c','d'};
    atd::xxd_print(std::cout, a);

    std::cout << "\n\n";
    atd::xxd_print(std::cout, a, 100);


    std::cout << "\n\n";
    print(std::cout, a);

}



int main()
{
try{
    test::header("atd_print");

    test_xxd_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





