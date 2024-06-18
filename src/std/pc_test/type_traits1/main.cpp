// Copyright (C) 2019-2023 Manuel Perez 
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

//
// TODO: estoy repitiendo el mismo test continuamente: ejecuto la función de
// mtd y la de std en distintos tipos comparando el resultado. ¿cómo
// generalizarlo?:
//
// Tipos básicos a probar:
// void
// char, int, long, long long, float, double
// Modificadores: char*, char&, char&&
// Modificadores: const char, volatile char
// Arrays: int[3], int[], int[][3]
// Clases: Class, Union, Enum, Enum_class
//         Class::*
//
//Ejemplo:
//void test_is_trivial()
//{
//    test::interface("is_trivial");
//
//    test_is_trivial<void>("void");
//    test_is_trivial<nullptr_t>("nullptr_t");
//
//    test_is_trivial<char>("char");
//    test_is_trivial<int>("int");
//    test_is_trivial<long>("long");
//    test_is_trivial<long long>("long long");
//    test_is_trivial<float>("float");
//    test_is_trivial<double>("double");
//
//    test_is_trivial<int[]>("int[]");
//    test_is_trivial<int[3]>("int[3]");
//    test_is_trivial<int[][3]>("int[][3]");
//
//    test_is_trivial<std::string>("std::string");
//
//    test_is_trivial<Class>("Class");
//    test_is_trivial<Class2>("Class2");
//    test_is_trivial<Union>("Union");
//    test_is_trivial<Enum>("Enum");
//    test_is_trivial<Enum_class>("Enum_class");
//
//    test_is_trivial<Class>("Class");
//    test_is_trivial<int Class::*>("int Class::*");
//    test_is_trivial<int (Class::*)()>("int (Class::*)()");
//}


#include "../../std_type_traits.h"
#include <iostream>
#define LOG (std::cout << __PRETTY_FUNCTION__ << '\n')

#include <alp_test.h>
#include <alp_string.h>

#include <typeinfo>

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




void test_is_integral()
{
    test::interface("is_integral");

    // is_integral
    CHECK_TRUE(	mtd::is_integral<bool>::value ==
		std::is_integral<bool>::value, "is_integral<bool>");
    CHECK_TRUE(	mtd::is_integral<const bool>::value ==
		std::is_integral<const bool>::value, "is_integral<const bool>");
    CHECK_TRUE(	mtd::is_integral<volatile bool>::value ==
		std::is_integral<volatile bool>::value, "is_integral<volatile bool>");
    CHECK_TRUE(	mtd::is_integral<char>::value ==
		std::is_integral<char>::value, "is_integral<char>");
    CHECK_TRUE(	mtd::is_integral<const char>::value ==
		std::is_integral<const char>::value, "is_integral<const char>");
    CHECK_TRUE(	mtd::is_integral<volatile char>::value ==
		std::is_integral<volatile char>::value, "is_integral<volatile char>");
    CHECK_TRUE(	mtd::is_integral<signed char>::value ==
		std::is_integral<signed char>::value, "is_integral<signed char>");
    CHECK_TRUE(	mtd::is_integral<const signed char>::value ==
		std::is_integral<const signed char>::value, "is_integral<const signed char>");
    CHECK_TRUE(	mtd::is_integral<volatile signed char>::value ==
		std::is_integral<volatile signed char>::value, "is_integral<volatile signed char>");
    CHECK_TRUE(	mtd::is_integral<unsigned char>::value ==
		std::is_integral<unsigned char>::value, "is_integral<unsigned char>");
    CHECK_TRUE(	mtd::is_integral<const unsigned char>::value ==
		std::is_integral<const unsigned char>::value, "is_integral<const unsigned char>");
    CHECK_TRUE(	mtd::is_integral<volatile unsigned char>::value ==
		std::is_integral<volatile unsigned char>::value, "is_integral<volatile unsigned char>");
    CHECK_TRUE(	mtd::is_integral<char16_t>::value ==
		std::is_integral<char16_t>::value, "is_integral<char16_t>");
    CHECK_TRUE(	mtd::is_integral<const char16_t>::value ==
		std::is_integral<const char16_t>::value, "is_integral<const char16_t>");
    CHECK_TRUE(	mtd::is_integral<volatile char16_t>::value ==
		std::is_integral<volatile char16_t>::value, "is_integral<volatile char16_t>");
    CHECK_TRUE(	mtd::is_integral<char32_t>::value ==
		std::is_integral<char32_t>::value, "is_integral<char32_t>");
    CHECK_TRUE(	mtd::is_integral<const char32_t>::value ==
		std::is_integral<const char32_t>::value, "is_integral<const char32_t>");
    CHECK_TRUE(	mtd::is_integral<volatile char32_t>::value ==
		std::is_integral<volatile char32_t>::value, "is_integral<volatile char32_t>");
    CHECK_TRUE(	mtd::is_integral<short>::value ==
		std::is_integral<short>::value, "is_integral<short>");
    CHECK_TRUE(	mtd::is_integral<const short>::value ==
		std::is_integral<const short>::value, "is_integral<const short>");
    CHECK_TRUE(	mtd::is_integral<volatile short>::value ==
		std::is_integral<volatile short>::value, "is_integral<volatile short>");
    CHECK_TRUE(	mtd::is_integral<unsigned short>::value ==
		std::is_integral<unsigned short>::value, "is_integral<unsigned short>");
    CHECK_TRUE(	mtd::is_integral<const unsigned short>::value ==
		std::is_integral<const unsigned short>::value, "is_integral<const unsigned short>");
    CHECK_TRUE(	mtd::is_integral<volatile unsigned short>::value ==
		std::is_integral<volatile unsigned short>::value, "is_integral<volatile unsigned short>");
    CHECK_TRUE(	mtd::is_integral<int>::value ==
		std::is_integral<int>::value, "is_integral<int>");
    CHECK_TRUE(	mtd::is_integral<const int>::value ==
		std::is_integral<const int>::value, "is_integral<const int>");
    CHECK_TRUE(	mtd::is_integral<volatile int>::value ==
		std::is_integral<volatile int>::value, "is_integral<volatile int>");
    CHECK_TRUE(	mtd::is_integral<unsigned int>::value ==
		std::is_integral<unsigned int>::value, "is_integral<unsigned int>");
    CHECK_TRUE(	mtd::is_integral<const unsigned int>::value ==
		std::is_integral<const unsigned int>::value, "is_integral<const unsigned int>");
    CHECK_TRUE(	mtd::is_integral<volatile unsigned int>::value ==
		std::is_integral<volatile unsigned int>::value, "is_integral<volatile unsigned int>");
    CHECK_TRUE(	mtd::is_integral<long>::value ==
		std::is_integral<long>::value, "is_integral<long>");
    CHECK_TRUE(	mtd::is_integral<const long>::value ==
		std::is_integral<const long>::value, "is_integral<const long>");
    CHECK_TRUE(	mtd::is_integral<volatile long>::value ==
		std::is_integral<volatile long>::value, "is_integral<volatile long>");
    CHECK_TRUE(	mtd::is_integral<unsigned long>::value ==
		std::is_integral<unsigned long>::value, "is_integral<unsigned long>");
    CHECK_TRUE(	mtd::is_integral<const unsigned long>::value ==
		std::is_integral<const unsigned long>::value, "is_integral<const unsigned long>");
    CHECK_TRUE(	mtd::is_integral<volatile unsigned long>::value ==
		std::is_integral<volatile unsigned long>::value, "is_integral<volatile unsigned long>");
    CHECK_TRUE(	mtd::is_integral<long long>::value ==
		std::is_integral<long long>::value, "is_integral<long long>");
    CHECK_TRUE(	mtd::is_integral<const long long>::value ==
		std::is_integral<const long long>::value, "is_integral<const long long>");
    CHECK_TRUE(	mtd::is_integral<volatile long long>::value ==
		std::is_integral<volatile long long>::value, "is_integral<volatile long long>");
    CHECK_TRUE(	mtd::is_integral<unsigned long long>::value ==
		std::is_integral<unsigned long long>::value, "is_integral<unsigned long long>");
    CHECK_TRUE(	mtd::is_integral<const unsigned long long>::value ==
		std::is_integral<const unsigned long long>::value, "is_integral<const unsigned long long>");
    CHECK_TRUE(	mtd::is_integral<volatile unsigned long long>::value ==
		std::is_integral<volatile unsigned long long>::value, "is_integral<volatile unsigned long long>");

    CHECK_TRUE(	mtd::is_integral<float>::value ==
		std::is_integral<float>::value, "is_integral<float>");
    CHECK_TRUE(	mtd::is_integral<const float>::value ==
		std::is_integral<const float>::value, "is_integral<const float>");
    CHECK_TRUE(	mtd::is_integral<volatile float>::value ==
		std::is_integral<volatile float>::value, "is_integral<volatile float>");
    CHECK_TRUE(	mtd::is_integral<double>::value ==
		std::is_integral<double>::value, "is_integral<double>");
    CHECK_TRUE(	mtd::is_integral<const double>::value ==
		std::is_integral<const double>::value, "is_integral<const double>");
    CHECK_TRUE(	mtd::is_integral<volatile double>::value ==
		std::is_integral<volatile double>::value, "is_integral<volatile double>");
    CHECK_TRUE(	mtd::is_integral<Class>::value ==
		std::is_integral<Class>::value, "is_integral<Class>");
    CHECK_TRUE(	mtd::is_integral<const Class>::value ==
		std::is_integral<const Class>::value, "is_integral<const Class>");
    CHECK_TRUE(	mtd::is_integral<volatile Class>::value ==
		std::is_integral<volatile Class>::value, "is_integral<volatile Class>");

    CHECK_TRUE(	mtd::is_integral_v<int> ==
		std::is_integral_v<int>, "is_integral_v<int>");
    CHECK_TRUE(	mtd::is_integral_v<Class> ==
		std::is_integral_v<Class>, "is_integral_v<Class>");
}

void test_is_floating_point()
{
    test::interface("is_floating_point");
    CHECK_TRUE(	mtd::is_floating_point<float>::value ==
		std::is_floating_point<float>::value, "is_floating_point<float>");
    CHECK_TRUE(	mtd::is_floating_point<const float>::value ==
		std::is_floating_point<const float>::value, "is_floating_point<const float>");
    CHECK_TRUE(	mtd::is_floating_point<volatile float>::value ==
		std::is_floating_point<volatile float>::value, "is_floating_point<volatile float>");
    CHECK_TRUE(	mtd::is_floating_point<double>::value ==
		std::is_floating_point<double>::value, "is_floating_point<double>");
    CHECK_TRUE(	mtd::is_floating_point<const double>::value ==
		std::is_floating_point<const double>::value, "is_floating_point<const double>");
    CHECK_TRUE(	mtd::is_floating_point<volatile double>::value ==
		std::is_floating_point<volatile double>::value, "is_floating_point<volatile double>");
    CHECK_TRUE(	mtd::is_floating_point<long double>::value ==
		std::is_floating_point<long double>::value, "is_floating_point<long double>");
    CHECK_TRUE(	mtd::is_floating_point<const long double>::value ==
		std::is_floating_point<const long double>::value, "is_floating_point<const long double>");
    CHECK_TRUE(	mtd::is_floating_point<volatile long double>::value ==
		std::is_floating_point<volatile long double>::value, "is_floating_point<volatile long double>");

    CHECK_TRUE(	mtd::is_floating_point<int>::value ==
		std::is_floating_point<int>::value, "is_floating_point<int>");
    CHECK_TRUE(	mtd::is_floating_point<Class>::value ==
		std::is_floating_point<Class>::value, "is_floating_point<Class>");

    CHECK_TRUE(	mtd::is_floating_point_v<long double> ==
		std::is_floating_point_v<long double>, "is_floating_point_v<long double>");
    CHECK_TRUE(	mtd::is_floating_point_v<char> ==
		std::is_floating_point_v<char>, "is_floating_point_v<char>");
}

template <typename T>
void test_is_arithmetic(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_arithmetic_v<T> == 
		std::is_arithmetic_v<T>, name_type);
    CHECK_TRUE(	mtd::is_arithmetic_v<T&> == 
		std::is_arithmetic_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_arithmetic_v<T&&> == 
		std::is_arithmetic_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_arithmetic_v<const T> == 
		std::is_arithmetic_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_arithmetic_v<const T&> == 
		std::is_arithmetic_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_arithmetic_v<const T&&> == 
		std::is_arithmetic_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_arithmetic_v<volatile T> == 
		std::is_arithmetic_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_arithmetic_v<volatile T&> == 
		std::is_arithmetic_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_arithmetic_v<volatile T&&> == 
		std::is_arithmetic_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");
}


void test_is_arithmetic()
{
    test::interface("is_arithmetic");

    test_is_arithmetic<char>("char");
    test_is_arithmetic<int>("int");
    test_is_arithmetic<long>("long");
    test_is_arithmetic<long long>("long long");
    test_is_arithmetic<float>("float");
    test_is_arithmetic<double>("double");

    test_is_arithmetic<Class>("Class");
    test_is_arithmetic<Union>("Union");
    test_is_arithmetic<Enum>("Enum");
    test_is_arithmetic<Enum_class>("Enum_class");
}


template <typename T>
void test_is_fundamental(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_fundamental_v<T> == 
		std::is_fundamental_v<T>, name_type);
    CHECK_TRUE(	mtd::is_fundamental_v<const T> == 
		std::is_fundamental_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_fundamental_v<volatile T> == 
		std::is_fundamental_v<volatile T>, alp::as_str() << "volatile " << name_type);
}


void test_is_fundamental()
{
    test::interface("is_fundamental");

    test_is_fundamental<void>("void");
    test_is_fundamental<nullptr_t>("nullptr_t");

    test_is_fundamental<char>("char");
    test_is_fundamental<int>("int");
    test_is_fundamental<long>("long");
    test_is_fundamental<long long>("long long");
    test_is_fundamental<float>("float");
    test_is_fundamental<double>("double");

    test_is_fundamental<Class>("Class");
    test_is_fundamental<Union>("Union");
    test_is_fundamental<Enum>("Enum");
    test_is_fundamental<Enum_class>("Enum_class");
}

void test_is_signed()
{
    test::interface("is_signed");

    CHECK_TRUE(mtd::is_signed<signed int>::value ==
		std::is_signed<signed int>::value, "is_signed<signed int>");
    CHECK_TRUE(mtd::is_signed<unsigned int>::value ==
		std::is_signed<unsigned int>::value, "is_signed<unsigned int>");

    CHECK_TRUE(mtd::is_signed_v<signed int> ==
		std::is_signed_v<signed int>, "is_signed_v<signed int>");
    CHECK_TRUE(mtd::is_signed_v<unsigned int> ==
		std::is_signed_v<unsigned int>, "is_signed_v<unsigned int>");
}

void test_is_unsigned()
{
    test::interface("is_unsigned");

    CHECK_TRUE(mtd::is_unsigned<signed int>::value ==
		std::is_unsigned<signed int>::value, "is_unsigned<signed int>");
    CHECK_TRUE(mtd::is_unsigned<unsigned int>::value ==
		std::is_unsigned<unsigned int>::value, "is_unsigned<unsigned int>");

    CHECK_TRUE(mtd::is_unsigned_v<signed int> ==
		std::is_unsigned_v<signed int>, "is_unsigned_v<signed int>");
    CHECK_TRUE(mtd::is_unsigned_v<unsigned int> ==
		std::is_unsigned_v<unsigned int>, "is_unsigned_v<unsigned int>");

}

