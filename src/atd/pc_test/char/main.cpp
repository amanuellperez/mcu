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


#include "../../atd_char.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;


void test_char_is_one_of()
{
    test::interface("Char.is_one_of");
    CHECK_TRUE(atd::Char('a').is_one_of("") == false, "empty");
    CHECK_TRUE(atd::Char('a').is_one_of("abc"), "abc");
    CHECK_TRUE(atd::Char('b').is_one_of("abc"), "abc");
    CHECK_TRUE(atd::Char('c').is_one_of("abc"), "abc");
    CHECK_TRUE(atd::Char('d').is_one_of("abc") == false, "abc");
}



int main()
{
try{
    test::header("Char");

    test_char_is_one_of();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





