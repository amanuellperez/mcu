// Copyright (C) 2019-2022 Manuel Perez 
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

#ifndef __DEV_LCD_OSTREAM_H__
#define __DEV_LCD_OSTREAM_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz al display HD44780
 *  
 *   - HISTORIA:
 *      Manuel Perez: 
 *      11/07/2017 v0.0
 *	29/07/2019 v0.1: Creo traductor.
 *	26/09/2019 v0.2: LCD_ostream y cambios menores.
 *	06/01/2020 v0.3: Elimino DPin a favor de Pin.
 *	28/02/2022	 Desdoblo Terminal en Screen y Terminal.
 *	                 De momento, Terminal es copia de lo que era Terminal.
 *	                 En el futuro se irán alejando. De hecho, Terminal es
 *	                 la implementación de este LCD ostream!!!
 *
 ****************************************************************************/
#include <ostream>
#include "dev_LCD_terminal.h"

namespace dev{

template <uint8_t cols, uint8_t rows, typename  LCD>
class LCD_streambuf : public std::streambuf{
public:
    using Terminal = LCD_terminal<cols, rows, LCD>;

    Terminal& terminal() {return lcd_;}
    const Terminal& terminal() const {return lcd_;}


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
    Terminal lcd_;   
};



template <uint8_t cols0, uint8_t rows0, typename LCD>
class LCD_ostream: public std::ostream{
public:
    using Terminal = LCD_terminal<cols0, rows0, LCD>;

    explicit LCD_ostream() : std::ostream{&sb_} {}

    /// Borra la pantalla.
    void clear() {sb_.terminal().clear();}


    /// Define la posición del cursor.
    void cursor_pos(uint8_t col, uint8_t row)
    {sb_.terminal().cursor_pos(col, row);}

    /// Enciende el LCD.
    void display_on(){sb_.terminal().display_on();}

    /// Apaga el LCD.
    void display_off(){sb_.terminal().display_off();}

    /// Muestra el cursor.
    void cursor_on()	{sb_.terminal().cursor_on();}

    /// No muestra el cursor.
    void cursor_off()	{sb_.terminal().cursor_off();}

    /// Hace que el cursor parpadee.
    void cursor_blink()	    {sb_.terminal().cursor_blink();}

    /// Hace que el cursor no parpadee.
    void cursor_no_blink()  {sb_.terminal().cursor_no_blink();}

    /// Acceso al terminal
    Terminal& terminal() {return sb_.terminal();}
    const Terminal& terminal() const {return sb_.terminal();}

    /// Número de filas del LCD
    static constexpr uint8_t rows() {return rows0;}

    /// Número de columnas del LCD
    static constexpr uint8_t cols() {return cols0;}

private:
    LCD_streambuf<cols0, rows0, LCD> sb_;
    
};

template <typename LCD>
using LCD_ostream_1602 = LCD_ostream<16, 2, LCD>;

template <typename LCD>
using LCD_ostream_2004 = LCD_ostream<20, 4, LCD>;

template <typename LCD>
using LCD_ostream_4004 = LCD_ostream<40, 4, LCD>;



}// namespace

#endif
