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


#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <string_view>
#include <cstring>  // strlen
#include <cctype>   // isdigit

#include <cstddef>
#include "../prj_locale.h"

using namespace test;


void test_read_uint()
{
    test::interface("read_uint");

{// basic
    const char str[] = "  12	345  678a";

    size_t N = std::strlen(str);
    const char* p = str;
    const char* pe = str + N;

    int n{};	
    std::tie(n, p) = read_uint(p, pe);
    CHECK_TRUE(n == 12 and p != pe, "12");

    std::tie(n, p) = read_uint(p, pe);
    CHECK_TRUE(n == 345 and p != pe, "345");


    std::tie(n, p) = read_uint(p, pe);
    CHECK_TRUE(n == 678 and p != pe, "678");

    std::tie(n, p) = read_uint(p, pe);
    CHECK_TRUE(n == -1, "n == -1");
}

{// null
    const char str[] = "";

    auto [n, p] = read_uint(str, str);
    CHECK_TRUE(n == -1, "empty str");
}

{// solo 1 número
    const char str[] = "234";
    size_t N = std::strlen(str);
    const char* p = str;
    const char* pe = str + N;

    int n{};	
    std::tie(n, p) = read_uint(p, pe);
    CHECK_TRUE(n == 234 and p == pe, "only one number");
}
}


void test_char_is_one_of()
{
    test::interface("Char.is_one_of");
    CHECK_TRUE(Char('a').is_one_of("") == false, "empty");
    CHECK_TRUE(Char('a').is_one_of("abc"), "abc");
    CHECK_TRUE(Char('b').is_one_of("abc"), "abc");
    CHECK_TRUE(Char('c').is_one_of("abc"), "abc");
    CHECK_TRUE(Char('d').is_one_of("abc") == false, "abc");
}


void test_read_word(const char* word, const char* str, size_t res)
{
    CHECK_TRUE(read_word(word, word + std::strlen(word),
			 str, str + std::strlen(str)) == res,
		alp::as_str() << "read_word(" << word << ", " << str << ")");
}


void test_read_word()
{
    test::interface("read_word");

    test_read_word("hours", "hours", 5);
    test_read_word("hours", "", 0);
    test_read_word("hours", "minutes", 0);
    test_read_word("hours", "h 24 min 30 s", 1);
}

void test_read_time_unit(const char* str, Time_unit res_unit, size_t n)
{
    auto [unit, p] = read_time_unit(str, str + std::strlen(str));

    CHECK_TRUE(unit == res_unit and p == str + n, 
	      alp::as_str() << "read_time_unit(" << str << ")");
}

void test_read_time_unit()
{
    test::interface("read_time_unit");

    test_read_time_unit("hours", Time_unit::hours, 5);
    test_read_time_unit("h 23 min", Time_unit::hours, 1);

    test_read_time_unit("minutes", Time_unit::minutes, 7);
    test_read_time_unit("min", Time_unit::minutes, 3);
    test_read_time_unit("m", Time_unit::minutes, 1);

    test_read_time_unit("seconds", Time_unit::seconds, 7);
    test_read_time_unit("sec", Time_unit::seconds, 3);
    test_read_time_unit("s", Time_unit::seconds, 1);

}

void test_convert(const char* str, int hours, int minutes, int seconds, bool res0)
{
    using time_point = std::chrono::system_clock::time_point;

    time_point t0;
    bool res = Str2time_point::convert(str, t0);

	print_time(std::cout, t0);
    CHECK_TRUE(res == res0, alp::as_str() << "convert(" << str << ")");
    if (res){
	std::chrono::hh_mm_ss t{t0.time_since_epoch()};
	CHECK_TRUE(t.hours().count() == hours and
		   t.minutes().count() ==  minutes and
		   t.seconds().count() == seconds,
		   "\ttime");

    }

}

void test_convert()
{
    test::interface("convert");
    test_convert("2h 3min 43s", 2, 3, 43, true);
    test_convert("2h 300min 43s", 2, 3, 43, false);
    test_convert("2h 3min 430s", 2, 3, 43, false);
    test_convert("200h 24min 19s", 200, 24, 19, true);
    test_convert("200 h 24 min 19 s", 200, 24, 19, true);
    test_convert("200 hours 24 minutes 19 seconds", 200, 24, 19, true);

    test_convert("43s", 0, 0, 43, true);
}

int main()
{
try{
    test::header("locale");

    test_read_uint();
    test_char_is_one_of();
    test_read_word();
    test_read_time_unit();

    test_convert();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





