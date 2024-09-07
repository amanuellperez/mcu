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
#include "../../atd_draw.h"
#include "dogica.h"

using namespace test;

template <size_t nrows, size_t ncols>
void print(const atd::Bitmatrix_row_1bit<nrows, ncols>& m)
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


template <size_t nrows, size_t ncols>
void print(const atd::Bitmatrix_col_1bit<nrows, ncols>& m)
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


void test_bitmatrix_row_1bit()
{
    test::interface("Bitmatrix_row_1bit");

    using BM = atd::Bitmatrix_row_1bit<3, 16>;

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

void test_bitmatrix_col_1bit()
{
    test::interface("Bitmatrix_col_1bit");

    constexpr uint8_t nrows = 16;
    constexpr uint8_t ncols = 30;
    using BM = atd::Bitmatrix_col_1bit<nrows, ncols>;
    using Coord_ij = BM::Coord_ij;

    BM bm;

    CHECK_TRUE(bm.rows() == nrows, "rows()");
    CHECK_TRUE(bm.cols() == ncols, "cols()");
    CHECK_TRUE(bm.col_size(0) == nrows / 8, "col_size()");

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

    bm(0,4) = 1;
    CHECK_TRUE(bm(0,4) == 1, "operator()");

    bm(0,4) = 0;
    CHECK_TRUE(bm(0,4) == 0, "operator()");

    using Font = rom::font_dogica_8x8_cr::Font;
    bm.clear();
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, 0}, 'H');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, 0}, 'H');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, 8}, 'e');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, 16}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{8, 0}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{8, 8}, 'o');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{8, 16}, '!');
    print(bm);
}


int main()
{
try{
    test::header("atd_bit_matrix");

    test_bitmatrix_row_1bit();
    test_bitmatrix_col_1bit();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





