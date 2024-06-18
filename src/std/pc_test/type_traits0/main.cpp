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

#include "../../std_type_traits0.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <string>


void print_truefalse(bool x, const std::string& msg)
{
    std::cout << msg << "? ";
    if (x) 
	std::cout << "true";
    else
	std::cout << "false";

    std::cout << '\n';
}

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

using namespace test;

void test_integral_constant()
{
    test::interface("integral_constant");

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
void test_is_lvalue_reference(const std::string& name_type)
{
    test::interface("is_lvalue_reference");
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
concept std_swap_compile =
	requires(T& a, T& b){
	    std::swap(a, b);
	};

template <typename T>
concept mtd_swap_compile =
	requires(T& a, T& b){
	    mtd::swap(a, b);
	};

template <typename T>
void test_swap_compile(const std::string& name_type)
{
    if constexpr (std_swap_compile<T> != mtd_swap_compile<T>){
	print_truefalse(std_swap_compile<T>, "std_swap_compile");
	print_truefalse(mtd_swap_compile<T>, "mtd_swap_compile");

	print_truefalse(noexcept(std::is_nothrow_move_constructible_v<T>),
		       "noexcept(std::is_nothrow_move_constructible_v)");
	print_truefalse(noexcept(mtd::is_nothrow_move_constructible_v<T>),
		       "noexcept(mtd::is_nothrow_move_constructible_v)");
	print_truefalse(noexcept(std::is_nothrow_move_assignable_v<T>),
		       "noexcept(std::is_nothrow_move_assignable_v)");
	print_truefalse(noexcept(mtd::is_nothrow_move_assignable_v<T>),
		       "noexcept(mtd::is_nothrow_move_assignable_v)");
	print_truefalse(std::is_move_constructible_v<T>,
		       "std::is_move_constructible_v");
	print_truefalse(mtd::is_move_constructible_v<T>,
		       "mtd::is_move_constructible_v");
	print_truefalse(std::is_move_assignable_v<T>,
		       "std::is_move_assignable_v");
	print_truefalse(mtd::is_move_assignable_v<T>,
		       "mtd::is_move_assignable_v");

	
    }

    CHECK_TRUE(std_swap_compile<T> ==
	       mtd_swap_compile<T>, 
	       alp::as_str() << "swap_compile(" << name_type << ")");
}

void test_swap_compile()
{
    test::interface("swap_compile");

    test_swap_compile<void>("void");
    test_swap_compile<const volatile void>("const volatile void");
    test_swap_compile<nullptr_t>("nullptr_t");

    test_swap_compile<char>("char");
    test_swap_compile<int>("int");
    test_swap_compile<long>("long");
    test_swap_compile<long long>("long long");
    test_swap_compile<float>("float");
    test_swap_compile<double>("double");

    test_swap_compile<int[]>("int[]");
    test_swap_compile<int[3]>("int[3]");
    test_swap_compile<int[][3]>("int[][3]");

    test_swap_compile<Class>("Class");
    test_swap_compile<Union>("Union");
    test_swap_compile<Enum>("Enum");
    test_swap_compile<Enum_class>("Enum_class");

    test_swap_compile<Class>("Class");
    test_swap_compile<Class2>("Class2");
    test_swap_compile<int Class::*>("int Class::*");
    test_swap_compile<int (Class::*)()>("int (Class::*)()");

    test_swap_compile<A>("A");
    test_swap_compile<B>("B");
    test_swap_compile<C>("C");
    test_swap_compile<D>("D");
}

template <typename T>
void test_swap(T x, T y)
{
    T sx = x;
    T sy= y;

    mtd::swap(x, y);
    std::swap(sx, sy);
    CHECK_TRUE(x == sx and y == sy, "swap");
    
}

void test_swap()
{
    test::interface("swap");

    test_swap_compile();

    test_swap<int>(2, 5);
    test_swap<std::string>("uno", "dos");

    // Probar a mirar que compile swap con diferentes tipos
    // Ahora es diferente std::swap(int[]) que mtd::swap(int[]);
}

void test_is_same()
{
    test::interface("is_same");

    CHECK_TRUE(mtd::is_same<int, int>::value == true, "is_same(true)");
    CHECK_TRUE(mtd::is_same<int, float>::value == false, "is_same(false)");

    CHECK_TRUE(mtd::is_same_v<char, char> == true, "is_same_v(true)");
    CHECK_TRUE(mtd::is_same_v<int, float> == false, "is_same_v(false)");
}


void test_remove_const()
{
    test::interface("remove_const");

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
void test_is_nothrow_move_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_move_constructible_v<T> ==
	       std::is_nothrow_move_constructible_v<T>, 
	       alp::as_str() << "is_nothrow_move_constructible(" << name_type << ")");
}

void test_is_nothrow_move_constructible()
{
    test::interface("is_nothrow_move_constructible");

    test_is_nothrow_move_constructible<void>("void");
    test_is_nothrow_move_constructible<const volatile void>("const volatile void");
    test_is_nothrow_move_constructible<nullptr_t>("nullptr_t");

    test_is_nothrow_move_constructible<char>("char");
    test_is_nothrow_move_constructible<int>("int");
    test_is_nothrow_move_constructible<long>("long");
    test_is_nothrow_move_constructible<long long>("long long");
    test_is_nothrow_move_constructible<float>("float");
    test_is_nothrow_move_constructible<double>("double");

    test_is_nothrow_move_constructible<int[]>("int[]");
    test_is_nothrow_move_constructible<int[3]>("int[3]");
    test_is_nothrow_move_constructible<int[][3]>("int[][3]");

    test_is_nothrow_move_constructible<Class>("Class");
    test_is_nothrow_move_constructible<Union>("Union");
    test_is_nothrow_move_constructible<Enum>("Enum");
    test_is_nothrow_move_constructible<Enum_class>("Enum_class");

    test_is_nothrow_move_constructible<Class>("Class");
    test_is_nothrow_move_constructible<Class2>("Class2");
    test_is_nothrow_move_constructible<int Class::*>("int Class::*");
    test_is_nothrow_move_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_move_constructible<A>("A");
    test_is_nothrow_move_constructible<B>("B");
    test_is_nothrow_move_constructible<C>("C");
    test_is_nothrow_move_constructible<D>("D");
}

template <typename T>
void test_is_nothrow_move_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_nothrow_move_assignable_v<T> ==
	       std::is_nothrow_move_assignable_v<T>, 
	       alp::as_str() << "is_nothrow_move_assignable(" << name_type << ")");
}

