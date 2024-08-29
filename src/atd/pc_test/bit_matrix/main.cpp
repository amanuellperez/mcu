// Copyright (C) 2024 Manuel Perez 
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

#include <cstddef>
#include "../../atd_bit_matrix.h"

using namespace test;

template <size_t nrows, size_t ncols>
void print(const atd::Bit_matrix_by_rows<nrows, ncols>& m)
{
    for (uint8_t i = 0; i < m.rows(); ++i){
	for (uint8_t j = 0; j < m.cols(); ++j){
	    if (m(i, j) == 1)
		std::cout << 'X';
	    else
		std::cout << '.';
	}
	std::cout << '\n';

    }
}


void test_bit_matrix_by_rows()
{
    test::interface("Bit_matrix_by_rows");

    using BM = atd::Bit_matrix_by_rows<3, 16>;

    BM bm;

    CHECK_TRUE(bm.rows() == 3, "rows()");
    CHECK_TRUE(bm.cols() == 16, "cols()");

    bm(0,0) = 1;
    CHECK_TRUE(bm(0,0) == 1, "operator()");

    bm(0,0) = 0;
    CHECK_TRUE(bm(0,0) == 0, "operator()");

    bm.fill(0); 
    std::cout << "\nZero matrix:\n";
    print(bm);

    bm.fill(1); 
    std::cout << "\nOne matrix:\n";
    print(bm);


}

int main()
{
try{
    test::header("atd_bit_matrix");

    test_bit_matrix_by_rows();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





