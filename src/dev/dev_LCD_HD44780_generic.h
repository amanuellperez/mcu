// Copyright (C) 2021 A.Manuel L.Perez 
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

#ifndef __DEV_LCD_HD44780_GENERIC_H__
#define __DEV_LCD_HD44780_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: "LCD virtual" de HD44780.
 *
 *  - COMENTARIOS: 
 *   (RRR) El diseño del interfaz lo ha marcado la función cursor_pos
 *         del LCD 40 x 04. Si se quiere mover el cursor en este display hay
 *         que saber el estado del cursor (on/off, si parpadeaba o no), si el
 *         display estaba encendido o no y en qué posición estaba el display.
 *         Todo esto lo marcamos en los flags. 
 *         Esto supone tener que desacoplar la función display_control del LCD
 *         separándola en display_on/off, cursor_on/off y
 *         cursor_blink/no_blink, lo cual parece buena idea ya que en otros
 *         LCD no tiene por qué estar implementado de esa forma.
 *
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    09/01/2021 v0.0
 *
 ****************************************************************************/
#include "dev_LCD_generic.h"
#include "dev_LCD_HD44780_basic.h" 

namespace dev{

// flags
// -----
enum class _LCD_HD44780_generic_flags {
    display_on_bit       = 1L << 0,
    increment_cursor_bit = 1L << 1,
    cursor_on_bit        = 1L << 2,
    cursor_blink_bit     = 1L << 3,
    E_pin_bit		 = 1L << 4  // para el LCD 40 x 04
};

inline constexpr _LCD_HD44780_generic_flags
operator&(_LCD_HD44780_generic_flags a, _LCD_HD44780_generic_flags b)
{
    return static_cast<_LCD_HD44780_generic_flags>(static_cast<int>(a) &
                                                   static_cast<int>(b));
}

inline constexpr _LCD_HD44780_generic_flags
operator|(_LCD_HD44780_generic_flags a, _LCD_HD44780_generic_flags b)
{
    return static_cast<_LCD_HD44780_generic_flags>(static_cast<int>(a) |
                                                   static_cast<int>(b));
}

inline constexpr _LCD_HD44780_generic_flags
operator^(_LCD_HD44780_generic_flags a, _LCD_HD44780_generic_flags b)
{
    return static_cast<_LCD_HD44780_generic_flags>(static_cast<int>(a) ^
                                                   static_cast<int>(b));
}

inline constexpr _LCD_HD44780_generic_flags
operator~(_LCD_HD44780_generic_flags a)
{
    return static_cast<_LCD_HD44780_generic_flags>(~static_cast<int>(a));
}

inline constexpr _LCD_HD44780_generic_flags&
operator&=(_LCD_HD44780_generic_flags& a, _LCD_HD44780_generic_flags b)
{
    return a = a & b;
}

inline constexpr _LCD_HD44780_generic_flags&
operator|=(_LCD_HD44780_generic_flags& a, _LCD_HD44780_generic_flags b)
{
    return a = a | b;
}

inline constexpr _LCD_HD44780_generic_flags&
operator^=(_LCD_HD44780_generic_flags& a, _LCD_HD44780_generic_flags b)
{
    return a = a ^ b;
}

inline constexpr bool operator==(_LCD_HD44780_generic_flags a, int b)
{
    return static_cast<int>(a) == b;
}

inline constexpr bool operator!=(_LCD_HD44780_generic_flags a, int b)
{
    return !(a == b);
}


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
    void init();

    /// Clears entire display and set cursor in (0,0)
    static void clear_display() {LCD::clear_display();}

    /// Enciende el LCD.
    void display_on();

    /// Apaga el LCD.
    void display_off();

    /// Muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_on()	{return cursor(true);}

    /// No muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_off()	{return cursor(false);}

    /// Hace que el cursor parpadee.
    void cursor_blink()	    {cursor_blink(true);}

    /// Hace que el cursor no parpadee.
    void cursor_no_blink()  {cursor_blink(false);}

