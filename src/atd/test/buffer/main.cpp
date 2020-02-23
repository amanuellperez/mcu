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

#include <iterator>

namespace std{ // A día de hoy gcc no suministra shift_left.
template <typename ForwardIt>
constexpr ForwardIt shift_left(ForwardIt p0, ForwardIt pe, 
		typename iterator_traits<ForwardIt>::difference_type n)
{
    if (n <= 0)
	return pe;

    if (n >= distance(p0, pe))
	return p0;

    ForwardIt q0 = p0;
    advance(p0, n);

    for (; p0 != pe; advance(p0, 1), advance(q0, 1))
    {
	*q0 = *p0;
    }

    return q0;
}
}


#include <iostream> // para depurar
#include "../../atd_buffer.h"

#include <alp_test.h>
#include <alp_string.h>
#include <vector>

#include <cstddef>

using namespace test;



void test_buffer()
{
    test::interfaz("TWI_iobuffer (input)");

    atd::TWI_iobuffer<10> buffer;
    buffer.reset_as_input();

    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    CHECK_TRUE(!buffer.in_is_full(), "!in_is_full");

    for (uint8_t i = 10; i < 15; ++i)
	buffer.in_write_one(std::byte{i});

    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    CHECK_TRUE(!buffer.in_is_full(), "!in_is_full");

// normal
    std::byte msg[10];
    int n = buffer.in_read_all_n(msg, 10);
    CHECK_TRUE(n == 5, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<std::byte>(10 + i), "read_all_n");

// empty
    n = buffer.in_read_all_n(msg, 10);
    CHECK_TRUE(n == 0, "read_all_n (empty)");

// full
    for (uint8_t i = 100; i < 110; ++i)
	buffer.in_write_one(std::byte{i});

    n = buffer.in_read_all_n(msg, 10); // leemos justo hasta el final
    CHECK_TRUE(n == 10, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<std::byte>(100 + i), "read_all_n");

// leemos de menos
    for (uint8_t i = 10; i < 20; ++i)
	buffer.in_write_one(std::byte{i});

    n = buffer.in_read_all_n(msg, 5);
    CHECK_TRUE(n == 5, "read_all_n");
    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<std::byte>(10 + i), "read_all_n");

    n = buffer.in_read_all_n(msg, 1);
    CHECK_TRUE(n == 1, "read_all_n");
    CHECK_TRUE(!buffer.in_is_empty(), "!in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<std::byte>(15 + i), "read_all_n");

    n = buffer.in_read_all_n(msg, 4);
    CHECK_TRUE(n == 4, "read_all_n");
    CHECK_TRUE(buffer.in_is_empty(), "in_is_empty");
    for (uint8_t i = 0; i < n; ++i)
	CHECK_TRUE(msg[i] == static_cast<std::byte>(16 + i), "read_all_n");



// Lo cambiamos a output
    test::interfaz("TWI_iobuffer (output)");

    buffer.reset_as_output();

    CHECK_TRUE(buffer.out_is_empty(), "out_is_empty");

    for (uint8_t i = 0; i < 10; ++i)
	msg[i] = static_cast<std::byte>(10 + i);

// solo 1 byte
    n = buffer.out_write_all_n(msg, 1);
    CHECK_TRUE(n == 1, "n == 1");
    CHECK_TRUE(buffer.out_size() == 1, "n == 1");
    CHECK_TRUE(!buffer.out_is_empty(), "!out_is_empty");

    {
	int i;
	for (i = 0; i < 10 and !buffer.out_is_empty(); ++i){
	    std::byte b = buffer.out_read_one();
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
	    std::byte b = buffer.out_read_one();
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
	    std::byte b = buffer.out_read_one();
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
	    std::byte b = buffer.out_read_one();
	    CHECK_TRUE(b == msg[i], "out_read_one");
	}

	CHECK_TRUE(i == 10, "i == 10");
    }
    CHECK_TRUE(buffer.out_is_empty(), "!out_is_empty");
}


void test_circular_buffer()
{
    test::interfaz("Circular_array");

    constexpr int N = 6;
    atd::Circular_array<int, N> buf;

// estado inicial
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

    int b[N];
    for (int i = 0; i < N; ++i)
	b[i] = 10 + i;

// no lo llenamos
    CHECK_TRUE(buf.ewrite(b, 2) == 2, "ewrite");
    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N - 2, "available");
    CHECK_TRUE(buf.size() == 2, "size");

    int res[N];
    CHECK_TRUE(buf.eread(res, 2) == 2, "eread");

    CHECK_EQUAL_CONTAINERS(b, b + 2, res, res + 2, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// leemos en el medio
    buf.ewrite(b, 4);
    buf.eread(res, 2);
    buf.eread(res, 2);

    CHECK_EQUAL_CONTAINERS(b + 2, b + 4, res, res + 2, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// lo llenamos
    CHECK_TRUE(buf.ewrite(b, N) == N, "ewrite");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 0, "available");
    CHECK_TRUE(buf.size() == N, "size");

    CHECK_TRUE(buf.eread(res, N) == N, "eread");
    CHECK_EQUAL_CONTAINERS(b, b + N, res, res + N, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");


// hacemos que de la vuelta
    buf.ewrite(b, N-1);
    buf.eread(res, 3);

    buf.eread(res, 1);
    CHECK_EQUAL_CONTAINERS(b + 3, b + 4, res, res + 1, "eread");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 5, "available");
    CHECK_TRUE(buf.size() == 1, "size");

    buf.ewrite(b, 3);

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 2, "available");
    CHECK_TRUE(buf.size() == 4, "size");

    buf.eread(res, 3);

    CHECK_EQUAL_CONTAINERS(b + 4, b + 5, res, res + 1, "eread");
    CHECK_EQUAL_CONTAINERS(b, b + 2, res + 1, res + 3, "eread");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 5, "available");
    CHECK_TRUE(buf.size() == 1, "size");
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
    test::interfaz("iobxtream");

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
    test_circular_buffer();
    test_iobxtream();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