void test_conditional()
{
    test::interface("conditional_t");

    CHECK_TRUE((mtd::is_same_v<	mtd::conditional_t<true, int, long>, 
				std::conditional_t<true, int, long>>), 
		"true");
    CHECK_TRUE((mtd::is_same_v<	mtd::conditional_t<false, int, long>, 
				std::conditional_t<false, int, long>>), 
		"false");
    CHECK_TRUE((mtd::is_same_v<	mtd::conditional_t<true, long, int>, 
				std::conditional_t<true, long, int>>), 
		"true");
    CHECK_TRUE((mtd::is_same_v<	mtd::conditional_t<false, long, int>, 
				std::conditional_t<false, long, int>>), 
		"false");
    CHECK_TRUE((mtd::is_same_v<	mtd::conditional_t<true, int, int>, 
				std::conditional_t<true, int, int>>), 
		"degenerado");
}




template <typename T>
void test_is_trivial(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivial_v<T> ==
	       std::is_trivial_v<T>, 
	       alp::as_str() << "is_trivial(" << name_type << ")");
}

void test_is_trivial()
{
    test::interface("is_trivial");

    test_is_trivial<void>("void");
    test_is_trivial<nullptr_t>("nullptr_t");

    test_is_trivial<char>("char");
    test_is_trivial<int>("int");
    test_is_trivial<long>("long");
    test_is_trivial<long long>("long long");
    test_is_trivial<float>("float");
    test_is_trivial<double>("double");

    test_is_trivial<int[]>("int[]");
    test_is_trivial<int[3]>("int[3]");

    test_is_trivial<Class>("Class");
    test_is_trivial<Union>("Union");
    test_is_trivial<Enum>("Enum");
    test_is_trivial<Enum_class>("Enum_class");

    test_is_trivial<Class>("Class");
    test_is_trivial<Class2>("Class2");
    test_is_trivial<int Class::*>("int Class::*");
    test_is_trivial<int (Class::*)()>("int (Class::*)()");
}


template <typename T>
void test_is_trivially_copyable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_copyable_v<T> ==
	       std::is_trivially_copyable_v<T>, 
	       alp::as_str() << "is_trivially_copyable(" << name_type << ")");
}

void test_is_trivially_copyable()
{
    test::interface("is_trivially_copyable");

    test_is_trivially_copyable<void>("void");
    test_is_trivially_copyable<nullptr_t>("nullptr_t");

    test_is_trivially_copyable<char>("char");
    test_is_trivially_copyable<int>("int");
    test_is_trivially_copyable<long>("long");
    test_is_trivially_copyable<long long>("long long");
    test_is_trivially_copyable<float>("float");
    test_is_trivially_copyable<double>("double");

    test_is_trivially_copyable<int[]>("int[]");
    test_is_trivially_copyable<int[3]>("int[3]");

    test_is_trivially_copyable<Class>("Class");
    test_is_trivially_copyable<Union>("Union");
    test_is_trivially_copyable<Enum>("Enum");
    test_is_trivially_copyable<Enum_class>("Enum_class");

    test_is_trivially_copyable<Class>("Class");
    test_is_trivially_copyable<Class2>("Class2");
    test_is_trivially_copyable<int Class::*>("int Class::*");
    test_is_trivially_copyable<int (Class::*)()>("int (Class::*)()");
}



template <typename T>
void test_is_standard_layout(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_standard_layout_v<T> ==
	       std::is_standard_layout_v<T>, 
	       alp::as_str() << "is_standard_layout(" << name_type << ")");
}

void test_is_standard_layout()
{
    test::interface("is_standard_layout");

    test_is_standard_layout<void>("void");
    test_is_standard_layout<nullptr_t>("nullptr_t");

    test_is_standard_layout<char>("char");
    test_is_standard_layout<int>("int");
    test_is_standard_layout<long>("long");
    test_is_standard_layout<long long>("long long");
    test_is_standard_layout<float>("float");
    test_is_standard_layout<double>("double");

    test_is_standard_layout<int[]>("int[]");
    test_is_standard_layout<int[3]>("int[3]");

    test_is_standard_layout<Class>("Class");
    test_is_standard_layout<Union>("Union");
    test_is_standard_layout<Enum>("Enum");
    test_is_standard_layout<Enum_class>("Enum_class");

    test_is_standard_layout<Class>("Class");
    test_is_standard_layout<Class2>("Class2");
    test_is_standard_layout<int Class::*>("int Class::*");
    test_is_standard_layout<int (Class::*)()>("int (Class::*)()");

    test_is_standard_layout<A>("A");
    test_is_standard_layout<B>("B");
    test_is_standard_layout<C>("C");
}


template <typename T>
void test_is_empty(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_empty_v<T> ==
	       std::is_empty_v<T>, 
	       alp::as_str() << "is_empty(" << name_type << ")");
}

void test_is_empty()
{
    test::interface("is_empty");

    test_is_empty<void>("void");
    test_is_empty<nullptr_t>("nullptr_t");

    test_is_empty<char>("char");
    test_is_empty<int>("int");
    test_is_empty<long>("long");
    test_is_empty<long long>("long long");
    test_is_empty<float>("float");
    test_is_empty<double>("double");

    test_is_empty<int[]>("int[]");
    test_is_empty<int[3]>("int[3]");

    test_is_empty<Class>("Class");
    test_is_empty<Union>("Union");
    test_is_empty<Enum>("Enum");
    test_is_empty<Enum_class>("Enum_class");

    test_is_empty<Class>("Class");
    test_is_empty<Class2>("Class2");
    test_is_empty<int Class::*>("int Class::*");
    test_is_empty<int (Class::*)()>("int (Class::*)()");

    test_is_empty<A>("A");
    test_is_empty<B>("B");
    test_is_empty<C>("C");
}


template <typename T>
void test_is_polymorphic(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_polymorphic_v<T> ==
	       std::is_polymorphic_v<T>, 
	       alp::as_str() << "is_polymorphic(" << name_type << ")");
}

void test_is_polymorphic()
{
    test::interface("is_polymorphic");

    test_is_polymorphic<void>("void");
    test_is_polymorphic<nullptr_t>("nullptr_t");

    test_is_polymorphic<char>("char");
    test_is_polymorphic<int>("int");
    test_is_polymorphic<long>("long");
    test_is_polymorphic<long long>("long long");
    test_is_polymorphic<float>("float");
    test_is_polymorphic<double>("double");

    test_is_polymorphic<int[]>("int[]");
    test_is_polymorphic<int[3]>("int[3]");

    test_is_polymorphic<Class>("Class");
    test_is_polymorphic<Union>("Union");
    test_is_polymorphic<Enum>("Enum");
    test_is_polymorphic<Enum_class>("Enum_class");

    test_is_polymorphic<Class>("Class");
    test_is_polymorphic<Class2>("Class2");
    test_is_polymorphic<int Class::*>("int Class::*");
    test_is_polymorphic<int (Class::*)()>("int (Class::*)()");

    test_is_polymorphic<A>("A");
    test_is_polymorphic<B>("B");
    test_is_polymorphic<C>("C");
}

template <typename T>
void test_is_abstract(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_abstract_v<T> ==
	       std::is_abstract_v<T>, 
	       alp::as_str() << "is_abstract(" << name_type << ")");
}

void test_is_abstract()
{
    test::interface("is_abstract");

    test_is_abstract<void>("void");
    test_is_abstract<nullptr_t>("nullptr_t");

    test_is_abstract<char>("char");
    test_is_abstract<int>("int");
    test_is_abstract<long>("long");
    test_is_abstract<long long>("long long");
    test_is_abstract<float>("float");
    test_is_abstract<double>("double");

    test_is_abstract<int[]>("int[]");
    test_is_abstract<int[3]>("int[3]");

    test_is_abstract<Class>("Class");
    test_is_abstract<Union>("Union");
    test_is_abstract<Enum>("Enum");
    test_is_abstract<Enum_class>("Enum_class");

    test_is_abstract<Class>("Class");
    test_is_abstract<Class2>("Class2");
    test_is_abstract<int Class::*>("int Class::*");
    test_is_abstract<int (Class::*)()>("int (Class::*)()");

    test_is_abstract<A>("A");
    test_is_abstract<B>("B");
    test_is_abstract<C>("C");
}

template <typename T>
void test_is_final(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_final_v<T> ==
	       std::is_final_v<T>, 
	       alp::as_str() << "is_final(" << name_type << ")");
}

void test_is_final()
{
    test::interface("is_final");

    test_is_final<void>("void");
    test_is_final<nullptr_t>("nullptr_t");

    test_is_final<char>("char");
    test_is_final<int>("int");
    test_is_final<long>("long");
    test_is_final<long long>("long long");
    test_is_final<float>("float");
    test_is_final<double>("double");

    test_is_final<int[]>("int[]");
    test_is_final<int[3]>("int[3]");

    test_is_final<Class>("Class");
    test_is_final<Union>("Union");
    test_is_final<Enum>("Enum");
    test_is_final<Enum_class>("Enum_class");

    test_is_final<Class>("Class");
    test_is_final<Class2>("Class2");
    test_is_final<int Class::*>("int Class::*");
    test_is_final<int (Class::*)()>("int (Class::*)()");

    test_is_final<A>("A");
    test_is_final<B>("B");
    test_is_final<C>("C");
}

template <typename T>
void test_is_aggregate(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_aggregate_v<T> ==
	       std::is_aggregate_v<T>, 
	       alp::as_str() << "is_aggregate(" << name_type << ")");
}

void test_is_aggregate()
{
    test::interface("is_aggregate");

    test_is_aggregate<void>("void");
    test_is_aggregate<nullptr_t>("nullptr_t");

    test_is_aggregate<char>("char");
    test_is_aggregate<int>("int");
    test_is_aggregate<long>("long");
    test_is_aggregate<long long>("long long");
    test_is_aggregate<float>("float");
    test_is_aggregate<double>("double");

    test_is_aggregate<int[]>("int[]");
    test_is_aggregate<int[3]>("int[3]");

    test_is_aggregate<Class>("Class");
    test_is_aggregate<Union>("Union");
    test_is_aggregate<Enum>("Enum");
    test_is_aggregate<Enum_class>("Enum_class");

    test_is_aggregate<Class>("Class");
    test_is_aggregate<Class2>("Class2");
    test_is_aggregate<int Class::*>("int Class::*");
    test_is_aggregate<int (Class::*)()>("int (Class::*)()");

    test_is_aggregate<A>("A");
    test_is_aggregate<B>("B");
    test_is_aggregate<C>("C");
}

template <typename T>
void test_is_bounded_array(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_bounded_array_v<T> ==
	       std::is_bounded_array_v<T>, 
	       alp::as_str() << "is_bounded_array(" << name_type << ")");
}

void test_is_bounded_array()
{
    test::interface("is_bounded_array");

    test_is_bounded_array<void>("void");
    test_is_bounded_array<nullptr_t>("nullptr_t");

    test_is_bounded_array<char>("char");
    test_is_bounded_array<int>("int");
    test_is_bounded_array<long>("long");
    test_is_bounded_array<long long>("long long");
    test_is_bounded_array<float>("float");
    test_is_bounded_array<double>("double");

    test_is_bounded_array<int[]>("int[]");
    test_is_bounded_array<int[3]>("int[3]");
    test_is_bounded_array<int[][3]>("int[][3]");

    test_is_bounded_array<Class>("Class");
    test_is_bounded_array<Union>("Union");
    test_is_bounded_array<Enum>("Enum");
    test_is_bounded_array<Enum_class>("Enum_class");

    test_is_bounded_array<Class>("Class");
    test_is_bounded_array<Class2>("Class2");
    test_is_bounded_array<int Class::*>("int Class::*");
    test_is_bounded_array<int (Class::*)()>("int (Class::*)()");

    test_is_bounded_array<A>("A");
    test_is_bounded_array<B>("B");
    test_is_bounded_array<C>("C");
}

template <typename T>
void test_is_unbounded_array(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_unbounded_array_v<T> ==
	       std::is_unbounded_array_v<T>, 
	       alp::as_str() << "is_unbounded_array(" << name_type << ")");
}

void test_is_unbounded_array()
{
    test::interface("is_unbounded_array");

    test_is_unbounded_array<void>("void");
    test_is_unbounded_array<nullptr_t>("nullptr_t");

    test_is_unbounded_array<char>("char");
    test_is_unbounded_array<int>("int");
    test_is_unbounded_array<long>("long");
    test_is_unbounded_array<long long>("long long");
    test_is_unbounded_array<float>("float");
    test_is_unbounded_array<double>("double");

    test_is_unbounded_array<int[]>("int[]");
    test_is_unbounded_array<int[3]>("int[3]");
    test_is_unbounded_array<int[][3]>("int[][3]");

    test_is_unbounded_array<Class>("Class");
    test_is_unbounded_array<Union>("Union");
    test_is_unbounded_array<Enum>("Enum");
    test_is_unbounded_array<Enum_class>("Enum_class");

    test_is_unbounded_array<Class>("Class");
    test_is_unbounded_array<Class2>("Class2");
    test_is_unbounded_array<int Class::*>("int Class::*");
    test_is_unbounded_array<int (Class::*)()>("int (Class::*)()");

    test_is_unbounded_array<A>("A");
    test_is_unbounded_array<B>("B");
    test_is_unbounded_array<C>("C");
}



template <typename T, typename... Args>
void test_is_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_constructible_v<T, Args...> ==
	       std::is_constructible_v<T, Args...>, 
	       alp::as_str() << "is_constructible(" << name_type << ")");
}

void test_is_constructible()
{
    test::interface("is_constructible");

    test_is_constructible<void>("void");
    test_is_constructible<nullptr_t>("nullptr_t");

    test_is_constructible<char>("char");
    test_is_constructible<int>("int");
    test_is_constructible<long>("long");
    test_is_constructible<long long>("long long");
    test_is_constructible<float>("float");
    test_is_constructible<double>("double");

    test_is_constructible<int[]>("int[]");
    test_is_constructible<int[3]>("int[3]");
    test_is_constructible<int[][3]>("int[][3]");

    test_is_constructible<Class>("Class");
    test_is_constructible<Union>("Union");
    test_is_constructible<Enum>("Enum");
    test_is_constructible<Enum_class>("Enum_class");

    test_is_constructible<Class>("Class");
    test_is_constructible<Class2>("Class2");
    test_is_constructible<int Class::*>("int Class::*");
    test_is_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_constructible<A>("A");
    test_is_constructible<B>("B");
    test_is_constructible<C>("C");

    test_is_constructible<D, int>("D, int");
    test_is_constructible<D_noexcept, int>("D_noexcept, int");
    test_is_constructible<D, int, float>("D, int, float");
    test_is_constructible<D_noexcept, int, float>("D_noexcept, int, float");


}

template <typename T>
void test_is_default_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_default_constructible_v<T> ==
	       std::is_default_constructible_v<T>, 
	       alp::as_str() << "is_default_constructible(" << name_type << ")");
}

void test_is_default_constructible()
{
    test::interface("is_default_constructible");

    test_is_default_constructible<void>("void");
    test_is_default_constructible<const volatile void>("const volatile void");
    test_is_default_constructible<nullptr_t>("nullptr_t");

    test_is_default_constructible<char>("char");
    test_is_default_constructible<int>("int");
    test_is_default_constructible<long>("long");
    test_is_default_constructible<long long>("long long");
    test_is_default_constructible<float>("float");
    test_is_default_constructible<double>("double");

    test_is_default_constructible<int[]>("int[]");
    test_is_default_constructible<int[3]>("int[3]");
    test_is_default_constructible<int[][3]>("int[][3]");

    test_is_default_constructible<Class>("Class");
    test_is_default_constructible<Union>("Union");
    test_is_default_constructible<Enum>("Enum");
    test_is_default_constructible<Enum_class>("Enum_class");

    test_is_default_constructible<Class>("Class");
    test_is_default_constructible<Class2>("Class2");
    test_is_default_constructible<int Class::*>("int Class::*");
    test_is_default_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_default_constructible<A>("A");
    test_is_default_constructible<B>("B");
    test_is_default_constructible<C>("C");
    test_is_default_constructible<D>("D");
    test_is_default_constructible<D_noexcept>("D_noexcept");



}