    /// Coloca el cursor en la posición (x,y), con  x = col; y = row.
    static void cursor_pos(uint8_t x, uint8_t y);


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

private:
    using Flags = _LCD_HD44780_generic_flags;

    // Flags que necesito para desacoplar las responsabilidades
    // de las instrucciones entry_mode y display_control
    static constexpr Flags display_on_bit       = Flags::display_on_bit;
    static constexpr Flags increment_cursor_bit = Flags::increment_cursor_bit;
    static constexpr Flags cursor_on_bit        = Flags::cursor_on_bit;
    static constexpr Flags cursor_blink_bit     = Flags::cursor_blink_bit;

    Flags flags_ = Flags::display_on_bit | 
		   Flags::increment_cursor_bit;

    // Añade los flags de flg a Flags_
    void set_flag(Flags flg) { flags_|= flg; }

    // Borra mask de Flags_
    void unset_flag(Flags mask) { flags_ &= ~mask;}

    // El bit del flag flg ¿is set?
    bool flag(Flags flg) const { return (flags_ & flg) != 0; }

    bool cursor(bool on);
    void cursor_blink(bool yes);
};


/***************************************************************************
 *			    IMPLEMENTACION (.tcc)
 ***************************************************************************/
template <typename pin>
void Generic_LCD<LCD_HD44780<pin>>::init() 
{
    LCD::init();
    display_on();
}



template <typename pin>
void Generic_LCD<LCD_HD44780<pin>>::display_on() 
{
    set_flag(display_on_bit);
    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));

}

template <typename pin>
void Generic_LCD<LCD_HD44780<pin>>::display_off() 
{
    unset_flag(display_on_bit);
    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}


// x = col; y = row
template <typename pin>
void Generic_LCD<LCD_HD44780<pin>>::cursor_pos(uint8_t x, uint8_t y)
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


template <typename pin>
bool Generic_LCD<LCD_HD44780<pin>>::cursor(bool on)
{
    bool res = flag(cursor_on_bit);

    if (on)
	set_flag(cursor_on_bit);
    else
	unset_flag(cursor_on_bit);

    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));

    return res;
}


template <typename pin>
void Generic_LCD<LCD_HD44780<pin>>::cursor_blink(bool yes)
{
    if (yes)
	set_flag(cursor_blink_bit);
    else
	unset_flag(cursor_blink_bit);

    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}



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
    void init();

    /// Clears entire display and set cursor in (0,0)
    void clear_display() 
    {
	LCD::clear_display();
	set_E1_pin();
    }

    /// Enciende el LCD.
    void display_on();

    /// Apaga el LCD.
    void display_off();

    /// Muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_on();

    /// No muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_off();

    /// Hace que el cursor parpadee.
    void cursor_blink();

    /// Hace que el cursor no parpadee.
    void cursor_no_blink();

    /// Coloca el cursor en la posición (x,y), con  x = col; y = row.
    void cursor_pos(uint8_t x, uint8_t y);

    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    // TODO: mejor llamarlo write(data)???? y otra para write_CGRAM(data)???
    void write_data_to_CG_or_DDRAM(char data);


    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    uint8_t read_data_from_CG_or_DDRAM();

private:

    using Flags = _LCD_HD44780_generic_flags;

    // Flags que necesito para desacoplar las responsabilidades
    // de las instrucciones entry_mode y display_control
    static constexpr Flags display_on_bit       = Flags::display_on_bit;
    static constexpr Flags increment_cursor_bit = Flags::increment_cursor_bit;
    static constexpr Flags cursor_on_bit        = Flags::cursor_on_bit;
    static constexpr Flags cursor_blink_bit     = Flags::cursor_blink_bit;
    static constexpr Flags E_pin_bit		= Flags::E_pin_bit;

