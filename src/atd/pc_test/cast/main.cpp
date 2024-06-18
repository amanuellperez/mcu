// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../atd_cast.h"


#include <iostream>

#include <alp_test.h>
#include <alp_string.h>


using namespace test;


void test_bounded_cast()
{    
    test::interface("bounded_cast");

    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{0}) == 0, "(0) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{255}) == 255, "(255) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(uint16_t{1000}) == 255, "(1000) uint16_t -> uint8_t");
    CHECK_TRUE(atd::bounded_cast<uint8_t>(int16_t{-20}) == 0, "(-20) uint16_t -> uint8_t");


}


template <typename Int, Int v>
struct Wrapper{
    constexpr static Int value = v;
};

void test_safe_static_cast()
{ 
    test::interface("safe_static_cast");

    // Lo meto dentro del Wraper para que se vea que se genera en tiempo de
    // compilación.
    std::cout << "static_cast: " << (int) 
	Wrapper<uint8_t, atd::safe_static_cast<uint8_t, uint16_t, 10u>()>::value
	<< " OK\n";

    // Este no compila
//    std::cout << "static_cast: " << (int) 
//	Wrapper<uint8_t, safe_static_cast<uint8_t, uint16_t, 256u>()>::value
//	<< " OK\n";
}

void test_to_integer()
{
    test::interface("to_integer");

    using Dec8_1 = atd::Decimal<uint8_t, 1>;

    Dec8_1 d{10,3};
    CHECK_TRUE(atd::to_integer<uint8_t>(d) == 10, "to_integer");
    CHECK_TRUE(atd::to_integer<long>(d) == 10UL, "to_integer");

{// bug
    using Dec1 = atd::Decimal<uint64_t, 3>;
    using Dec2 = atd::Decimal<uint32_t, 3>;
    Dec1 from{953, 67};
    Dec2 to = atd::to_integer<Dec2>(from);
    CHECK_TRUE(to.significand() == from.significand(), "b1");
}

{// bug
    using Dec = atd::Decimal<uint32_t, 3>;
    Dec from = Dec::significand(1090);
    Dec to = atd::to_integer<Dec>(from);
    std::cout << from << " --> " << to << '\n';
    CHECK_TRUE(to.significand() == from.significand(), "b2");
}

}

struct A{
    int x;
};

struct B{
    int y;
};

bool convert_x_into_y(const A& a, B& b)
{
    b.y = a.x + 2;
    return true;
}


void test_convert_into()
{
    A a;
    a.x = 10;
    B b;
    atd::convert(a).into(b);
    CHECK_TRUE(b.y == a.x + 2, "convert.into");
}

void test_little_endian()
{
    test::interface("little_endian_to");

{// Basic test
    uint8_t d[2];
    d[0] = 0xCD;
    d[1] = 0xAB;
    CHECK_TRUE(atd::little_endian_to<uint16_t>(d) == 0xABCD, "0xABCD");

    // CUIDADO al operar con ints!!! puede haber overflow. De hecho 
    // el siguiente test falla si se quita el casting de C.
    // Probar a sustituirlo por `int16_t{0xABCD}`. Da error.
    CHECK_TRUE(atd::little_endian_to<int16_t>(d) == (int16_t) 0xABCD, "0xABCD");
}
{// Basic test 2
    uint8_t d[4];
    d[0] = 0xCD;
    d[1] = 0xAB;
    d[2] = 0xEF;
    d[3] = 0x89;
    CHECK_TRUE(atd::little_endian_to<uint32_t>(d) == 0x89EFABCD, "0x89EFABCD");

    // CUIDADO al operar con ints!!! puede haber overflow. De hecho 
    // el siguiente test falla si se quita el casting de C.
    // Probar a sustituirlo por `int16_t{0xABCD}`. Da error.
    CHECK_TRUE(atd::little_endian_to<int32_t>(d) == (int32_t) 0x89EFABCD, "0x89EFABCD");
}

    // uint8_t
    for (uint8_t i = 0; i < 255; ++i){
	uint8_t d[1];
	d[0] = i;
	CHECK_TRUE(atd::little_endian_to<uint8_t>(d) == i, "little_endian_to<uint8_t>");
    }

    // uint16_t
    for (uint8_t b1 = 0; b1 < 255; ++b1){
	for (uint8_t b0 = 0; b0 < 255; ++b0){
	    uint8_t d[2];
	    d[0] = b0;
	    d[1] = b1;
	    uint16_t res = 256*b1 + b0;
//	    std::cout << "256 * " << (int) b1 << " + " << (int) b0 << " = " 
//		      << res << " =? " << 
//	    atd::little_endian_to<uint16_t>(d) << '\n';

	    CHECK_TRUE(atd::little_endian_to<uint16_t>(d) == res,
					    "little_endian_to<uint16_t>");
	}
    }


}

