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

#include <iterator>

#include <iostream> // para depurar
#include "../../atd_iobxtream.h"

#include <alp_test.h>
#include <alp_string.h>
#include <vector>

#include <cstddef>

using namespace test;



void test_buffer()
{
    test::interface("TWI_iobuffer (input)");

    atd::TWI_iobuffer<10> buffer;
    buffer.reset_as_input();

    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    CHECK_TRUE(!buffer.in_is_full(), "!in_is_full");

    for (uint8_t i = 10; i < 15; ++i)
	buffer.in_write_one(uint8_t{i});

    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    CHECK_TRUE(!buffer.in_is_full(), "!in_is_full");

// normal
    uint8_t msg[10];
    int n = buffer.in_read_all_n(msg, 10);
    CHECK_TRUE(n == 5, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<uint8_t>(10 + i), "read_all_n");

// empty
    n = buffer.in_read_all_n(msg, 10);
    CHECK_TRUE(n == 0, "read_all_n (empty)");

// full
    for (uint8_t i = 100; i < 110; ++i)
	buffer.in_write_one(uint8_t{i});

    n = buffer.in_read_all_n(msg, 10); // leemos justo hasta el final
    CHECK_TRUE(n == 10, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<uint8_t>(100 + i), "read_all_n");

// leemos de menos
    for (uint8_t i = 10; i < 20; ++i)
	buffer.in_write_one(uint8_t{i});

    n = buffer.in_read_all_n(msg, 5);
    CHECK_TRUE(n == 5, "read_all_n");
    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<uint8_t>(10 + i), "read_all_n");

    n = buffer.in_read_all_n(msg, 1);
    CHECK_TRUE(n == 1, "read_all_n");
    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<uint8_t>(15 + i), "read_all_n");

    n = buffer.in_read_all_n(msg, 4);
    CHECK_TRUE(n == 4, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<uint8_t>(16 + i), "read_all_n");



// Lo cambiamos a output
    test::interface("TWI_iobuffer (output)");

    buffer.reset_as_output();

    CHECK_TRUE(buffer.out_is_empty(), "out_is_empty");

    for (uint8_t i = 0; i < 10; ++i)
	msg[i] = static_cast<uint8_t>(10 + i);

// solo 1 byte
    n = buffer.out_write_all_n(msg, 1);
    CHECK_TRUE(n == 1, "n == 1");
    CHECK_TRUE(buffer.out_size() == 1, "n == 1");
    CHECK_TRUE(!buffer.out_is_empty(), "!out_is_empty");

    {
	int i;
	for (i = 0; i < 10 and !buffer.out_is_empty(); ++i){
	    uint8_t b = buffer.out_read_one();
	    CHECK_TRUE(b == msg[i], "out_read_one");
	}

	CHECK_TRUE(i == 1, "i == 1");
    }
    CHECK_TRUE(buffer.out_is_empty(), "out_is_empty");

// caso normal
    n = buffer.out_write_all_n(msg, 5);
    CHECK_TRUE(n == 5, "n == 5");
    CHECK_TRUE(buffer.out_size() == 5, "n == 5");
    CHECK_TRUE(!buffer.out_is_empty(), "!out_is_empty");

    {
	int i;
	for (i = 0; i < 10 and !buffer.out_is_empty(); ++i){
	    uint8_t b = buffer.out_read_one();
	    CHECK_TRUE(b == msg[i], "out_read_one");
	}

	CHECK_TRUE(i == 5, "i == 5");
    }
    CHECK_TRUE(buffer.out_is_empty(), "!out_is_empty");

// llenamos buffer
    n = buffer.out_write_all_n(msg, 10);
    CHECK_TRUE(n == 10, "n == 10");
    CHECK_TRUE(buffer.out_size() == 10, "n == 10");
    CHECK_TRUE(!buffer.out_is_empty(), "out_is_empty");

    {
	int i;
	for (i = 0; i < 10 and !buffer.out_is_empty(); ++i){
	    uint8_t b = buffer.out_read_one();
	    CHECK_TRUE(b == msg[i], "out_read_one");
	}

	CHECK_TRUE(i == 10, "i == 10");
    }
    CHECK_TRUE(buffer.out_is_empty(), "!out_is_empty");

// nos pasamos
    n = buffer.out_write_all_n(msg, 20);
    CHECK_TRUE(n == 10, "n == 10");
    CHECK_TRUE(buffer.out_size() == 10, "n == 10");
    CHECK_TRUE(!buffer.out_is_empty(), "!out_is_empty");

    {
	int i;
	for (i = 0; i < 10 and !buffer.out_is_empty(); ++i){
	    uint8_t b = buffer.out_read_one();
	    CHECK_TRUE(b == msg[i], "out_read_one");
	}

	CHECK_TRUE(i == 10, "i == 10");
    }
    CHECK_TRUE(buffer.out_is_empty(), "!out_is_empty");
}




template <typename Int>
void test_iobxtream(const std::vector<Int>& x)
{
    constexpr int N = 20;

    atd::iobxtream<N> bs;

    for (size_t i = 0; i < x.size(); ++i){
	if ((unsigned long)bs.available() < sizeof(Int)){
	    CHECK_TRUE(0, "bs.available() < sizeof(Int)");
	}

	bs << x[i];
    }

    for (size_t i = 0; i < x.size(); ++i){
	Int res{};  // para quitar warning
	bs >> res;  
	CHECK_TRUE(res == x[i], "operator<</>>");
    }

    // probamos a que el buffer este a medias
    bs.reset();
    for (size_t i = 0; i < N - 6; ++i)
	bs << 'x';

    for (size_t i = 0; i < N - 7; ++i){
	char c;
	bs >> c;
    }


    for (size_t i = 0; i < x.size(); ++i){
	if ((unsigned long)bs.available() < sizeof(Int)){
	    CHECK_TRUE(0, "bs.available() < sizeof(Int)");
	}

	bs << x[i];
    }

    char c;
    bs >> c;

    for (size_t i = 0; i < x.size(); ++i){
	if ((unsigned long)bs.size() < sizeof(Int)){
	    CHECK_TRUE(0, "bs.size() < sizeof(Int)");
	}
	Int res{};  // para quitar warning
	bs >> res;  
	CHECK_TRUE(res == x[i], "operator<</>>");
    }

}


void test_iobxtream()
{
    test::interface("iobxtream");

    test_iobxtream<char>({'a','b','c','d'});
    test_iobxtream<short>({1,2,3,4});
    test_iobxtream<int>({1,2,3,4});

    test_iobxtream<unsigned char>({'a','b','c','d'});
    test_iobxtream<unsigned short>({1,2,3,4});
    test_iobxtream<unsigned int>({1,2,3,4});
}


int main()
{
try{
    test::header("atd_buffer");

    test_buffer();
    test_iobxtream();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





