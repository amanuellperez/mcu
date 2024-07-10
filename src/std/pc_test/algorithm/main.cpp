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

#include "../../std_algorithm.h"
#include "../../std_functional.h"

#include <alp_test.h>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace test;

void test_for_each()
{
    test::interfaz("for_each");
    {
    std::vector v = {1, 2, 3, 4};
    std::vector res = {2, 3, 4, 5};

    mtd::for_each (v.begin(), v.end(), [](int& x){++x;});
    
    CHECK_EQUAL_CONTAINERS  ( v.begin(), v.end()
			    , res.begin(), res.end(), "for_each");
    }

    {
    std::vector v = {1, 2, 3, 4};
    std::vector res = {2, 3, 4, 5};
    mtd::for_each_n (v.begin(), v.size(), [](int& x){++x;});
    CHECK_EQUAL_CONTAINERS  ( v.begin(), v.end()
			    , res.begin(), res.end(), "for_each_n");
    }
}

void test_minmax()
{
    test::interfaz("min/max");

    CHECK_TRUE(mtd::min(4, 6) == 4, "min(4,6)");
    CHECK_TRUE(mtd::min(4, 4) == 4, "min(4,4)");
    CHECK_TRUE(mtd::min(-7, -4) == -7, "min(-7,-4)");


    CHECK_TRUE(mtd::max(4, 6) == 6, "max(4,6)");
    CHECK_TRUE(mtd::max(4, 4) == 4, "max(4,4)");
    CHECK_TRUE(mtd::max(-7, -4) == -4, "max(-7,-4)");


}


void test_minmax_element()
{
    test::interfaz("min/max_element");

{// caso normal
    std::vector<int> v {2, 7, 1, 8, 3, 1, 4, 1, 5};

    CHECK_TRUE((mtd::min_element(v.begin(), v.end()) == 
		std::min_element(v.begin(), v.end())), "min_element(...)");

    CHECK_TRUE((mtd::min_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::min_element(v.begin(), v.end(), std::less<int>{})), "min_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end()) == 
		std::max_element(v.begin(), v.end())), "max_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::max_element(v.begin(), v.end(), std::less<int>{})), "max_element(...)");
}

{ // caso extremo
    std::vector<int> v {};

    CHECK_TRUE((mtd::min_element(v.begin(), v.end()) == 
		std::min_element(v.begin(), v.end())), "min_element(...)");

    CHECK_TRUE((mtd::min_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::min_element(v.begin(), v.end(), std::less<int>{})), "min_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end()) == 
		std::max_element(v.begin(), v.end())), "max_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::max_element(v.begin(), v.end(), std::less<int>{})), "max_element(...)");
}


{ // caso todo constante
    std::vector<int> v {3,3,3,3,3,3,3};

    CHECK_TRUE((mtd::min_element(v.begin(), v.end()) == 
		std::min_element(v.begin(), v.end())), "min_element(...)");

    CHECK_TRUE((mtd::min_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::min_element(v.begin(), v.end(), std::less<int>{})), "min_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end()) == 
		std::max_element(v.begin(), v.end())), "max_element(...)");

    CHECK_TRUE((mtd::max_element(v.begin(), v.end(), mtd::less<int>{}) == 
		std::max_element(v.begin(), v.end(), std::less<int>{})), "max_element(...)");
}
}


void test_copy()
{
    test::interfaz("copy");
    
    std::array<int, 5> a = {1,2,3,4,5};
    int res[10];
    for (int i = 0; i < 10; ++i)
	res[i] = -1;

    auto q = mtd::copy(a.begin(), a.end(), res);
    CHECK_TRUE(q == (res + a.size()), "copy, return");
    CHECK_EQUAL_CONTAINERS(a.begin(), a.end(), res, res + 5, "copy");
    for (int i = 5; i < 10; ++i)
	CHECK_TRUE(res[i] == -1, "copy?");


    for (int i = 0; i < 10; ++i)
	res[i] = -1;
    int pru[10];
    q = mtd::copy(pru, pru, res);
    CHECK_TRUE(q == res, "copy, return");
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(res[i] == -1, "copy (copia nula)");
}

void test_copy_n()
{
    test::interfaz("copy_n");
    
    std::array<int, 5> a = {1,2,3,4,5};
    int res[10];
    for (int i = 0; i < 10; ++i)
	res[i] = -1;

    auto q = mtd::copy_n(a.begin(), a.size(), res);
    CHECK_TRUE(q == (res + a.size()), "copy_n, return");
    CHECK_EQUAL_CONTAINERS(a.begin(), a.end(), res, res + 5, "copy_n");
    for (int i = 5; i < 10; ++i)
	CHECK_TRUE(res[i] == -1, "copy_n?");


    for (int i = 0; i < 10; ++i)
	res[i] = -1;
    int pru[10];
    q = mtd::copy_n(pru, 0, res);
    CHECK_TRUE(q == res, "copy_n, return");
    for (int i = 0; i < 10; ++i)
	CHECK_TRUE(res[i] == -1, "copy_n (copia nula)");
}


void test_fill()
{
    test::interfaz("fill");

    int x[5];
    int* xe = mtd::fill(&x[0], x + 5, 40);
    CHECK_TRUE(xe == (x + 5), "fill(5)::return");

    std::array<int, 5> res = {40, 40, 40, 40, 40};
    CHECK_EQUAL_CONTAINERS(&x[0], xe, res.begin(), res.end(), "fill");

    xe = mtd::fill(x, x, 3);
    CHECK_TRUE(xe == x, "fill(0)::return");
}


