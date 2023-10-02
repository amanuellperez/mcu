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

typedef union
{
    int a;
    float b;
} Union;

enum Enum {black, white};
enum class Enum_class{ one, two, three };

void test_integral_constant()
{
    std::cout << "Probando mtd::integral_constant\n";
    std::cout << "-------------------------------\n";

    using two_t = mtd::integral_constant<int, 2>;
    using four_t = mtd::integral_constant<int, 4>;


    CHECK_TRUE(two_t::value == 2, "integral_constant::value");
    CHECK_TRUE(two_t::value * 2 == four_t::value, "varios");   

    two_t two;
    CHECK_TRUE(two() == 2, "operator()");
    int x = two;
    CHECK_TRUE(x == 2, "operator value_type()");


    CHECK_TRUE(mtd::true_type::value == true, "true_type");
    CHECK_TRUE(mtd::false_type::value == false, "false_type");


    // is_same
    CHECK_TRUE(mtd::is_same<int, int>::value == true, "is_same(true)");
    CHECK_TRUE(mtd::is_same<int, float>::value == false, "is_same(false)");

    CHECK_TRUE(mtd::is_same_v<char, char> == true, "is_same_v(true)");
    CHECK_TRUE(mtd::is_same_v<int, float> == false, "is_same_v(false)");

    // remove_const
    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_const<int>::type, int> == true
		, "remove_const(int)");
    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_const<const int>::type, int> == true
		, "remove_const(const int)");

    CHECK_TRUE(mtd::is_same_v<mtd::remove_const_t<int>, int> == true
		, "remove_const_t(int)");
    CHECK_TRUE(mtd::is_same_v<mtd::remove_const_t<const int>, int> == true
		, "remove_const_t(const int)");

    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_volatile<int>::type, int> == true
		, "remove_volatile(int)");
    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_volatile<volatile int>::type, int> == true
		, "remove_volatile(volatile int)");

    CHECK_TRUE(mtd::is_same_v<mtd::remove_volatile_t<int>, int> == true
		, "remove_volatile_t(int)");
    CHECK_TRUE(mtd::is_same_v<mtd::remove_volatile_t<volatile int>, int> == true
		, "remove_volatile_t(volatile int)");


    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_cv<int>::type, int> == true
		, "remove_cv(int)");
    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_cv<const int>::type, int> == true
		, "remove_cv(const int)");
    CHECK_TRUE(mtd::is_same_v<typename mtd::remove_cv<volatile int>::type, int> == true
		, "remove_cv(volatile int)");

    CHECK_TRUE((mtd::is_same_v<mtd::remove_cv_t<int>, int> == true)
		, "remove_cv_t(int)");
    CHECK_TRUE((mtd::is_same_v<mtd::remove_cv_t<const int>, int> == true)
		, "remove_cv_t(const int)");
    CHECK_TRUE((mtd::is_same_v<mtd::remove_cv_t<volatile int>, int> == true)
		, "remove_cv_t(volatile int)");
}

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

void test_is_const()
{
    test::interface("is_const");

    CHECK_TRUE(!mtd::is_const_v<char>, "char");
    CHECK_TRUE(!mtd::is_const_v<int>, "int");
    CHECK_TRUE(!mtd::is_const_v<long>, "long");
    CHECK_TRUE(!mtd::is_const_v<long long>, "long long");
    CHECK_TRUE(!mtd::is_const_v<float>, "float");
    CHECK_TRUE(!mtd::is_const_v<double>, "double");

    CHECK_TRUE(mtd::is_const_v<const char>, "const char");
    CHECK_TRUE(mtd::is_const_v<const int>, "const int");
    CHECK_TRUE(mtd::is_const_v<const long>, "const long");
    CHECK_TRUE(mtd::is_const_v<const long long>, "const long long");
    CHECK_TRUE(mtd::is_const_v<const float>, "const float");
    CHECK_TRUE(mtd::is_const_v<const double>, "const double");
}

template <typename T>
void test_is_volatile(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_volatile_v<T> ==
	       std::is_volatile_v<T>, 
	       alp::as_str() << "is_volatile(" << name_type << ")");
}

