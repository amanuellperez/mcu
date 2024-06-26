// Copyright (C) 2019-2024 Manuel Perez 
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

#include "../../atd_type_traits.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>

using namespace test;




template <typename Int, Int v>
struct Wrapper{
    constexpr static Int value = v;
};



void test_less_than()
{
    test::interface("Less_than");

    CHECK_TRUE((Wrapper<bool, atd::Less_than<int, 3, 4>::value>::value == true),
               "3 < 4?");
    CHECK_TRUE((Wrapper<bool, atd::Less_than<int, 4, 4>::value>::value == false),
               "4 < 4?");
    CHECK_TRUE((Wrapper<bool, atd::Less_than<int, 4, 3>::value>::value == false),
               "4 < 3?");
}


//void test_pertenece_al_intervalo_cerrado()
//{
//    test::interface("test_pertenece_al_intervalo_cerrado");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 atd::Pertenece_al_intervalo_cerrado<int, 5, 2, 8>::value>::value 
//		== true),
//        "5 pertenece a [2,8]?");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 atd::Pertenece_al_intervalo_cerrado<int, 1, 2, 8>::value>::value 
//		== false),
//        "1 pertenece a [2,8]?");
//
//    CHECK_TRUE(
//        (Wrapper<bool,
//                 atd::Pertenece_al_intervalo_cerrado<int, 10, 2, 8>::value>::value 
//		== false),
//        "10 pertenece a [2,8]?");
//}

template <int x>
inline constexpr int f()
{
    if constexpr (x == 0)
	return 1;
    else if constexpr (x == 1)
	return 30;
    else
	static_assert(atd::always_false_v<int>, "error");
}


void test_always_false()
{
    test::interface("always_false_v");

    CHECK_TRUE(f<0>() == 1, "always_false_v");
    CHECK_TRUE(f<1>() == 30, "always_false_v");
    CHECK_DONT_COMPILE(f<2>(), "f");
}



void test_has_same_sign()
{
    test::interface("has_same_sign");

    CHECK_TRUE(atd::has_same_sign<uint8_t, uint8_t>(), "has_same_sign");
    CHECK_TRUE(!atd::has_same_sign<uint8_t, int8_t>(), "has_same_sign");
    CHECK_TRUE(!atd::has_same_sign<int8_t, uint8_t>(), "has_same_sign");
    CHECK_TRUE(atd::has_same_sign<int8_t, int8_t>(), "has_same_sign");

    CHECK_TRUE(atd::has_same_sign<int, int>(), "has_same_sign");
    CHECK_TRUE(!atd::has_same_sign<int, unsigned int>(), "has_same_sign");

}

template <typename T, typename U>
void test_same_type_double_bits()
{
    CHECK_TRUE((std::is_same_v<atd::same_type_with_double_bits_t<T>, U>),
               "same_type_with_double_bits");
}

void test_same_type_double_bits()
{
    test::interface("same_type_with_double_bits");
    test_same_type_double_bits<uint8_t, uint16_t>();
    test_same_type_double_bits<uint16_t, uint32_t>();
    test_same_type_double_bits<uint32_t, uint64_t>();
    test_same_type_double_bits<uint64_t, uint64_t>();

    test_same_type_double_bits<int8_t, int16_t>();
    test_same_type_double_bits<int16_t, int32_t>();
    test_same_type_double_bits<int32_t, int64_t>();
    test_same_type_double_bits<int64_t, int64_t>();
}

template <typename T, typename U>
void test_same_type_at_least32()
{
    CHECK_TRUE((std::is_same_v<atd::same_type_at_least32_t<T>, U>),
               "same_type_at_least32");
}

void test_same_type_at_least32()
{
    test::interface("same_type_at_least32");
    test_same_type_at_least32<uint8_t, uint32_t>();
    test_same_type_at_least32<uint16_t, uint32_t>();
    test_same_type_at_least32<uint32_t, uint32_t>();
    test_same_type_at_least32<uint64_t, uint64_t>();

    test_same_type_at_least32<int8_t, int32_t>();
    test_same_type_at_least32<int16_t, int32_t>();
    test_same_type_at_least32<int32_t, int32_t>();
    test_same_type_at_least32<int64_t, int64_t>();
}

template <typename A, typename B, typename Res>
void test_same_sign_as()
{

    CHECK_TRUE((std::is_same_v<typename atd::__same_sign_as<A,B>::type, Res>)
	    , "__same_sign_as");
}

template <typename A, typename B, typename Res>
void test_make_type_same_sign_as()
{
    using T = atd::make_type<A>::template same_sign_as<B>;
    CHECK_TRUE( (std::is_same_v<T, Res>),
        "make_type_same_sign_as");
}

