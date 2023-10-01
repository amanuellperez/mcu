// Copyright (C) 2022 Manuel Perez 
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

#include "../../dev_miniclocks.h"
#include "../pru_generic_timer.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>



using namespace test;

using Miniclock_ms = dev::Miniclock_ms<pru::Timer_generic_counter>;

void test_miniclock_ms()
{
    Miniclock_ms::init();
    CHECK_TRUE(Miniclock_ms::read() == 0, "init()");

    Miniclock_ms::start();
    for (int i = 0; i < 5; ++i) // simulamos que damos 5 ticks al Timer
				// medimos 1 ms = 1000 us con cada tick
	pru::Timer_generic_counter::tick();

    CHECK_TRUE(Miniclock_ms::read() == 5000, "read()");

    Miniclock_ms::stop();
    CHECK_TRUE(Miniclock_ms::read() == 5000, "stop()");

    Miniclock_ms::start();
    CHECK_TRUE(Miniclock_ms::read() == 0, "start()");
}


int main()
{
try{
    test::header("miniclocks");

    test_miniclock_ms();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