void test_is_volatile()
{
    test::interface("is_volatile");

    test_is_volatile<char>("char");
    test_is_volatile<int>("int");
    test_is_volatile<long>("long");
    test_is_volatile<long long>("long long");
    test_is_volatile<float>("float");
    test_is_volatile<double>("double");

    test_is_volatile<volatile char>("volatile char");
    test_is_volatile<volatile int>("volatile int");
    test_is_volatile<volatile long>("volatile long");
    test_is_volatile<volatile long long>("volatile long long");
    test_is_volatile<volatile float>("volatile float");
    test_is_volatile<volatile double>("volatile double");
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
    test_is_constructible<D, int, float>("D, int, float");


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



}



template <typename T>
void test_extent(const std::string& name_type)
{
    CHECK_TRUE(mtd::extent_v<T> == 
	       std::extent_v<T>, name_type);
    CHECK_TRUE(mtd::extent_v<const T> == 
	       std::extent_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::extent_v<volatile T> == 
	       std::extent_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::extent_v<const volatile T> == 
	       std::extent_v<const volatile T>, alp::as_str() << "const volatile " << name_type);
}


void test_extent()
{
    test::interface("extent");

    test_extent<void>("void");
    test_extent<nullptr_t>("nullptr_t");

    test_extent<char>("char");
    test_extent<char>("char");
    test_extent<int>("int");
    test_extent<long>("long");
    test_extent<long long>("long long");
    test_extent<float>("float");
    test_extent<double>("double");

    test_extent<int[]>("int[]");

    test_extent<int[4]>("int[4]");
    test_extent<int[4][2]>("int[4][2]");
    test_extent<int[]>("int[]");
    test_extent<int[][3]>("int[][3]");
}


template <typename T>
void test_is_void(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_void_v<T> == 
	       std::is_void_v<T>, name_type);
    CHECK_TRUE(mtd::is_void_v<const T> == 
	       std::is_void_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_void_v<volatile T> == 
	       std::is_void_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_void_v<const volatile T> == 
	       std::is_void_v<const volatile T>, alp::as_str() << "const volatile " << name_type);
}

