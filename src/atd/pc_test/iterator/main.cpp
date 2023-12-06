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


#include "../../atd_iterator.h"
#include <alp_test.h>

#include <iostream> 

using namespace test;

template <size_t N>
struct My_array{
    using value_type = int;
    using size_type  = size_t;
    using iterator   = atd::ROM_iterator<My_array>;

    size_type size() const { return N; }
    const value_type operator[](size_type i) const {return data[i];}

    iterator begin() const {return iterator{*this, 0};}
    iterator end() const {return iterator{*this, size()};}

    value_type data[N];
};

void test_rom_iterator()
{
    test::interface("rom_iterator");

// basic test
    My_array<6> a = {1,2,3,4,5,6};


    size_t i  = 0; 
    auto p = a.begin();
    while (p  != a.end() and i < a.size() ){
	CHECK_TRUE(*p == a[i], "operator*");
	++p; ++i;
    }

    CHECK_TRUE(i == a.size(), "size");
}



int main()
{
try{
    test::header("atd_iterator");

    test_rom_iterator();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





