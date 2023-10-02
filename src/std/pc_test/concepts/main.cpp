// Copyright (C) 2022-2023 Manuel Perez 
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

#include "../../std_concepts.h"
#include <concepts>

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <string>


using namespace test;

class Class 
{ int x; };

class Class2
{ Class x; };

struct A {int m;};
struct B : public A {int m;};
struct C {virtual void f(); };

class D{
public:
    D(int n) : v1{n}, v2{} { }
    D(int n, double f) noexcept : v1{n}, v2{f} { }

private:
    int v1;
    double v2;
};

typedef union
{
    int a;
    float b;
} Union;

enum Enum {black, white};
enum class Enum_class{ one, two, three };

void test_same_as()
{
    test::interface("same_as");

    CHECK_TRUE(mtd::same_as<int, int> == std::same_as<int,int>, "same_as<int, int>");
    CHECK_TRUE(mtd::same_as<int, const int> == std::same_as<int,const int>, "same_as<int, const int>");
    CHECK_TRUE(mtd::same_as<int, int*> == std::same_as<int,int*>, "same_as<int, int*>");
}

void test_integral()
{
    test::interface("mtd::integral");

    CHECK_TRUE(mtd::integral<char> == std::integral<char>, "integral");
    CHECK_TRUE(mtd::integral<short> == std::integral<short>, "integral");
    CHECK_TRUE(mtd::integral<int> == std::integral<int>, "integral");
    CHECK_TRUE(mtd::integral<long> == std::integral<long>, "integral");
    CHECK_TRUE(mtd::integral<float> == std::integral<float>, "integral");
    CHECK_TRUE(mtd::integral<double> == std::integral<double>, "integral");

    CHECK_TRUE(mtd::integral<unsigned char> == std::integral<unsigned char>, "integral");
    CHECK_TRUE(mtd::integral<unsigned short> == std::integral<unsigned short>, "integral");
    CHECK_TRUE(mtd::integral<unsigned int> == std::integral<unsigned int>, "integral");
    CHECK_TRUE(mtd::integral<unsigned long> == std::integral<unsigned long>, "integral");

    CHECK_TRUE(mtd::integral<A> == std::integral<A>, "integral");
}

template <typename T>
void test_equality_comparable(const std::string& name_type)
{
    CHECK_TRUE(mtd::equality_comparable<T> 
	    == std::equality_comparable<T>, name_type);
}

void test_equality_comparable()
{
    test::interface("equality_comparable");
    
    test_equality_comparable<void>("void");
    test_equality_comparable<nullptr_t>("nullptr_t");

    test_equality_comparable<char>("char");
    test_equality_comparable<int>("int");
    test_equality_comparable<long>("long");
    test_equality_comparable<long long>("long long");
    test_equality_comparable<float>("float");
    test_equality_comparable<double>("double");

    test_equality_comparable<int[]>("int[]");
    test_equality_comparable<int[3]>("int[3]");
    test_equality_comparable<int[][3]>("int[][3]");

    test_equality_comparable<Class>("Class");
    test_equality_comparable<Class2>("Class2");
    test_equality_comparable<Union>("Union");
    test_equality_comparable<Enum>("Enum");
    test_equality_comparable<Enum_class>("Enum_class");

    test_equality_comparable<Class>("Class");
    test_equality_comparable<int Class::*>("int Class::*");
    test_equality_comparable<int (Class::*)()>("int (Class::*)()");
}

template <typename T, typename U>
void test_equality_comparable_with(const std::string& name_type1, 
				   const std::string& name_type2)
{
    CHECK_TRUE(mtd::equality_comparable_with<T, U> 
	    == std::equality_comparable_with<T, U>, 
	    alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_equality_comparable_with(const std::string& name_type)
{
    test_equality_comparable_with<T, void>(name_type, "void");
    test_equality_comparable_with<T, nullptr_t>(name_type, "nullptr_t");

    test_equality_comparable_with<T, char>(name_type, "char");
    test_equality_comparable_with<T, int>(name_type, "int");
    test_equality_comparable_with<T, long>(name_type, "long");
    test_equality_comparable_with<T, long long>(name_type, "long long");
    test_equality_comparable_with<T, float>(name_type, "float");
    test_equality_comparable_with<T, double>(name_type, "double");

    test_equality_comparable_with<T, int[]>(name_type, "int[]");
    test_equality_comparable_with<T, int[3]>(name_type, "int[3]");
    test_equality_comparable_with<T, int[][3]>(name_type, "int[][3]");

    test_equality_comparable_with<T, Class>(name_type, "Class");
    test_equality_comparable_with<T, Class2>(name_type, "Class2");
    test_equality_comparable_with<T, Union>(name_type, "Union");
    test_equality_comparable_with<T, Enum>(name_type, "Enum");
    test_equality_comparable_with<T, Enum_class>(name_type, "Enum_class");

    test_equality_comparable_with<T, Class>(name_type, "Class");
    test_equality_comparable_with<T, int Class::*>(name_type, "int Class::*");
    test_equality_comparable_with<T, int (Class::*)()>(name_type, "int (Class::*)()");
}

void test_equality_comparable_with()
{
    test::interface("equality_comparable_with");
    
    CHECK_TRUE(mtd::equality_comparable_with<int, float> 
	    == std::equality_comparable_with<int, float>, "kk");
//    test_equality_comparable_with<void>("void");
//    test_equality_comparable_with<nullptr_t>("nullptr_t");
//
//    test_equality_comparable_with<char>("char");
//    test_equality_comparable_with<int>("int");
//    test_equality_comparable_with<long>("long");
//    test_equality_comparable_with<long long>("long long");
//    test_equality_comparable_with<float>("float");
//    test_equality_comparable_with<double>("double");
//
//    test_equality_comparable_with<int[]>("int[]");
//    test_equality_comparable_with<int[3]>("int[3]");
//    test_equality_comparable_with<int[][3]>("int[][3]");
//
//    test_equality_comparable_with<Class>("Class");
//    test_equality_comparable_with<Class2>("Class2");
//    test_equality_comparable_with<Union>("Union");
//    test_equality_comparable_with<Enum>("Enum");
//    test_equality_comparable_with<Enum_class>("Enum_class");
//
//    test_equality_comparable_with<Class>("Class");
//    test_equality_comparable_with<int Class::*>("int Class::*");
//    test_equality_comparable_with<int (Class::*)()>("int (Class::*)()");
}


int main()
{
try{
    test::header("concepts");

    // language-related concepts
    // -------------------------
    test_same_as();
    test_integral();

    // arithmetic concepts
    // -------------------
    

    // comparison concepts
    // -------------------
    test_equality_comparable();
    test_equality_comparable_with();


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




