// Copyright (C) 2019-2020 A.Manuel L.Perez
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



void test_ostream()
{
    test::interfaz("only_date/time - write");

    tm t;
    t.tm_mday = 25;
    t.tm_mon = 10 - 1;
    t.tm_year = 2112 - 1900;
    t.tm_hour = 8;
    t.tm_min = 16;
    t.tm_sec = 2;

    {
    std::stringstream str;
    str << atd::only_date(t);
    CHECK_TRUE(str.str() == "25/10/2112", "<< only_date");
    }
    {
    std::stringstream str;
    str << atd::only_date(t, 'x');
    CHECK_TRUE(str.str() == "25x10x2112", "<< only_date");
    }

    {
    std::stringstream str;
    str << atd::only_time(t);
    CHECK_TRUE(str.str() == "08:16:02", "<< only_time");
    }

    {
    std::stringstream str;
    str << atd::only_time(t, '-');
    CHECK_TRUE(str.str() == "08-16-02", "<< only_time");
    }

}

void test_only_date_input(const std::string& date, char sep, bool fail = false)
{
    std::cout << "test: [" << date << "]\n";
    tm t;
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
 
int main()
{
try{
    test::header("atd_time");
    test_ostream();
    test_istream();
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}
}



