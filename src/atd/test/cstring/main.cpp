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

#include "../../atd_cstring.h"

#include <alp_test.h>
#include <iostream>
#include <vector>

#include <string.h>

using namespace test;


void test_length()
{
    test::interfaz("length");

    {
    constexpr const char* msg = "";
    constexpr int n = atd::length(msg);
    CHECK_TRUE(n == strlen(msg), msg);
    }
    {
    constexpr const char* msg = "hola";
    constexpr int n = atd::length(msg);
    CHECK_TRUE(n == strlen(msg), msg);
    }
}

void test_cstring()
{
    test::header("const_cstring");

    constexpr const char* msg = "esto es una prueba";

    constexpr atd::const_cstring s(msg);

    CHECK_TRUE(strlen(msg) == s.length(), "length");
    CHECK_TRUE(msg == s.data(), "data");

    CHECK_EQUAL_CONTAINERS(msg, msg + strlen(msg), 
			    s.begin(), s.end(), "begin/end");
    {
    std::stringstream res;
    res << s;
    CHECK_TRUE(res.str() == msg, "operator<<");
    }
    
}

void test_line_count(const char* s, size_t res)
{
    std::cout << "line_count: ";
    CHECK_TRUE(atd::line_count(s) == res, s);
}

void test_line_count()
{
    test::interfaz("line_count");

    test_line_count("", 0);
    test_line_count("uno", 1);
    test_line_count("uno\ndos", 2);
    test_line_count("uno\ndos\n", 3);

}



int main()
{
try{
    test::header("string");

    test_length();
    test_cstring();
    test_line_count();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