// Data
    Flags flags_ = Flags::display_on_bit | 
		   Flags::increment_cursor_bit;

    // Añade los flags de flg a Flags_
    void set_flag(Flags flg) { flags_|= flg; }

    // Borra mask de Flags_
    void unset_flag(Flags mask) { flags_ &= ~mask;}

    // El bit del flag flg ¿is set?
    bool flag(Flags flg) const { return (flags_ & flg) != 0; }

    // E_pin == false ==> seleccionadas filas 0,1 (pin E1)
    //       == true  ==>                     2,3 (pin E2)
    bool E1_pin() const {return !flag(E_pin_bit);}
    void set_E1_pin() {unset_flag(E_pin_bit);}
    void set_E2_pin() {set_flag(E_pin_bit);}


    void display_control_1_and_2();
    void display_control_1_or_2();

    // si el cursor está en on lo mueve al display E1
    void cursor_display_E1();

    // si el cursor está en on lo mueve al display E2
    void cursor_display_E2();
};


/***************************************************************************
 *			    IMPLEMENTACION (.tcc)
 ***************************************************************************/
template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::init() 
{
    LCD::init();
    display_on();
}



template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::display_on() 
{
    set_flag(display_on_bit);
    display_control_1_and_2();
}


template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::display_off() 
{
    unset_flag(display_on_bit);
    display_control_1_and_2();
}

template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::cursor_display_E1()
{
    if (flag(cursor_on_bit)){
	LCD::display_control1(flag(display_on_bit), 
			      true, flag(cursor_blink_bit));
	LCD::display_control2(flag(display_on_bit), 
			      false, flag(cursor_blink_bit));
    }
}



template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::cursor_display_E2()
{
    if (flag(cursor_on_bit)){
	LCD::display_control1(flag(display_on_bit), 
			      false, flag(cursor_blink_bit));
	LCD::display_control2(flag(display_on_bit), 
			      true, flag(cursor_blink_bit));
    }
}

// x = col; y = row
template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::cursor_pos(uint8_t x, uint8_t y)
{
    switch (y){
        case 0:
            LCD::set_ddram_address1(0x00 + x);
	    if (!E1_pin()){
		cursor_display_E1();
		set_E1_pin();
	    }
            break;

        case 1:
            LCD::set_ddram_address1(0x40 + x);
	    if (!E1_pin()){
		cursor_display_E1();
		set_E1_pin();
	    }
            break;
        case 2:
            LCD::set_ddram_address2(0x00 + x);
	    if (E1_pin()){
		cursor_display_E2();
		set_E2_pin();
	    }
            break;
        case 3:
            LCD::set_ddram_address2(0x40 + x);
	    if (E1_pin()){
		cursor_display_E2();
		set_E2_pin();
	    }
            break;

        default:
	    LCD::set_ddram_address1(0x00); // Para detectar errores
	    break;
    }
}


template <typename pin>
bool Generic_LCD<LCD_HD44780_4004<pin>>::cursor_on()
{
    bool res = flag(cursor_on_bit);

    set_flag(cursor_on_bit);
    display_control_1_or_2();

    return res;
}


template <typename pin>
bool Generic_LCD<LCD_HD44780_4004<pin>>::cursor_off()
{
    bool res = flag(cursor_on_bit);

    unset_flag(cursor_on_bit);
    display_control_1_and_2();

    return res;
}


template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::cursor_blink()
{
    set_flag(cursor_blink_bit);
    display_control_1_or_2();
}


template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::cursor_no_blink()
{
    unset_flag(cursor_blink_bit);

    display_control_1_and_2();
}


template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::write_data_to_CG_or_DDRAM(char data)
{
    if (E1_pin())
	LCD::write_data_to_CG_or_DDRAM1(data);
    else
	LCD::write_data_to_CG_or_DDRAM2(data);
}


template <typename pin>
uint8_t Generic_LCD<LCD_HD44780_4004<pin>>::read_data_from_CG_or_DDRAM()
{
    if (E1_pin())
	return LCD::read_data_from_CG_or_DDRAM1();
    else
	return LCD::read_data_from_CG_or_DDRAM2();

}

template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::display_control_1_and_2()
{
    LCD::display_control1(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
    LCD::display_control2(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
}


template <typename pin>
void Generic_LCD<LCD_HD44780_4004<pin>>::display_control_1_or_2()
{
    if (E1_pin())
	LCD::display_control1(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
    else
	LCD::display_control2(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
}



}// namespace



#endif