void test_same_sign_as()
{
    test::interface("make_type");

    test_same_sign_as<int8_t, int8_t, int8_t>();
    test_same_sign_as<uint8_t, int8_t, int8_t>();
    test_same_sign_as<int8_t, uint8_t, uint8_t>();
    test_same_sign_as<uint8_t, uint8_t, uint8_t>();

    test_same_sign_as<int16_t, int16_t, int16_t>();
    test_same_sign_as<uint16_t, int16_t, int16_t>();
    test_same_sign_as<int16_t, uint16_t, uint16_t>();
    test_same_sign_as<uint16_t, uint16_t, uint16_t>();

    test_same_sign_as<int32_t, int32_t, int32_t>();
    test_same_sign_as<uint32_t, int32_t, int32_t>();
    test_same_sign_as<int32_t, uint32_t, uint32_t>();
    test_same_sign_as<uint32_t, uint32_t, uint32_t>();

    test_same_sign_as<int64_t, int64_t, int64_t>();
    test_same_sign_as<uint64_t, int64_t, int64_t>();
    test_same_sign_as<int64_t, uint64_t, uint64_t>();
    test_same_sign_as<uint64_t, uint64_t, uint64_t>();

// make_type_same_sign_as
    test_make_type_same_sign_as<int8_t, int8_t, int8_t>();
    test_make_type_same_sign_as<uint8_t, int8_t, int8_t>();
    test_make_type_same_sign_as<int8_t, uint8_t, uint8_t>();
    test_make_type_same_sign_as<uint8_t, uint8_t, uint8_t>();

    test_make_type_same_sign_as<int16_t, int16_t, int16_t>();
    test_make_type_same_sign_as<uint16_t, int16_t, int16_t>();
    test_make_type_same_sign_as<int16_t, uint16_t, uint16_t>();
    test_make_type_same_sign_as<uint16_t, uint16_t, uint16_t>();

    test_make_type_same_sign_as<int32_t, int32_t, int32_t>();
    test_make_type_same_sign_as<uint32_t, int32_t, int32_t>();
    test_make_type_same_sign_as<int32_t, uint32_t, uint32_t>();
    test_make_type_same_sign_as<uint32_t, uint32_t, uint32_t>();

    test_make_type_same_sign_as<int64_t, int64_t, int64_t>();
    test_make_type_same_sign_as<uint64_t, int64_t, int64_t>();
    test_make_type_same_sign_as<int64_t, uint64_t, uint64_t>();
    test_make_type_same_sign_as<uint64_t, uint64_t, uint64_t>();
}

struct A {
    using value_type = long;
    using size_type  = unsigned long;
    using iterator   = double;
};

template <typename T, typename V>
void test_value_type_t(const std::string& name)
{
    CHECK_TRUE((std::is_same_v<atd::value_type_t<T>, V>), name);
}



void test_value_type_t()
{
    test::interface("value_type_t");
    test_value_type_t<int[10], int>("int[10]");
    test_value_type_t<A, long>("A::value_type");
}

template <typename T, typename V>
void test_size_type_t(const std::string& name)
{
    CHECK_TRUE((std::is_same_v<atd::size_type_t<T>, V>), name);
}

void test_size_type_t()
{
    test::interface("size_type_t");
    test_size_type_t<int[10], int>("int[10]");
    test_size_type_t<A, unsigned long>("A::size_type");
}


template <typename T, typename V>
void test_iterator_type_t(const std::string& name)
{
    CHECK_TRUE((std::is_same_v<atd::iterator_type_t<T>, V>), name);
}

void test_iterator_type_t()
{
    test::interface("iterator_type_t");
    test_iterator_type_t<int[10], int*>("int[10]");
    test_iterator_type_t<A, double>("A::iterator_type");
}

template <size_t n>
void test_number_constexpr(const std::string& msg)
{
    CHECK_TRUE(true, msg);
}

void test_sizeof_in_bits()
{
    test::interface("sizeof_in_bits");

    CHECK_TRUE(atd::sizeof_in_bits<uint8_t>() == 8, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<int8_t>() == 8, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<uint16_t>() == 16, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<int16_t>() == 16, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<uint32_t>() == 32, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<int32_t>() == 32, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<uint64_t>() == 64, "sizeof_in_bits");
    CHECK_TRUE(atd::sizeof_in_bits<int64_t>() == 64, "sizeof_in_bits");

    test_number_constexpr<atd::sizeof_in_bits<uint8_t>()>("sizeof_in_bits is constexpr");
}


