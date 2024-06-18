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

#include "../../std_atd.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <string>

using namespace test;

struct Class_with_type{
    using type = int;
};

struct Class_without_type{
};

class Class_with_destructor
{ };

class Class_without_destructor
{ 
    public:
	~Class_without_destructor() = delete;
};

void test_not()
{
    test::interface("not");

    CHECK_TRUE(mtd::atd_::static_not<true>() == false, "static_not<true>");
    CHECK_TRUE(mtd::atd_::static_not<false>() == true, "static_not<false>");

}


void test_and()
{
    test::interface("and");

    CHECK_TRUE(mtd::atd_::static_and<false,false>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<false,true>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,false>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,true>() == true, "static_and");


    CHECK_TRUE(mtd::atd_::static_and<false,false, true>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<false,true, true>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,false, true>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,true, true>() == true, "static_and");

    CHECK_TRUE(mtd::atd_::static_and<false,false,false>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<false,true,false>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,false,false>() == false, "static_and");
    CHECK_TRUE(mtd::atd_::static_and<true,true,false>() == false, "static_and");

}



void test_or()
{
    test::interface("and");

    CHECK_TRUE(mtd::atd_::static_or<false,false>() == false, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<false,true>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,false>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,true>() == true, "static_or");


    CHECK_TRUE(mtd::atd_::static_or<false,false, true>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<false,true, true>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,false, true>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,true, true>() == true, "static_or");

    CHECK_TRUE(mtd::atd_::static_or<false,false,false>() == false, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<false,true,false>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,false,false>() == true, "static_or");
    CHECK_TRUE(mtd::atd_::static_or<true,true,false>() == true, "static_or");

}

void test_type_member()
{
    test::interface("type_member");
    CHECK_TRUE(mtd::atd_::type_member<int> == false, "int");
    CHECK_TRUE(mtd::atd_::type_member<Class_with_type> == true, 
						    "Class_with_type");
    CHECK_TRUE(mtd::atd_::type_member<Class_without_type> == false, 
						    "Class_without_type");
	    
}

template <typename From, typename To, typename Res>
void test_copy_cv(const std::string& name_type_from,
		  const std::string& name_type_to)
{
    CHECK_TRUE(std::is_same_v<mtd::atd_::copy_cv_t<From, To>, Res>,
		alp::as_str() << "copy_cv_t<" << name_type_from << ", "
		              << name_type_to << ")");
}

void test_copy_cv()
{
    test::interface("copy_cv");

    test_copy_cv<char, char, char>("char", "char");
    test_copy_cv<char&, char&, char&>("char&", "char&");
    test_copy_cv<int&, int&, int&>("int&", "int&");

    test_copy_cv<std::string, int, int>("std::string", "int");
    test_copy_cv<const std::string, int, const int>("const std::string", "int");
    test_copy_cv<volatile std::string, int, volatile int>("volatile std::string", "int");
    test_copy_cv<const volatile std::string, int, const volatile int>("const volatile std::string", "int");

}

void test_has_destructor()
{
    test::interface("has_destructor");

    CHECK_TRUE(mtd::atd_::has_destructor<int> == true, "int");
    CHECK_TRUE(mtd::atd_::has_destructor<Class_without_destructor> == false, "Class_without_destructor");
    CHECK_TRUE(mtd::atd_::has_destructor<Class_with_destructor> == true, "Class_with_destructor");
}

void test_is_referenceable()
{
    test::interface("is_referenceable");
    CHECK_TRUE(mtd::atd_::is_referenceable<void> == false, "void");
    CHECK_TRUE(mtd::atd_::is_referenceable<int> == true, "int");
}


int main()
{
try{
    test::header("atd_");
    
    test_is_referenceable();
    test_not();
    test_and();
    test_or();
    test_type_member();
    test_copy_cv();
    test_has_destructor();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