void test_is_nothrow_move_assignable()
{
    test::interface("is_nothrow_move_assignable");

    test_is_nothrow_move_assignable<void>("void");
    test_is_nothrow_move_assignable<const volatile void>("const volatile void");
    test_is_nothrow_move_assignable<nullptr_t>("nullptr_t");

    test_is_nothrow_move_assignable<char>("char");
    test_is_nothrow_move_assignable<int>("int");
    test_is_nothrow_move_assignable<long>("long");
    test_is_nothrow_move_assignable<long long>("long long");
    test_is_nothrow_move_assignable<float>("float");
    test_is_nothrow_move_assignable<double>("double");

    test_is_nothrow_move_assignable<int[]>("int[]");
    test_is_nothrow_move_assignable<int[3]>("int[3]");
    test_is_nothrow_move_assignable<int[][3]>("int[][3]");

    test_is_nothrow_move_assignable<Class>("Class");
    test_is_nothrow_move_assignable<Union>("Union");
    test_is_nothrow_move_assignable<Enum>("Enum");
    test_is_nothrow_move_assignable<Enum_class>("Enum_class");

    test_is_nothrow_move_assignable<Class>("Class");
    test_is_nothrow_move_assignable<Class2>("Class2");
    test_is_nothrow_move_assignable<int Class::*>("int Class::*");
    test_is_nothrow_move_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_nothrow_move_assignable<A>("A");
    test_is_nothrow_move_assignable<B>("B");
    test_is_nothrow_move_assignable<C>("C");
    test_is_nothrow_move_assignable<D>("D");
}


template <typename T>
void test_is_move_constructible(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_move_constructible_v<T> ==
	       std::is_move_constructible_v<T>, 
	       alp::as_str() << "is_move_constructible(" << name_type << ")");
}

void test_is_move_constructible()
{
    test::interface("is_move_constructible");

    test_is_move_constructible<void>("void");
    test_is_move_constructible<const volatile void>("const volatile void");
    test_is_move_constructible<nullptr_t>("nullptr_t");

    test_is_move_constructible<char>("char");
    test_is_move_constructible<int>("int");
    test_is_move_constructible<long>("long");
    test_is_move_constructible<long long>("long long");
    test_is_move_constructible<float>("float");
    test_is_move_constructible<double>("double");

    test_is_move_constructible<int[]>("int[]");
    test_is_move_constructible<int[3]>("int[3]");
    test_is_move_constructible<int[][3]>("int[][3]");

    test_is_move_constructible<Class>("Class");
    test_is_move_constructible<Union>("Union");
    test_is_move_constructible<Enum>("Enum");
    test_is_move_constructible<Enum_class>("Enum_class");

    test_is_move_constructible<Class>("Class");
    test_is_move_constructible<Class2>("Class2");
    test_is_move_constructible<int Class::*>("int Class::*");
    test_is_move_constructible<int (Class::*)()>("int (Class::*)()");

    test_is_move_constructible<A>("A");
    test_is_move_constructible<B>("B");
    test_is_move_constructible<C>("C");
    test_is_move_constructible<D>("D");
}