template <typename T>
void test_is_copy_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_copy_constructible_v<T> ==
	       std::is_copy_constructible_v<T>, 
	       alp::as_str() << "is_copy_constructible(" << name_type << ")");
}

void test_is_copy_constructible()
{
    test::interface("is_copy_constructible");

    test_is_copy_constructible<void>("void");
    test_is_copy_constructible<const volatile void>("const volatile void");
    test_is_copy_constructible<nullptr_t>("nullptr_t");

    test_is_copy_constructible<char>("char");
    test_is_copy_constructible<int>("int");
    test_is_copy_constructible<long>("long");
    test_is_copy_constructible<long long>("long long");
    test_is_copy_constructible<float>("float");
    test_is_copy_constructible<double>("double");

    test_is_copy_constructible<int[]>("int[]");
    test_is_copy_constructible<int[3]>("int[3]");
    test_is_copy_constructible<int[][3]>("int[][3]");

    test_is_copy_constructible<Class>("Class");
    test_is_copy_constructible<Union>("Union");
    test_is_copy_constructible<Enum>("Enum");
    test_is_copy_constructible<Enum_class>("Enum_class");

    test_is_copy_constructible<Class>("Class");
    test_is_copy_constructible<Class2>("Class2");
    test_is_copy_constructible<int Class::*>("int Class::*");
    test_is_copy_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_copy_constructible<A>("A");
    test_is_copy_constructible<B>("B");
    test_is_copy_constructible<C>("C");
    test_is_copy_constructible<D>("D");
    test_is_copy_constructible<D_noexcept>("D_noexcept");



}


template <typename T, typename U>
void test_is_nothrow_swappable_with(const std::string& name_type1, 
			const std::string& name_type2)
{
    CHECK_TRUE(mtd::is_nothrow_swappable_with_v<T, U> ==
	       std::is_nothrow_swappable_with_v<T, U>, 
	       alp::as_str() << "is_nothrow_swappable_with(" << name_type1
			     << ", " << name_type2 << ")");
}


template <typename T>
void test_is_nothrow_swappable_with(const std::string& name_type1)
{
    test::interface("is_nothrow_swappable_with");

    test_is_nothrow_swappable_with<T, void>(name_type1, "void");
    test_is_nothrow_swappable_with<T, const volatile void>(name_type1, "const volatile void");
    test_is_nothrow_swappable_with<T, nullptr_t>(name_type1, "nullptr_t");

    test_is_nothrow_swappable_with<T, char>(name_type1, "char");
    test_is_nothrow_swappable_with<T, int>(name_type1, "int");
    test_is_nothrow_swappable_with<T, long>(name_type1, "long");
    test_is_nothrow_swappable_with<T, long long>(name_type1, "long long");
    test_is_nothrow_swappable_with<T, float>(name_type1, "float");
    test_is_nothrow_swappable_with<T, double>(name_type1, "double");

    test_is_nothrow_swappable_with<T, int[]>(name_type1, "int[]");
    test_is_nothrow_swappable_with<T, int[3]>(name_type1, "int[3]");
    test_is_nothrow_swappable_with<T, int[][3]>(name_type1, "int[][3]");

    test_is_nothrow_swappable_with<T, Class>(name_type1, "Class");
    test_is_nothrow_swappable_with<T, Union>(name_type1, "Union");
    test_is_nothrow_swappable_with<T, Enum>(name_type1, "Enum");
    test_is_nothrow_swappable_with<T, Enum_class>(name_type1, "Enum_class");

    test_is_nothrow_swappable_with<T, Class>(name_type1, "Class");
    test_is_nothrow_swappable_with<T, Class2>(name_type1, "Class2");
    test_is_nothrow_swappable_with<T, int Class::*>(name_type1, "int Class::*");
    test_is_nothrow_swappable_with<T, int (Class::*)()>(name_type1, "int (Class::*)()");

    test_is_nothrow_swappable_with<T, A>(name_type1, "A");
    test_is_nothrow_swappable_with<T, B>(name_type1, "B");
    test_is_nothrow_swappable_with<T, C>(name_type1, "C");
    test_is_nothrow_swappable_with<T, D>(name_type1, "D");
    test_is_nothrow_swappable_with<T, D_noexcept>(name_type1, "D_noexcept");

}

void test_is_nothrow_swappable_with()
{
    test::interface("is_nothrow_swappable_with");

    test_is_nothrow_swappable_with<void>("void");
    test_is_nothrow_swappable_with<const volatile void>("const volatile void");
    test_is_nothrow_swappable_with<nullptr_t>("nullptr_t");

    test_is_nothrow_swappable_with<char>("char");
    test_is_nothrow_swappable_with<int>("int");
    test_is_nothrow_swappable_with<long>("long");
    test_is_nothrow_swappable_with<long long>("long long");
    test_is_nothrow_swappable_with<float>("float");
    test_is_nothrow_swappable_with<double>("double");

    test_is_nothrow_swappable_with<int[]>("int[]");
    test_is_nothrow_swappable_with<int[3]>("int[3]");
    test_is_nothrow_swappable_with<int[][3]>("int[][3]");

    test_is_nothrow_swappable_with<Class>("Class");
    test_is_nothrow_swappable_with<Union>("Union");
    test_is_nothrow_swappable_with<Enum>("Enum");
    test_is_nothrow_swappable_with<Enum_class>("Enum_class");

    test_is_nothrow_swappable_with<Class>("Class");
    test_is_nothrow_swappable_with<Class2>("Class2");
    test_is_nothrow_swappable_with<int Class::*>("int Class::*");
    test_is_nothrow_swappable_with<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_swappable_with<A>("A");
    test_is_nothrow_swappable_with<B>("B");
    test_is_nothrow_swappable_with<C>("C");
    test_is_nothrow_swappable_with<D>("D");
    test_is_nothrow_swappable_with<D_noexcept>("D_noexcept");

}

template <typename T, typename U>
void test_is_assignable(const std::string& name_type1, 
			const std::string& name_type2)
{
    CHECK_TRUE(mtd::is_assignable_v<T, U> ==
	       std::is_assignable_v<T, U>, 
	       alp::as_str() << "is_assignable(" << name_type1
			     << ", " << name_type2 << ")");
}


template <typename T>
void test_is_assignable(const std::string& name_type1)
{
    test::interface("is_assignable");

    test_is_assignable<T, void>(name_type1, "void");
    test_is_assignable<T, const volatile void>(name_type1, "const volatile void");
    test_is_assignable<T, nullptr_t>(name_type1, "nullptr_t");

    test_is_assignable<T, char>(name_type1, "char");
    test_is_assignable<T, int>(name_type1, "int");
    test_is_assignable<T, long>(name_type1, "long");
    test_is_assignable<T, long long>(name_type1, "long long");
    test_is_assignable<T, float>(name_type1, "float");
    test_is_assignable<T, double>(name_type1, "double");

    test_is_assignable<T, int[]>(name_type1, "int[]");
    test_is_assignable<T, int[3]>(name_type1, "int[3]");
    test_is_assignable<T, int[][3]>(name_type1, "int[][3]");

    test_is_assignable<T, Class>(name_type1, "Class");
    test_is_assignable<T, Union>(name_type1, "Union");
    test_is_assignable<T, Enum>(name_type1, "Enum");
    test_is_assignable<T, Enum_class>(name_type1, "Enum_class");

    test_is_assignable<T, Class>(name_type1, "Class");
    test_is_assignable<T, Class2>(name_type1, "Class2");
    test_is_assignable<T, int Class::*>(name_type1, "int Class::*");
    test_is_assignable<T, int (Class::*)()>(name_type1, "int (Class::*)()");

    test_is_assignable<T, A>(name_type1, "A");
    test_is_assignable<T, B>(name_type1, "B");
    test_is_assignable<T, C>(name_type1, "C");
    test_is_assignable<T, D>(name_type1, "D");
    test_is_assignable<T, D_noexcept>(name_type1, "D_noexcept");

}

void test_is_assignable()
{
    test::interface("is_assignable");

    test_is_assignable<void>("void");
    test_is_assignable<const volatile void>("const volatile void");
    test_is_assignable<nullptr_t>("nullptr_t");

    test_is_assignable<char>("char");
    test_is_assignable<int>("int");
    test_is_assignable<long>("long");
    test_is_assignable<long long>("long long");
    test_is_assignable<float>("float");
    test_is_assignable<double>("double");

    test_is_assignable<int[]>("int[]");
    test_is_assignable<int[3]>("int[3]");
    test_is_assignable<int[][3]>("int[][3]");

    test_is_assignable<Class>("Class");
    test_is_assignable<Union>("Union");
    test_is_assignable<Enum>("Enum");
    test_is_assignable<Enum_class>("Enum_class");

    test_is_assignable<Class>("Class");
    test_is_assignable<Class2>("Class2");
    test_is_assignable<int Class::*>("int Class::*");
    test_is_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_assignable<A>("A");
    test_is_assignable<B>("B");
    test_is_assignable<C>("C");
    test_is_assignable<D>("D");
    test_is_assignable<D_noexcept>("D_noexcept");

}



template <typename T, typename U>
void test_is_nothrow_assignable(const std::string& name_type1, 
			const std::string& name_type2)
{
    CHECK_TRUE(mtd::is_assignable_v<T, U> ==
	       std::is_assignable_v<T, U>, 
	       alp::as_str() << "is_assignable(" << name_type1
			     << ", " << name_type2 << ")");
}


template <typename T>
void test_is_nothrow_assignable(const std::string& name_type1)
{
    test::interface("is_assignable");

    test_is_nothrow_assignable<T, void>(name_type1, "void");
    test_is_nothrow_assignable<T, const volatile void>(name_type1, "const volatile void");
    test_is_nothrow_assignable<T, nullptr_t>(name_type1, "nullptr_t");

    test_is_nothrow_assignable<T, char>(name_type1, "char");
    test_is_nothrow_assignable<T, int>(name_type1, "int");
    test_is_nothrow_assignable<T, long>(name_type1, "long");
    test_is_nothrow_assignable<T, long long>(name_type1, "long long");
    test_is_nothrow_assignable<T, float>(name_type1, "float");
    test_is_nothrow_assignable<T, double>(name_type1, "double");

    test_is_nothrow_assignable<T, int[]>(name_type1, "int[]");
    test_is_nothrow_assignable<T, int[3]>(name_type1, "int[3]");
    test_is_nothrow_assignable<T, int[][3]>(name_type1, "int[][3]");

    test_is_nothrow_assignable<T, Class>(name_type1, "Class");
    test_is_nothrow_assignable<T, Union>(name_type1, "Union");
    test_is_nothrow_assignable<T, Enum>(name_type1, "Enum");
    test_is_nothrow_assignable<T, Enum_class>(name_type1, "Enum_class");

    test_is_nothrow_assignable<T, Class>(name_type1, "Class");
    test_is_nothrow_assignable<T, Class2>(name_type1, "Class2");
    test_is_nothrow_assignable<T, int Class::*>(name_type1, "int Class::*");
    test_is_nothrow_assignable<T, int (Class::*)()>(name_type1, "int (Class::*)()");

    test_is_nothrow_assignable<T, A>(name_type1, "A");
    test_is_nothrow_assignable<T, B>(name_type1, "B");
    test_is_nothrow_assignable<T, C>(name_type1, "C");
    test_is_nothrow_assignable<T, D>(name_type1, "D");
    test_is_nothrow_assignable<T, D_noexcept>(name_type1, "D_noexcept");

}

void test_is_nothrow_assignable()
{
    test::interface("is_assignable");

    test_is_nothrow_assignable<void>("void");
    test_is_nothrow_assignable<const volatile void>("const volatile void");
    test_is_nothrow_assignable<nullptr_t>("nullptr_t");

    test_is_nothrow_assignable<char>("char");
    test_is_nothrow_assignable<int>("int");
    test_is_nothrow_assignable<long>("long");
    test_is_nothrow_assignable<long long>("long long");
    test_is_nothrow_assignable<float>("float");
    test_is_nothrow_assignable<double>("double");

    test_is_nothrow_assignable<int[]>("int[]");
    test_is_nothrow_assignable<int[3]>("int[3]");
    test_is_nothrow_assignable<int[][3]>("int[][3]");

    test_is_nothrow_assignable<Class>("Class");
    test_is_nothrow_assignable<Union>("Union");
    test_is_nothrow_assignable<Enum>("Enum");
    test_is_nothrow_assignable<Enum_class>("Enum_class");

    test_is_nothrow_assignable<Class>("Class");
    test_is_nothrow_assignable<Class2>("Class2");
    test_is_nothrow_assignable<int Class::*>("int Class::*");
    test_is_nothrow_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_assignable<A>("A");
    test_is_nothrow_assignable<B>("B");
    test_is_nothrow_assignable<C>("C");
    test_is_nothrow_assignable<D>("D");
    test_is_nothrow_assignable<D_noexcept>("D_noexcept");

}


template <typename T>
void test_is_copy_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_copy_assignable_v<T> ==
	       std::is_copy_assignable_v<T>, 
	       alp::as_str() << "is_copy_assignable(" << name_type << ")");
}

void test_is_copy_assignable()
{
    test::interface("is_copy_assignable");

    test_is_copy_assignable<void>("void");
    test_is_copy_assignable<const volatile void>("const volatile void");
    test_is_copy_assignable<nullptr_t>("nullptr_t");

    test_is_copy_assignable<char>("char");
    test_is_copy_assignable<int>("int");
    test_is_copy_assignable<long>("long");
    test_is_copy_assignable<long long>("long long");
    test_is_copy_assignable<float>("float");
    test_is_copy_assignable<double>("double");

    test_is_copy_assignable<int[]>("int[]");
    test_is_copy_assignable<int[3]>("int[3]");
    test_is_copy_assignable<int[][3]>("int[][3]");

    test_is_copy_assignable<Class>("Class");
    test_is_copy_assignable<Union>("Union");
    test_is_copy_assignable<Enum>("Enum");
    test_is_copy_assignable<Enum_class>("Enum_class");

    test_is_copy_assignable<Class>("Class");
    test_is_copy_assignable<Class2>("Class2");
    test_is_copy_assignable<int Class::*>("int Class::*");
    test_is_copy_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_copy_assignable<A>("A");
    test_is_copy_assignable<B>("B");
    test_is_copy_assignable<C>("C");
    test_is_copy_assignable<D>("D");
    test_is_copy_assignable<D_noexcept>("D_noexcept");



}







template <typename T>
void test_is_nothrow_copy_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_copy_constructible_v<T> ==
	       std::is_nothrow_copy_constructible_v<T>, 
	       alp::as_str() << "is_nothrow_copy_constructible(" << name_type << ")");
}

void test_is_nothrow_copy_constructible()
{
    test::interface("is_nothrow_copy_constructible");

    test_is_nothrow_copy_constructible<void>("void");
    test_is_nothrow_copy_constructible<const volatile void>("const volatile void");
    test_is_nothrow_copy_constructible<nullptr_t>("nullptr_t");

    test_is_nothrow_copy_constructible<char>("char");
    test_is_nothrow_copy_constructible<int>("int");
    test_is_nothrow_copy_constructible<long>("long");
    test_is_nothrow_copy_constructible<long long>("long long");
    test_is_nothrow_copy_constructible<float>("float");
    test_is_nothrow_copy_constructible<double>("double");

    test_is_nothrow_copy_constructible<int[]>("int[]");
    test_is_nothrow_copy_constructible<int[3]>("int[3]");
    test_is_nothrow_copy_constructible<int[][3]>("int[][3]");

    test_is_nothrow_copy_constructible<Class>("Class");
    test_is_nothrow_copy_constructible<Union>("Union");
    test_is_nothrow_copy_constructible<Enum>("Enum");
    test_is_nothrow_copy_constructible<Enum_class>("Enum_class");

    test_is_nothrow_copy_constructible<Class>("Class");
    test_is_nothrow_copy_constructible<Class2>("Class2");
    test_is_nothrow_copy_constructible<int Class::*>("int Class::*");
    test_is_nothrow_copy_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_copy_constructible<A>("A");
    test_is_nothrow_copy_constructible<B>("B");
    test_is_nothrow_copy_constructible<C>("C");
    test_is_nothrow_copy_constructible<D>("D");
    test_is_nothrow_copy_constructible<D_noexcept>("D_noexcept");
}



