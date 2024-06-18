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

#include "../../atd_time.h"

#include <iostream>
#include <sstream>
#include <alp_test.h>

using namespace test;



constexpr char weekday_as_str1[] = "DLMXJVS";
constexpr char weekday_as_str2[] = "DoLuMaMiJuViSa";

void test_ostream()
{
    test::interface("only_date/time - write");

    std::tm t;

    atd::Date_time_view<std::tm> gt{t};
    gt.day(25);
    gt.month(10);
    gt.year(2112);
    gt.hours(8);
    gt.minutes(16);
    gt.seconds(2);


    {
    std::stringstream str;
    str << atd::only_date(gt);
    CHECK_TRUE(str.str() == "25/10/2112", "<< only_date");
    }
    {
    std::stringstream str;
    str << atd::only_date(gt, 'x');
    CHECK_TRUE(str.str() == "25x10x2112", "<< only_date");
    }

    {
    std::stringstream str;
    std::cerr << atd::only_time(gt);
    str << atd::only_time(gt);
    CHECK_TRUE(str.str() == "08:16:02", "<< only_time");
    }

    {
    std::stringstream str;
    str << atd::only_time(gt, '-');
    CHECK_TRUE(str.str() == "08-16-02", "<< only_time");
    }

}

void test_only_date_input(const std::string& date, char sep, bool fail = false)
{
    std::cout << "test: [" << date << "]\n";
    std::tm t;

    std::stringstream in;
    in << date;
    in >> atd::only_date(t, sep);
    if (fail){
	CHECK_TRUE(in.fail(), "in");
	return;
    }

    CHECK_TRUE(!in.fail(), "in");
    std::stringstream out;
    out << atd::only_date(t, sep);
    CHECK_TRUE(in.str() == out.str(), "only_date");
}


void test_only_time_input(const std::string& time, char sep, bool fail = false)
{
    std::cout << "test: [" << time << "]\n";
    tm t;
    std::stringstream in;
    in << time;
    in >> atd::only_time(t, sep);

    if (fail){
	CHECK_TRUE(in.fail(), "in");
	return;
    }

    CHECK_TRUE(!in.fail(), "in");
    std::stringstream out;
    out << atd::only_time(t, sep);
    CHECK_TRUE(in.str() == out.str(), "only_time");
}

void test_istream()
{
    test::interface(">> only_date/time");

    test_only_date_input("02/11/2019", '/');
    test_only_date_input("02-11-2019", '-');
    test_only_time_input("16:31:04", ':');
    test_only_time_input("16|31|04", '|');

    test_only_date_input("", '/', true);
    test_only_date_input("02/", '/', true);
    test_only_date_input("02/11/", '/', true);
    test_only_date_input("50/11/2019", '/', true);
    test_only_date_input("00/11/2019", '/', true);
    test_only_date_input("25/-2/2019", '/', true);
    test_only_date_input("25/100/2019", '/', true);
    test_only_date_input("25/0/2019", '/', true);
    test_only_date_input("25/11/2019", ':', true);

    test_only_time_input("", ':', true);
    test_only_time_input("20:", ':', true);
    test_only_time_input("20:04:", ':', true);
    test_only_time_input("-3:04:00", ':', true);
    test_only_time_input("24:04:00", ':', true);
    test_only_time_input("22:-4:00", ':', true);
    test_only_time_input("22:100:00", ':', true);
    test_only_time_input("22:02:-5", ':', true);
    test_only_time_input("22:02:60", ':', true);
    test_only_time_input("22:02:40", '/', true);

}
 
void test_generic_time()
{
    test::interface("Generic_time");

    std::tm t;

    t.tm_sec = 1;
    t.tm_min = 2;
    t.tm_hour = 3;
    t.tm_mday = 4;
    t.tm_mon = 5;
    t.tm_year = 6;
    t.tm_wday = 3;

    atd::Date_time_view<std::tm> gt{t};
    CHECK_TRUE(gt.seconds() == 1, "seconds");
    CHECK_TRUE(gt.minutes() == 2, "minutes");
    CHECK_TRUE(gt.hours() == 3, "hours");
    CHECK_TRUE(gt.day() == 4, "day");
    CHECK_TRUE(gt.month() == 6, "month");
    CHECK_TRUE(gt.year() == 1906, "year");
    CHECK_TRUE(gt.weekday() == 3, "weekday");

    gt.seconds(42);
    gt.minutes(59);
    gt.hours(23);
    gt.day(28);
    gt.month(2);
    gt.year(2001);
    gt.weekday(2);

    CHECK_TRUE(gt.seconds() == 42, "seconds");
    CHECK_TRUE(gt.minutes() == 59, "minutes");
    CHECK_TRUE(gt.hours() == 23, "hours");
    CHECK_TRUE(gt.day() == 28, "day");
    CHECK_TRUE(gt.month() == 2, "month");
    CHECK_TRUE(gt.year() == 2001, "year");
    CHECK_TRUE(gt.weekday() == 2, "weekday");

    CHECK_TRUE(t.tm_sec == 42, "tm_sec");
    CHECK_TRUE(t.tm_min == 59, "tm_min");
    CHECK_TRUE(t.tm_hour == 23, "tm_hour");
    CHECK_TRUE(t.tm_mday == 28, "tm_mday");
    CHECK_TRUE(t.tm_mon == 2 - 1, "tm_mon");
    CHECK_TRUE(t.tm_year == 2001 - 1900, "tm_year");
    CHECK_TRUE(t.tm_wday == 2, "tm_wday");

    std::cout << "check[";
    print_time(std::cout, gt);
    std::cout << "]: 23:59:42\n";

    std::cout << "check["; print_date(std::cout, gt); std::cout << "]: 28/02/2001\n";
    std::cout << "check["; print_weekday<1>(std::cout, gt, weekday_as_str1);
    std::cout << "]: M\n";
    std::cout << "check["; print_weekday<2>(std::cout, gt, weekday_as_str2);
    std::cout << "]: Ma\n";
}


