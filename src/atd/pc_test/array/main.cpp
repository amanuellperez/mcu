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


#include <iostream> // para depurar
#include "../../atd_array.h"

#include <alp_test.h>
#include <alp_string.h>
#include <vector>

#include <cstddef>

using namespace test;


void test_circular_array()
{
    test::interfaz("Circular_array");

    constexpr int N = 6;
    atd::Circular_array<int, N> buf;

// estado inicial
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

    int b[N];
    for (int i = 0; i < N; ++i)
	b[i] = 10 + i;

// no lo llenamos
    CHECK_TRUE(buf.ewrite(b, 2) == 2, "ewrite");
    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N - 2, "available");
    CHECK_TRUE(buf.size() == 2, "size");

    int res[N];
    CHECK_TRUE(buf.eread(res, 2) == 2, "eread");

    CHECK_EQUAL_CONTAINERS(b, b + 2, res, res + 2, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// leemos en el medio
    buf.ewrite(b, 4);
    buf.eread(res, 2);
    buf.eread(res, 2);

    CHECK_EQUAL_CONTAINERS(b + 2, b + 4, res, res + 2, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// lo llenamos
    CHECK_TRUE(buf.ewrite(b, N) == N, "ewrite");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 0, "available");
    CHECK_TRUE(buf.size() == N, "size");

    CHECK_TRUE(buf.eread(res, N) == N, "eread");
    CHECK_EQUAL_CONTAINERS(b, b + N, res, res + N, "eread");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");


// hacemos que de la vuelta
    buf.ewrite(b, N-1);
    buf.eread(res, 3);

    buf.eread(res, 1);
    CHECK_EQUAL_CONTAINERS(b + 3, b + 4, res, res + 1, "eread");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 5, "available");
    CHECK_TRUE(buf.size() == 1, "size");

    buf.ewrite(b, 3);

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 2, "available");
    CHECK_TRUE(buf.size() == 4, "size");

    buf.eread(res, 3);

    CHECK_EQUAL_CONTAINERS(b + 4, b + 5, res, res + 1, "eread");
    CHECK_EQUAL_CONTAINERS(b, b + 2, res + 1, res + 3, "eread");

    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 5, "available");
    CHECK_TRUE(buf.size() == 1, "size");


// reset
    buf.reset();
// estado inicial
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// probamos a llenar linealmente el buffer
    for (int i = 0; i < N-1; ++i){
	buf.write(i);
	CHECK_TRUE(!buf.is_empty(), "is_empty");
	CHECK_TRUE(!buf.is_full(), "is_full");
	CHECK_TRUE(buf.capacity() == N, "capacity");
	CHECK_TRUE(buf.available() == N - i - 1, "available");
	CHECK_TRUE(buf.size() == i + 1, "size");
    }
    buf.write(N-1);
    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 0, "available");
    CHECK_TRUE(buf.size() == N, "size");

    for (int i = 0; i < N-1; ++i){
	int j;
	buf.read(j);
	CHECK_TRUE(i == j, "read(j)");
	CHECK_TRUE(!buf.is_empty(), "is_empty");
	CHECK_TRUE(!buf.is_full(), "is_full");
	CHECK_TRUE(buf.capacity() == N, "capacity");
	CHECK_TRUE(buf.available() == i + 1, "available");
	CHECK_TRUE(buf.size() == N - (i + 1), "size");
    }
    int j;
    buf.read(j);
    CHECK_TRUE(j == N-1, "read(j)");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// probamos a llenarlo circularmente
    for (int i = 0; i < 4; ++i)
	buf.write(10 + i);

    for (int i = 0; i < 3; ++i)
	buf.read(j);

    CHECK_TRUE(buf.size() == 1, "size");
    

    for (int i = 0; i < N-2; ++i){
	buf.write(i);
	CHECK_TRUE(!buf.is_empty(), "is_empty");
	CHECK_TRUE(!buf.is_full(), "is_full");
	CHECK_TRUE(buf.capacity() == N, "capacity");
	CHECK_TRUE(buf.available() == N - i - 2, "available");
	CHECK_TRUE(buf.size() == i + 2, "size");
    }

    buf.write(N-2);
    CHECK_TRUE(!buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == 0, "available");
    CHECK_TRUE(buf.size() == N, "size");

    buf.read(j);
    CHECK_TRUE(j == 13, "read(j)");

    for (int i = 0; i < N-2; ++i){
	int j;
	buf.read(j);
	CHECK_TRUE(i == j, "read(j)");
	CHECK_TRUE(!buf.is_empty(), "is_empty");
	CHECK_TRUE(!buf.is_full(), "is_full");
	CHECK_TRUE(buf.capacity() == N, "capacity");
	CHECK_TRUE(buf.available() == i + 2, "available");
	CHECK_TRUE(buf.size() == N - (i + 2), "size");
    }
    buf.read(j);
    CHECK_TRUE(j == N-2, "read(j)");
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(!buf.is_full(), "is_full");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");


    {// prueba que falló en TWI
    atd::Circular_array<std::byte, 10> buf;
    std::byte x{45};
    CHECK_TRUE((buf.ewrite(&x, 1) == 1), "ewrite");
    std::byte y;
    y = buf.read();
    CHECK_TRUE(x == y, "read");
    std::byte x2[2] = {std::byte{32}, std::byte{67}};
    CHECK_TRUE((buf.ewrite(x2, 2) == 2), "ewrite");

    
    }
}





int main()
{
try{
    test::header("atd_array");

    test_circular_array();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