template <typename T>
void test_is_nothrow_swappable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_swappable_v<T> ==
	       std::is_nothrow_swappable_v<T>, 
	       alp::as_str() << "is_nothrow_swappable(" << name_type << ")");
}

void test_is_nothrow_swappable()
{
    test::interface("is_nothrow_swappable");

    test_is_nothrow_swappable<void>("void");
    test_is_nothrow_swappable<const volatile void>("const volatile void");
    test_is_nothrow_swappable<nullptr_t>("nullptr_t");

    test_is_nothrow_swappable<char>("char");
    test_is_nothrow_swappable<int>("int");
    test_is_nothrow_swappable<long>("long");
    test_is_nothrow_swappable<long long>("long long");
    test_is_nothrow_swappable<float>("float");
    test_is_nothrow_swappable<double>("double");

    test_is_nothrow_swappable<int[]>("int[]");
    test_is_nothrow_swappable<int[3]>("int[3]");
    test_is_nothrow_swappable<int[][3]>("int[][3]");

    test_is_nothrow_swappable<Class>("Class");
    test_is_nothrow_swappable<Union>("Union");
    test_is_nothrow_swappable<Enum>("Enum");
    test_is_nothrow_swappable<Enum_class>("Enum_class");

    test_is_nothrow_swappable<Class>("Class");
    test_is_nothrow_swappable<Class2>("Class2");
    test_is_nothrow_swappable<int Class::*>("int Class::*");
    test_is_nothrow_swappable<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_swappable<A>("A");
    test_is_nothrow_swappable<B>("B");
    test_is_nothrow_swappable<C>("C");
    test_is_nothrow_swappable<D>("D");
    test_is_nothrow_swappable<D_noexcept>("D_noexcept");
}

template <typename T>
void test_is_nothrow_copy_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_copy_assignable_v<T> ==
	       std::is_nothrow_copy_assignable_v<T>, 
	       alp::as_str() << "is_nothrow_copy_assignable(" << name_type << ")");
}

void test_is_nothrow_copy_assignable()
{
    test::interface("is_nothrow_copy_assignable");

    test_is_nothrow_copy_assignable<void>("void");
    test_is_nothrow_copy_assignable<const volatile void>("const volatile void");
    test_is_nothrow_copy_assignable<nullptr_t>("nullptr_t");

    test_is_nothrow_copy_assignable<char>("char");
    test_is_nothrow_copy_assignable<int>("int");
    test_is_nothrow_copy_assignable<long>("long");
    test_is_nothrow_copy_assignable<long long>("long long");
    test_is_nothrow_copy_assignable<float>("float");
    test_is_nothrow_copy_assignable<double>("double");

    test_is_nothrow_copy_assignable<int[]>("int[]");
    test_is_nothrow_copy_assignable<int[3]>("int[3]");
    test_is_nothrow_copy_assignable<int[][3]>("int[][3]");

    test_is_nothrow_copy_assignable<Class>("Class");
    test_is_nothrow_copy_assignable<Union>("Union");
    test_is_nothrow_copy_assignable<Enum>("Enum");
    test_is_nothrow_copy_assignable<Enum_class>("Enum_class");

    test_is_nothrow_copy_assignable<Class>("Class");
    test_is_nothrow_copy_assignable<Class2>("Class2");
    test_is_nothrow_copy_assignable<int Class::*>("int Class::*");
    test_is_nothrow_copy_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_copy_assignable<A>("A");
    test_is_nothrow_copy_assignable<B>("B");
    test_is_nothrow_copy_assignable<C>("C");
    test_is_nothrow_copy_assignable<D>("D");
    test_is_nothrow_copy_assignable<D_noexcept>("D_noexcept");
}



template <typename T>
void test_is_nothrow_default_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_default_constructible_v<T> ==
	       std::is_nothrow_default_constructible_v<T>, 
	       alp::as_str() << "is_nothrow_default_constructible(" << name_type << ")");
}

void test_is_nothrow_default_constructible()
{
    test::interface("is_nothrow_default_constructible");

    test_is_nothrow_default_constructible<void>("void");
    test_is_nothrow_default_constructible<const volatile void>("const volatile void");
    test_is_nothrow_default_constructible<nullptr_t>("nullptr_t");

    test_is_nothrow_default_constructible<char>("char");
    test_is_nothrow_default_constructible<int>("int");
    test_is_nothrow_default_constructible<long>("long");
    test_is_nothrow_default_constructible<long long>("long long");
    test_is_nothrow_default_constructible<float>("float");
    test_is_nothrow_default_constructible<double>("double");

    test_is_nothrow_default_constructible<int[]>("int[]");
    test_is_nothrow_default_constructible<int[3]>("int[3]");
    test_is_nothrow_default_constructible<int[][3]>("int[][3]");

    test_is_nothrow_default_constructible<Class>("Class");
    test_is_nothrow_default_constructible<Union>("Union");
    test_is_nothrow_default_constructible<Enum>("Enum");
    test_is_nothrow_default_constructible<Enum_class>("Enum_class");

    test_is_nothrow_default_constructible<Class>("Class");
    test_is_nothrow_default_constructible<Class2>("Class2");
    test_is_nothrow_default_constructible<int Class::*>("int Class::*");
    test_is_nothrow_default_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_default_constructible<A>("A");
    test_is_nothrow_default_constructible<B>("B");
    test_is_nothrow_default_constructible<C>("C");
    test_is_nothrow_default_constructible<D>("D");
    test_is_nothrow_default_constructible<D_noexcept>("D_noexcept");
}




template <typename T>
void test_is_destructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_destructible_v<T> ==
	       std::is_destructible_v<T>, 
	       alp::as_str() << "is_destructible(" << name_type << ")");
}

void test_is_destructible()
{
    test::interface("is_destructible");

    test_is_destructible<void>("void");
    test_is_destructible<const volatile void>("const volatile void");
    test_is_destructible<nullptr_t>("nullptr_t");

    test_is_destructible<char>("char");
    test_is_destructible<int>("int");
    test_is_destructible<long>("long");
    test_is_destructible<long long>("long long");
    test_is_destructible<float>("float");
    test_is_destructible<double>("double");

    test_is_destructible<int[]>("int[]");
    test_is_destructible<int[3]>("int[3]");
    test_is_destructible<int[][3]>("int[][3]");

    test_is_destructible<Class>("Class");
    test_is_destructible<Union>("Union");
    test_is_destructible<Enum>("Enum");
    test_is_destructible<Enum_class>("Enum_class");

    test_is_destructible<Class>("Class");
    test_is_destructible<Class2>("Class2");
    test_is_destructible<int Class::*>("int Class::*");
    test_is_destructible<int (Class::*)()>("int (Class::*)()");

    test_is_destructible<A>("A");
    test_is_destructible<B>("B");
    test_is_destructible<C>("C");
    test_is_destructible<D>("D");
    test_is_destructible<D_noexcept>("D_noexcept");



}


template <typename T>
void test_is_trivially_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_constructible_v<T> ==
	       std::is_trivially_constructible_v<T>, 
	       alp::as_str() << "is_trivially_constructible(" << name_type << ")");
}

void test_is_trivially_constructible()
{
    test::interface("is_trivially_constructible");

    test_is_trivially_constructible<void>("void");
    test_is_trivially_constructible<const volatile void>("const volatile void");
    test_is_trivially_constructible<nullptr_t>("nullptr_t");

    test_is_trivially_constructible<char>("char");
    test_is_trivially_constructible<int>("int");
    test_is_trivially_constructible<long>("long");
    test_is_trivially_constructible<long long>("long long");
    test_is_trivially_constructible<float>("float");
    test_is_trivially_constructible<double>("double");

    test_is_trivially_constructible<int[]>("int[]");
    test_is_trivially_constructible<int[3]>("int[3]");
    test_is_trivially_constructible<int[][3]>("int[][3]");

    test_is_trivially_constructible<Class>("Class");
    test_is_trivially_constructible<Union>("Union");
    test_is_trivially_constructible<Enum>("Enum");
    test_is_trivially_constructible<Enum_class>("Enum_class");

    test_is_trivially_constructible<Class>("Class");
    test_is_trivially_constructible<Class2>("Class2");
    test_is_trivially_constructible<int Class::*>("int Class::*");
    test_is_trivially_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_constructible<A>("A");
    test_is_trivially_constructible<B>("B");
    test_is_trivially_constructible<C>("C");
    test_is_trivially_constructible<D>("D");
    test_is_trivially_constructible<D_noexcept>("D_noexcept");



}

template <typename T>
void test_is_trivially_default_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_default_constructible_v<T> ==
	       std::is_trivially_default_constructible_v<T>, 
	       alp::as_str() << "is_trivially_default_constructible(" << name_type << ")");
}

void test_is_trivially_default_constructible()
{
    test::interface("is_trivially_default_constructible");

    test_is_trivially_default_constructible<void>("void");
    test_is_trivially_default_constructible<const volatile void>("const volatile void");
    test_is_trivially_default_constructible<nullptr_t>("nullptr_t");

    test_is_trivially_default_constructible<char>("char");
    test_is_trivially_default_constructible<int>("int");
    test_is_trivially_default_constructible<long>("long");
    test_is_trivially_default_constructible<long long>("long long");
    test_is_trivially_default_constructible<float>("float");
    test_is_trivially_default_constructible<double>("double");

    test_is_trivially_default_constructible<int[]>("int[]");
    test_is_trivially_default_constructible<int[3]>("int[3]");
    test_is_trivially_default_constructible<int[][3]>("int[][3]");

    test_is_trivially_default_constructible<Class>("Class");
    test_is_trivially_default_constructible<Union>("Union");
    test_is_trivially_default_constructible<Enum>("Enum");
    test_is_trivially_default_constructible<Enum_class>("Enum_class");

    test_is_trivially_default_constructible<Class>("Class");
    test_is_trivially_default_constructible<Class2>("Class2");
    test_is_trivially_default_constructible<int Class::*>("int Class::*");
    test_is_trivially_default_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_default_constructible<A>("A");
    test_is_trivially_default_constructible<B>("B");
    test_is_trivially_default_constructible<C>("C");
    test_is_trivially_default_constructible<D>("D");
    test_is_trivially_default_constructible<D_noexcept>("D_noexcept");



}


template <typename T>
void test_is_trivially_copy_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_copy_constructible_v<T> ==
	       std::is_trivially_copy_constructible_v<T>, 
	       alp::as_str() << "is_trivially_copy_constructible(" << name_type << ")");
}

void test_is_trivially_copy_constructible()
{
    test::interface("is_trivially_copy_constructible");

    test_is_trivially_copy_constructible<void>("void");
    test_is_trivially_copy_constructible<const volatile void>("const volatile void");
    test_is_trivially_copy_constructible<nullptr_t>("nullptr_t");

    test_is_trivially_copy_constructible<char>("char");
    test_is_trivially_copy_constructible<int>("int");
    test_is_trivially_copy_constructible<long>("long");
    test_is_trivially_copy_constructible<long long>("long long");
    test_is_trivially_copy_constructible<float>("float");
    test_is_trivially_copy_constructible<double>("double");

    test_is_trivially_copy_constructible<int[]>("int[]");
    test_is_trivially_copy_constructible<int[3]>("int[3]");
    test_is_trivially_copy_constructible<int[][3]>("int[][3]");

    test_is_trivially_copy_constructible<Class>("Class");
    test_is_trivially_copy_constructible<Union>("Union");
    test_is_trivially_copy_constructible<Enum>("Enum");
    test_is_trivially_copy_constructible<Enum_class>("Enum_class");

    test_is_trivially_copy_constructible<Class>("Class");
    test_is_trivially_copy_constructible<Class2>("Class2");
    test_is_trivially_copy_constructible<int Class::*>("int Class::*");
    test_is_trivially_copy_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_copy_constructible<A>("A");
    test_is_trivially_copy_constructible<B>("B");
    test_is_trivially_copy_constructible<C>("C");
    test_is_trivially_copy_constructible<D>("D");
    test_is_trivially_copy_constructible<D_noexcept>("D_noexcept");



}


template <typename T>
void test_is_trivially_move_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_move_constructible_v<T> ==
	       std::is_trivially_move_constructible_v<T>, 
	       alp::as_str() << "is_trivially_move_constructible(" << name_type << ")");
}

void test_is_trivially_move_constructible()
{
    test::interface("is_trivially_move_constructible");

    test_is_trivially_move_constructible<void>("void");
    test_is_trivially_move_constructible<const volatile void>("const volatile void");
    test_is_trivially_move_constructible<nullptr_t>("nullptr_t");

    test_is_trivially_move_constructible<char>("char");
    test_is_trivially_move_constructible<int>("int");
    test_is_trivially_move_constructible<long>("long");
    test_is_trivially_move_constructible<long long>("long long");
    test_is_trivially_move_constructible<float>("float");
    test_is_trivially_move_constructible<double>("double");

    test_is_trivially_move_constructible<int[]>("int[]");
    test_is_trivially_move_constructible<int[3]>("int[3]");
    test_is_trivially_move_constructible<int[][3]>("int[][3]");

    test_is_trivially_move_constructible<Class>("Class");
    test_is_trivially_move_constructible<Union>("Union");
    test_is_trivially_move_constructible<Enum>("Enum");
    test_is_trivially_move_constructible<Enum_class>("Enum_class");

    test_is_trivially_move_constructible<Class>("Class");
    test_is_trivially_move_constructible<Class2>("Class2");
    test_is_trivially_move_constructible<int Class::*>("int Class::*");
    test_is_trivially_move_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_move_constructible<A>("A");
    test_is_trivially_move_constructible<B>("B");
    test_is_trivially_move_constructible<C>("C");
    test_is_trivially_move_constructible<D>("D");
    test_is_trivially_move_constructible<D_noexcept>("D_noexcept");



}

template <typename T>
void test_is_trivially_copy_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_copy_assignable_v<T> ==
	       std::is_trivially_copy_assignable_v<T>, 
	       alp::as_str() << "is_trivially_copy_assignable(" << name_type << ")");
}

void test_is_trivially_copy_assignable()
{
    test::interface("is_trivially_copy_assignable");

    test_is_trivially_copy_assignable<void>("void");
    test_is_trivially_copy_assignable<const volatile void>("const volatile void");
    test_is_trivially_copy_assignable<nullptr_t>("nullptr_t");

    test_is_trivially_copy_assignable<char>("char");
    test_is_trivially_copy_assignable<int>("int");
    test_is_trivially_copy_assignable<long>("long");
    test_is_trivially_copy_assignable<long long>("long long");
    test_is_trivially_copy_assignable<float>("float");
    test_is_trivially_copy_assignable<double>("double");

    test_is_trivially_copy_assignable<int[]>("int[]");
    test_is_trivially_copy_assignable<int[3]>("int[3]");
    test_is_trivially_copy_assignable<int[][3]>("int[][3]");

    test_is_trivially_copy_assignable<Class>("Class");
    test_is_trivially_copy_assignable<Union>("Union");
    test_is_trivially_copy_assignable<Enum>("Enum");
    test_is_trivially_copy_assignable<Enum_class>("Enum_class");

    test_is_trivially_copy_assignable<Class>("Class");
    test_is_trivially_copy_assignable<Class2>("Class2");
    test_is_trivially_copy_assignable<int Class::*>("int Class::*");
    test_is_trivially_copy_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_copy_assignable<A>("A");
    test_is_trivially_copy_assignable<B>("B");
    test_is_trivially_copy_assignable<C>("C");
    test_is_trivially_copy_assignable<D>("D");
    test_is_trivially_copy_assignable<D_noexcept>("D_noexcept");



}

