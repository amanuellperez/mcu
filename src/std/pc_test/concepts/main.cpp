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

class D_noexcept{
public:
    D_noexcept(int n) noexcept : v1{n} { }

private:
    int v1;
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


template <typename T>
void test_integral(const std::string& name_type)
{
    CHECK_TRUE(mtd::integral<T> == 
	       std::integral<T>, 
	       alp::as_str() << "integral(" << name_type << ")");
}

void test_integral()
{
    test::interface("mtd::integral");

    test_integral<char>("char");
    test_integral<short>("short");
    test_integral<int>("int");
    test_integral<long>("long");
    test_integral<float>("float");
    test_integral<double>("double");

    test_integral<unsigned char>("unsigned char");
    test_integral<unsigned short>("unsigned short");
    test_integral<unsigned int>("unsigned int");
    test_integral<unsigned long>("unsigned long");

    test_integral<A>("A");
}

template <typename T>
void test_signed_integral(const std::string& name_type)
{
    CHECK_TRUE(mtd::signed_integral<T> == 
	       std::signed_integral<T>, 
	       alp::as_str() << "signed_integral(" << name_type << ")");
}

void test_signed_integral()
{
    test::interface("mtd::signed_integral");

    test_signed_integral<char>("char");
    test_signed_integral<short>("short");
    test_signed_integral<int>("int");
    test_signed_integral<long>("long");
    test_signed_integral<float>("float");
    test_signed_integral<double>("double");

    test_signed_integral<unsigned char>("unsigned char");
    test_signed_integral<unsigned short>("unsigned short");
    test_signed_integral<unsigned int>("unsigned int");
    test_signed_integral<unsigned long>("unsigned long");

    test_signed_integral<A>("A");
}


template <typename T>
void test_unsigned_integral(const std::string& name_type)
{
    CHECK_TRUE(mtd::unsigned_integral<T> == 
	       std::unsigned_integral<T>, 
	       alp::as_str() << "unsigned_integral(" << name_type << ")");
}

void test_unsigned_integral()
{
    test::interface("mtd::unsigned_integral");

    test_unsigned_integral<char>("char");
    test_unsigned_integral<short>("short");
    test_unsigned_integral<int>("int");
    test_unsigned_integral<long>("long");
    test_unsigned_integral<float>("float");
    test_unsigned_integral<double>("double");

    test_unsigned_integral<unsigned char>("unsigned char");
    test_unsigned_integral<unsigned short>("unsigned short");
    test_unsigned_integral<unsigned int>("unsigned int");
    test_unsigned_integral<unsigned long>("unsigned long");

    test_unsigned_integral<A>("A");
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


template <typename T, typename U>
void test_swappable_with(const std::string& name_type1, 
				   const std::string& name_type2)
{
    CHECK_TRUE(mtd::swappable_with<T, U> 
	    == std::swappable_with<T, U>, 
	    alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_swappable_with(const std::string& name_type)
{
    test_swappable_with<T, void>(name_type, "void");
    test_swappable_with<T, nullptr_t>(name_type, "nullptr_t");

    test_swappable_with<T, char>(name_type, "char");
    test_swappable_with<T, int>(name_type, "int");
    test_swappable_with<T, long>(name_type, "long");
    test_swappable_with<T, long long>(name_type, "long long");
    test_swappable_with<T, float>(name_type, "float");
    test_swappable_with<T, double>(name_type, "double");

    test_swappable_with<T, int[]>(name_type, "int[]");
    test_swappable_with<T, int[3]>(name_type, "int[3]");
    test_swappable_with<T, int[][3]>(name_type, "int[][3]");

    test_swappable_with<T, Class>(name_type, "Class");
    test_swappable_with<T, Class2>(name_type, "Class2");
    test_swappable_with<T, Union>(name_type, "Union");
    test_swappable_with<T, Enum>(name_type, "Enum");
    test_swappable_with<T, Enum_class>(name_type, "Enum_class");

    test_swappable_with<T, Class>(name_type, "Class");
    test_swappable_with<T, int Class::*>(name_type, "int Class::*");
    test_swappable_with<T, int (Class::*)()>(name_type, "int (Class::*)()");

    test_swappable_with<T, A>(name_type, "A");
    test_swappable_with<T, B>(name_type, "B");
    test_swappable_with<T, C>(name_type, "C");
    test_swappable_with<T, D>(name_type, "D");
    test_swappable_with<T, D_noexcept>(name_type, "D_noexcept");
}

void test_swappable_with()
{
    test::interface("swappable_with");
    
    test_swappable_with<void>("void");
    test_swappable_with<nullptr_t>("nullptr_t");

    test_swappable_with<char>("char");
    test_swappable_with<int>("int");
    test_swappable_with<long>("long");
    test_swappable_with<long long>("long long");
    // float y double da muchos warnings
//    test_swappable_with<float>("float");
//    test_swappable_with<double>("double");

    test_swappable_with<int[]>("int[]");
    test_swappable_with<int[3]>("int[3]");
    test_swappable_with<int[][3]>("int[][3]");

    test_swappable_with<Class>("Class");
    test_swappable_with<Class2>("Class2");
    test_swappable_with<Union>("Union");
    //test_swappable_with<Enum>("Enum");
    test_swappable_with<Enum_class>("Enum_class");

    test_swappable_with<Class>("Class");
    test_swappable_with<int Class::*>("int Class::*");
    test_swappable_with<int (Class::*)()>("int (Class::*)()");

    test_swappable_with<A>("A");
    test_swappable_with<B>("B");
    test_swappable_with<C>("C");
    test_swappable_with<D>("D");
    test_swappable_with<D_noexcept>("D_noexcept");
}
template <typename T, typename U>
void test_common_with(const std::string& name_type1, 
				   const std::string& name_type2)
{
    CHECK_TRUE(mtd::common_with<T, U> 
	    == std::common_with<T, U>, 
	    alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_common_with(const std::string& name_type)
{
    test_common_with<T, void>(name_type, "void");
    test_common_with<T, nullptr_t>(name_type, "nullptr_t");

    test_common_with<T, char>(name_type, "char");
    test_common_with<T, int>(name_type, "int");
    test_common_with<T, long>(name_type, "long");
    test_common_with<T, long long>(name_type, "long long");
    test_common_with<T, float>(name_type, "float");
    test_common_with<T, double>(name_type, "double");

    test_common_with<T, int[]>(name_type, "int[]");
    test_common_with<T, int[3]>(name_type, "int[3]");
    test_common_with<T, int[][3]>(name_type, "int[][3]");

    test_common_with<T, Class>(name_type, "Class");
    test_common_with<T, Class2>(name_type, "Class2");
    test_common_with<T, Union>(name_type, "Union");
    test_common_with<T, Enum>(name_type, "Enum");
    test_common_with<T, Enum_class>(name_type, "Enum_class");

    test_common_with<T, Class>(name_type, "Class");
    test_common_with<T, int Class::*>(name_type, "int Class::*");
    test_common_with<T, int (Class::*)()>(name_type, "int (Class::*)()");

    test_common_with<T, A>(name_type, "A");
    test_common_with<T, B>(name_type, "B");
    test_common_with<T, C>(name_type, "C");
    test_common_with<T, D>(name_type, "D");
    test_common_with<T, D_noexcept>(name_type, "D_noexcept");
}

void test_common_with()
{
    test::interface("common_with");
    
    test_common_with<void>("void");
    test_common_with<nullptr_t>("nullptr_t");

    test_common_with<char>("char");
    test_common_with<int>("int");
    test_common_with<long>("long");
    test_common_with<long long>("long long");
    // float y double da muchos warnings
//    test_common_with<float>("float");
//    test_common_with<double>("double");

    test_common_with<int[]>("int[]");
    test_common_with<int[3]>("int[3]");
    test_common_with<int[][3]>("int[][3]");

    test_common_with<Class>("Class");
    test_common_with<Class2>("Class2");
    test_common_with<Union>("Union");
    //test_common_with<Enum>("Enum");
    test_common_with<Enum_class>("Enum_class");

    test_common_with<Class>("Class");
    test_common_with<int Class::*>("int Class::*");
    test_common_with<int (Class::*)()>("int (Class::*)()");

    test_common_with<A>("A");
    test_common_with<B>("B");
    test_common_with<C>("C");
    test_common_with<D>("D");
    test_common_with<D_noexcept>("D_noexcept");
}

template <typename T, typename U>
void test_assignable_from(const std::string& name_type1, 
				   const std::string& name_type2)
{
    CHECK_TRUE(mtd::assignable_from<T, U> 
	    == std::assignable_from<T, U>, 
	    alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_assignable_from(const std::string& name_type)
{
    test_assignable_from<T, void>(name_type, "void");
    test_assignable_from<T, nullptr_t>(name_type, "nullptr_t");

    test_assignable_from<T, char>(name_type, "char");
    test_assignable_from<T, int>(name_type, "int");
    test_assignable_from<T, long>(name_type, "long");
    test_assignable_from<T, long long>(name_type, "long long");
    test_assignable_from<T, float>(name_type, "float");
    test_assignable_from<T, double>(name_type, "double");

    test_assignable_from<T, int[]>(name_type, "int[]");
    test_assignable_from<T, int[3]>(name_type, "int[3]");
    test_assignable_from<T, int[][3]>(name_type, "int[][3]");

    test_assignable_from<T, Class>(name_type, "Class");
    test_assignable_from<T, Class2>(name_type, "Class2");
    test_assignable_from<T, Union>(name_type, "Union");
    test_assignable_from<T, Enum>(name_type, "Enum");
    test_assignable_from<T, Enum_class>(name_type, "Enum_class");

    test_assignable_from<T, Class>(name_type, "Class");
    test_assignable_from<T, int Class::*>(name_type, "int Class::*");
    test_assignable_from<T, int (Class::*)()>(name_type, "int (Class::*)()");

    test_assignable_from<T, A>(name_type, "A");
    test_assignable_from<T, B>(name_type, "B");
    test_assignable_from<T, C>(name_type, "C");
    test_assignable_from<T, D>(name_type, "D");
    test_assignable_from<T, D_noexcept>(name_type, "D_noexcept");
}

void test_assignable_from()
{
    test::interface("assignable_from");
    
    test_assignable_from<void>("void");
    test_assignable_from<nullptr_t>("nullptr_t");

    test_assignable_from<char>("char");
    test_assignable_from<int>("int");
    test_assignable_from<long>("long");
    test_assignable_from<long long>("long long");
    // float y double da muchos warnings
//    test_assignable_from<float>("float");
//    test_assignable_from<double>("double");

    test_assignable_from<int[]>("int[]");
    test_assignable_from<int[3]>("int[3]");
    test_assignable_from<int[][3]>("int[][3]");

    test_assignable_from<Class>("Class");
    test_assignable_from<Class2>("Class2");
    test_assignable_from<Union>("Union");
    //test_assignable_from<Enum>("Enum");
    test_assignable_from<Enum_class>("Enum_class");

    test_assignable_from<Class>("Class");
    test_assignable_from<int Class::*>("int Class::*");
    test_assignable_from<int (Class::*)()>("int (Class::*)()");

    test_assignable_from<A>("A");
    test_assignable_from<B>("B");
    test_assignable_from<C>("C");
    test_assignable_from<D>("D");
    test_assignable_from<D_noexcept>("D_noexcept");
}


template <typename T>
void test_default_initializable(const std::string& name_type)
{
    CHECK_TRUE(mtd::default_initializable<T>
	    == std::default_initializable<T>, name_type );
}

void test_default_initializable()
{
    test::interface("default_initializable");
    
    test_default_initializable<void>("void");
    test_default_initializable<nullptr_t>("nullptr_t");

    test_default_initializable<char>("char");
    test_default_initializable<int>("int");
    test_default_initializable<long>("long");
    test_default_initializable<long long>("long long");
    // float y double da muchos warnings
//    test_default_initializable<float>("float");
//    test_default_initializable<double>("double");

    test_default_initializable<int[]>("int[]");
    test_default_initializable<int[3]>("int[3]");
    test_default_initializable<int[][3]>("int[][3]");

    test_default_initializable<Class>("Class");
    test_default_initializable<Class2>("Class2");
    test_default_initializable<Union>("Union");
    //test_default_initializable<Enum>("Enum");
    test_default_initializable<Enum_class>("Enum_class");

    test_default_initializable<Class>("Class");
    test_default_initializable<int Class::*>("int Class::*");
    test_default_initializable<int (Class::*)()>("int (Class::*)()");

    test_default_initializable<A>("A");
    test_default_initializable<B>("B");
    test_default_initializable<C>("C");
    test_default_initializable<D>("D");
    test_default_initializable<D_noexcept>("D_noexcept");
}

template <typename T>
void test_swappable(const std::string& name_type)
{
    CHECK_TRUE(mtd::swappable<T>
	    == std::swappable<T>, name_type );
}

void test_swappable()
{
    test::interface("swappable");
    
    test_swappable<void>("void");
    test_swappable<nullptr_t>("nullptr_t");

    test_swappable<char>("char");
    test_swappable<int>("int");
    test_swappable<long>("long");
    test_swappable<long long>("long long");
    // float y double da muchos warnings
//    test_swappable<float>("float");
//    test_swappable<double>("double");

    test_swappable<int[]>("int[]");
    test_swappable<int[3]>("int[3]");
    test_swappable<int[][3]>("int[][3]");

    test_swappable<Class>("Class");
    test_swappable<Class2>("Class2");
    test_swappable<Union>("Union");
    //test_swappable<Enum>("Enum");
    test_swappable<Enum_class>("Enum_class");

    test_swappable<Class>("Class");
    test_swappable<int Class::*>("int Class::*");
    test_swappable<int (Class::*)()>("int (Class::*)()");

    test_swappable<A>("A");
    test_swappable<B>("B");
    test_swappable<C>("C");
    test_swappable<D>("D");
    test_swappable<D_noexcept>("D_noexcept");
}

template <typename T>
void test_copy_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::move_constructible<T>
	    == std::move_constructible<T>, name_type );
}

void test_copy_constructible()
{
    test::interface("move_constructible");
    
    test_copy_constructible<void>("void");
    test_copy_constructible<nullptr_t>("nullptr_t");

    test_copy_constructible<char>("char");
    test_copy_constructible<int>("int");
    test_copy_constructible<long>("long");
    test_copy_constructible<long long>("long long");
    // float y double da muchos warnings
//    test_copy_constructible<float>("float");
//    test_copy_constructible<double>("double");

    test_copy_constructible<int[]>("int[]");
    test_copy_constructible<int[3]>("int[3]");
    test_copy_constructible<int[][3]>("int[][3]");

    test_copy_constructible<Class>("Class");
    test_copy_constructible<Class2>("Class2");
    test_copy_constructible<Union>("Union");
    //test_copy_constructible<Enum>("Enum");
    test_copy_constructible<Enum_class>("Enum_class");

    test_copy_constructible<Class>("Class");
    test_copy_constructible<int Class::*>("int Class::*");
    test_copy_constructible<int (Class::*)()>("int (Class::*)()");

    test_copy_constructible<A>("A");
    test_copy_constructible<B>("B");
    test_copy_constructible<C>("C");
    test_copy_constructible<D>("D");
    test_copy_constructible<D_noexcept>("D_noexcept");
}


template <typename T>
void test_semiregular(const std::string& name_type)
{
    CHECK_TRUE(mtd::semiregular<T>
	    == std::semiregular<T>, name_type );
}

void test_semiregular()
{
    test::interface("semiregular");
    
    test_semiregular<void>("void");
    test_semiregular<nullptr_t>("nullptr_t");

    test_semiregular<char>("char");
    test_semiregular<int>("int");
    test_semiregular<long>("long");
    test_semiregular<long long>("long long");
    // float y double da muchos warnings
//    test_semiregular<float>("float");
//    test_semiregular<double>("double");

    test_semiregular<int[]>("int[]");
    test_semiregular<int[3]>("int[3]");
    test_semiregular<int[][3]>("int[][3]");

    test_semiregular<Class>("Class");
    test_semiregular<Class2>("Class2");
    test_semiregular<Union>("Union");
    //test_semiregular<Enum>("Enum");
    test_semiregular<Enum_class>("Enum_class");

    test_semiregular<Class>("Class");
    test_semiregular<int Class::*>("int Class::*");
    test_semiregular<int (Class::*)()>("int (Class::*)()");

    test_semiregular<A>("A");
    test_semiregular<B>("B");
    test_semiregular<C>("C");
    test_semiregular<D>("D");
    test_semiregular<D_noexcept>("D_noexcept");
}

template <typename T>
void test_regular(const std::string& name_type)
{
    CHECK_TRUE(mtd::regular<T>
	    == std::regular<T>, name_type );
}

void test_regular()
{
    test::interface("regular");
    
    test_regular<void>("void");
    test_regular<nullptr_t>("nullptr_t");

    test_regular<char>("char");
    test_regular<int>("int");
    test_regular<long>("long");
    test_regular<long long>("long long");
    // float y double da muchos warnings
//    test_regular<float>("float");
//    test_regular<double>("double");

    test_regular<int[]>("int[]");
    test_regular<int[3]>("int[3]");
    test_regular<int[][3]>("int[][3]");

    test_regular<Class>("Class");
    test_regular<Class2>("Class2");
    test_regular<Union>("Union");
    //test_regular<Enum>("Enum");
    test_regular<Enum_class>("Enum_class");

    test_regular<Class>("Class");
    test_regular<int Class::*>("int Class::*");
    test_regular<int (Class::*)()>("int (Class::*)()");

    test_regular<A>("A");
    test_regular<B>("B");
    test_regular<C>("C");
    test_regular<D>("D");
    test_regular<D_noexcept>("D_noexcept");
}

template <typename T>
void test_copyable(const std::string& name_type)
{
    CHECK_TRUE(mtd::copyable<T>
	    == std::copyable<T>, name_type );
}

void test_copyable()
{
    test::interface("copyable");
    
    test_copyable<void>("void");
    test_copyable<nullptr_t>("nullptr_t");

    test_copyable<char>("char");
    test_copyable<int>("int");
    test_copyable<long>("long");
    test_copyable<long long>("long long");
    // float y double da muchos warnings
//    test_copyable<float>("float");
//    test_copyable<double>("double");

    test_copyable<int[]>("int[]");
    test_copyable<int[3]>("int[3]");
    test_copyable<int[][3]>("int[][3]");

    test_copyable<Class>("Class");
    test_copyable<Class2>("Class2");
    test_copyable<Union>("Union");
    //test_copyable<Enum>("Enum");
    test_copyable<Enum_class>("Enum_class");

    test_copyable<Class>("Class");
    test_copyable<int Class::*>("int Class::*");
    test_copyable<int (Class::*)()>("int (Class::*)()");

    test_copyable<A>("A");
    test_copyable<B>("B");
    test_copyable<C>("C");
    test_copyable<D>("D");
    test_copyable<D_noexcept>("D_noexcept");
}

template <typename T>
void test_movable(const std::string& name_type)
{
    CHECK_TRUE(mtd::movable<T>
	    == std::movable<T>, name_type );
}

void test_movable()
{
    test::interface("movable");
    
    test_movable<void>("void");
    test_movable<nullptr_t>("nullptr_t");

    test_movable<char>("char");
    test_movable<int>("int");
    test_movable<long>("long");
    test_movable<long long>("long long");
    // float y double da muchos warnings
//    test_movable<float>("float");
//    test_movable<double>("double");

    test_movable<int[]>("int[]");
    test_movable<int[3]>("int[3]");
    test_movable<int[][3]>("int[][3]");

    test_movable<Class>("Class");
    test_movable<Class2>("Class2");
    test_movable<Union>("Union");
    //test_movable<Enum>("Enum");
    test_movable<Enum_class>("Enum_class");

    test_movable<Class>("Class");
    test_movable<int Class::*>("int Class::*");
    test_movable<int (Class::*)()>("int (Class::*)()");

    test_movable<A>("A");
    test_movable<B>("B");
    test_movable<C>("C");
    test_movable<D>("D");
    test_movable<D_noexcept>("D_noexcept");
}


template <typename T>
void test_move_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::move_constructible<T>
	    == std::move_constructible<T>, name_type );
}

void test_move_constructible()
{
    test::interface("move_constructible");
    
    test_move_constructible<void>("void");
    test_move_constructible<nullptr_t>("nullptr_t");

    test_move_constructible<char>("char");
    test_move_constructible<int>("int");
    test_move_constructible<long>("long");
    test_move_constructible<long long>("long long");
    // float y double da muchos warnings
//    test_move_constructible<float>("float");
//    test_move_constructible<double>("double");

    test_move_constructible<int[]>("int[]");
    test_move_constructible<int[3]>("int[3]");
    test_move_constructible<int[][3]>("int[][3]");

    test_move_constructible<Class>("Class");
    test_move_constructible<Class2>("Class2");
    test_move_constructible<Union>("Union");
    //test_move_constructible<Enum>("Enum");
    test_move_constructible<Enum_class>("Enum_class");

    test_move_constructible<Class>("Class");
    test_move_constructible<int Class::*>("int Class::*");
    test_move_constructible<int (Class::*)()>("int (Class::*)()");

    test_move_constructible<A>("A");
    test_move_constructible<B>("B");
    test_move_constructible<C>("C");
    test_move_constructible<D>("D");
    test_move_constructible<D_noexcept>("D_noexcept");
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
    test_common_with();


    // arithmetic concepts
    // -------------------
    test_integral();
    test_signed_integral();
    test_unsigned_integral();
    test_assignable_from();
    test_swappable();
    test_swappable_with();
    test_copy_constructible();
    // test_destructible(); 
    // test_constructible_from();
    test_default_initializable();
    test_move_constructible();
    
    // comparison concepts
    // -------------------
    test_equality_comparable();
    test_equality_comparable_with();
    test_totally_ordered();
    test_totally_ordered_with();

    // object concepts
    // ---------------
    test_movable();
    test_copyable();
    test_semiregular();
    test_regular();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}




