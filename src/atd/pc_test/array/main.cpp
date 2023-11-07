// Copyright (C) 2019-2021 Manuel Perez 
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


#include "../../atd_array.h"

#include <alp_test.h>
#include <alp_string.h>
#include <vector>

#include <cstddef>
#include <iostream> 

using namespace test;


template <typename T, size_t N>
void print(std::ostream& out, const atd::Linear_array<T,N>& buf)
{
    for (auto x: buf)
	out << x;

}

void test_circular_array()
{
    test::interface("Circular_array");

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

// prueba con nullptr
    buf.reset();
    for (int i = 0; i < N; ++i)
	buf.write(i);

    CHECK_TRUE(buf.size() == N, "write");
    buf.ewrite(nullptr, 10);
    CHECK_TRUE(buf.is_empty(), "is_empty");
    CHECK_TRUE(buf.capacity() == N, "capacity");
    CHECK_TRUE(buf.available() == N, "available");
    CHECK_TRUE(buf.size() == 0, "size");

// prueba con size == 0
    buf.reset();
    for (int i = 0; i < N; ++i)
	buf.write(i);

    CHECK_TRUE(buf.size() == N, "write");
    buf.ewrite(b, 0);
    CHECK_TRUE(buf.is_empty(), "is_empty");
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


void test_linear_array()
{
    test::interface("Linear_array");

    {// caso extremo
    atd::Linear_array<int, 5> buffer;
    CHECK_TRUE(buffer.size() == 0, "size");
    }

    {
    // construction
    atd::Linear_array<int, 6> buf{10,20,30};

    CHECK_TRUE(buf.size() == 3, "size");
    CHECK_TRUE(buf.max_size() == 6, "size");
    CHECK_TRUE(buf[0] == 10, "operator[]");
    CHECK_TRUE(buf[1] == 20, "operator[]");
    CHECK_TRUE(buf[2] == 30, "operator[]");

    // back
    buf.push_back(40);

    CHECK_TRUE(buf.size() == 4, "push_back");
    CHECK_TRUE(buf[3] == 40, "operator[]");

    buf.pop_back();
    CHECK_TRUE(buf.size() == 3, "pop_back");

    // front
    int x = buf.pop_front();
    CHECK_TRUE(x == 10, "pop_front()");
    CHECK_TRUE(buf.size() == 2, "pop_front");
    CHECK_TRUE(buf[0] == 20, "operator[]");
    CHECK_TRUE(buf[1] == 30, "operator[]");

    x = buf.pop_front();
    CHECK_TRUE(x == 20, "pop_front()");
    CHECK_TRUE(buf.size() == 1, "pop_front");
    CHECK_TRUE(buf[0] == 30, "operator[]");
    
    buf.push_front(20);
    CHECK_TRUE(buf.size() == 2, "push_front");
    CHECK_TRUE(buf[0] == 20, "operator[]");


    buf.clear();
    CHECK_TRUE(buf.size() == 0, "clear");

    }
    {// insert... normal
    atd::Linear_array<int, 10> buf{0,1,2,3,4,5};
    buf.remove(2);
    CHECK_TRUE(buf.size() == 5, "remove");
    CHECK_TRUE(buf[0] == 0 and buf[1] == 1 and buf[2] == 3
	      and buf[3] == 4 and buf[4] == 5, "remove");

    buf.insert(2, 9);
    CHECK_TRUE(buf.size() == 6, "remove");
    CHECK_TRUE(buf[0] == 0 and buf[1] == 1 and buf[2] == 9
	      and buf[3] == 3 and buf[4] == 4 and buf[5] == 5, "insert");

    buf.remove(5);
    CHECK_TRUE(buf.size() == 5, "remove");
    CHECK_TRUE(buf[0] == 0 and buf[1] == 1 and buf[2] == 9
	      and buf[3] == 3 and buf[4] == 4, "insert");
    }

    {// insert... overflow
    atd::Linear_array<int, 4> buf{0,1,2,3};

    buf.insert(2, 9);
    CHECK_TRUE(buf.size() == 4, "insert");
    print(std::cout, buf); std::cout << '\n';
    CHECK_TRUE(buf[0] == 0 and buf[1] == 1 and buf[2] == 9
	      and buf[3] == 2, "insert");

    }

}


void test_array()
{
    test::interface("Array");

    {
	atd::Array<int, 4> x;
	CHECK_TRUE(x.size() == 0, "size");
	CHECK_TRUE(x.empty(), "empty");
	CHECK_TRUE(!x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");

	x.push_back(34);
	CHECK_TRUE(x.size() == 1, "size");
	CHECK_TRUE(!x.empty(), "empty");
	CHECK_TRUE(!x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");
	CHECK_TRUE(x[0] == 34, "operator[]");
	
	x.pop_back();
	CHECK_TRUE(x.size() == 0, "size");
	CHECK_TRUE(x.empty(), "empty");
	CHECK_TRUE(!x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");

	x.pop_back(); // intento fallido
	CHECK_TRUE(x.size() == 0, "size");
	CHECK_TRUE(x.empty(), "empty");
	CHECK_TRUE(!x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");

	x.push_back(10);
	x.push_back(20);
	x.push_back(30);
	x.push_back(40);
	CHECK_TRUE(x.size() == 4, "size");
	CHECK_TRUE(!x.empty(), "empty");
	CHECK_TRUE(x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");
	CHECK_TRUE(x[0] == 10 and x[1] == 20 and x[2] == 30 and x[3] == 40
		  , "operator[]");

	x.push_back(50); // intento fallido
	CHECK_TRUE(x.size() == 4, "size");
	CHECK_TRUE(!x.empty(), "empty");
	CHECK_TRUE(x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");
	CHECK_TRUE(x[0] == 10 and x[1] == 20 and x[2] == 30 and x[3] == 40
		  , "operator[]");

	x.clear();
	CHECK_TRUE(x.size() == 0, "size");
	CHECK_TRUE(x.empty(), "empty");
	CHECK_TRUE(!x.full(), "full");
	CHECK_TRUE(x.capacity() == 4, "capacity");

    }
}


void test_array_view()
{
    test::interface("Array_view");

    {// caso extremo
    int x[4];
    atd::Array_view<int> view{x, 0, 4};
    CHECK_TRUE(view.size() == 0, "size");
    CHECK_TRUE(view.capacity() == 4, "capacity");
    }

    {// caso normal
    int x[4];
    x[0] = 20;
    x[1] = 30;
    atd::Array_view<int> view{x, 2, 4};
    CHECK_TRUE(view.size() == 2, "size");
    CHECK_TRUE(view.capacity() == 4, "capacity");
    CHECK_TRUE(view[0] == x[0], "operator[]");
    CHECK_TRUE(view[1] == x[1], "operator[]");

    view.push_back(87);
    CHECK_TRUE(view.size() == 3, "size");
    CHECK_TRUE(x[2] == 87, "operator[]");
    CHECK_TRUE(view[2] == 87, "operator[]");


    }

}



//void test_container_view()
//{
//    test::interface("const_Container_view");
//
//    {// array
//    int x[2];
//    x[0] = 20;
//    x[1] = 30;
//    auto view = atd::const_container_view(x);
//    CHECK_TRUE(view.size() == 2, "size");
//    CHECK_TRUE(view[0] == x[0], "operator[]");
//    CHECK_TRUE(view[1] == x[1], "operator[]");
//    }
//
//    {// std::array
//    std::array<int,2> x = {11, 22};
//    auto view = atd::const_container_view(x);
//    CHECK_TRUE(view.size() == x.size(), "size");
//    CHECK_TRUE(view[0] == x[0], "operator[]");
//    CHECK_TRUE(view[1] == x[1], "operator[]");
//    }
//
//}

int main()
{
try{
    test::header("atd_array");

    test_circular_array();
    test_linear_array();
    test_array();
    test_array_view();
//    test_container_view();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