template <typename T>
void test_is_trivially_move_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_move_assignable_v<T> ==
	       std::is_trivially_move_assignable_v<T>, 
	       alp::as_str() << "is_trivially_move_assignable(" << name_type << ")");
}

void test_is_trivially_move_assignable()
{
    test::interface("is_trivially_move_assignable");

    test_is_trivially_move_assignable<void>("void");
    test_is_trivially_move_assignable<const volatile void>("const volatile void");
    test_is_trivially_move_assignable<nullptr_t>("nullptr_t");

    test_is_trivially_move_assignable<char>("char");
    test_is_trivially_move_assignable<int>("int");
    test_is_trivially_move_assignable<long>("long");
    test_is_trivially_move_assignable<long long>("long long");
    test_is_trivially_move_assignable<float>("float");
    test_is_trivially_move_assignable<double>("double");

    test_is_trivially_move_assignable<int[]>("int[]");
    test_is_trivially_move_assignable<int[3]>("int[3]");
    test_is_trivially_move_assignable<int[][3]>("int[][3]");

    test_is_trivially_move_assignable<Class>("Class");
    test_is_trivially_move_assignable<Union>("Union");
    test_is_trivially_move_assignable<Enum>("Enum");
    test_is_trivially_move_assignable<Enum_class>("Enum_class");

    test_is_trivially_move_assignable<Class>("Class");
    test_is_trivially_move_assignable<Class2>("Class2");
    test_is_trivially_move_assignable<int Class::*>("int Class::*");
    test_is_trivially_move_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_move_assignable<A>("A");
    test_is_trivially_move_assignable<B>("B");
    test_is_trivially_move_assignable<C>("C");
    test_is_trivially_move_assignable<D>("D");
    test_is_trivially_move_assignable<D_noexcept>("D_noexcept");



}


template <typename T>
void test_is_trivially_destructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_trivially_destructible_v<T> ==
	       std::is_trivially_destructible_v<T>, 
	       alp::as_str() << "is_trivially_destructible(" << name_type << ")");
}

void test_is_trivially_destructible()
{
    test::interface("is_trivially_destructible");

    test_is_trivially_destructible<void>("void");
    test_is_trivially_destructible<const volatile void>("const volatile void");
    test_is_trivially_destructible<nullptr_t>("nullptr_t");

    test_is_trivially_destructible<char>("char");
    test_is_trivially_destructible<int>("int");
    test_is_trivially_destructible<long>("long");
    test_is_trivially_destructible<long long>("long long");
    test_is_trivially_destructible<float>("float");
    test_is_trivially_destructible<double>("double");

    test_is_trivially_destructible<int[]>("int[]");
    test_is_trivially_destructible<int[3]>("int[3]");
    test_is_trivially_destructible<int[][3]>("int[][3]");

    test_is_trivially_destructible<Class>("Class");
    test_is_trivially_destructible<Union>("Union");
    test_is_trivially_destructible<Enum>("Enum");
    test_is_trivially_destructible<Enum_class>("Enum_class");

    test_is_trivially_destructible<Class>("Class");
    test_is_trivially_destructible<Class2>("Class2");
    test_is_trivially_destructible<int Class::*>("int Class::*");
    test_is_trivially_destructible<int (Class::*)()>("int (Class::*)()");

    test_is_trivially_destructible<A>("A");
    test_is_trivially_destructible<B>("B");
    test_is_trivially_destructible<C>("C");
    test_is_trivially_destructible<D>("D");
    test_is_trivially_destructible<D_noexcept>("D_noexcept");



}

template <typename T>
void test_is_nothrow_destructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_destructible_v<T> ==
	       std::is_nothrow_destructible_v<T>, 
	       alp::as_str() << "is_nothrow_destructible(" << name_type << ")");
}

void test_is_nothrow_destructible()
{
    test::interface("is_nothrow_destructible");

    test_is_nothrow_destructible<void>("void");
    test_is_nothrow_destructible<const volatile void>("const volatile void");
    test_is_nothrow_destructible<nullptr_t>("nullptr_t");

    test_is_nothrow_destructible<char>("char");
    test_is_nothrow_destructible<int>("int");
    test_is_nothrow_destructible<long>("long");
    test_is_nothrow_destructible<long long>("long long");
    test_is_nothrow_destructible<float>("float");
    test_is_nothrow_destructible<double>("double");

    test_is_nothrow_destructible<int[]>("int[]");
    test_is_nothrow_destructible<int[3]>("int[3]");
    test_is_nothrow_destructible<int[][3]>("int[][3]");

    test_is_nothrow_destructible<Class>("Class");
    test_is_nothrow_destructible<Union>("Union");
    test_is_nothrow_destructible<Enum>("Enum");
    test_is_nothrow_destructible<Enum_class>("Enum_class");

    test_is_nothrow_destructible<Class>("Class");
    test_is_nothrow_destructible<Class2>("Class2");
    test_is_nothrow_destructible<int Class::*>("int Class::*");
    test_is_nothrow_destructible<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_destructible<A>("A");
    test_is_nothrow_destructible<B>("B");
    test_is_nothrow_destructible<C>("C");
    test_is_nothrow_destructible<D>("D");
    test_is_nothrow_destructible<D_noexcept>("D_noexcept");



}

template <typename T>
void test_has_unique_object_representations(const std::string& name_type)
{
    CHECK_TRUE(mtd::has_unique_object_representations_v<T> ==
	       std::has_unique_object_representations_v<T>, 
	       alp::as_str() << "has_unique_object_representations(" << name_type << ")");
}

void test_has_unique_object_representations()
{
    test::interface("has_unique_object_representations");

    test_has_unique_object_representations<void>("void");
    test_has_unique_object_representations<const volatile void>("const volatile void");
    test_has_unique_object_representations<nullptr_t>("nullptr_t");

    test_has_unique_object_representations<char>("char");
    test_has_unique_object_representations<int>("int");
    test_has_unique_object_representations<long>("long");
    test_has_unique_object_representations<long long>("long long");
    test_has_unique_object_representations<float>("float");
    test_has_unique_object_representations<double>("double");

    test_has_unique_object_representations<int[]>("int[]");
    test_has_unique_object_representations<int[3]>("int[3]");
    test_has_unique_object_representations<int[][3]>("int[][3]");

    test_has_unique_object_representations<Class>("Class");
    test_has_unique_object_representations<Union>("Union");
    test_has_unique_object_representations<Enum>("Enum");
    test_has_unique_object_representations<Enum_class>("Enum_class");

    test_has_unique_object_representations<Class>("Class");
    test_has_unique_object_representations<Class2>("Class2");
    test_has_unique_object_representations<int Class::*>("int Class::*");
    test_has_unique_object_representations<int (Class::*)()>("int (Class::*)()");

    test_has_unique_object_representations<A>("A");
    test_has_unique_object_representations<B>("B");
    test_has_unique_object_representations<C>("C");
    test_has_unique_object_representations<D>("D");
    test_has_unique_object_representations<D_noexcept>("D_noexcept");



}

template <typename T>
void test_has_virtual_destructor(const std::string& name_type)
{
    CHECK_TRUE(mtd::has_virtual_destructor_v<T> ==
	       std::has_virtual_destructor_v<T>, 
	       alp::as_str() << "has_virtual_destructor(" << name_type << ")");
}

void test_has_virtual_destructor()
{
    test::interface("has_virtual_destructor");

    test_has_virtual_destructor<void>("void");
    test_has_virtual_destructor<const volatile void>("const volatile void");
    test_has_virtual_destructor<nullptr_t>("nullptr_t");

    test_has_virtual_destructor<char>("char");
    test_has_virtual_destructor<int>("int");
    test_has_virtual_destructor<long>("long");
    test_has_virtual_destructor<long long>("long long");
    test_has_virtual_destructor<float>("float");
    test_has_virtual_destructor<double>("double");

    test_has_virtual_destructor<int[]>("int[]");
    test_has_virtual_destructor<int[3]>("int[3]");
    test_has_virtual_destructor<int[][3]>("int[][3]");

    test_has_virtual_destructor<Class>("Class");
    test_has_virtual_destructor<Union>("Union");
    test_has_virtual_destructor<Enum>("Enum");
    test_has_virtual_destructor<Enum_class>("Enum_class");

    test_has_virtual_destructor<Class>("Class");
    test_has_virtual_destructor<Class2>("Class2");
    test_has_virtual_destructor<int Class::*>("int Class::*");
    test_has_virtual_destructor<int (Class::*)()>("int (Class::*)()");

    test_has_virtual_destructor<A>("A");
    test_has_virtual_destructor<B>("B");
    test_has_virtual_destructor<C>("C");
    test_has_virtual_destructor<D>("D");
    test_has_virtual_destructor<D_noexcept>("D_noexcept");



}








template <typename T>
void test_is_null_pointer(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_null_pointer_v<T> == 
	       std::is_null_pointer_v<T>, name_type);
    CHECK_TRUE(mtd::is_null_pointer_v<const T> == 
	       std::is_null_pointer_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_null_pointer_v<volatile T> == 
	       std::is_null_pointer_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_null_pointer_v<const volatile T> ==
	       std::is_null_pointer_v<const volatile T>, alp::as_str() << "const volatile " << name_type);
}

void test_is_null_pointer()
{
    test::interface("is_null_pointer");

    test_is_null_pointer<nullptr_t>("nullptr_t");

    test_is_null_pointer<void>("void");

    test_is_null_pointer<char>("char");
    test_is_null_pointer<int>("int");
    test_is_null_pointer<long>("long");
    test_is_null_pointer<long long>("long long");
    test_is_null_pointer<float>("float");
    test_is_null_pointer<double>("double");

    test_is_null_pointer<unsigned char>("unsigned char");
    test_is_null_pointer<unsigned int>("unsigned int");
    test_is_null_pointer<unsigned long>("unsigned long");
    test_is_null_pointer<unsigned long long>("unsigned long long");

// references
    test_is_null_pointer<char&>("char&");
    test_is_null_pointer<int&>("int&");
    test_is_null_pointer<long&>("long&");
    test_is_null_pointer<long long&>("long& long");
    test_is_null_pointer<float&>("float&");
    test_is_null_pointer<double&>("double&");

    test_is_null_pointer<unsigned char&>("unsigned char&");
    test_is_null_pointer<unsigned int&>("unsigned int&");
    test_is_null_pointer<unsigned long&>("unsigned long&");
    test_is_null_pointer<unsigned long long&>("unsigned long long&");

// pointers
    test_is_null_pointer<void*>("void*");

    test_is_null_pointer<char*>("char*");
    test_is_null_pointer<int*>("int*");
    test_is_null_pointer<long*>("long*");
    test_is_null_pointer<long long*>("long* long");
    test_is_null_pointer<float*>("float*");
    test_is_null_pointer<double*>("double*");

    test_is_null_pointer<unsigned char*>("unsigned char*");
    test_is_null_pointer<unsigned int*>("unsigned int*");
    test_is_null_pointer<unsigned long*>("unsigned long*");
    test_is_null_pointer<unsigned long long*>("unsigned long long*");
}

template <typename T>
void test_is_pointer(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_pointer_v<T> == std::is_pointer_v<T>, name_type);
    CHECK_TRUE(mtd::is_pointer_v<const T> == std::is_pointer_v<const T>
				    , alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_pointer_v<volatile T> == std::is_pointer_v<volatile T>, 
				    alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_pointer_v<const volatile T> 
	    == std::is_pointer_v<const volatile T>
			, alp::as_str() << "const volatile " << name_type);
}

void test_is_pointer()
{
    test::interface("is_pointer");

    test_is_pointer<nullptr_t>("nullptr_t"); // OJO: nullptr_t NO es un puntero!!!

    test_is_pointer<void>("void");

    test_is_pointer<char>("char");
    test_is_pointer<int>("int");
    test_is_pointer<long>("long");
    test_is_pointer<long long>("long long");
    test_is_pointer<float>("float");
    test_is_pointer<double>("double");

    test_is_pointer<unsigned char>("unsigned char");
    test_is_pointer<unsigned int>("unsigned int");
    test_is_pointer<unsigned long>("unsigned long");
    test_is_pointer<unsigned long long>("unsigned long long");

// references
    test_is_pointer<char&>("char&");
    test_is_pointer<int&>("int&");
    test_is_pointer<long&>("long&");
    test_is_pointer<long long&>("long& long");
    test_is_pointer<float&>("float&");
    test_is_pointer<double&>("double&");

    test_is_pointer<unsigned char&>("unsigned char&");
    test_is_pointer<unsigned int&>("unsigned int&");
    test_is_pointer<unsigned long&>("unsigned long&");
    test_is_pointer<unsigned long long&>("unsigned long long&");

// pointers
    test_is_pointer<void*>("void*");

    test_is_pointer<char*>("char*");
    test_is_pointer<int*>("int*");
    test_is_pointer<long*>("long*");
    test_is_pointer<long long*>("long* long");
    test_is_pointer<float*>("float*");
    test_is_pointer<double*>("double*");

    test_is_pointer<unsigned char*>("unsigned char*");
    test_is_pointer<unsigned int*>("unsigned int*");
    test_is_pointer<unsigned long*>("unsigned long*");
    test_is_pointer<unsigned long long*>("unsigned long long*");

}



template <typename T>
void test_is_member_object_pointer(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_member_object_pointer_v<T> == 
	       std::is_member_object_pointer_v<T>, 
	       alp::as_str() << "is_member_object_pointer(" << name_type << ")");
}

void test_is_member_object_pointer()
{
    test::interface("is_member_object_pointer");

    test_is_member_object_pointer<void>("void");
    test_is_member_object_pointer<char>("char");
    test_is_member_object_pointer<int>("int");
    test_is_member_object_pointer<long>("long");
    test_is_member_object_pointer<long long>("long long");
    test_is_member_object_pointer<float>("float");
    test_is_member_object_pointer<double>("double");

    test_is_member_object_pointer<Class>("Class");
    test_is_member_object_pointer<int Class::*>("int Class::*");
    test_is_member_object_pointer<int (Class::*)()>("int (Class::*)()");

}


template <typename T>
void test_is_member_function_pointer(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_member_function_pointer_v<T> == 
	       std::is_member_function_pointer_v<T>, 
	       alp::as_str() << "is_member_function_pointer(" << name_type << ")");
}

void test_is_member_function_pointer()
{
    test::interface("is_member_function_pointer");

    test_is_member_function_pointer<void>("void");
    test_is_member_function_pointer<char>("char");
    test_is_member_function_pointer<int>("int");
    test_is_member_function_pointer<long>("long");
    test_is_member_function_pointer<long long>("long long");
    test_is_member_function_pointer<float>("float");
    test_is_member_function_pointer<double>("double");

    test_is_member_function_pointer<Class>("Class");
    test_is_member_function_pointer<int Class::*>("int Class::*");
    test_is_member_function_pointer<int (Class::*)()>("int (Class::*)()");

}



