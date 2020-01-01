// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_LCD_HD4780_OSTREAM_H__
#define __DEV_LCD_HD4780_OSTREAM_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz al display HD44780
 *  
 *   - HISTORIA:
 *      A. Manuel López	- 11/07/2017 v0.0
 *			  29/07/2019 v0.1: Creo traductor.
 *			  26/09/2019 v0.2: LCD_ostream y cambios menores.
 *
 *
 ****************************************************************************/
#include <ostream>
#include "dev_LCD_HD44780_screen.h"

namespace dev{

template <uint8_t cols, uint8_t rows>
class LCD_HD44780_streambuf : public std::streambuf{
public:
    using LCD_screen = LCD_HD44780_screen<cols, rows>;

    /// Conectamos el HD44780 con el interface 4 bits.
    LCD_HD44780_streambuf(  LCD_HD44780::DPin_RS rs
			, LCD_HD44780::DPin_RW rw
			, LCD_HD44780::DPin_E e
			, const LCD_HD44780::DPin_D4& d)
	:lcd_{rs, rw, e, d}{}

    LCD_screen& screen() {return lcd_;}
    const LCD_screen& screen() const {return lcd_;}


protected:
    virtual int_type overflow(int_type c = traits_type::eof()) override
    { 
	lcd_.print(traits_type::to_char_type(c));
	return 1;
    }

    // Esta no es necesaria definirla. Si la defino es así es un pelín más
    // eficiente, ya que no tiene que mirar a gestionar la get area.
    virtual std::streamsize xsputn(const char_type* s, std::streamsize n) override
    { 
	for (int i = 0; i < n; ++i)
	    lcd_.print(s[i]);

	return n;
    }


private:
    LCD_screen lcd_;   
};


template <uint8_t cols, uint8_t rows>
class LCD_HD44780_ostream: public std::ostream{
public:
    using LCD_screen = LCD_HD44780_screen<cols, rows>;

    explicit LCD_HD44780_ostream(LCD_HD44780::DPin_RS rs
			, LCD_HD44780::DPin_RW rw
			, LCD_HD44780::DPin_E e
			, const LCD_HD44780::DPin_D4& d):
	std::ostream{&sb_}, sb_{rs, rw, e, d} {}

    /// Borra la pantalla.
    void clear() {sb_.screen().clear();}


    /// Define la posición del cursor.
    void cursor_pos(uint8_t col, uint8_t row)
    {sb_.screen().cursor_pos(col, row);}

    /// Enciende el LCD.
    void display_on(){sb_.screen().display_on();}

    /// Apaga el LCD.
    void display_off(){sb_.screen().display_off();}

    /// Muestra el cursor.
    void cursor_on()	{sb_.screen().cursor_on();}

    /// No muestra el cursor.
    void cursor_off()	{sb_.screen().cursor_off();}

    /// Hace que el cursor parpadee.
    void cursor_blink()	    {sb_.screen().cursor_blink();}

    /// Hace que el cursor no parpadee.
    void cursor_no_blink()  {sb_.screen().cursor_no_blink();}

    /// Acceso a la screen.
    LCD_screen& screen() {return sb_.screen();}
    const LCD_screen& screen() const {return sb_.screen();}

private:
    LCD_HD44780_streambuf<cols, rows> sb_;
    
};

using LCD_HD44780_1602_ostream = LCD_HD44780_ostream<16, 2>;
using LCD_HD44780_2004_ostream = LCD_HD44780_ostream<20, 4>;




}// namespace

#endif
