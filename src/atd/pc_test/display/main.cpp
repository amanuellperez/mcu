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
using Text_block =  atd::Text_block<Cfg>;

template <typename Cfg>
using Subtext_block =  atd::Subtext_block<Cfg>;

template <typename Cfg>
using Text_block_with_view =  atd::Text_block_with_view<Cfg>;

struct Cout {
};

struct Text_block_cfg{
// Text buffer
    using index_type = int;
    //using index_type = unsigned int;
    static constexpr index_type text_rows = 4;
    static constexpr index_type text_cols = 10;

// Ventana que vemos del buffer
    static constexpr bool cylinder_type{};
    static constexpr index_type subtext_rows = 2;
    static constexpr index_type subtext_cols = 5;
    
// Windows
    using Font = rom::font_dogica_8x8_cr::Font;
    using Bitmatrix = atd::Bitmatrix_col_1bit<16, 32>; // la del MAX7219

// Display output
    using Display    = Cout;
};

template <typename C>
void print_buffer(std::ostream& out, const Text_block<C>& txt)
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
void print(std::ostream& out, const Text_block<C>& txt)
{
    using index_t = Text_block<C>::index_type;

    out << "\n>>>-------------------------------\n";
    for (index_t i = 0; i < txt.rows(); ++i){
	for (index_t j = 0; j < txt.cols(); ++j){
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

template <typename Cfg>
void print(std::ostream& out, const Subtext_block<Cfg>& sub)
{
    using index_t = Subtext_block<Cfg>::index_type;

    for (index_t i = 0; i < sub.rows(); ++i){
	for (index_t j = 0; j < sub.cols(); ++j){
	    out << sub.read({i,j}) << ' ' ;
	}
	out << '\n';
    }
}

template <typename C>
void print_bg(std::ostream& out, const Text_block_with_view<C>& txt)
{
    using index_t = Text_block<C>::index_type;

    out << "\n>>>-------------------------------\n";
    for (index_t i = 0; i < txt.bg_rows(); ++i){
	for (index_t j = 0; j < txt.bg_cols(); ++j){
	    char c = txt.bg_read({i, j});
	    if (c == '\0')
		out << "_";
	    else
		out << c;
	}
	out << '\n';
    }
    out << "<<<-------------------------------\n";
}


template <typename C>
void print_view(std::ostream& out, const Text_block_with_view<C>& txt)
{
    using index_t = Text_block<C>::index_type;

    out << "\n>>>-------------------------------\n";
    for (index_t i = 0; i < txt.view_rows(); ++i){
	for (index_t j = 0; j < txt.view_cols(); ++j){
	    char c = txt.view_read({i, j});
	    if (c == '\0')
		out << "_";
	    else
		out << c;
	}
	out << '\n';
    }
    out << "<<<-------------------------------\n";
}
void test_text_block()
{
    test::interface("Text_block");
    Text_block<Text_block_cfg> txt;
    using index_t = Text_block<Text_block_cfg>::index_type;

    txt.clear();
    print(std::cout, txt);

    txt.write({0,0}, "abcd");
    print(std::cout, txt);

    txt.write({3,0}, "123456789012345");

    print(std::cout, txt);

    txt.write({0,0}, "1234567890abcdefghijklmnopqrstuvwxyzABCD");
    print(std::cout, txt);

    Subtext_block<Text_block_cfg> sub{txt};
    std::cout << "\nSub(0,0)\n"
	           "--------\n";
    print(std::cout, sub);

    sub.move_down(1);
    std::cout << "\nmove_down(1)\n"
	           "--------------\n";
    print(std::cout, sub);

    for (index_t i = 0; i < 4; ++i){
	sub.move_right(2);

	std::cout << "\nmove_right(2)\n"
		       "--------------\n";
	print(std::cout, sub);
    }

    for (index_t i = 0; i < 4; ++i){
	sub.move_left(2);
	std::cout << "\nmove_left(2)\n"
		       "--------------\n";
	print(std::cout, sub);
    }
}

void test_text_block_with_view()
{
    test::interface("Text_block_with_view");
    
    using Cfg = Text_block_cfg;
    Text_block_with_view<Cfg> txt;
    CHECK_TRUE(txt.bg_rows() == Cfg::text_rows, "bg_rows");
    CHECK_TRUE(txt.bg_cols() == Cfg::text_cols, "bg_cols");

    txt.bg_write({0,0}, "1234567890abcdefghijklmnopqrstuvwxyzABCD");
    print_bg(std::cout, txt);

    std::cout << "View:";
    print_view(std::cout, txt);
    txt.view_move_up(2);
    std::cout << "move_up(2):";
    print_view(std::cout, txt);

    txt.view_move_down(2);
    std::cout << "move_down(2):";
    print_view(std::cout, txt);


    txt.view_move_left(2);
    std::cout << "move_left(2):";
    print_view(std::cout, txt);

    txt.view_move_right(2);
    std::cout << "move_right(2):";
    print_view(std::cout, txt);
    

}

void test_monochromatic_text_display()
{
    test::interface("Monochromatic_text_display");

}

int main()
{
try{
    test::header("atd_display");
    std::cerr << "NO SON AUTOMÁTICAS!!!";

    test_text_block();
    test_text_block_with_view();
//    test_monochromatic_text_display();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