template <typename T>
void test_is_object(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_object_v<T> ==
		std::is_object_v<T>, name_type);
    CHECK_TRUE(	mtd::is_object_v<T&> ==
		std::is_object_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_object_v<T&&> ==
		std::is_object_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_object_v<const T> ==
		std::is_object_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_object_v<const T&> ==
		std::is_object_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_object_v<const T&&> ==
		std::is_object_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_object_v<volatile T> ==
		std::is_object_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_object_v<volatile T&> ==
		std::is_object_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_object_v<volatile T&&> ==
		std::is_object_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_object()
{
    test::interface("is_object");
    
    test_is_object<char>("char");
    test_is_object<int>("int");
    test_is_object<long>("long");
    test_is_object<long long>("long long");
    test_is_object<float>("float");
    test_is_object<double>("double");

    test_is_object<Class>("Class");
    test_is_object<Union>("Union");
    test_is_object<Enum>("Enum");
    test_is_object<Enum_class>("Enum_class");
}


template <typename T>
void test_is_scalar(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_scalar_v<T> ==
		std::is_scalar_v<T>, name_type);
    CHECK_TRUE(	mtd::is_scalar_v<T&> ==
		std::is_scalar_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_scalar_v<T&&> ==
		std::is_scalar_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_scalar_v<const T> ==
		std::is_scalar_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_scalar_v<const T&> ==
		std::is_scalar_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_scalar_v<const T&&> ==
		std::is_scalar_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_scalar_v<volatile T> ==
		std::is_scalar_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_scalar_v<volatile T&> ==
		std::is_scalar_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_scalar_v<volatile T&&> ==
		std::is_scalar_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_scalar()
{
    test::interface("is_scalar");
    
    test_is_scalar<nullptr_t>("nullptr_t");

    test_is_scalar<char>("char");
    test_is_scalar<int>("int");
    test_is_scalar<long>("long");
    test_is_scalar<long long>("long long");
    test_is_scalar<float>("float");
    test_is_scalar<double>("double");

    test_is_scalar<Class>("Class");
    test_is_scalar<int Class::*>("int Class::*");
    test_is_scalar<int (Class::*)()>("int (Class::*)()");
    test_is_scalar<Union>("Union");
    test_is_scalar<Enum>("Enum");
    test_is_scalar<Enum_class>("Enum_class");
}



template <typename T>
void test_is_compound(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_compound_v<T> ==
		std::is_compound_v<T>, name_type);
    CHECK_TRUE(	mtd::is_compound_v<T&> ==
		std::is_compound_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_compound_v<T&&> ==
		std::is_compound_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_compound_v<const T> ==
		std::is_compound_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_compound_v<const T&> ==
		std::is_compound_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_compound_v<const T&&> ==
		std::is_compound_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_compound_v<volatile T> ==
		std::is_compound_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_compound_v<volatile T&> ==
		std::is_compound_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_compound_v<volatile T&&> ==
		std::is_compound_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_compound()
{
    test::interface("is_compound");
    
    test_is_compound<nullptr_t>("nullptr_t");

    test_is_compound<char>("char");
    test_is_compound<int>("int");
    test_is_compound<long>("long");
    test_is_compound<long long>("long long");
    test_is_compound<float>("float");
    test_is_compound<double>("double");

    test_is_compound<Class>("Class");
    test_is_compound<int Class::*>("int Class::*");
    test_is_compound<int (Class::*)()>("int (Class::*)()");
    test_is_compound<Union>("Union");
    test_is_compound<Enum>("Enum");
    test_is_compound<Enum_class>("Enum_class");

}

template <typename T>
void test_is_member_pointer(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_member_pointer_v<T> ==
		std::is_member_pointer_v<T>, name_type);
    CHECK_TRUE(	mtd::is_member_pointer_v<T&> ==
		std::is_member_pointer_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_member_pointer_v<T&&> ==
		std::is_member_pointer_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_member_pointer_v<const T> ==
		std::is_member_pointer_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_member_pointer_v<const T&> ==
		std::is_member_pointer_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_member_pointer_v<const T&&> ==
		std::is_member_pointer_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_member_pointer_v<volatile T> ==
		std::is_member_pointer_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_member_pointer_v<volatile T&> ==
		std::is_member_pointer_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_member_pointer_v<volatile T&&> ==
		std::is_member_pointer_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_member_pointer()
{
    test::interface("is_member_pointer");
    
    test_is_member_pointer<char>("char");
    test_is_member_pointer<int>("int");
    test_is_member_pointer<long>("long");
    test_is_member_pointer<long long>("long long");
    test_is_member_pointer<float>("float");
    test_is_member_pointer<double>("double");

    test_is_member_pointer<Class>("Class");
    test_is_member_pointer<int Class::*>("int Class::*");
    test_is_member_pointer<int (Class::*)()>("int (Class::*)()");
    test_is_member_pointer<Union>("Union");
    test_is_member_pointer<Enum>("Enum");
    test_is_member_pointer<Enum_class>("Enum_class");
}



template <typename T>
void test_add_lvalue_reference(const std::string& name_type)
{
    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<T>, 
				std::add_lvalue_reference_t<T>>),
               name_type);

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<T&>, 
				std::add_lvalue_reference_t<T&>>),
               alp::as_str() << name_type << "&");

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<const T>, 
				std::add_lvalue_reference_t<const T>>),
               alp::as_str() << "const " << name_type << "");

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<const T&>, 
				std::add_lvalue_reference_t<const T&>>),
               alp::as_str() << "const " << name_type << "&");
}

void test_add_lvalue_reference()
{
    test::interface("add_lvalue_reference");
    
    test_add_lvalue_reference<char>("char");
    test_add_lvalue_reference<int>("int");
    test_add_lvalue_reference<long>("long");
    test_add_lvalue_reference<long long>("long long");
    test_add_lvalue_reference<float>("float");
    test_add_lvalue_reference<double>("double");
    test_add_lvalue_reference<int (&)()>("int(&)()");
}

template <typename T>
void test_add_rvalue_reference(const std::string& name_type)
{
    CHECK_TRUE( (std::is_same_v<mtd::add_rvalue_reference_t<T>, 
				std::add_rvalue_reference_t<T>>),
               name_type);

    CHECK_TRUE( (std::is_same_v<mtd::add_rvalue_reference_t<T&>, 
				std::add_rvalue_reference_t<T&>>),
               alp::as_str() << name_type << "&");

    CHECK_TRUE( (std::is_same_v<mtd::add_rvalue_reference_t<const T>, 
				std::add_rvalue_reference_t<const T>>),
               alp::as_str() << "const " << name_type << "");

    CHECK_TRUE( (std::is_same_v<mtd::add_rvalue_reference_t<const T&>, 
				std::add_rvalue_reference_t<const T&>>),
               alp::as_str() << "const " << name_type << "&");
}

void test_add_rvalue_reference()
{
    test::interface("add_rvalue_reference");
    
    test_add_rvalue_reference<char>("char");
    test_add_rvalue_reference<int>("int");
    test_add_rvalue_reference<long>("long");
    test_add_rvalue_reference<long long>("long long");
    test_add_rvalue_reference<float>("float");
    test_add_rvalue_reference<double>("double");
    test_add_rvalue_reference<int (&)()>("int(&)()");
}




void test_is_base_of()
{
    test::interface("is_base_of");
    class A {};
    class B : A {};
    class C : B {};
    class D {};
    union E {};
    using I = int;

    CHECK_TRUE(mtd::is_base_of_v<A, A> == std::is_base_of_v<A, A>, "<A, A>");
    CHECK_TRUE(mtd::is_base_of_v<A, B> == std::is_base_of_v<A, B>, "<A, B>");
    CHECK_TRUE(mtd::is_base_of_v<A, C> == std::is_base_of_v<A, C>, "<A, C>");
    CHECK_TRUE(mtd::is_base_of_v<A, D> == std::is_base_of_v<A, D>, "<A, D>");
    CHECK_TRUE(mtd::is_base_of_v<B, A> == std::is_base_of_v<B, A>, "<B, A>");
    CHECK_TRUE(mtd::is_base_of_v<E, E> == std::is_base_of_v<E, E>, "<E, E>");
    CHECK_TRUE(mtd::is_base_of_v<I, I> == std::is_base_of_v<I, I>, "<I, I>");

}


template <typename From, typename To>
void test_is_convertible(const std::string& name_type1,
			 const std::string& name_type2)
{
    CHECK_TRUE((mtd::is_convertible_v<From, To> ==
                   std::is_convertible_v<From, To>),
	    alp::as_str() << "is_convertible_v<" << name_type1 
			  << ", " << name_type2 << ">");
}

template <typename T>
void test_is_convertible(const std::string& name_type1)
{
    test::interface("is_convertible");

    test_is_convertible<T, void>(name_type1, "void");
    test_is_convertible<T, const volatile void>(name_type1, "const volatile void");
    test_is_convertible<T, nullptr_t>(name_type1, "nullptr_t");

    test_is_convertible<T, char>(name_type1, "char");
    test_is_convertible<T, int>(name_type1, "int");
    test_is_convertible<T, long>(name_type1, "long");
    test_is_convertible<T, long long>(name_type1, "long long");
    test_is_convertible<T, float>(name_type1, "float");
    test_is_convertible<T, double>(name_type1, "double");

    test_is_convertible<T, int[]>(name_type1, "int[]");
    test_is_convertible<T, int[3]>(name_type1, "int[3]");
    test_is_convertible<T, int[][3]>(name_type1, "int[][3]");

    test_is_convertible<T, Class>(name_type1, "Class");
    test_is_convertible<T, Union>(name_type1, "Union");
    test_is_convertible<T, Enum>(name_type1, "Enum");
    test_is_convertible<T, Enum_class>(name_type1, "Enum_class");

    test_is_convertible<T, Class>(name_type1, "Class");
    test_is_convertible<T, Class2>(name_type1, "Class2");
    test_is_convertible<T, int Class::*>(name_type1, "int Class::*");
    test_is_convertible<T, int (Class::*)()>(name_type1, "int (Class::*)()");

    test_is_convertible<T, A>(name_type1, "A");
    test_is_convertible<T, B>(name_type1, "B");
    test_is_convertible<T, C>(name_type1, "C");
    test_is_convertible<T, D>(name_type1, "D");
    test_is_convertible<T, D_noexcept>(name_type1, "D_noexcept");

}

void test_is_convertible()
{
    test::interface("is_convertible");

    test_is_convertible<void>("void");
    test_is_convertible<const volatile void>("const volatile void");
    test_is_convertible<nullptr_t>("nullptr_t");

    test_is_convertible<char>("char");
    test_is_convertible<int>("int");
    test_is_convertible<long>("long");
    test_is_convertible<long long>("long long");
    test_is_convertible<float>("float");
    test_is_convertible<double>("double");

    test_is_convertible<int[]>("int[]");
    test_is_convertible<int[3]>("int[3]");
    test_is_convertible<int[][3]>("int[][3]");

    test_is_convertible<Class>("Class");
    test_is_convertible<Union>("Union");
    test_is_convertible<Enum>("Enum");
    test_is_convertible<Enum_class>("Enum_class");

    test_is_convertible<Class>("Class");
    test_is_convertible<Class2>("Class2");
    test_is_convertible<int Class::*>("int Class::*");
    test_is_convertible<int (Class::*)()>("int (Class::*)()");

    test_is_convertible<A>("A");
    test_is_convertible<B>("B");
    test_is_convertible<C>("C");
    test_is_convertible<D>("D");
    test_is_convertible<D_noexcept>("D_noexcept");

// Algunos a más
    test_is_convertible<char, void>("char", "void");
    test_is_convertible<char[], char*>("char[]", "char*");
    test_is_convertible<char*, char[]>("char*", "char[]");
    test_is_convertible<const int, int>("const int", "int");
    test_is_convertible<int, const int>("int", "const int");
    test_is_convertible<int, const int*>("int", "const int*");
}

template <typename From, typename To>
void test_is_nothrow_convertible(const std::string& name_type1,
			 const std::string& name_type2)
{
    CHECK_TRUE((mtd::is_nothrow_convertible_v<From, To> ==
                   std::is_nothrow_convertible_v<From, To>),
	    alp::as_str() << "is_nothrow_convertible_v<" << name_type1 
			  << ", " << name_type2 << ">");
}

template <typename T>
void test_is_nothrow_convertible(const std::string& name_type1)
{
    test::interface("is_nothrow_convertible");

    test_is_nothrow_convertible<T, void>(name_type1, "void");
    test_is_nothrow_convertible<T, const volatile void>(name_type1, "const volatile void");
    test_is_nothrow_convertible<T, nullptr_t>(name_type1, "nullptr_t");

    test_is_nothrow_convertible<T, char>(name_type1, "char");
    test_is_nothrow_convertible<T, int>(name_type1, "int");
    test_is_nothrow_convertible<T, long>(name_type1, "long");
    test_is_nothrow_convertible<T, long long>(name_type1, "long long");
    test_is_nothrow_convertible<T, float>(name_type1, "float");
    test_is_nothrow_convertible<T, double>(name_type1, "double");

    test_is_nothrow_convertible<T, int[]>(name_type1, "int[]");
    test_is_nothrow_convertible<T, int[3]>(name_type1, "int[3]");
    test_is_nothrow_convertible<T, int[][3]>(name_type1, "int[][3]");

    test_is_nothrow_convertible<T, Class>(name_type1, "Class");
    test_is_nothrow_convertible<T, Union>(name_type1, "Union");
    test_is_nothrow_convertible<T, Enum>(name_type1, "Enum");
    test_is_nothrow_convertible<T, Enum_class>(name_type1, "Enum_class");

    test_is_nothrow_convertible<T, Class>(name_type1, "Class");
    test_is_nothrow_convertible<T, Class2>(name_type1, "Class2");
    test_is_nothrow_convertible<T, int Class::*>(name_type1, "int Class::*");
    test_is_nothrow_convertible<T, int (Class::*)()>(name_type1, "int (Class::*)()");

    test_is_nothrow_convertible<T, A>(name_type1, "A");
    test_is_nothrow_convertible<T, B>(name_type1, "B");
    test_is_nothrow_convertible<T, C>(name_type1, "C");
    test_is_nothrow_convertible<T, D>(name_type1, "D");
    test_is_nothrow_convertible<T, D_noexcept>(name_type1, "D_noexcept");

}

void test_is_nothrow_convertible()
{
    test::interface("is_convertible");

    test_is_nothrow_convertible<void>("void");
    test_is_nothrow_convertible<const volatile void>("const volatile void");
    test_is_nothrow_convertible<nullptr_t>("nullptr_t");

    test_is_nothrow_convertible<char>("char");
    test_is_nothrow_convertible<int>("int");
    test_is_nothrow_convertible<long>("long");
    test_is_nothrow_convertible<long long>("long long");
    test_is_nothrow_convertible<float>("float");
    test_is_nothrow_convertible<double>("double");

    test_is_nothrow_convertible<int[]>("int[]");
    test_is_nothrow_convertible<int[3]>("int[3]");
    test_is_nothrow_convertible<int[][3]>("int[][3]");

    test_is_nothrow_convertible<Class>("Class");
    test_is_nothrow_convertible<Union>("Union");
    test_is_nothrow_convertible<Enum>("Enum");
    test_is_nothrow_convertible<Enum_class>("Enum_class");

    test_is_nothrow_convertible<Class>("Class");
    test_is_nothrow_convertible<Class2>("Class2");
    test_is_nothrow_convertible<int Class::*>("int Class::*");
    test_is_nothrow_convertible<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_convertible<A>("A");
    test_is_nothrow_convertible<B>("B");
    test_is_nothrow_convertible<C>("C");
    test_is_nothrow_convertible<D>("D");
    test_is_nothrow_convertible<D_noexcept>("D_noexcept");

// Algunos a más
    test_is_nothrow_convertible<char, void>("char", "void");
    test_is_nothrow_convertible<char[], char*>("char[]", "char*");
    test_is_nothrow_convertible<char*, char[]>("char*", "char[]");
    test_is_nothrow_convertible<const int, int>("const int", "int");
    test_is_nothrow_convertible<int, const int>("int", "const int");
    test_is_nothrow_convertible<int, const int*>("int", "const int*");
}


template <typename T>
void test_remove_pointer(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::remove_pointer_t<T>, std::remove_pointer_t<T>>),
        name_type);
}

void test_remove_pointer()
{
    test::interface("remove_pointer");

    test_remove_pointer<int>("int");
    test_remove_pointer<int&>("int&");
    test_remove_pointer<const int>("const int");
    test_remove_pointer<const int&>("const int&");
    test_remove_pointer<volatile int>("volatile int");
    test_remove_pointer<volatile int&>("volatile int&");
    test_remove_pointer<int*>("int*");
    test_remove_pointer<const int*>("const int*");
    test_remove_pointer<volatile int*>("volatile int*");
    test_remove_pointer<const volatile int*>("const volatile int*");
}


