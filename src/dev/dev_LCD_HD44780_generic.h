// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_LCD_HD44780_GENERIC_H__
#define __DEV_LCD_HD44780_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: "LCD virtual" de HD44780.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    09/01/2020 v0.0
 *
 ****************************************************************************/
#include "dev_LCD_generic.h"
#include "dev_LCD_HD44780_basic.h" 

namespace dev{

/*!
 *  \brief  Capa genérica del LCD_HD44780.
 *
 *  Implementación mínima: añadir funcionalidad según se vaya necesitando.
 *  
 */
template <typename pin>
class Generic_LCD<LCD_HD44780<pin>>{
public:
    using LCD = LCD_HD44780<pin>;

    Generic_LCD() {init();}

    /// Init display
    static void init() {LCD::init();}

    /// Clears entire display and set cursor in (0,0)
    static void clear_display() {LCD::clear_display();}


    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    // display_on = false apaga el display. Esto lo único que hace es no
    // mostrar en pantalla el mensaje correspondiente. El LCD sigue
    // funcionando.
    static void display_control(bool display_on, 
				bool cursor_on, bool cursor_blink)
    {LCD::display_control(display_on, cursor_on, cursor_blink);}

    // x = col; y = row
    static void cursor_pos(uint8_t x, uint8_t y)
    {
	switch (y){
            case 0: LCD::set_ddram_address(0x00 + x); break;
            case 1: LCD::set_ddram_address(0x40 + x); break;
            case 2: LCD::set_ddram_address(0x14 + x); break;
            case 3: LCD::set_ddram_address(0x54 + x); break;

            default:
                LCD::set_ddram_address(0x00); // Para detectar errores
                break;
        }
    }


    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    // TODO: mejor llamarlo write(data)???? y otra para write_CGRAM(data)???
    static void write_data_to_CG_or_DDRAM(char data)
    {LCD::write_data_to_CG_or_DDRAM(data);}

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    static uint8_t read_data_from_CG_or_DDRAM()
    {return LCD::read_data_from_CG_or_DDRAM();}

};


/*!
 *  \brief  Capa genérica del LCD_HD44780_4004
 *
 *  Concebimos el LCD como 4 filas x 40 columnas.
 *
 */
template <typename pin>
class Generic_LCD<LCD_HD44780_4004<pin>>{
public:
    using LCD = LCD_HD44780_4004<pin>;

    Generic_LCD() {init();}

    /// Init display
    void init() 
    {
	LCD::init();
	E_pin_ = 1;
    }

    /// Clears entire display and set cursor in (0,0)
    void clear_display() 
    {
	LCD::clear_display();
	E_pin_ = 1;
    }


    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    // display_on = false apaga el display. Esto lo único que hace es no
    // mostrar en pantalla el mensaje correspondiente. El LCD sigue
    // funcionando.
    void display_control(bool display_on, 
				bool cursor_on, bool cursor_blink)
    {
	if (E_pin_ == 1)
	    LCD::display_control1(display_on, cursor_on, cursor_blink);
	else
	    LCD::display_control1(display_on, cursor_on, cursor_blink);
    }

    // x = col; y = row
    void cursor_pos(uint8_t x, uint8_t y)
    {
	switch (y){
            case 0: LCD::set_ddram_address1(0x00 + x); E_pin_ = 1; break;
            case 1: LCD::set_ddram_address1(0x40 + x); E_pin_ = 1; break;
            case 2: LCD::set_ddram_address2(0x00 + x); E_pin_ = 2; break;
            case 3: LCD::set_ddram_address2(0x40 + x); E_pin_ = 2; break;

            default:
                LCD::set_ddram_address1(0x00); // Para detectar errores
                break;
        }
    }


    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    // TODO: mejor llamarlo write(data)???? y otra para write_CGRAM(data)???
    void write_data_to_CG_or_DDRAM(char data)
    {
	if (E_pin_ == 1)
	    LCD::write_data_to_CG_or_DDRAM1(data);
	else
	    LCD::write_data_to_CG_or_DDRAM2(data);
    }


    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    uint8_t read_data_from_CG_or_DDRAM()
    {
	if (E_pin_ == 1)
	    return LCD::read_data_from_CG_or_DDRAM1();
	else
	    return LCD::read_data_from_CG_or_DDRAM2();

    }

private:
    uint8_t E_pin_; // 1 or 2?
};

}// namespace



#endif