void test_const_generic_time()
{
    test::interface("const_Generic_time");

    std::tm t;

    t.tm_sec = 1;
    t.tm_min = 2;
    t.tm_hour = 3;
    t.tm_mday = 4;
    t.tm_mon = 5;
    t.tm_year = 6;
    t.tm_wday = 3;

    atd::Date_time_view<const std::tm> gt{t};
    CHECK_TRUE(gt.seconds() == 1, "seconds");
    CHECK_TRUE(gt.minutes() == 2, "minutes");
    CHECK_TRUE(gt.hours() == 3, "hours");
    CHECK_TRUE(gt.day() == 4, "day");
    CHECK_TRUE(gt.month() == 6, "month");
    CHECK_TRUE(gt.year() == 1906, "year");
    CHECK_TRUE(gt.weekday() == 3, "weekday");
}


void print_date(const atd::Date_time& t)
{
std::cout << t.day() << '/'
	  << t.month() << '/'
	  << t.year()  << ' '
	  << t.hours() << ':'
	  << t.minutes() << ':'
	  << t.seconds() <<  '\n';
}

void test_date_time()
{
    test::interface("Date_time");

    atd::Date_time t;
    t.day(6);
    t.month(1);
    t.year(2023);
    t.weekday(5);

    t.hours(00);
    t.minutes(22);
    t.seconds(32);

    CHECK_TRUE(t.day() == 6, "day");
    CHECK_TRUE(t.month() == 1, "month");
    CHECK_TRUE(t.year() == 2023, "year");
    CHECK_TRUE(t.weekday() == 5, "weekday");

    CHECK_TRUE(t.hours() == 00, "hours");
    CHECK_TRUE(t.minutes() == 22, "minutes");
    CHECK_TRUE(t.seconds() == 32, "seconds");

    std::time_t t0 = t.to_time_t();
    t0 += 24*3600; // 1 dia
    t.from_time_t(t0);


    CHECK_TRUE(t.day() == 7, "day");
    CHECK_TRUE(t.month() == 1, "month");
    CHECK_TRUE(t.year() == 2023, "year");
    CHECK_TRUE(t.weekday() == 6, "weekday");

    CHECK_TRUE(t.hours() == 00, "hours");
    CHECK_TRUE(t.minutes() == 22, "minutes");
    CHECK_TRUE(t.seconds() == 32, "seconds");

}

std::ostream& operator<<(std::ostream& out, const atd::Time_ms& t)
{
    return out  << (int) t.hours << ':'
		<< (int) t.minutes << ':'
		<< (int) t.seconds << ' '
		<< t.milliseconds << " ms";
}


void test_time_ms()
{
    test::interface("Time_ms");

    {// normal
    std::chrono::milliseconds ms{7427259};
    atd::Time_ms t{ms};
//    std::cout << t << '\n';

    CHECK_TRUE(t.hours == 2, "hours");
    CHECK_TRUE(t.minutes == 3, "minutes");
    CHECK_TRUE(t.seconds == 47, "seconds");
    CHECK_TRUE(t.milliseconds == 259, "milliseconds");

    CHECK_TRUE(t.as_milliseconds() == ms, "as_milliseconds");
    }
    {// 0
    atd::Time_ms t;
//    std::cout << t << '\n';

    CHECK_TRUE(t.hours == 0, "hours");
    CHECK_TRUE(t.minutes == 0, "minutes");
    CHECK_TRUE(t.seconds == 0, "seconds");
    CHECK_TRUE(t.milliseconds == 0, "milliseconds");

    CHECK_TRUE(t.as_milliseconds() == std::chrono::milliseconds{0}, "as_milliseconds");
    }
}


int main()
{
try{
    test::header("atd_time");

    test_ostream();
    test_istream();
    test_generic_time();
    test_const_generic_time();
    test_date_time();
    test_time_ms();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}