void test_fill_n()
{
    test::interfaz("fill_n");

    int x[5];
    int* xe = mtd::fill_n(x, 5, 20);
    CHECK_TRUE(xe == (x + 5), "fill_n(5)::return");

    std::array<int, 5> res = {20, 20, 20, 20, 20};
    CHECK_EQUAL_CONTAINERS(x, xe, res.begin(), res.end(), "fill_n");

    xe = mtd::fill_n(x, 0, 3);
    CHECK_TRUE(xe == x, "fill_n(0)::return");

}

template <typename It>
void test_find(It p0, It pe, char x, const std::string& res)
{
    auto p = mtd::find (p0, pe, x);
    std::string y{p0, p};
    CHECK_TRUE(res == y, "find");
}

struct Is_one{
    bool operator()(char x) const {return x == '1';}
};

template <typename It, typename UnaryPredicate>
void test_find_if(It p0, It pe, UnaryPredicate pred, const std::string& res)
{
    auto p = mtd::find_if (p0, pe, pred);
    std::string y{p0, p};
    CHECK_TRUE(res == y, "find_if");
}



template <typename It, typename UnaryPredicate>
void test_find_if_not(It p0, It pe, UnaryPredicate pred, const std::string& res)
{
    auto p = mtd::find_if_not (p0, pe, pred);
    std::string y{p0, p};
    CHECK_TRUE(res == y, "find_if_not");
}

void test_find()
{
    test::interfaz("find");
    {
    std::string s{"abcde"};

    test_find(s.begin(), s.end(), 'c', "ab");
    test_find(s.begin(), s.end(), 'A', s);
    test_find(s.begin(), s.end(), 'a', "");
    }
    {
    std::string s{"00123"};
    test_find_if(s.begin(), s.end(), Is_one{}, "00");
    s = "";
    test_find_if(s.begin(), s.end(), Is_one{}, "");
    s = "0000";
    test_find_if(s.begin(), s.end(), Is_one{}, "0000");
    }

    {
    std::string s{"111011"};
    test_find_if_not(s.begin(), s.end(), Is_one{}, "111");
    s = "";
    test_find_if_not(s.begin(), s.end(), Is_one{}, "");
    s = "1111";
    test_find_if_not(s.begin(), s.end(), Is_one{}, "1111");
    }
}

template <typename It, typename T>
void test_count(It p0, It pe, const T& x, const std::string& msg)
{
    std::cout << "count: ";
    CHECK_TRUE((std::count(p0, pe, '\n')
		== mtd::count(p0, pe, '\n')), msg);

}

template <typename It, typename UnaryPredicate>
void test_count_if(It p0, It pe, UnaryPredicate p, const std::string& msg)
{
    std::cout << "count_if: ";
    CHECK_TRUE((std::count_if(p0, pe, p)
		== mtd::count_if(p0, pe, p)), msg);

}


void test_count()
{
    test::interfaz("count");

    std::string msg = "uno-dos-tres";
    test_count(msg.begin(), msg.end(), '-', msg);

    msg = "";
    test_count(msg.begin(), msg.end(), '-', msg);
    
    msg = "-";
    test_count(msg.begin(), msg.end(), '-', msg);


    std::vector v = {1,2,3,4,5,6,7,8,9,10};
    test_count_if(v.begin(), v.end(), [](int x){return x % 3;}, "impares");
    test_count_if(v.begin(), v.end(), [](int x){return false;}, "nada");
    test_count_if(v.begin(), v.end(), [](int x){return true;}, "todos");

    v.clear();
    test_count_if(v.begin(), v.end(), [](int x){return x % 3;}, "vacio");
}

template <typename It1, typename It2>
void test_shift_left(int n, It1 p0, It1 pe, It2 y0, It2 y1, It1 res)
{
    auto y = mtd::shift_left(p0, pe, n);
    CHECK_EQUAL_CONTAINERS(p0, pe, y0, y1, "shift_left");
    CHECK_TRUE(y == res, "shift_left");
}

void test_shift_left()
{
    test::interfaz("shift_left");

    constexpr int sz = 7;
    int x[sz];
    {
    for (int i = 0; i < sz; ++i)
	x[i] = i;
    std::array res = {3,4,5,6,4,5,6};
    test_shift_left(0, x, x, x, x, x); // contenedor vacío!!!
    test_shift_left(0, x, x + sz, x, x + sz, x + sz);
    test_shift_left(sz, x, x + sz, x, x + sz, x);
    test_shift_left(2*sz, x, x + sz, x, x + sz, x);
    test_shift_left(3, x, x + sz, res.begin(), res.end(), x + 4);
    }

}



void test_reverse(int* x, int* sx, size_t N)
{
    mtd::reverse(x, x + N);
    std::reverse(sx, sx + N);

    CHECK_EQUAL_CONTAINERS(x, x + N, sx, sx + N, "reverse");
}


void test_reverse()
{
    test::interfaz("reverse");

{// extreme: 0
    int* x{};
    int* sx{};
    test_reverse(x, sx, 0);
}

{// extreme: 1
    int x[1]{2};
    int sx[1]{2};
    test_reverse(x, sx, 1);
}

{// normal: even
    int x[4] = {1,2,3,4};
    int sx[4] = {1,2,3,4};
    test_reverse(x, sx, 4);
}

{// normal: odd
    int x[5] = {1,2,3,4,5};
    int sx[5] = {1,2,3,4,5};
    test_reverse(x, sx, 5);
}
}


int main()
{
try{
    test::header("algorithm");

    test_for_each();
    test_minmax();
    test_minmax_element();
    test_copy();
    test_copy_n();
    test_fill();
    test_fill_n();
    test_find();
    test_count();
    test_shift_left();
    test_reverse();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