template <typename T>
void test_add_pointer_helper(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::add_pointer_t<T>, std::add_pointer_t<T>>),
        name_type);
}

template <typename T>
void test_add_pointer(const std::string& tname)
{
    test_add_pointer_helper<T>(tname);
    test_add_pointer_helper<T&>(alp::as_str() << tname << '&');
    test_add_pointer_helper<const T>(alp::as_str() << "const " << tname);
    test_add_pointer_helper<const T&>(alp::as_str() << "const " << tname << '&');
    test_add_pointer_helper<volatile T>(alp::as_str() << "volatile " << tname);
    test_add_pointer_helper<volatile T&>(alp::as_str()<< "volatile " << tname << '&');
    test_add_pointer_helper<T*>(alp::as_str() << tname << '*');
    test_add_pointer_helper<const T*>(alp::as_str() << "const " << tname << '*');
    test_add_pointer_helper<volatile T*>(alp::as_str() << "volatile " << tname << '*');
    test_add_pointer_helper<const volatile T*>(alp::as_str() << "const volatile " << tname << '*');
}


void test_add_pointer()
{
    test::interface("add_pointer");

    test_add_pointer<char>("char");
    test_add_pointer<int>("int");
    test_add_pointer<std::string>("string");


    // void
    test_add_pointer_helper<void>("void");
    test_add_pointer_helper<const void>("const void");
    test_add_pointer_helper<void*>("void*");
    test_add_pointer_helper<const void*>("const void*");
}

template <typename T>
void test_remove_extent(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::remove_extent_t<T>, 
			      std::remove_extent_t<T>>, name_type);
}

void test_remove_extent()
{
    test::interface("remove_extent");

    test_remove_extent<void>("void");
    test_remove_extent<nullptr_t>("nullptr_t");

    test_remove_extent<char>("char");
    test_remove_extent<int>("int");
    test_remove_extent<long>("long");
    test_remove_extent<long long>("long long");
    test_remove_extent<float>("float");
    test_remove_extent<double>("double");

    test_remove_extent<int[]>("int[]");
    test_remove_extent<int[3]>("int[3]");
    test_remove_extent<int[][3]>("int[][3]");

    test_remove_extent<std::string>("std::string");
    test_remove_extent<std::string[4]>("std::string[4]");
    test_remove_extent<std::string[][4]>("std::string[][4]");

    test_remove_extent<Class>("Class");
    test_remove_extent<Class2>("Class2");
    test_remove_extent<Union>("Union");
    test_remove_extent<Enum>("Enum");
    test_remove_extent<Enum_class>("Enum_class");

    test_remove_extent<Class>("Class");
    test_remove_extent<int Class::*>("int Class::*");
    test_remove_extent<int (Class::*)()>("int (Class::*)()");
}


template <typename T>
void test_remove_all_extents(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::remove_all_extents_t<T>, 
			      std::remove_all_extents_t<T>>, name_type);
}

void test_remove_all_extents()
{
    test::interface("remove_all_extents");

    test_remove_all_extents<void>("void");
    test_remove_all_extents<nullptr_t>("nullptr_t");

    test_remove_all_extents<char>("char");
    test_remove_all_extents<int>("int");
    test_remove_all_extents<long>("long");
    test_remove_all_extents<long long>("long long");
    test_remove_all_extents<float>("float");
    test_remove_all_extents<double>("double");

    test_remove_all_extents<int[]>("int[]");
    test_remove_all_extents<int[3]>("int[3]");
    test_remove_all_extents<int[][3]>("int[][3]");

    test_remove_all_extents<std::string>("std::string");
    test_remove_all_extents<std::string[4]>("std::string[4]");
    test_remove_all_extents<std::string[][4]>("std::string[][4]");

    test_remove_all_extents<Class>("Class");
    test_remove_all_extents<Class2>("Class2");
    test_remove_all_extents<Union>("Union");
    test_remove_all_extents<Enum>("Enum");
    test_remove_all_extents<Enum_class>("Enum_class");

    test_remove_all_extents<Class>("Class");
    test_remove_all_extents<int Class::*>("int Class::*");
    test_remove_all_extents<int (Class::*)()>("int (Class::*)()");
}




template <typename T>
void test_decay_helper(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::decay_t<T>, std::decay_t<T>>, name_type);
}

template <typename T>
void test_decay(const std::string& tname)
{
    test_decay_helper<T>(tname);
//    test_decay_helper<T&>(alp::as_str() << tname << '&');
//    test_decay_helper<T&&>(alp::as_str() << tname << "&&");
//    test_decay_helper<T[]>(alp::as_str() << tname << "[]");
//    test_decay_helper<const T>(alp::as_str() << "const " << tname);
//    test_decay_helper<const T&>(alp::as_str() << "const " << tname << '&');
//    test_decay_helper<volatile T>(alp::as_str() << "volatile " << tname);
//    test_decay_helper<volatile T&>(alp::as_str()<< "volatile " << tname << '&');
//    test_decay_helper<T*>(alp::as_str() << tname << '*');
//    test_decay_helper<const T*>(alp::as_str() << "const " << tname << '*');
//    test_decay_helper<volatile T*>(alp::as_str() << "volatile " << tname << '*');
//    test_decay_helper<const volatile T*>(alp::as_str() << "const volatile " << tname << '*');
}


void test_decay()
{
    test::interface("decay");

    test_decay<void>("void");
    test_decay<nullptr_t>("nullptr_t");

    test_decay<char>("char");
    test_decay<int>("int");
    test_decay<long>("long");
    test_decay<long long>("long long");
    test_decay<float>("float");
    test_decay<double>("double");

    test_decay<int[]>("int[]");
    test_decay<int[3]>("int[3]");
    test_decay<int[][3]>("int[][3]");

    test_decay<std::string>("string");

    test_decay<Class>("Class");
    test_decay<Class2>("Class2");
    test_decay<Union>("Union");
    test_decay<Enum>("Enum");
    test_decay<Enum_class>("Enum_class");

    test_decay<Class>("Class");
    test_decay<int Class::*>("int Class::*");
    test_decay<int (Class::*)()>("int (Class::*)()");
}


template <typename T>
void test_enable_if(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::enable_if_t<true, T>, std::enable_if_t<true, T>>),
        name_type);

    // Esto no tiene que compilar!!!
//    CHECK_TRUE(
//        (std::is_same_v<mtd::enable_if_t<false, T>, std::enable_if_t<false, T>>),
//        name_type);
}


void test_enable_if()
{
    test::interface("enable_if");

    test_enable_if<char>("char");
    test_enable_if<int>("int");
    test_enable_if<std::string>("string");
}






// mtd_common_type_defined
// -----------------------
template <typename T, typename U, typename = void>
struct mtd_common_type_defined : std::false_type { };

template <typename T, typename U>
struct mtd_common_type_defined<T, U, std::void_t<mtd::common_type_t<T,U>>> 
	    : std::true_type { };

template <typename T, typename U>
inline constexpr bool mtd_common_type_defined_v = mtd_common_type_defined<T,U>::value;

template <typename T, typename U, typename = void>
struct std_common_type_defined : std::false_type { };

template <typename T, typename U>
struct std_common_type_defined<T, U, std::void_t<std::common_type_t<T,U>>> 
	    : std::true_type { };

template <typename T, typename U>
inline constexpr bool std_common_type_defined_v = std_common_type_defined<T,U>::value;


template <typename T, typename U>
void test_common_type(const std::string& name_type1, 
		      const std::string& name_type2)
{
    CHECK_TRUE(mtd_common_type_defined_v<T, U> ==
	       std_common_type_defined_v<T, U>,
	       alp::as_str() << "both compile or not? (" 
			      << name_type1 << ", " << name_type2 << ")");

    if constexpr (mtd_common_type_defined_v<T, U>){
	CHECK_TRUE(std::is_same_v<mtd::common_type_t<T, U>, 
				 std::common_type_t<T, U>>,
		    alp::as_str() << name_type1 << ", " << name_type2);
    }
}


template <typename T>
void test_common_type(const std::string& name_type)
{
    test_common_type<T, void>(name_type, "void");

    test_common_type<T, nullptr_t>(name_type, "nullptr_t");

    test_common_type<T, char>(name_type, "char");
    test_common_type<T, int>(name_type, "int");
    test_common_type<T, long>(name_type, "long");
    test_common_type<T, long long>(name_type, "long long");
    test_common_type<T, float>(name_type, "float");
    test_common_type<T, double>(name_type, "double");

    test_common_type<T, int[]>(name_type, "int[]");
    test_common_type<T, int[3]>(name_type, "int[3]");
    test_common_type<T, int[][3]>(name_type, "int[][3]");

    test_common_type<T, Class>(name_type, "Class");
    test_common_type<T, Class2>(name_type, "Class2");
    test_common_type<T, Union>(name_type, "Union");
// Al comparar con Enum da un montón de warnings
//    test_common_type<T, Enum>(name_type, "Enum");
    test_common_type<T, Enum_class>(name_type, "Enum_class");

    test_common_type<T, Class>(name_type, "Class");
    test_common_type<T, int Class::*>(name_type, "int Class::*");
//    test_common_type<T, int (name_type, Class::*)()>("int (Class::*)()");

    test_common_type<T, std::string>(name_type, "std::string"); 
}

void test_common_type()
{
    test::interface("common_type");

    test_common_type<void>("void");
    test_common_type<nullptr_t>("nullptr_t");

    test_common_type<char>("char");
    test_common_type<int>("int");
    test_common_type<long>("long");
    test_common_type<long long>("long long");
    test_common_type<float>("float");
    test_common_type<double>("double");

    test_common_type<int[]>("int[]");
    test_common_type<int[3]>("int[3]");
    test_common_type<int[][3]>("int[][3]");

    test_common_type<Class>("Class");
    test_common_type<Class2>("Class2");
    test_common_type<Union>("Union");
// Al comparar con Enum da un montón de warnings
//    test_common_type<Enum>("Enum");
    test_common_type<Enum_class>("Enum_class");

    test_common_type<Class>("Class");
    test_common_type<int Class::*>("int Class::*");
    test_common_type<int (Class::*)()>("int (Class::*)()");
}



// mtd_common_reference_defined
// -----------------------
template <typename T, typename U, typename = void>
struct mtd_common_reference_defined : std::false_type { };

template <typename T, typename U>
struct mtd_common_reference_defined<T, U, 
				std::void_t<mtd::common_reference_t<T,U>>> 
	    : std::true_type { };

template <typename T, typename U>
inline constexpr 
bool mtd_common_reference_defined_v = mtd_common_reference_defined<T,U>::value;

template <typename T, typename U, typename = void>
struct std_common_reference_defined : std::false_type { };

template <typename T, typename U>
struct std_common_reference_defined<T, U, std::void_t<std::common_reference_t<T,U>>> 
	    : std::true_type { };

template <typename T, typename U>
inline constexpr bool std_common_reference_defined_v = std_common_reference_defined<T,U>::value;


template <typename T, typename U>
void test_common_reference(const std::string& name_type1, 
		      const std::string& name_type2)
{
    CHECK_TRUE(mtd_common_reference_defined_v<T, U> ==
	       std_common_reference_defined_v<T, U>,
	       alp::as_str() << "both compile or not? (" 
			      << name_type1 << ", " << name_type2 << ")");

    if constexpr (mtd_common_reference_defined_v<T, U>){
	CHECK_TRUE(std::is_same_v<mtd::common_reference_t<T, U>, 
				 std::common_reference_t<T, U>>,
		    alp::as_str() << name_type1 << ", " << name_type2);
    }
}


template <typename T>
void test_common_reference(const std::string& name_type)
{
    test_common_reference<T, void>(name_type, "void");

    test_common_reference<T, nullptr_t>(name_type, "nullptr_t");

    test_common_reference<T, char>(name_type, "char");
    test_common_reference<T, char&>(name_type, "char&");
    test_common_reference<T, int>(name_type, "int");
    test_common_reference<T, int&>(name_type, "int&");
    test_common_reference<T, long>(name_type, "long");
    test_common_reference<T, long&>(name_type, "long&");
    test_common_reference<T, long long>(name_type, "long long");
    test_common_reference<T, long long&>(name_type, "long long&");
    test_common_reference<T, float>(name_type, "float");
    test_common_reference<T, float&>(name_type, "float&");
    test_common_reference<T, double>(name_type, "double");
    test_common_reference<T, double&>(name_type, "double&");

    test_common_reference<T, int[]>(name_type, "int[]");
    test_common_reference<T, int[3]>(name_type, "int[3]");
    test_common_reference<T, int[][3]>(name_type, "int[][3]");

    test_common_reference<T, Class>(name_type, "Class");
    test_common_reference<T, Class&>(name_type, "Class&");
    test_common_reference<T, Class2>(name_type, "Class2");
    test_common_reference<T, Class2&>(name_type, "Class2&");
    test_common_reference<T, Union>(name_type, "Union");
// Al comparar con Enum da un montón de warnings
//    test_common_reference<T, Enum>(name_type, "Enum");
    test_common_reference<T, Enum_class>(name_type, "Enum_class");

    test_common_reference<T, Class>(name_type, "Class");
    test_common_reference<T, int Class::*>(name_type, "int Class::*");
//    test_common_reference<T, int (name_type, Class::*)()>("int (Class::*)()");

    test_common_reference<T, std::string>(name_type, "std::string"); 
}


void test_common_reference()
{
    test::interface("common_reference");

    test_common_reference<void>("void");
    test_common_reference<nullptr_t>("nullptr_t");

    test_common_reference<char>("char");
    test_common_reference<char&>("char&");
    test_common_reference<int>("int");
    test_common_reference<int&>("int&");
    test_common_reference<long>("long");
    test_common_reference<long&>("long&");
    test_common_reference<long long>("long long");
    test_common_reference<long long&>("long long&");
    test_common_reference<float>("float");
    test_common_reference<float&>("float&");
    test_common_reference<double>("double");
    test_common_reference<double&>("double&");

    test_common_reference<int[]>("int[]");
    test_common_reference<int[3]>("int[3]");
    test_common_reference<int[][3]>("int[][3]");

    test_common_reference<Class>("Class");
    test_common_reference<Class&>("Class&");
    test_common_reference<Class2>("Class2");
    test_common_reference<Class2&>("Class2&");
    test_common_reference<Union>("Union");
// Al comparar con Enum da un montón de warnings
//    test_common_reference<Enum>("Enum");
    test_common_reference<Enum_class>("Enum_class");

    test_common_reference<Class>("Class");
    test_common_reference<int Class::*>("int Class::*");
    test_common_reference<int (Class::*)()>("int (Class::*)()");
}




