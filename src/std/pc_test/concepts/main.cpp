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

class Class_derived : Class{ };

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

template <typename From, typename To>
void test_derived_from(const std::string& name_type_from,
		       const std::string& name_type_to)
{
    CHECK_TRUE(mtd::derived_from<From, To> 
	    == std::derived_from<From, To>, 
	    alp::as_str() << "test_derived_from(" << name_type_from << ", " 
			  << name_type_to << ")");
}

void test_derived_from()
{
    test::interface("derived_from");
    
    test_derived_from<void, nullptr_t>("void", "nullptr_t");
    test_derived_from<int, int*>("int", "int*");
    test_derived_from<Class_derived, Class>("Class_derived", "Class");
    test_derived_from<Class, Class_derived>("Class", "Class_derived");
}


template <typename From, typename To>
void test_convertible_to(const std::string& name_type_from,
		         const std::string& name_type_to)
{
    CHECK_TRUE(mtd::convertible_to<From, To> 
	    == std::convertible_to<From, To>, 
	    alp::as_str() << "test_convertible_to(" << name_type_from << ", " 
			  << name_type_to << ")");
}

template <typename From>
void test_convertible_to(const std::string& name_type_from)
{
    test::interface("convertible_to");
    
    test_convertible_to<From, void>(name_type_from, "void");
    test_convertible_to<From, nullptr_t>(name_type_from, "nullptr_t");

    test_convertible_to<From, char>(name_type_from, "char");
    test_convertible_to<From, int>(name_type_from, "int");
    test_convertible_to<From, long>(name_type_from, "long");
    test_convertible_to<From, long long>(name_type_from, "long long");
    test_convertible_to<From, float>(name_type_from, "float");
    test_convertible_to<From, double>(name_type_from, "double");

    test_convertible_to<From, int[]>(name_type_from, "int[]");
    test_convertible_to<From, int[3]>(name_type_from, "int[3]");
    test_convertible_to<From, int[][3]>(name_type_from, "int[][3]");

    test_convertible_to<From, Class>(name_type_from, "Class");
    test_convertible_to<From, Class2>(name_type_from, "Class2");
    test_convertible_to<From, Class_derived>(name_type_from, "Class_derived");
    test_convertible_to<From, Union>(name_type_from, "Union");
    test_convertible_to<From, Enum>(name_type_from, "Enum");
    test_convertible_to<From, Enum_class>(name_type_from, "Enum_class");

    test_convertible_to<From, Class>(name_type_from, "Class");
    test_convertible_to<From, int Class::*>(name_type_from, "int Class::*");
//    test_convertible_to<From, int (name_type_from, Class::*)()>("int (Class::*)()");
}

void test_convertible_to()
{
    test::interface("convertible_to");
    
    test_convertible_to<void>("void");
    test_convertible_to<nullptr_t>("nullptr_t");

    test_convertible_to<char>("char");
    test_convertible_to<int>("int");
    test_convertible_to<long>("long");
    test_convertible_to<long long>("long long");
    test_convertible_to<float>("float");
    test_convertible_to<double>("double");

    test_convertible_to<int[]>("int[]");
    test_convertible_to<int[3]>("int[3]");
    test_convertible_to<int[][3]>("int[][3]");

    test_convertible_to<Class>("Class");
    test_convertible_to<Class2>("Class2");
    test_convertible_to<Class_derived>("Class_derived");
    test_convertible_to<Union>("Union");
    test_convertible_to<Enum>("Enum");
    test_convertible_to<Enum_class>("Enum_class");

    test_convertible_to<Class>("Class");
    test_convertible_to<int Class::*>("int Class::*");
    test_convertible_to<int (Class::*)()>("int (Class::*)()");
}


template <typename From, typename To>
void test_common_reference_with(const std::string& name_type_from,
		         const std::string& name_type_to)
{
    CHECK_TRUE(mtd::common_reference_with<From, To> 
	    == std::common_reference_with<From, To>, 
	    alp::as_str() << "test_common_reference_with(" << name_type_from << ", " 
			  << name_type_to << ")");
}

