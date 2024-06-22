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

#include <time.h>
void set_system_time(time_t x){}
void system_tick(){}

#include "../../../mcu_system_clock.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <numeric>
#include <ratio>

using namespace test;


struct Timer0{
    using counter_type = uint8_t;

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    template <counter_type timer_period_in_us>
    void on(){}

    void enable_output_compare_A_match_interrupt(){}
    void top_OCRA(uint16_t system_clock_ocr1a){}
};


struct Timer1{
    using counter_type = uint16_t;

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    template <uint16_t timer_period_in_us>
    void on(){}

    void enable_output_compare_A_match_interrupt(){}
    void top_OCRA(uint16_t system_clock_ocr1a){}
};

void test_system_clock_top()
{
//    CHECK_DONT_COMPILE(
//    {
//    constexpr uint8_t top = mcu::__system_clock_top<Timer0, 32>();
//    CHECK_TRUE(top == 31250, "__system_clock_top(32)");
//    }
//    )
//    CHECK_DONT_COMPILE(
//    {
//    constexpr uint8_t top = mcu::__system_clock_top<Timer0, 64>();
//    CHECK_TRUE(top == 15625, "__system_clock_top(64)");
//    }
//    )


    {
    constexpr uint16_t top = mcu::__system_clock_top<Timer1, 32>();
    CHECK_TRUE(top == 31250, "__system_clock_top(32)");
    }
    {
    constexpr uint16_t top = mcu::__system_clock_top<Timer1, 64>();
    CHECK_TRUE(top == 15625, "__system_clock_top(64)");
    }
}


int main()
{
try{
    test::header("system_clock");

    test_system_clock_top();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





