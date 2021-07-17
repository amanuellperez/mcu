// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../atd_buffer.h"

#include <alp_test.h>
#include <alp_string.h>

using namespace test;

void test_buffer()
{
    test::interfaz("Buffer");

    {// caso extremo
    atd::Buffer<int, 5> buffer;
    CHECK_TRUE(buffer.size() == 0, "size");
    }

    {
    atd::Buffer<int, 5> buf{10,20,30};
    CHECK_TRUE(buf.size() == 3, "size");
    CHECK_TRUE(buf[0] = 10, "operator[]");
    CHECK_TRUE(buf[1] = 20, "operator[]");
    CHECK_TRUE(buf[2] = 30, "operator[]");
    buf.push_back(40);
    CHECK_TRUE(buf.size() == 4, "push_back");
    CHECK_TRUE(buf[3] = 40, "operator[]");
    buf.pop_back();
    CHECK_TRUE(buf.size() == 3, "pop_back");
    buf.clear();
    CHECK_TRUE(buf.size() == 0, "clear");

    }

}



int main()
{
try{
    test::header("atd_buffer");

    test_buffer();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





