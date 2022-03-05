// Copyright (C) 2019-2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
#pragma once

#ifndef __DEV_LCD_SCREEN_TCC__
#define __DEV_LCD_SCREEN_TCC__

//#include <iterator>	    // std::reverse_iterator
//#include <algorithm>	    // min
//#include <array>

//#include <atd_bit.h>
//#include <atd_algorithm.h>
//#include <atd_string.h>	    // atd::int_to_string
//#include <atd_double.h>
//#include <avr_time.h>

#include <atd_math.h>	    // Digits_of



namespace dev{

/***************************************************************************
 *			    LCD_screen
 ***************************************************************************/
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
template <typename Font>
inline void LCD_screen<num_cols, num_rows, LCD>::load()
{ Font::load(*this); }



template <uint8_t num_cols, uint8_t num_rows, typename LCD>
uint8_t LCD_screen<num_cols, num_rows, LCD>::read(uint8_t x, uint8_t y)
{
    cursor_pos(x, y);
    uint8_t c = lcd_.read();
    cursor_pos(x_, y_);
    return c;
}



template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::clear_row(uint8_t i)
{
    cursor_pos(0, i);
    for (uint8_t j = 0; j < cols(); ++j)
	lcd_.print(' ');
}




template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::cursor_move()
{
    ++x_;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_printable_char(char c)
{
    lcd_.print(c);
    cursor_move();
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_return()
{
    ++y_;   // este es '\n'
    if (y_ == rows()){
	--y_;
    }
    cursor_pos(0, y_);
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
bool LCD_screen<num_cols, num_rows, LCD>::print(char c) 
{
    static_assert(sizeof(char) == sizeof(uint8_t)
		 , "sizeof(char) != 1 byte!!!");

    if (x_ == cols())
	return false;

    if (c == '\n')
	print_return();
    else
	print_printable_char(c);

    return true;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
bool LCD_screen<num_cols, num_rows, LCD>::print_extended(char c) 
{
    static_assert(sizeof(char) == sizeof(uint8_t)
		 , "sizeof(char) != 1 byte!!!");

    lcd_.print(c);
    cursor_move();

    return true;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
uint8_t LCD_screen<num_cols, num_rows, LCD>::print(const char* c)
{
    uint8_t n = 0;
    while(*c){
	if (!print(*c++))
	    return n;

	++n;
    }

    return n;
}


// TODO: cambiar '0' + *p por una función. Está atd::digit_to_ascii, pero no
// me gusta el nombre.
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
template <typename Int>
uint8_t LCD_screen<num_cols, num_rows, LCD>::print_unsigned_number(const Int& x,
                                                                int ndigits)
{
    atd::Digits_from_left_to_right d{x, ndigits};

    uint8_t n = 0; 
    for (auto p = d.begin(); p != d.end(); ++p, ++n){
	if (!print(static_cast<char>('0' + *p)))
	    return n;
    }
    
    return n;
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
template <typename Int>
uint8_t LCD_screen<num_cols, num_rows, LCD>::print_signed_number(const Int& x,
                                                              int ndigits)
{
    atd::Digits_from_left_to_right d{x, ndigits};

    if (x < 0){
	if (ndigits > 0)
	    --ndigits;

	if (!print('-'))
	    return 0;

	return print_unsigned_number(-x, ndigits) + 1;
    }

    else 
	return print_unsigned_number(x, ndigits);
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(uint16_t n)
{ return print_unsigned_number(n); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(const uint32_t& n)
{ return print_unsigned_number(n); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(const uint64_t& n)
{ return print_unsigned_number(n); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(int16_t n)
{ return print_signed_number(n); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(const int32_t& n)
{ return print_signed_number(n); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(const int64_t& n)
{ return print_signed_number(n); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print_number(uint8_t n
    , const atd::Width<int>& w)
{ return print_unsigned_number(n, w); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(uint16_t n
    , const atd::Width<int>& w)
{ return print_unsigned_number(n, w); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(uint32_t n
    , const atd::Width<int>& w)
{ return print_unsigned_number(n, w); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(uint64_t n
    , const atd::Width<int>& w)
{ return print_unsigned_number(n, w); }



template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print_number(int8_t n
    , const atd::Width<int>& w)
{ return print_signed_number(n, w); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(int16_t n
    , const atd::Width<int>& w)
{ return print_signed_number(n, w); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(int32_t n
    , const atd::Width<int>& w)
{ return print_signed_number(n, w); }


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print(int64_t n
    , const atd::Width<int>& w)
{ return print_signed_number(n, w); }

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
template <typename Font>
inline uint8_t LCD_screen<num_cols, num_rows, LCD>::print_number(uint8_t n, 
		const atd::Width<int>& w)
{ return Font::print_number(*this, n, w);}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::cursor_pos(uint8_t col, uint8_t row)
{
    lcd_.cursor_pos(col, row);

    x_ = col;
    y_ = row;
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::new_extended_char(uint8_t c,
                                                            const uint8_t glyph[8])
{
    lcd_.new_extended_char(c, glyph);
    cursor_pos(x_, y_);
}



template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::new_extended_char(uint8_t c
				, const avr::Progmem_array<uint8_t, 8>& glyph)
{
    lcd_.new_extended_char(c, glyph);
    cursor_pos(x_, y_);
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::new_extended_char(uint8_t c
				, const avr::Progmem_array_view<uint8_t, 8>& glyph)
{
    lcd_.new_extended_char(c, glyph);
    cursor_pos(x_, y_);
}

}// namespace


#endif