void test_sizeof_in_bytes()
{
    test::interface("sizeof_in_bytes");

    CHECK_TRUE(atd::sizeof_in_bytes<uint8_t>() == 1, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<int8_t>() == 1, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<uint16_t>() == 2, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<int16_t>() == 2, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<uint32_t>() == 4, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<int32_t>() == 4, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<uint64_t>() == 8, "sizeof_in_bytes");
    CHECK_TRUE(atd::sizeof_in_bytes<int64_t>() == 8, "sizeof_in_bytes");

    test_number_constexpr<atd::sizeof_in_bytes<uint8_t>()>("sizeof_in_bytes is constexpr");
}

template <typename T>
void test_is_integer(bool res, const std::string& name_type)
{
    CHECK_TRUE(	atd::is_integer_v<T> == res
		, alp::as_str() << "is_integer_v<" << name_type << ">");
}
void test_is_integer()
{
    test::interface("is_integer");

    test_is_integer<bool>(false, "bool");

    // No funciona con `unsigned char` ya que este es `uint8_t`!!!
//    test_is_integer<char>(false, "char");
//    test_is_integer<unsigned>(false, "unsigned char");

    test_is_integer<uint8_t>(true, "uint8_t");
    test_is_integer<uint16_t>(true, "uint16_t");
    test_is_integer<uint32_t>(true, "uint32_t");
    test_is_integer<uint64_t>(true, "uint64_t");

    test_is_integer<int8_t>(true, "int8_t");
    test_is_integer<int16_t>(true, "int16_t");
    test_is_integer<int32_t>(true, "int32_t");
    test_is_integer<int64_t>(true, "int64_t");

    test_is_integer<float>(  false, "float");
    test_is_integer<double>( false, "double");

    test_is_integer<char*>( false, "char*");
    test_is_integer<int*>( false, "int*");
    test_is_integer<float*>( false, "float*");
}


template <typename T>
void test_is_decimal(bool res, const std::string& name_type)
{
    CHECK_TRUE(	atd::is_decimal_v<T> == res
		, alp::as_str() << "is_decimal_v<" << name_type << ">");
}
void test_is_decimal()
{
    test::interface("is_decimal");

    test_is_decimal<bool>(false, "bool");

    test_is_decimal<char>(false, "char");
    test_is_decimal<unsigned>(false, "unsigned char");

    test_is_decimal<uint8_t>(false, "uint8_t");
    test_is_decimal<uint16_t>(false, "uint16_t");
    test_is_decimal<uint32_t>(false, "uint32_t");
    test_is_decimal<uint64_t>(false, "uint64_t");

    test_is_decimal<int8_t>(false, "int8_t");
    test_is_decimal<int16_t>(false, "int16_t");
    test_is_decimal<int32_t>(false, "int32_t");
    test_is_decimal<int64_t>(false, "int64_t");

    test_is_decimal<float>( true, "float");
    test_is_decimal<double>( true, "double");

    test_is_decimal<char*>( false, "char*");
    test_is_decimal<int*>( false, "int*");
    test_is_decimal<float*>( false, "float*");
}

template <uint64_t N, typename Res>
void test_least_uint_t_to_represent()
{
    using Y = atd::least_uint_t_to_represent_t<N>;
    CHECK_TRUE(std::is_same_v<Y, Res>, "least_uint_t_to_represent_t");
}

void test_least_uint_t_to_represent()
{
    test::interface("least_uint_t_to_represent");

    test_least_uint_t_to_represent<0u, uint8_t>();
    test_least_uint_t_to_represent<100u, uint8_t>();
    test_least_uint_t_to_represent<255u, uint8_t>();
    test_least_uint_t_to_represent<256u, uint16_t>();
    test_least_uint_t_to_represent<std::numeric_limits<uint16_t>::max(), uint16_t>();
    test_least_uint_t_to_represent<std::numeric_limits<uint16_t>::max()+1, uint32_t>();
    test_least_uint_t_to_represent<std::numeric_limits<uint32_t>::max(), uint32_t>();
    test_least_uint_t_to_represent<std::numeric_limits<uint32_t>::max()+uint64_t{1}, uint64_t>();
}



int main()
{
try{
    test::header("atd_type_traits");

    test_less_than();
//    test_pertenece_al_intervalo_cerrado();
    test_always_false();
    test_has_same_sign();
    test_same_type_double_bits();
    test_same_type_at_least32();
    test_same_sign_as();
    test_value_type_t();
    test_size_type_t();
    test_iterator_type_t();
    test_sizeof_in_bits();
    test_sizeof_in_bytes();
    test_is_integer();
    test_is_decimal();
    test_least_uint_t_to_represent();
}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





