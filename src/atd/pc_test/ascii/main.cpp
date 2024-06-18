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

#include "../../atd_ascii.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

using namespace test;


void tst()
{    
    CHECK_TRUE(atd::digit_to_ascii(5) == '5', "digit_to_ascii");
    
    CHECK_TRUE(atd::ascii_to_digit('5') == 5, "digit_to_ascii");

    std::vector<char> v = {1, 2, 3, 4};
    std::vector<char> res = {'1', '2', '3', '4'};
 
    atd::transform_digit_to_ascii(v.begin(), v.end());
    CHECK_EQUAL_CONTAINERS(v.begin(), v.end(), res.begin(), res.end()
			    , "transform_digit_to_ascii");

}



int main()
{
try{
    test::header("atd_ascii");

    tst();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