template <typename T>
void test_is_swappable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_swappable_v<T> ==
	       std::is_swappable_v<T>, 
	       alp::as_str() << "is_swappable(" << name_type << ")");
}

void test_is_swappable()
{
    test::interface("is_swappable");

    test_is_swappable<void>("void");
    test_is_swappable<const volatile void>("const volatile void");
    test_is_swappable<nullptr_t>("nullptr_t");

    test_is_swappable<char>("char");
    test_is_swappable<int>("int");
    test_is_swappable<long>("long");
    test_is_swappable<long long>("long long");
    test_is_swappable<float>("float");
    test_is_swappable<double>("double");

    test_is_swappable<int[]>("int[]");
    test_is_swappable<int[3]>("int[3]");
    test_is_swappable<int[][3]>("int[][3]");

    test_is_swappable<Class>("Class");
    test_is_swappable<Union>("Union");
    test_is_swappable<Enum>("Enum");
    test_is_swappable<Enum_class>("Enum_class");

    test_is_swappable<Class>("Class");
    test_is_swappable<Class2>("Class2");
    test_is_swappable<int Class::*>("int Class::*");
    test_is_swappable<int (Class::*)()>("int (Class::*)()");

    test_is_swappable<A>("A");
    test_is_swappable<B>("B");
    test_is_swappable<C>("C");
    test_is_swappable<D>("D");



}

template <typename T>
void test_is_move_assignable(const std::string& name_type)
{
    CHECK_TRUE(mtd::is_move_assignable_v<T> ==
	       std::is_move_assignable_v<T>, 
	       alp::as_str() << "is_move_assignable(" << name_type << ")");
}

void test_is_move_assignable()
{
    test::interface("is_move_assignable");

    test_is_move_assignable<void>("void");
    test_is_move_assignable<const volatile void>("const volatile void");
    test_is_move_assignable<nullptr_t>("nullptr_t");

    test_is_move_assignable<char>("char");
    test_is_move_assignable<int>("int");
    test_is_move_assignable<long>("long");
    test_is_move_assignable<long long>("long long");
    test_is_move_assignable<float>("float");
    test_is_move_assignable<double>("double");

    test_is_move_assignable<int[]>("int[]");
    test_is_move_assignable<int[3]>("int[3]");
    test_is_move_assignable<int[][3]>("int[][3]");

    test_is_move_assignable<Class>("Class");
    test_is_move_assignable<Union>("Union");
    test_is_move_assignable<Enum>("Enum");
    test_is_move_assignable<Enum_class>("Enum_class");

    test_is_move_assignable<Class>("Class");
    test_is_move_assignable<Class2>("Class2");
    test_is_move_assignable<int Class::*>("int Class::*");
    test_is_move_assignable<int (Class::*)()>("int (Class::*)()");

    test_is_move_assignable<A>("A");
    test_is_move_assignable<B>("B");
    test_is_move_assignable<C>("C");
    test_is_move_assignable<D>("D");



}

int main()
{
try{
    test::header("type_traits0");
    
    // helper class
    // ------------{
    test_integral_constant();
    test_is_same();

    test_is_const();
    test_is_volatile();

    // primary type categories
    // -----------------------
    test_is_void();
    test_is_array();
    test_is_rvalue_reference();
    test_is_lvalue_reference();

    // composite type categories
    // -------------------------
    test_is_reference();

    // const-volatile modifications
    // ----------------------------
    test_remove_const();

    // type properties
    // ----------------
    test_is_move_constructible();
    test_is_move_assignable();
    test_is_swappable();
    test_is_nothrow_move_constructible();
    test_is_nothrow_move_assignable();

    // type properties queries
    // -----------------------
    test_extent();

    // reference modifications
    // -----------------------
    test_remove_reference();

    // other transformations
    // ---------------------

    // others
    // ------
    //test_move();
    test_swap();
    //test_forward();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





