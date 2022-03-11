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

#include "../../atd_ostream.h"

#include <alp_test.h>
#include <iostream>



using namespace test;


#define CHECK_WIDTH(number, w, res_ok)	\
    {std::cout << "check[" << __FILE__ << '-' << __LINE__ <<"]: ";  \
	std::cout << '[';   \
	 atd::print(std::cout, number, nm::Width{w});	    \
	std::cout << "] =? [" << res_ok << "]\n";}

void test_print()
{
    test::interfaz("print");
    CHECK_WIDTH(2, 3, "  2");
    CHECK_WIDTH(34, 5, "   34");
}


int main()
{
try{
    test::header("atd_ostream");

    test_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





