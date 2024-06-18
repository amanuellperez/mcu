// Copyright (C) 2019-2023 Manuel Perez 
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

#include "../../std_chrono.h"

#include <alp_test.h>
#include <iostream>
#include <chrono>

#include <time.h>

using namespace test;

struct Mtd_clock{
    using rep        = std::int32_t;
    using period     = mtd::ratio<1>;
    using duration   = mtd::chrono::duration<rep, period>;
    using time_point = mtd::chrono::time_point<Mtd_clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept
    {
	return time_point{duration{::time(nullptr)}};
    }
};



struct Std_clock{
    using rep        = std::int32_t;
    using period     = std::ratio<1>;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<Std_clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept
    {
	return time_point{duration{::time(nullptr)}};
    }
};



void test_asserts()
{
    static_assert(!mtd::chrono::__is_ratio<int>::value);
    static_assert(mtd::chrono::__is_ratio<mtd::ratio<1>>::value);
    static_assert(mtd::chrono::__is_ratio<mtd::ratio<1,2>>::value);
}


void test_duration()
{
    using Mtd = mtd::chrono::duration<int, mtd::ratio<1,2>>;
    using Mtd2 = mtd::chrono::duration<int, mtd::ratio<1,3>>;
    using Std = std::chrono::duration<int, std::ratio<1,2>>;
    using Std2 = std::chrono::duration<int, std::ratio<1,3>>;

    CHECK_TRUE((std::is_same_v<Mtd::rep, Std::rep>), "rep");
    CHECK_TRUE(Mtd::period::num == Std::period::num, "num");
    CHECK_TRUE(Mtd::period::den == Std::period::den, "den");

    Mtd mtd_clk {10};
    Std std_clk {10};
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "count");

    CHECK_TRUE((+mtd_clk).count() == (+std_clk).count(), "operator+");
    CHECK_TRUE((-mtd_clk).count() == (-std_clk).count(), "operator-");

    ++mtd_clk; ++std_clk;
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator++");

    CHECK_TRUE((mtd_clk++).count() == (std_clk++).count(), "operator++(int)");
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator++(int)");

    --mtd_clk; --std_clk;
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator--");

    CHECK_TRUE((mtd_clk--).count() == (std_clk--).count(), "operator--(int)");
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator--(int)");

    mtd_clk += Mtd{4}; std_clk += Std{4};
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator+=");

    mtd_clk -= Mtd{4}; std_clk -= Std{4};
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator-=");

    mtd_clk *= 2; std_clk *= 2;
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator*=");

    mtd_clk /= 2; std_clk /= 2;
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator/=");

    mtd_clk %= 4; std_clk %= 4;
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator%=");

    mtd_clk %= Mtd{4}; std_clk %= Std{4};
    CHECK_TRUE(mtd_clk.count() == std_clk.count(), "operator%=(duration)");

    {
	using MTD_CD = mtd::common_type_t<Mtd, Mtd2>;
	using STD_CD = std::common_type_t<Std, Std2>;
	CHECK_TRUE((std::is_same_v<MTD_CD::rep, STD_CD::rep> and
		    MTD_CD::period::num == STD_CD::period::num and
		    MTD_CD::period::den == STD_CD::period::den)
		    , "common_type_t");
    }

    // operators
    // ---------
    Mtd2 mtd2_clk{20};
    Std2 std2_clk{20};
    {
    auto mtd_res = mtd_clk + mtd2_clk;
    auto std_res = std_clk + std2_clk;
    CHECK_TRUE(mtd_res.count() == std_res.count(), "operator+");
    }
    {
    auto mtd_res = mtd_clk - mtd2_clk;
    auto std_res = std_clk - std2_clk;
    CHECK_TRUE(mtd_res.count() == std_res.count(), "operator-");
    }

    {
    auto mtd_res = 10*mtd_clk;
    auto std_res = 10*std_clk;
    CHECK_TRUE(mtd_res.count() == std_res.count(), "operator*");
    }
    {
    auto mtd_res = mtd_clk*20;
    auto std_res = std_clk*20;
    CHECK_TRUE(mtd_res.count() == std_res.count(), "operator*");
    }
    {
    auto mtd_res = mtd_clk/20;
    auto std_res = std_clk/20;
    CHECK_TRUE(mtd_res.count() == std_res.count(), "operator/");
    }
    {
    auto mtd_res = mtd2_clk / mtd_clk;
    auto std_res = std2_clk / std_clk;
    CHECK_TRUE(mtd_res == std_res, "operator/");
    }
    {
        auto mtd_res = mtd_clk % 2;
        auto std_res = std_clk % 2;
        CHECK_TRUE(mtd_res.count() == std_res.count(), "operator%");
    }
    {
	auto mtd_res = mtd2_clk % mtd_clk;
        auto std_res = std2_clk % std_clk;
        CHECK_TRUE(mtd_res.count() == std_res.count(), "operator%");
    }

    // comparisons
    // -----------
    {// operator==
	Mtd mtd_clk{20};
	Mtd2 mtd2_clk{30};
	CHECK_TRUE(mtd_clk == mtd2_clk, "operator== (==)");
	CHECK_FALSE(mtd_clk != mtd2_clk, "operator== (!=)");
	CHECK_FALSE(mtd_clk < mtd2_clk, "operator== (<)");
	CHECK_TRUE(mtd_clk <= mtd2_clk, "operator== (<=)");
	CHECK_FALSE(mtd_clk > mtd2_clk, "operator== (>)");
	CHECK_TRUE(mtd_clk >= mtd2_clk, "operator== (>=)");
    }
    {// operator<
	Mtd mtd_clk{20};
	Mtd2 mtd2_clk{60};
	CHECK_FALSE(mtd_clk == mtd2_clk, "operator< (==)");
	CHECK_TRUE(mtd_clk != mtd2_clk, "operator< (!=)");
	CHECK_TRUE(mtd_clk < mtd2_clk, "operator< (<)");
	CHECK_TRUE(mtd_clk <= mtd2_clk, "operator< (<=)");
	CHECK_FALSE(mtd_clk > mtd2_clk, "operator< (>)");
	CHECK_FALSE(mtd_clk >= mtd2_clk, "operator< (>=)");
    }
    {// operator>
	Mtd mtd_clk{100};
	Mtd2 mtd2_clk{30};
	CHECK_FALSE(mtd_clk == mtd2_clk, "operator> (==)");
	CHECK_TRUE(mtd_clk != mtd2_clk, "operator> (!=)");
	CHECK_FALSE(mtd_clk < mtd2_clk, "operator> (<)");
	CHECK_FALSE(mtd_clk <= mtd2_clk, "operator> (<=)");
	CHECK_TRUE(mtd_clk > mtd2_clk, "operator> (>)");
	CHECK_TRUE(mtd_clk >= mtd2_clk, "operator> (>=)");
    }

}

