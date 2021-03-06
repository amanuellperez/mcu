// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#include <iostream>

using namespace atd;
using namespace test;




template <typename Int, Int v>
struct Wrapper{
    constexpr static Int value = v;
};



void test_less_than()
{
    test::interfaz("Less_than");

    CHECK_TRUE((Wrapper<bool, Less_than<int, 3, 4>::value>::value == true),
               "3 < 4?");
    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 4>::value>::value == false),
               "4 < 4?");
    CHECK_TRUE((Wrapper<bool, Less_than<int, 4, 3>::value>::value == false),
               "4 < 3?");
}


void test_pertenece_al_intervalo_cerrado()
{
    test::interfaz("test_pertenece_al_intervalo_cerrado");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 5, 2, 8>::value>::value 
		== true),
        "5 pertenece a [2,8]?");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 1, 2, 8>::value>::value 
		== false),
        "1 pertenece a [2,8]?");

    CHECK_TRUE(
        (Wrapper<bool,
                 Pertenece_al_intervalo_cerrado<int, 10, 2, 8>::value>::value 
		== false),
        "10 pertenece a [2,8]?");
}

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
    test::interfaz("always_false_v");

    CHECK_TRUE(f<0>() == 1, "always_false_v");
    CHECK_TRUE(f<1>() == 30, "always_false_v");
    CHECK_DONT_COMPILE(f<2>(), "f");
}



void test_has_same_sign()
{
    test::interfaz("has_same_sign");

    CHECK_TRUE((atd::has_same_sign<int, int>()), "has_same_sign");
    CHECK_TRUE((!atd::has_same_sign<int, unsigned int>()), "has_same_sign");

}

template <typename T, typename U>
void test_same_type_double_bits()
{
    CHECK_TRUE((std::is_same_v<atd::same_type_with_double_bits<T>, U>),
               "same_type_with_double_bits");
}

void test_same_type_double_bits()
{
    test::interfaz("same_type_with_double_bits");
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
    CHECK_TRUE((std::is_same_v<atd::same_type_at_least32<T>, U>),
               "same_type_at_least32");
}

void test_same_type_at_least32()
{
    test::interfaz("same_type_at_least32");
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
    test::interfaz("make_type");

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

int main()
{
try{
    test::header("atd_type_traits");

    test_less_than();
    test_pertenece_al_intervalo_cerrado();
    test_always_false();
    test_has_same_sign();
    test_same_type_double_bits();
    test_same_type_at_least32();
    test_same_sign_as();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





