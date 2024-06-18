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

#include "../../atd_concepts.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>

using namespace test;

struct  My_ostream : std::ostream{};
struct  My_istream : std::istream{};
struct  My_iostream : std::iostream{};

void test_ostream()
{
    test::interface("Ostream");

    CHECK_TRUE(Type::Ostream<std::ostream> == true, "Ostream<ostream>");
    CHECK_TRUE(Type::Ostream<std::istream> == false, "Ostream<istream>");
    CHECK_TRUE(Type::Ostream<std::iostream> == true, "Ostream<iostream>");


    CHECK_TRUE(Type::Ostream<My_ostream> == true, "Ostream<My_ostream>");
    CHECK_TRUE(Type::Ostream<My_istream> == false, "Ostream<My_istream>");
    CHECK_TRUE(Type::Ostream<My_iostream> == true, "Ostream<My_iostream>");

    CHECK_TRUE(Type::Ostream<int> == false, "Ostream<int>");
}


void test_istream()
{
    test::interface("Istream");

    CHECK_TRUE(Type::Istream<std::ostream> == false, "Istream<ostream>");
    CHECK_TRUE(Type::Istream<std::istream> == true, "Istream<istream>");
    CHECK_TRUE(Type::Istream<std::iostream> == true, "Istream<iostream>");


    CHECK_TRUE(Type::Istream<My_ostream> == false, "Istream<My_ostream>");
    CHECK_TRUE(Type::Istream<My_istream> == true, "Istream<My_istream>");
    CHECK_TRUE(Type::Istream<My_iostream> == true, "Istream<My_iostream>");

    CHECK_TRUE(Type::Istream<int> == false, "Istream<int>");
}


void test_iostream()
{
    test::interface("IOstream");

    CHECK_TRUE(Type::IOstream<std::ostream> == true, "IOstream<ostream>");
    CHECK_TRUE(Type::IOstream<std::istream> == true, "IOstream<istream>");
    CHECK_TRUE(Type::IOstream<std::iostream> == true, "IOstream<iostream>");


    CHECK_TRUE(Type::IOstream<My_ostream> == true, "IOstream<My_ostream>");
    CHECK_TRUE(Type::IOstream<My_istream> == true, "IOstream<My_istream>");
    CHECK_TRUE(Type::IOstream<My_iostream> == true, "IOstream<My_iostream>");

    CHECK_TRUE(Type::IOstream<int> == false, "IOstream<int>");
}


int main()
{
try{
    test::header("atd_concepts");
    
    test_ostream();
    test_istream();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





