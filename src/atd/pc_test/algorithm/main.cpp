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

#include "../../atd_algorithm.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>



using namespace test;

void test_find_c(const char* msg, char c, const std::string& res)
{
    const char* p = atd::find_c(msg, c);
    std::string y{msg, p};

    CHECK_TRUE(y == res, alp::as_str() << '['  << y << "] != [" << res << ']');
}

void test_find_c(const char* msg, char c, const std::string& res, int n)
{
    const char* p = atd::find_c(msg, c, n);
    std::string y{msg, p};

    CHECK_TRUE(y == res, alp::as_str() << '['  << y << "] != [" << res << ']');
}

void test_find_c()
{    
    test::interface("find_c");

    test_find_c("1.Opción 1\n2. Otra\n3. La tercera", '\n',
		   "1.Opción 1");

    test_find_c("abc", 'a',
		   "");

    test_find_c("", '\n',
		   "");

    test_find_c("esto no tiene", '\n',
		   "esto no tiene");


    test_find_c("21.Opción 1\n2. Otra\n3. La tercera", '\n',
		"21.Opción 1\n2. Otra", 2);

    test_find_c("21.Opción 1\n2. Otra\n3. La tercera", '\n',
		"21.Opción 1\n2. Otra\n3. La tercera", 3);

    test_find_c("", '\n',
		"", 3);

    test_find_c("aaa", 'a',
		"aa", 3);

    test_find_c("Sin el caracter buscado", '\n',
		"Sin el caracter buscado", 3);
}


void test_copy()
{
    test::interface("copy");

    {// p > q
    std::vector<int> p {1,2,3,4};
    std::vector<int> q {5,6};

    auto [p0, q0] = atd::copy(p.begin(), p.end(), q.begin(), q.end());
    CHECK_TRUE(*p0 == 3 and q0 == q.end(), "copy(p > q)");
    CHECK_EQUAL_CONTAINERS(p.begin(), p0, q.begin(), q.end(), "copy(p > q)");
    }

    {// p == q
    std::vector<int> p {1,2,3,4};
    std::vector<int> q {5,6,7,8};

    auto [p0, q0] = atd::copy(p.begin(), p.end(), q.begin(), q.end());
    CHECK_TRUE(p0 == p.end() and q0 == q.end(), "copy(p == q)");
    CHECK_EQUAL_CONTAINERS(p.begin(), p0, q.begin(), q.end(), "copy(p == q)");
    }

    {// p < q
    std::vector<int> p {1,2};
    std::vector<int> q {5,6,7,8};

    auto [p0, q0] = atd::copy(p.begin(), p.end(), q.begin(), q.end());
    CHECK_TRUE(p0 == p.end() and *q0 == 7, "copy(p < q)");
    CHECK_EQUAL_CONTAINERS(p.begin(), p0, q.begin(), q0, "copy(p < q)");
    }
    {// degenerado: p == empty
    std::vector<int> p;
    std::vector<int> q {5,6,7,8};

    auto [p0, q0] = atd::copy(p.begin(), p.end(), q.begin(), q.end());
    CHECK_TRUE(p0 == p.end() and *q0 == 5, "copy(p empty)");
    CHECK_EQUAL_CONTAINERS(p.begin(), p0, q.begin(), q0, "copy(p empty)");
    }
}

int main()
{
try{
    test::header("atd_algorithm");

    test_find_c();
    test_copy();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





