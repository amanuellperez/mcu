// Copyright (C) 2019-2021 A.Manuel L.Perez
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

#include <atd_bit.h>
#include <atd_algorithm.h>
#include <std_algorithm.h>  // min

#include <avr_time.h>

namespace dev{

/***************************************************************************
 *			    LCD_screen
 ***************************************************************************/
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
uint8_t LCD_screen<num_cols, num_rows, LCD>::read_byte(uint8_t x, uint8_t y)
{
    cursor_pos(x, y);
    uint8_t c = lcd_.read_data_from_CG_or_DDRAM();
    cursor_pos(x_, y_);
    return c;
}

// OJO: no mantiene la posición del cursor
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::copia_esta_fila_en_la_fila_anterior(uint8_t i)
{
    cursor_pos(0, i);
    
    uint8_t buf[cols()];
    for (uint8_t j = 0; j < cols(); ++j)
	buf[j] = lcd_.read_data_from_CG_or_DDRAM();

    cursor_pos(0, i-1);
    for (uint8_t j = 0; j < cols(); ++j)
	lcd_.write_data_to_CG_or_DDRAM(buf[j]);

}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::clear_row(uint8_t i)
{
    cursor_pos(0, i);
    for (uint8_t j = 0; j < cols(); ++j)
	lcd_.write_data_to_CG_or_DDRAM(' ');
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::scroll_text_up()
{
    uint8_t x0 = x_, y0 = y_;

    for (uint8_t i = 1; i < rows(); ++i)
	copia_esta_fila_en_la_fila_anterior(i);

    clear_row(rows() - 1);

    x_ = x0;
    y_ = y0;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::cursor_move()
{
    ++x_;

    if (x_ == cols()){
	if (y_ != (rows() - 1) or !flag(stop_brcorner_bit)) // leer la negación, se entiende mejor
	    print_return();
    }
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_imprimible_char(char c)
{
    lcd_.write_data_to_CG_or_DDRAM(c);
    cursor_move();
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_return()
{
    ++y_;   // este es '\n'
    if (y_ == rows()){
	scroll_text_up();
	--y_;
    }
    cursor_pos(0, y_);
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
bool LCD_screen<num_cols, num_rows, LCD>::print(char c) 
{
    static_assert(sizeof(char) == sizeof(uint8_t)
		 , "sizeof(char) != 1 byte!!!");

    if (c == '\n')
	print_return();
    else
	print_imprimible_char(c);

    return true;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print(const char* c)
{
    if (flag(wrap_bit))
	return print_wrap(c);

    else 
	return print_nowrap(c);
}

template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_wrap(const char* c)
{
//    uint8_t n = 0;
    while(*c){
	if (!print(*c++))
	    return;// n;

//	++n;
    }

//   return n;
}


// precondition: y_ < rows() and *p != '\0'
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::print_nowrap(const char* p)
{
    p = print_line_nowrap(p);

    while (*p != '\0' and y_ != rows() - 1){
	x_ = 0;
	++y_;
	cursor_pos(x_, y_);

	p = print_line_nowrap(p);
    }
}

// Devuelve un puntero a la primera letra de la siguiente linea, en caso de
// que lo haya, o a '\0'.
// OJO: el cursor queda colocado fuera del LCD  (x_ == cols()!!!) ¿Algún
// problema?
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
const char*
LCD_screen<num_cols, num_rows, LCD>::print_line_nowrap(const char* p,
                                                          uint8_t num_max_char)
{
    uint8_t x_end = x_ + std::min<uint8_t>(num_max_char, cols() - x_);

    for(;*p != '\0' and *p != '\n' and x_ < x_end; ++p){
	lcd_.write_data_to_CG_or_DDRAM(*p);
	++x_;
//	cursor_pos(x_, y_); // write_data_to_CG_or_DDRAM actualiza el cursor.
    }

    if (x_ == x_end)
	p = atd::find_c(p, '\n');

    fill_line(x_end - x_, ' ');

    if (*p == '\n')
	return ++p;

    return p;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::fill_line(uint8_t n, char c)
{
    n = x_ + std::min<uint8_t>(n, cols() - x_);

    for(;x_ < n; ++x_)
	lcd_.write_data_to_CG_or_DDRAM(c);
}




// Voy a concebir el LCD como con 4 filas
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::cursor_pos(uint8_t col, uint8_t row)
{
    lcd_.cursor_pos(col, row);

    x_ = col;
    y_ = row;
}


// Enciende el LCD.
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::display_on() 
{
    set_flag(display_on_bit);
    lcd_.display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));

    cursor_pos(x_, y_);
}

// Apaga el LCD.
template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::display_off() 
{
    unset_flag(display_on_bit);
    lcd_.display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}



template <uint8_t num_cols, uint8_t num_rows, typename LCD>
bool LCD_screen<num_cols, num_rows, LCD>::cursor(bool on)
{
    bool res = flag(cursor_on_bit);

    if (on)
	set_flag(cursor_on_bit);
    else
	unset_flag(cursor_on_bit);

    lcd_.display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));

    return res;
}


template <uint8_t num_cols, uint8_t num_rows, typename LCD>
void LCD_screen<num_cols, num_rows, LCD>::cursor_blink(bool yes)
{
    if (yes)
	set_flag(cursor_blink_bit);
    else
	unset_flag(cursor_blink_bit);

    lcd_.display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}



}// namespace


#endif