template <typename T, typename U>
void test_is_swappable_with(const std::string& name_type1, 
		      const std::string& name_type2)
{
    CHECK_TRUE(mtd::is_swappable_with_v<T, U> ==
			     std::is_swappable_with_v<T, U>,
		alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_is_swappable_with(const std::string& name_type)
{
    test_is_swappable_with<T, void>(name_type, "void");

    test_is_swappable_with<T, nullptr_t>(name_type, "nullptr_t");

    test_is_swappable_with<T, char>(name_type, "char");
    test_is_swappable_with<T, char&>(name_type, "char&");
    test_is_swappable_with<T, int>(name_type, "int");
    test_is_swappable_with<T, int&>(name_type, "int&");
    test_is_swappable_with<T, long>(name_type, "long");
    test_is_swappable_with<T, long&>(name_type, "long&");
    test_is_swappable_with<T, long long>(name_type, "long long");
    test_is_swappable_with<T, long long&>(name_type, "long long&");
    test_is_swappable_with<T, float>(name_type, "float");
    test_is_swappable_with<T, float&>(name_type, "float&");
    test_is_swappable_with<T, double>(name_type, "double");
    test_is_swappable_with<T, double&>(name_type, "double&");

    test_is_swappable_with<T, int[]>(name_type, "int[]");
    test_is_swappable_with<T, int[3]>(name_type, "int[3]");
    test_is_swappable_with<T, int[][3]>(name_type, "int[][3]");

    test_is_swappable_with<T, Class>(name_type, "Class");
    test_is_swappable_with<T, Class&>(name_type, "Class&");
    test_is_swappable_with<T, Class2>(name_type, "Class2");
    test_is_swappable_with<T, Class2&>(name_type, "Class2&");
    test_is_swappable_with<T, Union>(name_type, "Union");
    test_is_swappable_with<T, Enum>(name_type, "Enum");
    test_is_swappable_with<T, Enum_class>(name_type, "Enum_class");

    test_is_swappable_with<T, Class>(name_type, "Class");
    test_is_swappable_with<T, int Class::*>(name_type, "int Class::*");
//    test_is_swappable_with<T, int (name_type, Class::*)()>("int (Class::*)()");

    test_is_swappable_with<T, std::string>(name_type, "std::string"); 
}


void test_is_swappable_with()
{
    test::interface("is_swappable_with");

    test_is_swappable_with<void>("void");
    test_is_swappable_with<nullptr_t>("nullptr_t");

    test_is_swappable_with<char>("char");
    test_is_swappable_with<char&>("char&");
    test_is_swappable_with<int>("int");
    test_is_swappable_with<int&>("int&");
    test_is_swappable_with<long>("long");
    test_is_swappable_with<long&>("long&");
    test_is_swappable_with<long long>("long long");
    test_is_swappable_with<long long&>("long long&");
    test_is_swappable_with<float>("float");
    test_is_swappable_with<float&>("float&");
    test_is_swappable_with<double>("double");
    test_is_swappable_with<double&>("double&");

    test_is_swappable_with<int[]>("int[]");
    test_is_swappable_with<int[3]>("int[3]");
    test_is_swappable_with<int[][3]>("int[][3]");

    test_is_swappable_with<Class>("Class");
    test_is_swappable_with<Class&>("Class&");
    test_is_swappable_with<Class2>("Class2");
    test_is_swappable_with<Class2&>("Class2&");
    test_is_swappable_with<Union>("Union");
    test_is_swappable_with<Enum>("Enum");
    test_is_swappable_with<Enum_class>("Enum_class");

    test_is_swappable_with<Class>("Class");
    test_is_swappable_with<int Class::*>("int Class::*");
    test_is_swappable_with<int (Class::*)()>("int (Class::*)()");
}


template <typename T, typename U>
void test_is_trivially_assignable(const std::string& name_type1, 
		      const std::string& name_type2)
{
    CHECK_TRUE(mtd::is_trivially_assignable_v<T, U> ==
			     std::is_trivially_assignable_v<T, U>,
		alp::as_str() << name_type1 << ", " << name_type2);
}


template <typename T>
void test_is_trivially_assignable(const std::string& name_type)
{
    test_is_trivially_assignable<T, void>(name_type, "void");

    test_is_trivially_assignable<T, nullptr_t>(name_type, "nullptr_t");

    test_is_trivially_assignable<T, char>(name_type, "char");
    test_is_trivially_assignable<T, char&>(name_type, "char&");
    test_is_trivially_assignable<T, int>(name_type, "int");
    test_is_trivially_assignable<T, int&>(name_type, "int&");
    test_is_trivially_assignable<T, long>(name_type, "long");
    test_is_trivially_assignable<T, long&>(name_type, "long&");
    test_is_trivially_assignable<T, long long>(name_type, "long long");
    test_is_trivially_assignable<T, long long&>(name_type, "long long&");
    test_is_trivially_assignable<T, float>(name_type, "float");
    test_is_trivially_assignable<T, float&>(name_type, "float&");
    test_is_trivially_assignable<T, double>(name_type, "double");
    test_is_trivially_assignable<T, double&>(name_type, "double&");

    test_is_trivially_assignable<T, int[]>(name_type, "int[]");
    test_is_trivially_assignable<T, int[3]>(name_type, "int[3]");
    test_is_trivially_assignable<T, int[][3]>(name_type, "int[][3]");

    test_is_trivially_assignable<T, Class>(name_type, "Class");
    test_is_trivially_assignable<T, Class&>(name_type, "Class&");
    test_is_trivially_assignable<T, Class2>(name_type, "Class2");
    test_is_trivially_assignable<T, Class2&>(name_type, "Class2&");
    test_is_trivially_assignable<T, Union>(name_type, "Union");
    test_is_trivially_assignable<T, Enum>(name_type, "Enum");
    test_is_trivially_assignable<T, Enum_class>(name_type, "Enum_class");

    test_is_trivially_assignable<T, Class>(name_type, "Class");
    test_is_trivially_assignable<T, int Class::*>(name_type, "int Class::*");
//    test_is_trivially_assignable<T, int (name_type, Class::*)()>("int (Class::*)()");

    test_is_trivially_assignable<T, std::string>(name_type, "std::string"); 
}


void test_is_trivially_assignable()
{
    test::interface("is_trivially_assignable");

    test_is_trivially_assignable<void>("void");
    test_is_trivially_assignable<nullptr_t>("nullptr_t");

    test_is_trivially_assignable<char>("char");
    test_is_trivially_assignable<char&>("char&");
    test_is_trivially_assignable<int>("int");
    test_is_trivially_assignable<int&>("int&");
    test_is_trivially_assignable<long>("long");
    test_is_trivially_assignable<long&>("long&");
    test_is_trivially_assignable<long long>("long long");
    test_is_trivially_assignable<long long&>("long long&");
    test_is_trivially_assignable<float>("float");
    test_is_trivially_assignable<float&>("float&");
    test_is_trivially_assignable<double>("double");
    test_is_trivially_assignable<double&>("double&");

    test_is_trivially_assignable<int[]>("int[]");
    test_is_trivially_assignable<int[3]>("int[3]");
    test_is_trivially_assignable<int[][3]>("int[][3]");

    test_is_trivially_assignable<Class>("Class");
    test_is_trivially_assignable<Class&>("Class&");
    test_is_trivially_assignable<Class2>("Class2");
    test_is_trivially_assignable<Class2&>("Class2&");
    test_is_trivially_assignable<Union>("Union");
    test_is_trivially_assignable<Enum>("Enum");
    test_is_trivially_assignable<Enum_class>("Enum_class");

    test_is_trivially_assignable<Class>("Class");
    test_is_trivially_assignable<int Class::*>("int Class::*");
    test_is_trivially_assignable<int (Class::*)()>("int (Class::*)()");
}






template <typename T>
void test_make_signed(const std::string& name)
{
    CHECK_TRUE((std::is_same_v<mtd::make_signed_t<T>, std::make_signed_t<T>>),
               name);
}

void test_make_signed()
{
    test::interface("make_signed");

    test_make_signed<char>("char");
    test_make_signed<short>("short");
    test_make_signed<int>("int");
    test_make_signed<long>("long");
    test_make_signed<long long>("long long");

    test_make_signed<unsigned char>("unsigned char");
    test_make_signed<unsigned short>("unsigned short");
    test_make_signed<unsigned int>("unsigned int");
    test_make_signed<unsigned long>("unsigned long");
    test_make_signed<unsigned long long>("unsigned long long");

}

template <typename T>
void test_make_unsigned(const std::string& name)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::make_unsigned_t<T>, std::make_unsigned_t<T>>),
        name);
}



void test_make_unsigned()
{
    test::interface("make_unsigned");

    test_make_unsigned<char>("char");
    test_make_unsigned<short>("short");
    test_make_unsigned<int>("int");
    test_make_unsigned<long>("long");
    test_make_unsigned<long long>("long long");

    test_make_unsigned<unsigned char>("unsigned char");
    test_make_unsigned<unsigned short>("unsigned short");
    test_make_unsigned<unsigned int>("unsigned int");
    test_make_unsigned<unsigned long>("unsigned long");
    test_make_unsigned<unsigned long long>("unsigned long long");
}

void test_is_enum()
{
    test::interface("is_enum");
    CHECK_TRUE(mtd::is_enum_v<Class> == std::is_enum_v<Class>, "Class");
    CHECK_TRUE(mtd::is_enum_v<Union> == std::is_enum_v<Union>, "Union");
    CHECK_TRUE(mtd::is_enum_v<Enum> == std::is_enum_v<Enum>, "Enum");
    CHECK_TRUE(mtd::is_enum_v<Enum_class> == std::is_enum_v<Enum_class>, "Enum_class");
    CHECK_TRUE(mtd::is_enum_v<int> == std::is_enum_v<int>, "int");
}


void test_is_union()
{
     
    struct C { Union d; };

    test::interface("is_union");
    CHECK_TRUE(mtd::is_union_v<Class> == std::is_union_v<Class>, "Class");
    CHECK_TRUE(mtd::is_union_v<Union> == std::is_union_v<Union>, "Union");
    CHECK_TRUE(mtd::is_union_v<C> == std::is_union_v<C>, "C");
    CHECK_TRUE(mtd::is_union_v<int> == std::is_union_v<int>, "int");
}


void test_is_class()
{
    struct A {};
    class B {};
    enum class E {};
    union U { class UC {}; };
    CHECK_TRUE(mtd::is_class_v<U> == std::is_class_v<U>, "U");
    CHECK_TRUE(mtd::is_class_v<U::UC> == std::is_class_v<U::UC>, "U::UC");
 
    CHECK_TRUE(mtd::is_class<A>::value == std::is_class<A>::value, "A");
    CHECK_TRUE(mtd::is_class_v<B> == std::is_class_v<B>, "B");
    CHECK_TRUE(mtd::is_class_v<B*> == std::is_class_v<B*>, "B*");
    CHECK_TRUE(mtd::is_class_v<B&> == std::is_class_v<B&>, "B&");
    CHECK_TRUE(mtd::is_class_v<const B> == std::is_class_v<const B>, "const B");
    CHECK_TRUE(mtd::is_class<E>::value == std::is_class<E>::value, "E");
    CHECK_TRUE(mtd::is_class_v<int> == std::is_class_v<int>, "int");
    CHECK_TRUE(mtd::is_class_v<struct S> == std::is_class_v<struct S>, "S");
    CHECK_TRUE(mtd::is_class_v<class C> == std::is_class_v<class C>, "C");
}

template <typename T>
void test_add_const(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::add_const_t<T>, std::add_const_t<T>>, 
		alp::as_str() << "add_const(" << name_type << ")");
}

void test_add_const()
{
    test::interface("add_const");
 
    test_add_const<int>("int");
    test_add_const<const int>("const int");
    test_add_const<volatile int>("volatile int");
    test_add_const<const volatile int>("const volatile int");
}

template <typename T>
void test_add_volatile(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::add_volatile_t<T>, std::add_volatile_t<T>>, 
		alp::as_str() << "add_volatile(" << name_type << ")");
}

void test_add_volatile()
{
    test::interface("add_volatile");
 
    test_add_volatile<int>("int");
    test_add_volatile<const int>("const int");
    test_add_volatile<volatile int>("volatile int");
    test_add_volatile<const volatile int>("const volatile int");
}


template <typename T>
void test_add_cv(const std::string& name_type)
{
    CHECK_TRUE(std::is_same_v<mtd::add_cv_t<T>, std::add_cv_t<T>>, 
		alp::as_str() << "add_cv(" << name_type << ")");
}
void test_add_cv()
{
    test::interface("add_cv");

    test_add_cv<int>("int");
    test_add_cv<const int>("const int");
    test_add_cv<volatile int>("volatile int");
    test_add_cv<const volatile int>("const volatile int");
}

template <typename T>
void test_remove_cvref(const char* name_type)
{
    bool res = std::is_same_v<mtd::remove_cvref_t<T>, std::remove_cvref_t<T>>;
    CHECK_TRUE(res, alp::as_str() << "remove_cvref_t(" << name_type << ")");
}

void test_remove_cvref()
{
    test::interface("remove_cvref");

    test_remove_cvref<int>("int");
    test_remove_cvref<int&>("int&");
    test_remove_cvref<int&&>("int&&");
    test_remove_cvref<const int&>("const int&");
    test_remove_cvref<const int[2]>("const int[2]");
    test_remove_cvref<const int(&)[2]>("const int(&)[2]");
    test_remove_cvref<int(int)>("const int(int)");
}



int main()
{
try{
    // helper class
    // -----------
    // test_integral_constant(); (en std_first.h)
    // test_is_same();		 (en std_first.h)

    // primary type categories
    // -----------------------
    // test_is_void();		    (en std_first.h)
    test_is_null_pointer();
    test_is_integral();
    test_is_floating_point();
//    test_is_array();		    (en std_first.h)
    test_is_pointer();
//    test_is_lvalue_reference();   (en std_first.h)
//    test_is_rvalue_reference();   (en std_first.h)
    test_is_member_object_pointer();
//    test_is_member_function_pointer(); <--?
    test_is_enum();
    test_is_union();
    test_is_class();

//    test_is_function();   TODO: ¿cómo la pruebo?
//    test_is_function();   TOD_noexceptO: ¿cómo la pruebo?

    
    // composite type categories
    // -------------------------
    // test_is_reference();	    (en std_first.h)
    test_is_arithmetic();
    test_is_fundamental();
    test_is_object();
    test_is_scalar();
    test_is_compound();
    test_is_member_pointer();

    // type properties
    // ---------------
    // test_is_const(); (en std_first.h)
    // test_is_volatile(); (en std_first.h)
    test_is_trivial();
    test_is_trivially_copyable();
    test_is_standard_layout();
    test_is_empty();
    test_is_polymorphic();
    test_is_abstract();
    test_is_final();
    test_is_aggregate();
    test_is_signed();
    test_is_unsigned();
    test_is_bounded_array();
    test_is_unbounded_array();
    test_is_constructible();
    test_is_default_constructible();
    test_is_copy_constructible();
    // test_is_move_constructible();		(en std_first.h)
    test_is_assignable();
    test_is_copy_assignable();
    // test_is_move_assignable();		(en std_first.h)
    test_is_swappable_with();
    // test_is_swappable();			(en std_first.h)

    // ¿cómo hacer el test de is_nothrow_constructible???
    // test_is_nothrow_constructible();
    test_is_nothrow_default_constructible();
    test_is_nothrow_copy_constructible();
    // test_is_nothrow_move_constructible();	(en std_first.h)
    test_is_nothrow_assignable();
    test_is_nothrow_copy_assignable();
    // test_is_nothrow_move_assignable();	(en std_first.h)
    test_is_nothrow_swappable_with();
    test_is_nothrow_swappable();

    test_is_destructible();
    test_is_trivially_constructible();
    test_is_trivially_default_constructible();
    test_is_trivially_constructible();
    test_is_trivially_copy_constructible();;
    test_is_trivially_move_constructible();;
    test_is_trivially_assignable();;
    test_is_trivially_copy_assignable();;
    test_is_trivially_move_assignable();;
    test_is_trivially_destructible();;


    test_is_nothrow_destructible();
    test_has_virtual_destructor();
    test_has_unique_object_representations();

    // type properties queries
    // -----------------------
    // test_extent();		(en std_first.h)

    // reference modifications
    // -----------------------
    // test_remove_reference(); (en std_first.h)
    test_add_lvalue_reference();
    test_add_rvalue_reference();

    // sign modifications
    // ------------------
    test_make_signed();
    test_make_unsigned();

    // type relations
    // --------------
    test_is_base_of();
    test_is_convertible();
    test_is_nothrow_convertible();
 
    // const-volatile modifications
    // ----------------------------
    // test_remove_const();	(en std_first.h)
    test_add_const();
    test_add_volatile();
    test_add_cv();
	
    // array modifications
    // --------------------
    test_remove_extent();
    test_remove_all_extents();

    // pointer modifications
    // ---------------------
    test_remove_pointer();
    test_add_pointer();

    // other transformations
    // ---------------------
    test_remove_cvref();
    test_decay();
    test_enable_if();
    test_common_type();
    test_common_reference();
    test_conditional();


}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





