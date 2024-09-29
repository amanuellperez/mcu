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

template <typename Cfg>
using Text_display = atd::Monochromatic_text_display<Cfg>;

struct Cout_by_columns {
    static constexpr int rows = 2;
    static constexpr int cols = 6;
    using Bitmatrix = atd::Bitmatrix_col_1bit<rows*8, cols * 8>;

    void write(const Bitmatrix& m)
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

};

template <int rows0, int cols0>
struct Cout_by_rows_t{
    static constexpr int rows = rows0;
    static constexpr int cols = cols0;
    using Bitmatrix = atd::Bitmatrix_row_1bit<rows*8, cols * 8>;

    void write(const Bitmatrix& m)
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

};

using Cout_by_rows = Cout_by_rows_t<2, 6>;

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
};


template <typename Font0, typename Cout, int text_rows0 = 4, int text_cols0 = 10>
struct Display_cfg{
// Text buffer
    using index_type = int;
    //using index_type = unsigned int;
    static constexpr index_type text_rows = text_rows0;
    static constexpr index_type text_cols = text_cols0;

// Ventana que vemos del buffer
    static constexpr bool cylinder_type{};
    
// Fuente
    using Font = Font0;

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
    
    txt.view_write({0,0}, '(');
    txt.view_write({0,4}, ')');
    txt.view_write({1,0}, '<');
    txt.view_write({1,4}, '>');
    print_view(std::cout, txt);

}

template <typename Font, typename Cout>
void test_monochromatic_text_display(const std::string& str)
{
    test::interface("Monochromatic_text_display");
    std::cout << "\n***********************************\n"
	      << str
	      << "\n***********************************\n";

    Text_display<Display_cfg<Font, Cout>> display;

    display.bg_write({0,0}, "1234567890abcdefghijklmnopqrstuvwxyzABCD");
    print_bg(std::cout, display);
    display.flush();

    std::cout << "Move to the right\n";
    for (int i = 0; i < 5; ++i){
	display.view_move_right(2);
	std::cout << "\n";
	display.flush();
    }

    std::cout << "Move down\n";
    for (int i = 0; i < 4; ++i){
	display.view_move_down(1);
	std::cout << "\n";
	display.flush();
    }

}


template <typename Font, typename Cout>
void test_bug(const std::string& str)
{
    test::interface("test_bug");
    std::cout << "\n***********************************\n"
	      << str
	      << "\n***********************************\n";

    Text_display<Display_cfg<Font, Cout, 2, 20>> display;

    display.bg_write({0,0}, "ew to               "
			    "        the right!  ");
    print_bg(std::cout, display);
    display.flush();

    display.view_move_right(1);
    std::cout << "\n";
    display.flush();
    print_bg(std::cout, display);
    print_view(std::cout, display);

}

int main()
{
try{
    test::header("atd_display");
    std::cerr << "NO SON AUTOMÁTICAS!!!";

    test_text_block();
    test_text_block_with_view();

    using Dogica_cr = rom::font_dogica_8x8_cr::Font;
    test_monochromatic_text_display<Dogica_cr, Cout_by_columns>("Dogica_cr");

    using Dogica_rf = rom::font_dogica_8x8_rf::Font;
    test_monochromatic_text_display<Dogica_rf, Cout_by_rows>("Dogica_rf");

    using Upheavtt_cr = rom::font_upheavtt_13x14_cr::Font;
    test_monochromatic_text_display<Upheavtt_cr , Cout_by_columns>("Upheavtt_cr");

    using Depixel_rf = rom::font_DePixelBreitFett_23x13_rf::Font;
    test_monochromatic_text_display<Depixel_rf, Cout_by_rows>("Depixel_rf");

    using Upheavtt_rf = rom::font_upheavtt_13x14_rf::Font;
    test_bug<Upheavtt_rf, Cout_by_rows_t<2, 4>>("Upheavtt_rf");

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





