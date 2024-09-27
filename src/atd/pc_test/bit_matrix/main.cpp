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

#include "my_rom.h"
#include "dogica_cr.h"
#include "dogica_rf.h"

#include "upheavtt_cr.h"
#include "upheavtt_rf.h"

#include "vcr_cr.h"
#include "depixel_rf.h"

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

template <size_t nrows, size_t ncols>
void print_bytes(const atd::Bitmatrix_col_1bit<nrows, ncols>& m)
{
    std::cout << std::hex;

    for (uint8_t i = 0; i < m.rows_in_bytes(); ++i){
	for (uint8_t j = 0; j < m.cols_in_bytes(); ++j){
		std::cout << "0x" << (int) m.read_byte(i,j) << ' ';
	}
	std::cout << '\n';

    }
}

template <typename Font, uint16_t nrows, uint16_t ncols>
void test_font_row(const std::string& msg)
{
    using BM = atd::Bitmatrix_row_1bit<nrows, ncols>;
    using Coord_ij = BM::Coord_ij;
    static uint16_t w = 8*Font::cols_in_bytes; // ancho de una letra
    static uint16_t h = Font::rows; // alto de una letra

    BM bm;
    std::cout << "\n\n";
    bm.clear();
    uint16_t k = 0;
    for (auto c: msg){
	uint16_t j = k * w;
	atd::write<Font, nrows, ncols>(bm, Coord_ij{0, j}, c);
	++k;
    }

    k = 0;
    for (auto c: msg){
	uint16_t i = h + 4;
	uint16_t j = k * w;
	atd::write<Font, nrows, ncols>(bm, Coord_ij{i, j}, c);
	++k;
    }
    print(bm);
//    print_bytes(bm);
}


void test_bitmatrix_row_1bit()
{
    test::interface("Bitmatrix_row_1bit");

    constexpr uint8_t rows = 8;
    constexpr uint8_t cols = 8*5;

    using BM = atd::Bitmatrix_row_1bit<rows, cols>;

    BM bm;

    CHECK_TRUE(bm.rows() == rows, "rows()");
    CHECK_TRUE(bm.cols() == cols, "cols()");

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

// write_byte
// TODO: no es automático!!!
    bm.fill(0);
    bm.write_byte(127, 0,0);
    bm.write_byte(231, 0,8);

    std::cout << "From 0 to (row - 1): ";
    for (auto c = bm.row_begin(0); c != bm.row_end(0); ++c)
	std::cout << (int) *c << ' ';
    std::cout << '\n';

    std::cout << "From (row - 1) to 0: ";
    for (auto c = bm.rrow_begin(0); c != bm.rrow_end(0); ++c)
	std::cout << (int) *c << ' ';
    std::cout << '\n';

// font
// ----
// TODO: no es automático!!!
    using Dogica = rom::font_dogica_8x8_rf::Font;
    using Upheavtt = rom::font_upheavtt_13x14_rf::Font;
    using Depixel= rom::font_DePixelBreitFett_23x13_rf::Font;

// Entra todo
    test_font_row<Dogica  , 2*Dogica::rows, 8*Dogica::cols_in_bytes * 6>("Hello");
    test_font_row<Upheavtt, 2*Upheavtt::rows, 8*Upheavtt::cols_in_bytes * 6>("Hello");
    test_font_row<Depixel , 2*Depixel::rows, 8*Depixel::cols_in_bytes*6>("Hello");


}

template <typename Font, uint8_t nrows, uint8_t ncols>
void test_font_col()
{
    using BM = atd::Bitmatrix_col_1bit<nrows, ncols>;
    using Coord_ij = BM::Coord_ij;
    static uint16_t w = Font::cols; // ancho de una letra
    static uint16_t h = 8*Font::rows_in_bytes; // alto de una letra

    BM bm;
    std::cout << "\n\n";
    bm.clear();
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, 0}, 'H');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, uint16_t(w)}, 'e');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, uint16_t(2*w)}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, uint16_t(3*w)}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{0, uint16_t(4*w)}, 'o');

    // hacemos un pequeño scroll
    atd::write<Font, nrows, ncols>(bm, Coord_ij{h, uint16_t(w + 1)}, 'H');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{h, uint16_t(2*w + 1u)}, 'e');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{h, uint16_t(3*w + 1u)}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{h, uint16_t(4*w + 1u)}, 'l');
    atd::write<Font, nrows, ncols>(bm, Coord_ij{h, uint16_t(5*w + 4)}, 'o');
    print(bm);
//    print_bytes(bm);
}

void test_bitmatrix_col_1bit()
{
    test::interface("Bitmatrix_col_1bit");

    constexpr uint8_t nrows = 16;
    constexpr uint8_t ncols = 30;
    using BM = atd::Bitmatrix_col_1bit<nrows, ncols>;

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


    using Dogica = rom::font_dogica_8x8_cr::Font;
    using Upheavtt = rom::font_upheavtt_13x14_cr::Font;
    using VCR = rom::font_VCR_12x17_cr::Font;

// Entra todo
    test_font_col<Dogica  , 2*8*Dogica::rows_in_bytes, Dogica::cols * 6>();
    test_font_col<Upheavtt, 2*8*Upheavtt::rows_in_bytes, Upheavtt::cols*6>();
    test_font_col<VCR     , 2*8*VCR::rows_in_bytes, VCR::cols*6>();

//  No entra
    std::cout << "\nTrying to write outside of matrix:\n";
    test_font_col<Upheavtt, 3 * 8, Upheavtt::cols*6>();
    test_font_col<VCR , 2*8*VCR::rows_in_bytes - 8, VCR::cols*6>();

    bm.write_byte(127, 0,0);
    bm.write_byte(231, 8,0);

    std::cout << "From 0 to (row - 1): ";
    for (auto c = bm.col_begin(0); c != bm.col_end(0); ++c)
	std::cout << (int) *c << ' ';
    std::cout << '\n';

    std::cout << "From (row - 1) to 0: ";
    for (auto c = bm.rcol_begin(0); c != bm.rcol_end(0); ++c)
	std::cout << (int) *c << ' ';
    std::cout << '\n';
}


int main()
{
try{
    test::header("atd_bit_matrix");

//    test_bitmatrix_row_1bit();
    test_bitmatrix_col_1bit();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





