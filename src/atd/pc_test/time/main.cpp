// Copyright (C) 2019-2020 A.Manuel L.Perez 
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

#include "../../atd_time.h"

#include <iostream>
#include <sstream>
#include <alp_test.h>

using namespace test;



constexpr char weekday_as_str1[] = "DLMXJVS";
constexpr char weekday_as_str2[] = "DoLuMaMiJuViSa";

void test_ostream()
{
    test::interfaz("only_date/time - write");

    std::tm t;

    atd::Generic_time_view<std::tm> gt{t};
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
    test::interfaz(">> only_date/time");

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
    test::interfaz("Generic_time");

    std::tm t;

    t.tm_sec = 1;
    t.tm_min = 2;
    t.tm_hour = 3;
    t.tm_mday = 4;
    t.tm_mon = 5;
    t.tm_year = 6;
    t.tm_wday = 3;

    atd::Generic_time_view<std::tm> gt{t};
    CHECK_TRUE(gt.seconds() == 1, "seconds");
    CHECK_TRUE(gt.minutes() == 2, "minutes");
    CHECK_TRUE(gt.hours() == 3, "hours");
    CHECK_TRUE(gt.day() == 4, "day");
    CHECK_TRUE(gt.month() == 6, "month");
    CHECK_TRUE(gt.year() == 1906, "year");
    CHECK_TRUE(gt.weekday() == 3, "weekday");

    gt.seconds(9);
    gt.minutes(8);
    gt.hours(7);
    gt.day(10);
    gt.month(12);
    gt.year(2020);
    gt.weekday(2);

    CHECK_TRUE(gt.seconds() == 9, "seconds");
    CHECK_TRUE(gt.minutes() == 8, "minutes");
    CHECK_TRUE(gt.hours() == 7, "hours");
    CHECK_TRUE(gt.day() == 10, "day");
    CHECK_TRUE(gt.month() == 12, "month");
    CHECK_TRUE(gt.year() == 2020, "year");
    CHECK_TRUE(gt.weekday() == 2, "weekday");

    CHECK_TRUE(t.tm_sec == 9, "tm_sec");
    CHECK_TRUE(t.tm_min == 8, "tm_min");
    CHECK_TRUE(t.tm_hour == 7, "tm_hour");
    CHECK_TRUE(t.tm_mday == 10, "tm_mday");
    CHECK_TRUE(t.tm_mon == 11, "tm_mon");
    CHECK_TRUE(t.tm_year == 2020 - 1900, "tm_year");
    CHECK_TRUE(t.tm_wday == 2, "tm_wday");

    std::cout << "check[";
    print_time(std::cout, gt);
    std::cout << "]: 07:08:09\n";

    std::cout << "check["; print_date(std::cout, gt); std::cout << "]: 10/02/2020\n";
    std::cout << "check["; print_weekday<1>(std::cout, gt, weekday_as_str1);
    std::cout << "]: M\n";
    std::cout << "check["; print_weekday<2>(std::cout, gt, weekday_as_str2);
    std::cout << "]: Ma\n";
}


int main()
{
try{
    test::header("atd_time");
    test_ostream();
    test_istream();
    test_generic_time();
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}