template <typename From>
void test_common_reference_with(const std::string& name_type_from)
{
    test::interface("common_reference_with");
    
    test_common_reference_with<From, void>(name_type_from, "void");
    test_common_reference_with<From, nullptr_t>(name_type_from, "nullptr_t");

    test_common_reference_with<From, char>(name_type_from, "char");
    test_common_reference_with<From, int>(name_type_from, "int");
    test_common_reference_with<From, long>(name_type_from, "long");
    test_common_reference_with<From, long long>(name_type_from, "long long");
    test_common_reference_with<From, float>(name_type_from, "float");
    test_common_reference_with<From, double>(name_type_from, "double");

    test_common_reference_with<From, int[]>(name_type_from, "int[]");
    test_common_reference_with<From, int[3]>(name_type_from, "int[3]");
    test_common_reference_with<From, int[][3]>(name_type_from, "int[][3]");

    test_common_reference_with<From, Class>(name_type_from, "Class");
    test_common_reference_with<From, Class2>(name_type_from, "Class2");
    test_common_reference_with<From, Class_derived>(name_type_from, "Class_derived");
    test_common_reference_with<From, Union>(name_type_from, "Union");
//    test_common_reference_with<From, Enum>(name_type_from, "Enum");
    test_common_reference_with<From, Enum_class>(name_type_from, "Enum_class");

    test_common_reference_with<From, Class>(name_type_from, "Class");
    test_common_reference_with<From, int Class::*>(name_type_from, "int Class::*");
//    test_common_reference_with<From, int (name_type_from, Class::*)()>("int (Class::*)()");
}

void test_common_reference_with()
{
    test::interface("common_reference_with");
    
    test_common_reference_with<void>("void");
    test_common_reference_with<nullptr_t>("nullptr_t");

    test_common_reference_with<char>("char");
    test_common_reference_with<int>("int");
    test_common_reference_with<long>("long");
    test_common_reference_with<long long>("long long");
    test_common_reference_with<float>("float");
    test_common_reference_with<double>("double");

    test_common_reference_with<int[]>("int[]");
    test_common_reference_with<int[3]>("int[3]");
    test_common_reference_with<int[][3]>("int[][3]");

    test_common_reference_with<Class>("Class");
    test_common_reference_with<Class2>("Class2");
    test_common_reference_with<Class_derived>("Class_derived");
    test_common_reference_with<Union>("Union");
//    test_common_reference_with<Enum>("Enum");
    test_common_reference_with<Enum_class>("Enum_class");

    test_common_reference_with<Class>("Class");
    test_common_reference_with<int Class::*>("int Class::*");
    test_common_reference_with<int (Class::*)()>("int (Class::*)()");
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
    
    test_equality_comparable_with<void>("void");
    test_equality_comparable_with<nullptr_t>("nullptr_t");

    test_equality_comparable_with<char>("char");
    test_equality_comparable_with<int>("int");
    test_equality_comparable_with<long>("long");
    test_equality_comparable_with<long long>("long long");
    // float y double da muchos warnings
//    test_equality_comparable_with<float>("float");
//    test_equality_comparable_with<double>("double");

    test_equality_comparable_with<int[]>("int[]");
    test_equality_comparable_with<int[3]>("int[3]");
    test_equality_comparable_with<int[][3]>("int[][3]");

    test_equality_comparable_with<Class>("Class");
    test_equality_comparable_with<Class2>("Class2");
    test_equality_comparable_with<Union>("Union");
    //test_equality_comparable_with<Enum>("Enum");
    test_equality_comparable_with<Enum_class>("Enum_class");

    test_equality_comparable_with<Class>("Class");
    test_equality_comparable_with<int Class::*>("int Class::*");
    test_equality_comparable_with<int (Class::*)()>("int (Class::*)()");
}




template <typename T>
void test_totally_ordered(const std::string& name_type)
{
    CHECK_TRUE(mtd::totally_ordered<T> 
	    == std::totally_ordered<T>, name_type);
}

