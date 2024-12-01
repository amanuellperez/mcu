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

#pragma once

#ifndef __LCD_H__
#define __LCD_H__

#include <dev_HD44780.h>
#include <dev_HD44780_charset.h>
#include <dev_LCD_ostream.h>

#include "instcodes.h"

using Code = Instructions_code;

template <typename Generic_LCD_t>
class LCD_calculator : public dev::LCD_ostream_1602<Generic_LCD_t>
{
public:
    void init();
    void print(char c);

private:
//    void new_glyph(const char glyph[8]);
//    void save_new_glyphs();

//    void print_lcd_new_symbol(char c);
    using symbol = dev::HD44780_charset_A00;
    void print_lcd_symbol(char c);

    using Parent = dev::LCD_ostream_1602<Generic_LCD_t>;
    //using LCD = dev::HD44780<LCD_pins>;
};


template <typename P>
void LCD_calculator<P>::init()
{
    Parent::terminal().stop_brcorner(true);// I'm not going to use it as a terminal
    Parent::terminal().nowrap(); 
//    save_new_glyphs();
}



//template <typename P>
//void LCD_calculator<P>::new_glyph(const char glyph[8])
//{
//    for (uint8_t i  = 0; i < 8; ++i)
//	LCD::write_data_to_CG_or_DDRAM(glyph[i]);
//}


//template <typename P>
//void LCD_calculator<P>::save_new_glyphs()
//{
//    using namespace dev::character_glyphs_basic;
//
//    LCD::set_cgram_address(0x00);
//    new_glyph(exponent2); // mismo orden que en keycodes.h!!!
//			    // TODO: cómo imponer esto por código?
////    new_glyph(lcd, g2);
////    new_glyph(lcd, g3);
////    new_glyph(lcd, g4);
////    new_glyph(lcd, g5);
////    new_glyph(lcd, g6);
////    new_glyph(lcd, g7);
////    new_glyph(lcd, g8);
//
//
//    LCD::set_ddram_address(0x00);
//
//}

template <typename P>
void LCD_calculator<P>::print(char c)
{
    if (Code::first_symbol <= c and c <= Code::last_symbol)
	print_lcd_symbol(c);

//    else if (Code::first_new_symbol <= c and c <= Code::last_new_symbol)
//	print_lcd_new_symbol(c);

    else
	*this << c;

}


//template <typename P>
//inline void LCD_calculator<P>::print_lcd_new_symbol(char c) 
//{
//    LCD::write_data_to_CG_or_DDRAM(c - Code::first_new_symbol);
//}
//
template <typename P>
void LCD_calculator<P>::print_lcd_symbol(char c)
{
    switch(c){
	break; case Code::sqrt: Parent::terminal().print(symbol::of("√"));
//	break; case Code::frac :  Parent::terminal().print(symbol::of("｣"));
//	break; case Code::inverse: Parent::terminal().print(symbol::of("-1"));
	break; default: *this << '?';
    }
}


#endif


