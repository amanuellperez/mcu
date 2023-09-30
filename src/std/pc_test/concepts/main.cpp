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


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




