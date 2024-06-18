// Copyright (C) 2022 Manuel Perez 
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

#include "../../atd_names.h"

#include <alp_test.h>
#include <iostream>



using namespace test;

void f(const nm::Width<short>& x)
{
    CHECK_STDOUT((short) x, "20");
}

void test_width()
{
    test::interface("Width");

    nm::Width x{10};
    CHECK_TRUE(x == int{10}, "Width<int>");

    short s = 20;
    f(s); // para ver que funciona conversión implícita
    f(nm::Width{s});
}

enum Type{
    from_to,
    from_size
};

Type print(nm::From<int> x0, nm::To<int> x1)
{
    std::cout << "from " << x0 << " to " << x1 << '\n';

    return Type::from_to;
}


Type print(nm::From<int> x0, nm::Size<int> x1)
{
    std::cout << "from " << x0 << " size " << x1 << '\n';

    return Type::from_size;
}

void test_fromto()
{
    test::interface("from, to...");

    CHECK_TRUE ((print(nm::From{2}, nm::To{10}) == Type::from_to), "from_to");
    CHECK_TRUE ((print(nm::From{2}, nm::Size{10}) == Type::from_size), "from_size");
}


int main()
{
try{
    test::header("atd_names");

    test_width();
    test_fromto();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