void test_totally_ordered()
{
    test::interface("totally_ordered");
    
    test_totally_ordered<void>("void");
    test_totally_ordered<nullptr_t>("nullptr_t");

    test_totally_ordered<char>("char");
    test_totally_ordered<int>("int");
    test_totally_ordered<long>("long");
    test_totally_ordered<long long>("long long");
    test_totally_ordered<float>("float");
    test_totally_ordered<double>("double");

    test_totally_ordered<int[]>("int[]");
    test_totally_ordered<int[3]>("int[3]");
    test_totally_ordered<int[][3]>("int[][3]");

    test_totally_ordered<Class>("Class");
    test_totally_ordered<Class2>("Class2");
    test_totally_ordered<Union>("Union");
    test_totally_ordered<Enum>("Enum");
    test_totally_ordered<Enum_class>("Enum_class");

    test_totally_ordered<Class>("Class");
    test_totally_ordered<int Class::*>("int Class::*");
    test_totally_ordered<int (Class::*)()>("int (Class::*)()");
}


template <typename T, typename U>
void test_totally_ordered_with(const std::string& name_type1, 
				   const std::string& name_type2)
{
    CHECK_TRUE(mtd::totally_ordered_with<T, U> 
	    == std::totally_ordered_with<T, U>, 
	    alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_totally_ordered_with(const std::string& name_type)
{
    test_totally_ordered_with<T, void>(name_type, "void");
    test_totally_ordered_with<T, nullptr_t>(name_type, "nullptr_t");

    test_totally_ordered_with<T, char>(name_type, "char");
    test_totally_ordered_with<T, int>(name_type, "int");
    test_totally_ordered_with<T, long>(name_type, "long");
    test_totally_ordered_with<T, long long>(name_type, "long long");
    test_totally_ordered_with<T, float>(name_type, "float");
    test_totally_ordered_with<T, double>(name_type, "double");

    test_totally_ordered_with<T, int[]>(name_type, "int[]");
    test_totally_ordered_with<T, int[3]>(name_type, "int[3]");
    test_totally_ordered_with<T, int[][3]>(name_type, "int[][3]");

    test_totally_ordered_with<T, Class>(name_type, "Class");
    test_totally_ordered_with<T, Class2>(name_type, "Class2");
    test_totally_ordered_with<T, Union>(name_type, "Union");
    test_totally_ordered_with<T, Enum>(name_type, "Enum");
    test_totally_ordered_with<T, Enum_class>(name_type, "Enum_class");

    test_totally_ordered_with<T, Class>(name_type, "Class");
    test_totally_ordered_with<T, int Class::*>(name_type, "int Class::*");
    test_totally_ordered_with<T, int (Class::*)()>(name_type, "int (Class::*)()");
}

void test_totally_ordered_with()
{
    test::interface("totally_ordered_with");
    
    test_totally_ordered_with<void>("void");
    test_totally_ordered_with<nullptr_t>("nullptr_t");

    test_totally_ordered_with<char>("char");
    test_totally_ordered_with<int>("int");
    test_totally_ordered_with<long>("long");
    test_totally_ordered_with<long long>("long long");
    // float y double da muchos warnings
//    test_totally_ordered_with<float>("float");
//    test_totally_ordered_with<double>("double");

    test_totally_ordered_with<int[]>("int[]");
    test_totally_ordered_with<int[3]>("int[3]");
    test_totally_ordered_with<int[][3]>("int[][3]");

    test_totally_ordered_with<Class>("Class");
    test_totally_ordered_with<Class2>("Class2");
    test_totally_ordered_with<Union>("Union");
    //test_totally_ordered_with<Enum>("Enum");
    test_totally_ordered_with<Enum_class>("Enum_class");

    test_totally_ordered_with<Class>("Class");
    test_totally_ordered_with<int Class::*>("int Class::*");
    test_totally_ordered_with<int (Class::*)()>("int (Class::*)()");
}

int main()
{
try{
    test::header("concepts");

    // language-related concepts
    // -------------------------
    test_same_as();
    test_derived_from();
    test_convertible_to();
    test_common_reference_with();
    test_integral();

    // arithmetic concepts
    // -------------------
    

    // comparison concepts
    // -------------------
    test_equality_comparable();
    test_equality_comparable_with();
    test_totally_ordered();
    test_totally_ordered_with();


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