void test_is_void()
{
    test::interface("is_void");

    test_is_void<nullptr_t>("nullptr_t");

    test_is_void<void>("void");

    test_is_void<char>("char");
    test_is_void<int>("int");
    test_is_void<long>("long");
    test_is_void<long long>("long long");
    test_is_void<float>("float");
    test_is_void<double>("double");

    test_is_void<unsigned char>("unsigned char");
    test_is_void<unsigned int>("unsigned int");
    test_is_void<unsigned long>("unsigned long");
    test_is_void<unsigned long long>("unsigned long long");

// references
    test_is_void<char&>("char&");
    test_is_void<int&>("int&");
    test_is_void<long&>("long&");
    test_is_void<long long&>("long& long");
    test_is_void<float&>("float&");
    test_is_void<double&>("double&");

    test_is_void<unsigned char&>("unsigned char&");
    test_is_void<unsigned int&>("unsigned int&");
    test_is_void<unsigned long&>("unsigned long&");
    test_is_void<unsigned long long&>("unsigned long long&");

// pointers
    test_is_void<void*>("void*");

    test_is_void<char*>("char*");
    test_is_void<int*>("int*");
    test_is_void<long*>("long*");
    test_is_void<long long*>("long* long");
    test_is_void<float*>("float*");
    test_is_void<double*>("double*");

    test_is_void<unsigned char*>("unsigned char*");
    test_is_void<unsigned int*>("unsigned int*");
    test_is_void<unsigned long*>("unsigned long*");
    test_is_void<unsigned long long*>("unsigned long long*");
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
void test_is_lvalue_reference(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_lvalue_reference_v<T> ==
		std::is_lvalue_reference_v<T>, name_type);
    CHECK_TRUE(	mtd::is_lvalue_reference_v<T&> ==
		std::is_lvalue_reference_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_lvalue_reference_v<T&&> ==
		std::is_lvalue_reference_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_lvalue_reference_v<const T> ==
		std::is_lvalue_reference_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_lvalue_reference_v<const T&> ==
		std::is_lvalue_reference_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_lvalue_reference_v<const T&&> ==
		std::is_lvalue_reference_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_lvalue_reference_v<volatile T> ==
		std::is_lvalue_reference_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_lvalue_reference_v<volatile T&> ==
		std::is_lvalue_reference_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_lvalue_reference_v<volatile T&&> ==
		std::is_lvalue_reference_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_lvalue_reference()
{
    test::interface("is_lvalue_reference");
    
    test_is_lvalue_reference<char>("char");
    test_is_lvalue_reference<int>("int");
    test_is_lvalue_reference<long>("long");
    test_is_lvalue_reference<long long>("long long");
    test_is_lvalue_reference<float>("float");
    test_is_lvalue_reference<double>("double");
}


template <typename T>
void test_is_rvalue_reference(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_rvalue_reference_v<T> ==
		std::is_rvalue_reference_v<T>, name_type);
    CHECK_TRUE(	mtd::is_rvalue_reference_v<T&> ==
		std::is_rvalue_reference_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_rvalue_reference_v<T&&> ==
		std::is_rvalue_reference_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_rvalue_reference_v<const T> ==
		std::is_rvalue_reference_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_rvalue_reference_v<const T&> ==
		std::is_rvalue_reference_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_rvalue_reference_v<const T&&> ==
		std::is_rvalue_reference_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_rvalue_reference_v<volatile T> ==
		std::is_rvalue_reference_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_rvalue_reference_v<volatile T&> ==
		std::is_rvalue_reference_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_rvalue_reference_v<volatile T&&> ==
		std::is_rvalue_reference_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_rvalue_reference()
{
    test::interface("is_rvalue_reference");
    
    test_is_rvalue_reference<char>("char");
    test_is_rvalue_reference<int>("int");
    test_is_rvalue_reference<long>("long");
    test_is_rvalue_reference<long long>("long long");
    test_is_rvalue_reference<float>("float");
    test_is_rvalue_reference<double>("double");
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
void test_is_reference(const std::string& name_type)
{
    CHECK_TRUE(	mtd::is_reference_v<T> ==
		std::is_reference_v<T>, name_type);
    CHECK_TRUE(	mtd::is_reference_v<T&> ==
		std::is_reference_v<T&>, alp::as_str() << name_type << '&');
    CHECK_TRUE(	mtd::is_reference_v<T&&> ==
		std::is_reference_v<T&&>, alp::as_str() << name_type << "&&");

    CHECK_TRUE(	mtd::is_reference_v<const T> ==
		std::is_reference_v<const T>, alp::as_str() << "const " << name_type);
    CHECK_TRUE(	mtd::is_reference_v<const T&> ==
		std::is_reference_v<const T&>, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(	mtd::is_reference_v<const T&&> ==
		std::is_reference_v<const T&&>, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(	mtd::is_reference_v<volatile T> ==
		std::is_reference_v<volatile T>, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(	mtd::is_reference_v<volatile T&> ==
		std::is_reference_v<volatile T&>, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(	mtd::is_reference_v<volatile T&&> ==
		std::is_reference_v<volatile T&&>, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_reference()
{
    test::interface("is_reference");
    
    test_is_reference<char>("char");
    test_is_reference<int>("int");
    test_is_reference<long>("long");
    test_is_reference<long long>("long long");
    test_is_reference<float>("float");
    test_is_reference<double>("double");

    test_is_reference<Class>("Class");
    test_is_reference<Union>("Union");
    test_is_reference<Enum>("Enum");
    test_is_reference<Enum_class>("Enum_class");
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
void test_remove_reference_helper(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::remove_reference_t<T>, std::remove_reference_t<T>>),
        name_type);
}

template <typename T>
void test_remove_reference(const std::string& tname)
{
    test_remove_reference_helper<T>(tname);
    test_remove_reference_helper<T&>(alp::as_str() << tname << '&');
    test_remove_reference_helper<T&&>(alp::as_str() << tname << "&&");
    test_remove_reference_helper<T[]>(alp::as_str() << tname << "[]");
    test_remove_reference_helper<const T>(alp::as_str() << "const " << tname);
    test_remove_reference_helper<const T&>(alp::as_str() << "const " << tname << '&');
    test_remove_reference_helper<volatile T>(alp::as_str() << "volatile " << tname);
    test_remove_reference_helper<volatile T&>(alp::as_str()<< "volatile " << tname << '&');
    test_remove_reference_helper<T*>(alp::as_str() << tname << '*');
    test_remove_reference_helper<const T*>(alp::as_str() << "const " << tname << '*');
    test_remove_reference_helper<volatile T*>(alp::as_str() << "volatile " << tname << '*');
    test_remove_reference_helper<const volatile T*>(alp::as_str() << "const volatile " << tname << '*');
}


void test_remove_reference()
{
    test::interface("remove_reference");
    
    test_remove_reference<char>("char");
    test_remove_reference<int>("int");
    test_remove_reference<long>("long");
    test_remove_reference<long long>("long long");
    test_remove_reference<float>("float");
    test_remove_reference<double>("double");
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


template <typename T>
void test_is_array(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_array_v<T> == std::is_array_v<T>, name_type);
}

void test_is_array()
{
    test::interface("is_array");

    test_is_array<char>("char");
    test_is_array<char[]>("char[]");
    test_is_array<char[20]>("char[20]");
    test_is_array<char*>("char*");
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
void test_is_convertible(const std::string& name_type)
{
    CHECK_TRUE((mtd::is_convertible_v<From, To> ==
                   std::is_convertible_v<From, To>),
               name_type);
}

void test_is_convertible()
{
    test::interface("is_convertible");

    // TODO: for (from = char...) for (to = char...)
    test_is_convertible<char, void>("char -> void");
    test_is_convertible<char[], char*>("char[] -> char*");
    test_is_convertible<char*, char[]>("char* -> char[]");
    test_is_convertible<const int, int>("const int -> int");
    test_is_convertible<int, const int>("int -> const int");
    test_is_convertible<int, const int*>("int -> const int*");
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
void test_remove_extent_helper(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::remove_extent_t<T>, std::remove_extent_t<T>>),
        name_type);
}

template <typename T>
void test_remove_extent(const std::string& tname)
{
    test_remove_extent_helper<T>(tname);
    test_remove_extent_helper<T&>(alp::as_str() << tname << '&');
    test_remove_extent_helper<const T>(alp::as_str() << "const " << tname);
    test_remove_extent_helper<const T&>(alp::as_str() << "const " << tname << '&');
    test_remove_extent_helper<volatile T>(alp::as_str() << "volatile " << tname);
    test_remove_extent_helper<volatile T&>(alp::as_str()<< "volatile " << tname << '&');
    test_remove_extent_helper<T*>(alp::as_str() << tname << '*');
    test_remove_extent_helper<const T*>(alp::as_str() << "const " << tname << '*');
    test_remove_extent_helper<volatile T*>(alp::as_str() << "volatile " << tname << '*');
    test_remove_extent_helper<const volatile T*>(alp::as_str() << "const volatile " << tname << '*');
}


void test_remove_extent()
{
    test::interface("remove_extent");

    test_remove_extent<char>("char");
    test_remove_extent<int>("int");
    test_remove_extent<std::string>("string");
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




int main()
{
try{
    // helper class
    // -----------
    test_integral_constant();

    // primary type categories
    // -----------------------
    test_is_void();
    test_is_null_pointer();
    test_is_integral();
    test_is_floating_point();
    test_is_array();
    test_is_pointer();
    test_is_lvalue_reference();
    test_is_rvalue_reference();
    test_is_member_object_pointer();
    test_is_member_function_pointer();
    test_is_enum();
    test_is_union();
    test_is_class();

//    test_is_function();   TODO: ¿cómo la pruebo?

    
    // composite type categories
    // -------------------------
    test_is_reference();
    test_is_arithmetic();
    test_is_fundamental();
    test_is_object();
    test_is_scalar();
    test_is_compound();
    test_is_member_pointer();

    // type properties
    // ---------------
    test_is_const();
    test_is_volatile();
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

    // type properties queries
    // -----------------------
    test_extent();

    // reference modifications
    // -----------------------
    test_remove_reference();
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
 
    // const-volatile modifications
    // ----------------------------
    test_add_const();
    test_add_volatile();
    test_add_cv();
	
    // array modifications
    // --------------------
    test_remove_extent();

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





