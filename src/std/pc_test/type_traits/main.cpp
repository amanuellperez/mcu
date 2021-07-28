// Copyright (C) 2019-2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

// Tipos básicos a probar:
// void
// char, int, long, long long, float, double
// Modificadores: char*, char&, char&&
// Modificadores: const char, volatile char
//
// TODO: cambiar los test como los últimos. Comparar el comportamiento 
// de mtd frente a std, de esa forma es más sencillo probarlo todo.
#include "../../std_type_traits.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>

using namespace test;

struct Prueba{};

void test_integral_constant()
{
    std::cout << "Probando mtd::integral_constant\n";
    std::cout << "-------------------------------\n";

    using two_t = mtd::integral_constant<int, 2>;
    using four_t = mtd::integral_constant<int, 4>;


    check_true(two_t::value == 2, "integral_constant::value");
    check_true(two_t::value * 2 == four_t::value, "varios");   

    two_t two;
    check_true(two() == 2, "operator()");
    int x = two;
    check_true(x == 2, "operator value_type()");


    check_true(mtd::true_type::value == true, "true_type");
    check_true(mtd::false_type::value == false, "false_type");


    // is_same
    check_true(mtd::is_same<int, int>::value == true, "is_same(true)");
    check_true(mtd::is_same<int, float>::value == false, "is_same(false)");

    check_true(mtd::is_same_v<char, char> == true, "is_same_v(true)");
    check_true(mtd::is_same_v<int, float> == false, "is_same_v(false)");

    // remove_const
    check_true(mtd::is_same_v<typename mtd::remove_const<int>::type, int> == true
		, "remove_const(int)");
    check_true(mtd::is_same_v<typename mtd::remove_const<const int>::type, int> == true
		, "remove_const(const int)");

    check_true(mtd::is_same_v<mtd::remove_const_t<int>, int> == true
		, "remove_const_t(int)");
    check_true(mtd::is_same_v<mtd::remove_const_t<const int>, int> == true
		, "remove_const_t(const int)");

    check_true(mtd::is_same_v<typename mtd::remove_volatile<int>::type, int> == true
		, "remove_volatile(int)");
    check_true(mtd::is_same_v<typename mtd::remove_volatile<volatile int>::type, int> == true
		, "remove_volatile(volatile int)");

    check_true(mtd::is_same_v<mtd::remove_volatile_t<int>, int> == true
		, "remove_volatile_t(int)");
    check_true(mtd::is_same_v<mtd::remove_volatile_t<volatile int>, int> == true
		, "remove_volatile_t(volatile int)");


    check_true(mtd::is_same_v<typename mtd::remove_cv<int>::type, int> == true
		, "remove_cv(int)");
    check_true(mtd::is_same_v<typename mtd::remove_cv<const int>::type, int> == true
		, "remove_cv(const int)");
    check_true(mtd::is_same_v<typename mtd::remove_cv<volatile int>::type, int> == true
		, "remove_cv(volatile int)");

    check_true((mtd::is_same_v<mtd::remove_cv_t<int>, int> == true)
		, "remove_cv_t(int)");
    check_true((mtd::is_same_v<mtd::remove_cv_t<const int>, int> == true)
		, "remove_cv_t(const int)");
    check_true((mtd::is_same_v<mtd::remove_cv_t<volatile int>, int> == true)
		, "remove_cv_t(volatile int)");

    // is_integral
    check_true(mtd::is_integral<bool>::value == true, "is_integral<bool>");
    check_true(mtd::is_integral<const bool>::value == true, "is_integral<const bool>");
    check_true(mtd::is_integral<volatile bool>::value == true, "is_integral<volatile bool>");
    check_true(mtd::is_integral<char>::value == true, "is_integral<char>");
    check_true(mtd::is_integral<const char>::value == true, "is_integral<const char>");
    check_true(mtd::is_integral<volatile char>::value == true, "is_integral<volatile char>");
    check_true(mtd::is_integral<signed char>::value == true, "is_integral<signed char>");
    check_true(mtd::is_integral<const signed char>::value == true, "is_integral<const signed char>");
    check_true(mtd::is_integral<volatile signed char>::value == true, "is_integral<volatile signed char>");
    check_true(mtd::is_integral<unsigned char>::value == true, "is_integral<unsigned char>");
    check_true(mtd::is_integral<const unsigned char>::value == true, "is_integral<const unsigned char>");
    check_true(mtd::is_integral<volatile unsigned char>::value == true, "is_integral<volatile unsigned char>");
    check_true(mtd::is_integral<char16_t>::value == true, "is_integral<char16_t>");
    check_true(mtd::is_integral<const char16_t>::value == true, "is_integral<const char16_t>");
    check_true(mtd::is_integral<volatile char16_t>::value == true, "is_integral<volatile char16_t>");
    check_true(mtd::is_integral<char32_t>::value == true, "is_integral<char32_t>");
    check_true(mtd::is_integral<const char32_t>::value == true, "is_integral<const char32_t>");
    check_true(mtd::is_integral<volatile char32_t>::value == true, "is_integral<volatile char32_t>");
    check_true(mtd::is_integral<short>::value == true, "is_integral<short>");
    check_true(mtd::is_integral<const short>::value == true, "is_integral<const short>");
    check_true(mtd::is_integral<volatile short>::value == true, "is_integral<volatile short>");
    check_true(mtd::is_integral<unsigned short>::value == true, "is_integral<unsigned short>");
    check_true(mtd::is_integral<const unsigned short>::value == true, "is_integral<const unsigned short>");
    check_true(mtd::is_integral<volatile unsigned short>::value == true, "is_integral<volatile unsigned short>");
    check_true(mtd::is_integral<int>::value == true, "is_integral<int>");
    check_true(mtd::is_integral<const int>::value == true, "is_integral<const int>");
    check_true(mtd::is_integral<volatile int>::value == true, "is_integral<volatile int>");
    check_true(mtd::is_integral<unsigned int>::value == true, "is_integral<unsigned int>");
    check_true(mtd::is_integral<const unsigned int>::value == true, "is_integral<const unsigned int>");
    check_true(mtd::is_integral<volatile unsigned int>::value == true, "is_integral<volatile unsigned int>");
    check_true(mtd::is_integral<long>::value == true, "is_integral<long>");
    check_true(mtd::is_integral<const long>::value == true, "is_integral<const long>");
    check_true(mtd::is_integral<volatile long>::value == true, "is_integral<volatile long>");
    check_true(mtd::is_integral<unsigned long>::value == true, "is_integral<unsigned long>");
    check_true(mtd::is_integral<const unsigned long>::value == true, "is_integral<const unsigned long>");
    check_true(mtd::is_integral<volatile unsigned long>::value == true, "is_integral<volatile unsigned long>");
    check_true(mtd::is_integral<long long>::value == true, "is_integral<long long>");
    check_true(mtd::is_integral<const long long>::value == true, "is_integral<const long long>");
    check_true(mtd::is_integral<volatile long long>::value == true, "is_integral<volatile long long>");
    check_true(mtd::is_integral<unsigned long long>::value == true, "is_integral<unsigned long long>");
    check_true(mtd::is_integral<const unsigned long long>::value == true, "is_integral<const unsigned long long>");
    check_true(mtd::is_integral<volatile unsigned long long>::value == true, "is_integral<volatile unsigned long long>");

    check_true(mtd::is_integral<float>::value == false, "is_integral<float>");
    check_true(mtd::is_integral<const float>::value == false, "is_integral<const float>");
    check_true(mtd::is_integral<volatile float>::value == false, "is_integral<volatile float>");
    check_true(mtd::is_integral<double>::value == false, "is_integral<double>");
    check_true(mtd::is_integral<const double>::value == false, "is_integral<const double>");
    check_true(mtd::is_integral<volatile double>::value == false, "is_integral<volatile double>");
    check_true(mtd::is_integral<Prueba>::value == false, "is_integral<Prueba>");
    check_true(mtd::is_integral<const Prueba>::value == false, "is_integral<const Prueba>");
    check_true(mtd::is_integral<volatile Prueba>::value == false, "is_integral<volatile Prueba>");

    check_true(mtd::is_integral_v<int> == true, "is_integral_v<int>");
    check_true(mtd::is_integral_v<Prueba> == false, "is_integral_v<Prueba>");

    check_true(mtd::is_floating_point<float>::value == true, "is_floating_point<float>");
    check_true(mtd::is_floating_point<const float>::value == true, "is_floating_point<const float>");
    check_true(mtd::is_floating_point<volatile float>::value == true, "is_floating_point<volatile float>");
    check_true(mtd::is_floating_point<double>::value == true, "is_floating_point<double>");
    check_true(mtd::is_floating_point<const double>::value == true, "is_floating_point<const double>");
    check_true(mtd::is_floating_point<volatile double>::value == true, "is_floating_point<volatile double>");
    check_true(mtd::is_floating_point<long double>::value == true, "is_floating_point<long double>");
    check_true(mtd::is_floating_point<const long double>::value == true, "is_floating_point<const long double>");
    check_true(mtd::is_floating_point<volatile long double>::value == true, "is_floating_point<volatile long double>");

    check_true(mtd::is_floating_point<int>::value == false, "is_floating_point<int>");
    check_true(mtd::is_floating_point<Prueba>::value == false, "is_floating_point<Prueba>");

    check_true(mtd::is_floating_point_v<long double> == true, "is_floating_point_v<long double>");
    check_true(mtd::is_floating_point_v<char> == false, "is_floating_point_v<char>");


    // is_arithmetic
    check_true(mtd::is_arithmetic<Prueba>::value == false, "is_arithmetic<Prueba>");
    check_true(mtd::is_arithmetic<int>::value == true , "is_arithmetic<int>");
    check_true(mtd::is_arithmetic<int&>::value == false, "is_arithmetic<int&>");
    check_true(mtd::is_arithmetic<int*>::value == false, "is_arithmetic<int*>");
    check_true(mtd::is_arithmetic<float>::value == true, "is_arithmetic<float>");
    check_true(mtd::is_arithmetic<float&>::value == false, "is_arithmetic<float&>");
    check_true(mtd::is_arithmetic<float*>::value == false, "is_arithmetic<float*>");

    check_true(mtd::is_arithmetic_v<float> == true, "is_arithmetic_v<float>");
    check_true(mtd::is_arithmetic_v<float*> == false, "is_arithmetic_v<float*>");

    // is_signed
    check_true(mtd::is_signed<signed int>::value == true, "is_signed<signed int>");
    check_true(mtd::is_signed<unsigned int>::value == false, "is_signed<unsigned int>");

    check_true(mtd::is_signed_v<signed int> == true, "is_signed_v<signed int>");
    check_true(mtd::is_signed_v<unsigned int> == false, "is_signed_v<unsigned int>");

    check_true(mtd::is_unsigned<signed int>::value == false, "is_unsigned<signed int>");
    check_true(mtd::is_unsigned<unsigned int>::value == true, "is_unsigned<unsigned int>");

    check_true(mtd::is_unsigned_v<signed int> == false, "is_unsigned_v<signed int>");
    check_true(mtd::is_unsigned_v<unsigned int> == true, "is_unsigned_v<unsigned int>");

}