void test_time_point()
{
    test::interfaz("time_point");
    
    auto mtd_time = Mtd_clock::now();
    auto std_time = Std_clock::now();

    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "time_since_epoch");

    mtd_time += Mtd_clock::duration{10};
    std_time += Std_clock::duration{10};
    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "operator+=");

    mtd_time -= Mtd_clock::duration{10};
    std_time -= Std_clock::duration{10};
    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "operator-=");


    mtd_time = mtd_time + Mtd_clock::duration{10};
    std_time = std_time + Std_clock::duration{10};
    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "operator+");

    mtd_time = Mtd_clock::duration{10} + mtd_time;
    std_time = Std_clock::duration{10} + std_time;
    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "operator+");

    mtd_time = mtd_time - Mtd_clock::duration{20};
    std_time = std_time - Std_clock::duration{20};
    CHECK_TRUE(mtd_time.time_since_epoch().count() ==
					    std_time.time_since_epoch().count(),
               "operator-");


    auto mtd_time2 = Mtd_clock::now();
    auto std_time2 = Std_clock::now();
    auto mtd_d = mtd_time2 - mtd_time;
    auto std_d = std_time2 - std_time;
    CHECK_TRUE(mtd_d.count() == std_d.count(), "operator-"); 


    {// operator==
	auto mtd_time = Mtd_clock::now();
	auto mtd2_time = mtd_time;
	CHECK_TRUE(mtd_time == mtd2_time, "operator== (==)");
	CHECK_FALSE(mtd_time != mtd2_time, "operator== (!=)");
	CHECK_FALSE(mtd_time < mtd2_time, "operator== (<)");
	CHECK_TRUE(mtd_time <= mtd2_time, "operator== (<=)");
	CHECK_FALSE(mtd_time > mtd2_time, "operator== (>)");
	CHECK_TRUE(mtd_time >= mtd2_time, "operator== (>=)");
    }
}



void test_clocks()
{
    test::interfaz("system_clock");

    auto mtd_time = mtd::chrono::system_clock::now().time_since_epoch().count();

    auto std_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();

    CHECK_TRUE(mtd_time == std_time, "system_clock::now()");

    time_t t = 100;
    auto from = mtd::chrono::system_clock::from_time_t(t);
    auto to = mtd::chrono::system_clock::to_time_t(from);
    CHECK_TRUE(t == to, "system_clock::from_time_t/to_time_t");
}


template <typename stdD, typename mtdD>
void test_hh_mm_ss(stdD st, mtdD mt)
{
    std::chrono::hh_mm_ss sh{st};
    mtd::chrono::hh_mm_ss mh{mt};

    CHECK_TRUE(sh.hours().count() == mh.hours().count()
		    and
	       sh.minutes().count() == mh.minutes().count()
		    and
	       sh.seconds().count() == mh.seconds().count()
		    and
	       sh.subseconds().count() == mh.subseconds().count()
	       , "hh_mm_ss");
}

void test_hh_mm_ss()
{
    test::interface("hh_mm_ss");
    test_hh_mm_ss(std::chrono::seconds{0},
	          mtd::chrono::seconds{0});
    test_hh_mm_ss(std::chrono::seconds{340},
	          mtd::chrono::seconds{340});
    test_hh_mm_ss(std::chrono::minutes{340},
	          mtd::chrono::minutes{340});
    test_hh_mm_ss(std::chrono::hours{30},
	          mtd::chrono::hours{30});
}


int main()
{
try{
    test::header("chrono");

    test_asserts();
    test_duration();
    test_time_point();
    test_hh_mm_ss();
    test_clocks();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





