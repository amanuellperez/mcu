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

#include "../../atd_istream.h"
#include "../../atd_ostream.h"

#include <streambuf>
#include "alp_streambuf.h"

#include <alp_test.h>
#include <iostream>
#include <vector>



using namespace test;

class prueba_ostream : public std::ostream{
public:
    explicit prueba_ostream(LCD& lcd):
	ostream{&sb_}, 
	sb_{lcd} {}

    virtual ~prueba_ostream() { }

private:
    prueba_streambuf sb_;
    
};

class prueba_istream : public std::istream{
public:
    explicit prueba_istream(UART& uart):
	istream{&sb_}, sb_{uart} 
    {
	sb_.as_input();
    }

private:
    fix_streambuf sb_;
};

void test_ostream()
{    
    test::interface("atd_ostream");

    LCD lcd;
    prueba_ostream out{lcd};

    {

    uint8_t x = 65;
    std::cout << "out: [65] =? [";
    out << atd::write_as_uint8_t(x);
    std::cout << "]\n";

    x = 0;
    std::cout << "out: [0] =? [";
    out << atd::write_as_uint8_t(x);
    std::cout << "]\n";
    }

    {
    int8_t x = 65;
    std::cout << "out: [65] =? [";
    out << atd::write_as_int8_t(x);
    std::cout << "]\n";

    x = 0;
    std::cout << "out: [0] =? [";
    out << atd::write_as_int8_t(x);
    std::cout << "]\n";

    x = -87;
    std::cout << "out: [-87] =? [";
    out << atd::write_as_int8_t(x);
    std::cout << "]\n";
    }

}

void test_istream()
{
    test::interface("atd_istream");

    UART uart{"12 0 56 0 -78"};
    prueba_istream in{uart};
    
    {
    uint8_t x{4};
    in >> atd::read_as_uint8_t(x);
    CHECK_TRUE(!in.fail(), "in.good");
    CHECK_TRUE(x == uint8_t{12}, "read_as_uint8_t(12)");

    in >> atd::read_as_uint8_t(x);
    CHECK_TRUE(!in.fail(), "in.good");
    CHECK_TRUE(x == uint8_t{0}, "read_as_uint8_t(0)");
    }

    {
    int8_t x{4};
    in >> atd::read_as_int8_t(x);
    CHECK_TRUE(!in.fail(), "in.good");
    CHECK_TRUE(x == int8_t{56}, "read_as_int8_t(56)");

    in >> atd::read_as_int8_t(x);
    CHECK_TRUE(!in.fail(), "in.good");
    CHECK_TRUE(x == int8_t{0}, "read_as_int8_t(0)");

    in >> atd::read_as_int8_t(x);
    CHECK_TRUE(!in.fail(), "in.good");
    CHECK_TRUE(x == int8_t{-78}, "read_as_int8_t(-78)");
    }

}


int main()
{
try{
    test::header("atd_ostream/istream");

    test_ostream();
    test_istream();

}catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





