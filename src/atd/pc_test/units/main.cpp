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

#include "../../atd_units.h"

#include <alp_test.h>
#include <iostream>


using namespace test;


void test_unit()
{
    test::interface("Unit");
    
    using U1 = atd::Unit<1,2,3,4,0>;
    using U2 = atd::Unit<5,6,7,8,0>;

    using Up = atd::Unit_multiply<U1, U2>;
    CHECK_TRUE(Up::m == U1::m + U2::m and
	       Up::kg == U1::kg + U2::kg and
	       Up::s == U1::s + U2::s and
	       Up::K == U1::K + U2::K, "Unit_plus");

    using Um = atd::Unit_divide<U1, U2>;
    CHECK_TRUE(Um::m == U1::m - U2::m and
	       Um::kg == U1::kg - U2::kg and
	       Um::s == U1::s - U2::s and
	       Um::K == U1::K - U2::K, "Unit_minus");

    {
    bool res =
        std::is_same_v<atd::Unit_divide<atd::Units_length, atd::Units_time>,
	atd::Units_velocity>;
    CHECK_TRUE( res, "velocity = length / time");
    }
    {
    bool res =
        std::is_same_v<atd::Unit_multiply<atd::Units_frequency, atd::Units_time>,
	atd::Units_scalar>;
    CHECK_TRUE( res, "time * freq = scalar");
    }
    {
    bool res =
        std::is_same_v<atd::Unit_inverse<atd::Units_frequency>,
	atd::Units_time>;
    CHECK_TRUE( res, "1/freq = time");
    }


}


int main()
{
try{
    test::header("atd_units");
    
    test_unit();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}