void test_conditional()
{
    test::interfaz("conditional_t");

    CHECK_TRUE((mtd::is_same_v<mtd::conditional_t<true, int, long>, int>), 
		"true");
    CHECK_TRUE((mtd::is_same_v<mtd::conditional_t<false, int, long>, long>), 
		"false");
    CHECK_TRUE((mtd::is_same_v<mtd::conditional_t<true, long, int>, long>), 
		"true");
    CHECK_TRUE((mtd::is_same_v<mtd::conditional_t<false, long, int>, int>), 
		"false");
    CHECK_TRUE((mtd::is_same_v<mtd::conditional_t<true, int, int>, int>), 
		"degenerado");
}

void test_is_const()
{
    test::interfaz("is_const");

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

void test_is_volatile()
{
    test::interfaz("is_volatile");

    CHECK_TRUE(!mtd::is_volatile_v<char>, "char");
    CHECK_TRUE(!mtd::is_volatile_v<int>, "int");
    CHECK_TRUE(!mtd::is_volatile_v<long>, "long");
    CHECK_TRUE(!mtd::is_volatile_v<long long>, "long long");
    CHECK_TRUE(!mtd::is_volatile_v<float>, "float");
    CHECK_TRUE(!mtd::is_volatile_v<double>, "double");

    CHECK_TRUE(mtd::is_volatile_v<volatile char>, "volatile char");
    CHECK_TRUE(mtd::is_volatile_v<volatile int>, "volatile int");
    CHECK_TRUE(mtd::is_volatile_v<volatile long>, "volatile long");
    CHECK_TRUE(mtd::is_volatile_v<volatile long long>, "volatile long long");
    CHECK_TRUE(mtd::is_volatile_v<volatile float>, "volatile float");
    CHECK_TRUE(mtd::is_volatile_v<volatile double>, "volatile double");
}

template <typename T>
void test_is_void(const std::string& name_type, bool res)
{
    CHECK_TRUE(mtd::is_void_v<T> == res, name_type);
    CHECK_TRUE(mtd::is_void_v<const T> == res, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_void_v<volatile T> == res, alp::as_str() << "volatile " << name_type);
}

void test_is_void()
{
    test::interfaz("is_void");

    test_is_void<void>("void", true);

    test_is_void<char>("char", false);
    test_is_void<int>("int", false);
    test_is_void<long>("long", false);
    test_is_void<long long>("long long", false);
    test_is_void<float>("float", false);
    test_is_void<double>("double", false);

    test_is_void<unsigned char>("unsigned char", false);
    test_is_void<unsigned int>("unsigned int", false);
    test_is_void<unsigned long>("unsigned long", false);
    test_is_void<unsigned long long>("unsigned long long", false);
}

template <typename T>
void test_is_lvalue_reference(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_lvalue_reference_v<T> == false, name_type);
    CHECK_TRUE(mtd::is_lvalue_reference_v<T&> == true, alp::as_str() << name_type << '&');
    CHECK_TRUE(mtd::is_lvalue_reference_v<T&&> == false, alp::as_str() << name_type << "&&");

    CHECK_TRUE(mtd::is_lvalue_reference_v<const T> == false, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_lvalue_reference_v<const T&> == true, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(mtd::is_lvalue_reference_v<const T&&> == false, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(mtd::is_lvalue_reference_v<volatile T> == false, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_lvalue_reference_v<volatile T&> == true, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(mtd::is_lvalue_reference_v<volatile T&&> == false, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_lvalue_reference()
{
    test::interfaz("is_lvalue_reference");
    
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
    CHECK_TRUE(mtd::is_rvalue_reference_v<T> == false, name_type);
    CHECK_TRUE(mtd::is_rvalue_reference_v<T&> == false, alp::as_str() << name_type << '&');
    CHECK_TRUE(mtd::is_rvalue_reference_v<T&&> == true, alp::as_str() << name_type << "&&");

    CHECK_TRUE(mtd::is_rvalue_reference_v<const T> == false, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_rvalue_reference_v<const T&> == false, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(mtd::is_rvalue_reference_v<const T&&> == true, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(mtd::is_rvalue_reference_v<volatile T> == false, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_rvalue_reference_v<volatile T&> == false, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(mtd::is_rvalue_reference_v<volatile T&&> == true, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_rvalue_reference()
{
    test::interfaz("is_rvalue_reference");
    
    test_is_rvalue_reference<char>("char");
    test_is_rvalue_reference<int>("int");
    test_is_rvalue_reference<long>("long");
    test_is_rvalue_reference<long long>("long long");
    test_is_rvalue_reference<float>("float");
    test_is_rvalue_reference<double>("double");
}


template <typename T>
void test_is_reference(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_reference_v<T> == false, name_type);
    CHECK_TRUE(mtd::is_reference_v<T&> == true, alp::as_str() << name_type << '&');
    CHECK_TRUE(mtd::is_reference_v<T&&> == true, alp::as_str() << name_type << "&&");

    CHECK_TRUE(mtd::is_reference_v<const T> == false, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_reference_v<const T&> == true, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(mtd::is_reference_v<const T&&> == true, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(mtd::is_reference_v<volatile T> == false, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_reference_v<volatile T&> == true, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(mtd::is_reference_v<volatile T&&> == true, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_reference()
{
    test::interfaz("is_reference");
    
    test_is_reference<char>("char");
    test_is_reference<int>("int");
    test_is_reference<long>("long");
    test_is_reference<long long>("long long");
    test_is_reference<float>("float");
    test_is_reference<double>("double");
}



template <typename T>
void test_is_object(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_object_v<T> == true, name_type);
    CHECK_TRUE(mtd::is_object_v<T&> == false, alp::as_str() << name_type << '&');
    CHECK_TRUE(mtd::is_object_v<T&&> == false, alp::as_str() << name_type << "&&");

    CHECK_TRUE(mtd::is_object_v<const T> == true, alp::as_str() << "const " << name_type);
    CHECK_TRUE(mtd::is_object_v<const T&> == false, alp::as_str() << "const " << name_type << '&');
    CHECK_TRUE(mtd::is_object_v<const T&&> == false, alp::as_str() << "const " << name_type << "&&");

    CHECK_TRUE(mtd::is_object_v<volatile T> == true, alp::as_str() << "volatile " << name_type);
    CHECK_TRUE(mtd::is_object_v<volatile T&> == false, alp::as_str() << "volatile " << name_type << '&');
    CHECK_TRUE(mtd::is_object_v<volatile T&&> == false, alp::as_str() << "volatile " << name_type << "&&");

}

void test_is_object()
{
    test::interfaz("is_object");
    
    test_is_object<char>("char");
    test_is_object<int>("int");
    test_is_object<long>("long");
    test_is_object<long long>("long long");
    test_is_object<float>("float");
    test_is_object<double>("double");
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
    test::interfaz("remove_reference");
    
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
    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<T>, T&>),
               name_type);

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<T&>, T&>),
               alp::as_str() << name_type << "&");

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<const T>, const T&>),
               alp::as_str() << "const " << name_type << "");

    CHECK_TRUE( (std::is_same_v<mtd::add_lvalue_reference_t<const T&>, const T&>),
               alp::as_str() << "const " << name_type << "&");
}

void test_add_lvalue_reference()
{
    test::interfaz("add_lvalue_reference");
    
    test_add_lvalue_reference<char>("char");
    test_add_lvalue_reference<int>("int");
    test_add_lvalue_reference<long>("long");
    test_add_lvalue_reference<long long>("long long");
    test_add_lvalue_reference<float>("float");
    test_add_lvalue_reference<double>("double");
}

template <typename T>
void test_is_array(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_array_v<T> == std::is_array_v<T>, name_type);
}

void test_is_array()
{
    test::interfaz("is_array");

    test_is_array<char>("char");
    test_is_array<char[]>("char[]");
    test_is_array<char[20]>("char[20]");
    test_is_array<char*>("char*");
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
    test::interfaz("is_convertible");

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
    test::interfaz("remove_pointer");

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
    test::interfaz("add_pointer");

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
    test::interfaz("remove_extent");

    test_remove_extent<char>("char");
    test_remove_extent<int>("int");
    test_remove_extent<std::string>("string");
}




template <typename T>
void test_decay_helper(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::decay_t<T>, std::decay_t<T>>),
        name_type);
}

template <typename T>
void test_decay(const std::string& tname)
{
    test_decay_helper<T>(tname);
    test_decay_helper<T&>(alp::as_str() << tname << '&');
    test_decay_helper<T&&>(alp::as_str() << tname << "&&");
    test_decay_helper<T[]>(alp::as_str() << tname << "[]");
    test_decay_helper<const T>(alp::as_str() << "const " << tname);
    test_decay_helper<const T&>(alp::as_str() << "const " << tname << '&');
    test_decay_helper<volatile T>(alp::as_str() << "volatile " << tname);
    test_decay_helper<volatile T&>(alp::as_str()<< "volatile " << tname << '&');
    test_decay_helper<T*>(alp::as_str() << tname << '*');
    test_decay_helper<const T*>(alp::as_str() << "const " << tname << '*');
    test_decay_helper<volatile T*>(alp::as_str() << "volatile " << tname << '*');
    test_decay_helper<const volatile T*>(alp::as_str() << "const volatile " << tname << '*');
}


void test_decay()
{
    test::interfaz("decay");

    test_decay<char>("char");
    test_decay<int>("int");
    test_decay<std::string>("string");
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
    test::interfaz("enable_if");

    test_enable_if<char>("char");
    test_enable_if<int>("int");
    test_enable_if<std::string>("string");
}





template <typename... Ts>
void test_common_type(const std::string& name_type)
{
    CHECK_TRUE(
        (std::is_same_v<mtd::common_type_t<Ts...>, std::common_type_t<Ts...>>),
        name_type);
}


void test_common_type()
{
    test::interfaz("common_type");

//    test_common_type<>("nada"); // tiene que dar error de compilación
    test_common_type<char>("char");
    test_common_type<char, int>("char, int");
    test_common_type<int, unsigned int>("int, unsigned int");
    test_common_type<int, long>("int, long");
//    test_common_type<int, std::string>("int, long"); // esto no tiene que
//    compilar
}

template <typename T>
void test_make_signed(const std::string& name)
{
    CHECK_TRUE((std::is_same_v<mtd::make_signed_t<T>, std::make_signed_t<T>>),
               name);
}

void test_make_signed()
{
    test::interfaz("make_signed");

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
    test::interfaz("make_unsigned");

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

int main()
{
try{
    // helper class
    // -----------
    test_integral_constant();

    // primary type categories
    // -----------------------
    test_is_void();
    test_is_array();
    test_is_lvalue_reference();
    test_is_rvalue_reference();
//    test_is_function();   TODO: ¿cómo la pruebo?

    // composite type categories
    // -------------------------
    test_is_reference();
    test_is_object();

    // type properties
    // ---------------
    test_is_const();
    test_is_volatile();

    // reference modifications
    // -----------------------
    test_remove_reference();
    test_add_lvalue_reference();

    // sign modifications
    // ------------------
    test_make_signed();
    test_make_unsigned();

    // type relations
    // --------------
    test_is_convertible();
 
    // array modifications
    // --------------------
    test_remove_extent();

    // pointer modifications
    // ---------------------
    test_remove_pointer();
    test_add_pointer();

    // other transformations
    // ---------------------
    test_decay();
    test_enable_if();
    test_common_type();
    test_conditional();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