void test_big_endian()
{
    test::interface("big_endian_to");

{// Basic test
    uint8_t d[2];
    d[0] = 0xCD;
    d[1] = 0xAB;
    CHECK_TRUE(atd::big_endian_to<uint16_t>(d) == 0xCDAB, "0xCDAB");

    // CUIDADO al operar con ints!!! puede haber overflow. De hecho 
    // el siguiente test falla si se quita el casting de C.
    // Probar a sustituirlo por `int16_t{0xABCD}`. Da error.
    CHECK_TRUE(atd::big_endian_to<int16_t>(d) == (int16_t) 0xCDAB, "0xCDAB");
}
{// Basic test 2
    uint8_t d[4];
    d[0] = 0xCD;
    d[1] = 0xAB;
    d[2] = 0xEF;
    d[3] = 0x89;
    CHECK_TRUE(atd::big_endian_to<uint32_t>(d) == 0xCDABEF89, "0xCDABEF89");

    // CUIDADO al operar con ints!!! puede haber overflow. De hecho 
    // el siguiente test falla si se quita el casting de C.
    // Probar a sustituirlo por `int16_t{0xABCD}`. Da error.
    CHECK_TRUE(atd::big_endian_to<int32_t>(d) == (int32_t) 0xCDABEF89, "0xCDABEF89");
}

    // uint8_t
    for (uint8_t i = 0; i < 255; ++i){
	uint8_t d[1];
	d[0] = i;
	CHECK_TRUE(atd::big_endian_to<uint8_t>(d) == i, "big_endian_to<uint8_t>");
    }

    // uint16_t
    for (uint8_t b1 = 0; b1 < 255; ++b1){
	for (uint8_t b0 = 0; b0 < 255; ++b0){
	    uint8_t d[2];
	    d[0] = b1;
	    d[1] = b0;
	    uint16_t res = 256*b1 + b0;
//	    std::cout << "256 * " << (int) b1 << " + " << (int) b0 << " = " 
//		      << res << " =? " << 
//	    atd::big_endian_to<uint16_t>(d) << '\n';

	    CHECK_TRUE(atd::big_endian_to<uint16_t>(d) == res,
					    "big_endian_to<uint16_t>");
	}
    }


}

template <typename From, typename To>
void test_floating_cast(From x, To res)
{
    std::cout << "floating_cast(" << x << "): ";
    CHECK_TRUE(atd::floating_cast<To>(x) == res, "");
}
void test_floating_cast()
{
    test::interface("floating_cast");

    test_floating_cast(float{3.14}, float{3.14});
    test_floating_cast(atd::Decimal<int, 2>{4}, atd::Decimal<int,0>{4});
    test_floating_cast(atd::Decimal<int, 3>{3,141}, atd::Decimal<int,1>{3,1});

}


int main()
{
try{
    test::header("atd_cast");

    test_bounded_cast();
    test_safe_static_cast();
    test_to_integer();
    test_floating_cast();
    test_convert_into();
    test_little_endian();
    test_big_endian();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





