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

#include "../chronometer.h"

#include <alp_test.h>
#include <iostream>


using namespace test;

void test_sexagesimal_time()
{
    test::interfaz("__Chronometer_sexagtime");

    using Sexag = dev::__Chronometer_sexagesimal_ms;

    {// 0
    Sexag t;
    t.milliseconds = 0;
    t.seconds = 0;
    t.minutes = 0;
    t.hours = 0;
    CHECK_TRUE(t.to_milliseconds() == 0, "to_milliseconds(0)");
    CHECK_TRUE(Sexag{0} == t, "from_milliseconds");
    }

    {
    Sexag t;
    t.milliseconds = 100;
    t.seconds = 0;
    t.minutes = 0;
    t.hours = 0;
    CHECK_TRUE(t.to_milliseconds() == 100, "to_milliseconds(100)");
    CHECK_TRUE(Sexag{100} == t, "from_milliseconds");
    }
    {
    Sexag t;
    t.milliseconds = 100;
    t.seconds = 10;
    t.minutes = 0;
    t.hours = 0;
    CHECK_TRUE(t.to_milliseconds() == 10100, "to_milliseconds(10''100''')");
    CHECK_TRUE(Sexag{10100} == t, "from_milliseconds");
    }

    {
    Sexag t;
    t.milliseconds = 100;
    t.seconds = 10;
    t.minutes = 4;
    t.hours = 0;
    CHECK_TRUE(t.to_milliseconds() == 250100, "to_milliseconds(4'10''100''')");
    CHECK_TRUE(Sexag{250100} == t, "from_milliseconds");
    }
    {
    Sexag t;
    t.milliseconds = 100;
    t.seconds = 10;
    t.minutes = 4;
    t.hours = 3;
    CHECK_TRUE(t.to_milliseconds() == 11050100, "to_milliseconds(3h4'10''100''')");
    CHECK_TRUE(Sexag{11050100} == t, "from_milliseconds");
    }

    {
    Sexag t;
    t.milliseconds = 999;
    t.seconds = 59;
    t.minutes = 59;
    t.hours = 23;
    CHECK_TRUE(t.to_milliseconds() == 86399999, "to_milliseconds(23h59'59''999''')");
    CHECK_TRUE(Sexag{86399999} == t, "from_milliseconds");
    }

}


int main()
{
try{
    test::header("chronometer");

    test_sexagesimal_time();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}



