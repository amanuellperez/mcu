// Copyright (C) 2024-2025 Manuel Perez 
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


#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <sstream>

#include <cstddef>
#include "../../atd_basic_types.h"

using namespace test;

// res = x en orden contrario
template <typename Int>
void test_digit_iterator(Int x, const std::string& res)
{
    atd::View_of_int v{x};

    std::stringstream out;
    for (auto d = v.digit_begin();
	      d != v.digit_end();
	      ++d){
	out << (int) *d; // (int) ya que si es uint8_t lo trata como char
    }
    CHECK_TRUE(out.str() == res, res);

}

void test_digit_iterator()
{
    test::interface("Digit_iterator");

    test_digit_iterator<int>(0, "0");
    test_digit_iterator<int>(123, "321");
    test_digit_iterator<int>(123456789, "987654321");

    test_digit_iterator<const int>(0, "0");
    test_digit_iterator<const int>(123, "321");
    test_digit_iterator<const int>(123456789, "987654321");

    test_digit_iterator<uint8_t>(0, "0");
    test_digit_iterator<uint8_t>(123, "321");
}


void test_uninitialized()
{
    test::interface("Uninitialized");

    atd::Uninitialized<int> x;

    CHECK_TRUE(!x, "Uninitialized");

    x = 10;

// Igualdad
    CHECK_TRUE(!x == false, "initialized");
    CHECK_TRUE(x == 10, "operator==");
    CHECK_TRUE(10 == x, "operator==");

// Orden
    CHECK_TRUE(x < 20, "operator<");
    CHECK_TRUE(x <= 20, "operator<=");
    CHECK_TRUE(x > 2, "operator>");
    CHECK_TRUE(x >= 2, "operator>=");

// Algebra
    auto y = ++x;
    CHECK_TRUE(y == 11, "operator++");
    CHECK_TRUE(x == 11, "operator++");

    y = x++;
    CHECK_TRUE(y == 11, "operator++");
    CHECK_TRUE(x == 12, "operator++");

    y = --x;
    CHECK_TRUE(y == 11, "operator--");
    CHECK_TRUE(x == 11, "operator--");

    y = x--;
    CHECK_TRUE(y == 11, "operator--");
    CHECK_TRUE(x == 10, "operator--");

    x += 2;
    CHECK_TRUE(x == 12, "operator+=");

    x -= 2;
    CHECK_TRUE(x == 10, "operator-=");

    y = x + 4;
    CHECK_TRUE(y == 14, "operator+");

    y = 4 + x;
    CHECK_TRUE(y == 14, "operator+");

    y = x - 4;
    CHECK_TRUE(y == 6, "operator-");

    x = 2;
    y =  10 - x;
    CHECK_TRUE(y == 8, "operator-");

    x = 50;
    x *= 2;
    CHECK_TRUE(x == 100, "operator*=");

    x /= 2;
    CHECK_TRUE(x == 50, "operator/=");

    y = x * 4;
    CHECK_TRUE(y == 200, "operator *");

    y = 4 * x;
    CHECK_TRUE(y == 200, "operator *");

    y = x / 2;
    CHECK_TRUE(y == 25, "operator /");

    x = 4;
    y = 100 / x;
    CHECK_TRUE(y == 25, "operator /");

}


int main()
{
try{
    test::header("atd_basic_types");

    test_digit_iterator();
    test_uninitialized();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





