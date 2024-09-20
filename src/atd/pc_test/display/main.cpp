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
#include "../../atd_display.h"

#include "my_rom.h"
#include "dogica_cr.h"
#include "dogica_rf.h"

#include "upheavtt_cr.h"
#include "upheavtt_rf.h"

#include "vcr_cr.h"
#include "depixel_rf.h"

using namespace test;

template <typename Cfg>
using Text_display =  atd::Monochromatic_text_display<Cfg>;

struct Cout {
};

struct Text_display_cfg{
// Text buffer
    using index_type = int;
    static constexpr index_type text_rows = 4;
    static constexpr index_type text_cols = 10;

// Windows
    using Font = rom::font_dogica_8x8_cr::Font;
    using Bitmatrix = atd::Bitmatrix_col_1bit<16, 32>; // la del MAX7219

// Display output
    using Display    = Cout;
};

template <typename C>
void print_buffer(std::ostream& out, const Text_display<C>& txt)
{
    out << "\n>>>-------------------------------\n";
    for (auto c: txt){
	if (c == '\0')
	    out << "\\0";
	else
	    out << c;
    }

    out << "\n<<<-------------------------------\n";
}

template <typename C>
void print(std::ostream& out, const Text_display<C>& txt)
{
    out << "\n>>>-------------------------------\n";
    for (int i = 0; i < txt.text_rows(); ++i){
	for (int j = 0; j < txt.text_cols(); ++j){
	    char c = txt.read({i, j});
	    if (c == '\0')
		out << "_";
	    else
		out << c;
	}
	out << '\n';
    }
    out << "<<<-------------------------------\n";
}
void test_text_display()
{
    test::interface("Monochromatic_text_display");
    Text_display<Text_display_cfg> txt;

    txt.clear();
    print(std::cout, txt);

    txt.write({0,0}, "abcd");
    print(std::cout, txt);

    txt.write({3,0}, "123456789012345");

    print(std::cout, txt);
}

int main()
{
try{
    test::header("atd_display");

    test_text_display();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





