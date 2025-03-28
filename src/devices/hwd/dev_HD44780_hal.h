// Copyright (C) 2021 Manuel Perez 
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

#ifndef __DEV_HD44780_HAL_H__
#define __DEV_HD44780_HAL_H__
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
 *  TODO: no usar esta clase. Repensarlo: realmente sería un Text_display que
 *  es más genérico que esto.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    09/01/2021 v0.0
 *    06/02/2022 v0.1: Generalizo creando el concept de Generic_LCD. La 
 *                     idea es no tener que volver a usar un LCD concreto a
 *                     partir de ahora y poder usar siempre Generic_LCD
 *                     (o mejor LCD_screen/ostream).
 *    21/10/2024 Elimino template (se lee bien, pero da la impresión de
 *               complicar más el código). Paso de Generic_LCD<LCD> a clases
 *               particulares: HD44780 y HD44780_4004.
 *
 *               TODO: Necesito un criterio para nombrar las clases.
 *               Al final el traductor en lugar de llamarlo HD44780,
 *               prefiero llamarlo igual que el device: HD44780. Pero ¿cómo
 *               llamar el dispositivo genérico? ¿HD44780?
 *               ¿generic::HD44780? ¿HD44780_g? ¿HD44780_generic? @_@
 *               El primero no sirve para otros tipos de devices, mientras que
 *               los otros 3 sí.
 *
 *
 ****************************************************************************/
#include "not_generic.h"
#include <atd_memory.h>

#include "dev_HD44780_hwd.h" 

namespace dev{

// flags
// -----
enum class _HD44780_generic_flags {
    display_on_bit       = 1L << 0,
    increment_cursor_bit = 1L << 1,
    cursor_on_bit        = 1L << 2,
    cursor_blink_bit     = 1L << 3,
    E_pin_bit		 = 1L << 4  // para el LCD 40 x 04
};

inline constexpr _HD44780_generic_flags
operator&(_HD44780_generic_flags a, _HD44780_generic_flags b)
{
    return static_cast<_HD44780_generic_flags>(static_cast<int>(a) &
                                                   static_cast<int>(b));
}

inline constexpr _HD44780_generic_flags
operator|(_HD44780_generic_flags a, _HD44780_generic_flags b)
{
    return static_cast<_HD44780_generic_flags>(static_cast<int>(a) |
                                                   static_cast<int>(b));
}

inline constexpr _HD44780_generic_flags
operator^(_HD44780_generic_flags a, _HD44780_generic_flags b)
{
    return static_cast<_HD44780_generic_flags>(static_cast<int>(a) ^
                                                   static_cast<int>(b));
}

inline constexpr _HD44780_generic_flags
operator~(_HD44780_generic_flags a)
{
    return static_cast<_HD44780_generic_flags>(~static_cast<int>(a));
}

inline constexpr _HD44780_generic_flags&
operator&=(_HD44780_generic_flags& a, _HD44780_generic_flags b)
{
    return a = a & b;
}

inline constexpr _HD44780_generic_flags&
operator|=(_HD44780_generic_flags& a, _HD44780_generic_flags b)
{
    return a = a | b;
}

inline constexpr _HD44780_generic_flags&
operator^=(_HD44780_generic_flags& a, _HD44780_generic_flags b)
{
    return a = a ^ b;
}

inline constexpr bool operator==(_HD44780_generic_flags a, int b)
{
    return static_cast<int>(a) == b;
}

inline constexpr bool operator!=(_HD44780_generic_flags a, int b)
{
    return !(a == b);
}


/*!
 *  \brief  Capa genérica del HD44780.
 *
 *  Implementación mínima: añadir funcionalidad según se vaya necesitando.
 *  
 */
template <typename Micro, typename pin>
class HD44780 {
public:
    using LCD = hwd::HD44780<Micro, pin>;

    HD44780() {init();}

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

    /// Imprime el caracter c en el display.
    static void print(char c)
    {LCD::write_data_to_CG_or_DDRAM(c);}

    /// Imprime el caracter extendido c en el display.
    static void print_extended(char c) {print(c);}

    /// Lee el caracter mostrado en el display.
    // El modo normal de trabajo es leer/escribir en el display (la DDRAM).
    // Por defecto dejaremos la DDRAM seleccionada. Por eso, el usuario no
    // tiene que saber nada de esta selección. 
    static uint8_t read()
    {return LCD::read_data_from_CG_or_DDRAM();}

    /// Crea un nuevo caracter 'c' de 8 filas en la página de memoria extendida.
    /// Deja el cursor en la posición inicial.
    /// Precondition: 0 <= c < 7
    template <typename Array>
    static void new_extended_char(uint8_t c, const Array& glyph);


  private:
    using Flags = _HD44780_generic_flags;

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
template <typename M, typename pin>
void HD44780<M, pin>::init() 
{
    LCD::init();
    display_on();
}



template <typename M, typename pin>
void HD44780<M, pin>::display_on() 
{
    set_flag(display_on_bit);
    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));

}

template <typename M, typename pin>
void HD44780<M, pin>::display_off() 
{
    unset_flag(display_on_bit);
    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}


// x = col; y = row
template <typename M, typename pin>
void HD44780<M, pin>::cursor_pos(uint8_t x, uint8_t y)
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


template <typename M, typename pin>
bool HD44780<M, pin>::cursor(bool on)
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


template <typename M, typename pin>
void HD44780<M, pin>::cursor_blink(bool yes)
{
    if (yes)
	set_flag(cursor_blink_bit);
    else
	unset_flag(cursor_blink_bit);

    LCD::display_control(flag(display_on_bit),
			 flag(cursor_on_bit),
			 flag(cursor_blink_bit));
}

template <typename M, typename pin>
template <typename Array>
void HD44780<M, pin>::new_extended_char(uint8_t c, const Array& glyph)
{
    LCD::set_cgram_address(c*8);

    for (uint8_t j  = 0; j < 8; ++j)
	LCD::write_data_to_CG_or_DDRAM(glyph[j]);

    LCD::set_ddram_address(0x00); 
}


/*!
 *  \brief  Capa genérica del HD44780_4004
 *
 *  Concebimos el LCD como 4 filas x 40 columnas.
 *
 */
template <typename Micro, typename pin>
class HD44780_4004{
public:
    using LCD = hwd::HD44780_4004<Micro, pin>;

    HD44780_4004() {init();}

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

    /// Imprime el caracter c en el display.
    static void print(char c);

    /// Imprime el caracter extendido c en el display.
    static void print_extended(char c) {print(c);}

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    static uint8_t read();

    /// Crea un nuevo caracter 'c' de 8 filas en la página de memoria extendida.
    /// Deja el cursor en la posición inicial.
    /// Precondition: 0 <= c < 7
    template <typename Array>
    static void new_extended_char(uint8_t c, const Array& glyph);

private:

    using Flags = _HD44780_generic_flags;

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
template <typename M, typename pin>
void HD44780_4004<M, pin>::init() 
{
    LCD::init();
    display_on();
}



template <typename M, typename pin>
void HD44780_4004<M, pin>::display_on() 
{
    set_flag(display_on_bit);
    display_control_1_and_2();
}


template <typename M, typename pin>
void HD44780_4004<M, pin>::display_off() 
{
    unset_flag(display_on_bit);
    display_control_1_and_2();
}

template <typename M, typename pin>
void HD44780_4004<M, pin>::cursor_display_E1()
{
    if (flag(cursor_on_bit)){
	LCD::display_control1(flag(display_on_bit), 
			      true, flag(cursor_blink_bit));
	LCD::display_control2(flag(display_on_bit), 
			      false, flag(cursor_blink_bit));
    }
}



template <typename M, typename pin>
void HD44780_4004<M, pin>::cursor_display_E2()
{
    if (flag(cursor_on_bit)){
	LCD::display_control1(flag(display_on_bit), 
			      false, flag(cursor_blink_bit));
	LCD::display_control2(flag(display_on_bit), 
			      true, flag(cursor_blink_bit));
    }
}

// x = col; y = row
template <typename M, typename pin>
void HD44780_4004<M, pin>::cursor_pos(uint8_t x, uint8_t y)
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


template <typename M, typename pin>
bool HD44780_4004<M, pin>::cursor_on()
{
    bool res = flag(cursor_on_bit);

    set_flag(cursor_on_bit);
    display_control_1_or_2();

    return res;
}


template <typename M, typename pin>
bool HD44780_4004<M, pin>::cursor_off()
{
    bool res = flag(cursor_on_bit);

    unset_flag(cursor_on_bit);
    display_control_1_and_2();

    return res;
}


template <typename M, typename pin>
void HD44780_4004<M, pin>::cursor_blink()
{
    set_flag(cursor_blink_bit);
    display_control_1_or_2();
}


template <typename M, typename pin>
void HD44780_4004<M, pin>::cursor_no_blink()
{
    unset_flag(cursor_blink_bit);

    display_control_1_and_2();
}


template <typename M, typename pin>
void HD44780_4004<M, pin>::print(char data)
{
    if (E1_pin())
	LCD::write_data_to_CG_or_DDRAM1(data);
    else
	LCD::write_data_to_CG_or_DDRAM2(data);
}


template <typename M, typename pin>
uint8_t HD44780_4004<M, pin>::read()
{
    if (E1_pin())
	return LCD::read_data_from_CG_or_DDRAM1();
    else
	return LCD::read_data_from_CG_or_DDRAM2();

}

template <typename M, typename pin>
void HD44780_4004<M, pin>::display_control_1_and_2()
{
    LCD::display_control1(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
    LCD::display_control2(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
}


template <typename M, typename pin>
void HD44780_4004<M, pin>::display_control_1_or_2()
{
    if (E1_pin())
	LCD::display_control1(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
    else
	LCD::display_control2(flag(display_on_bit), 
			  flag(cursor_on_bit), flag(cursor_blink_bit));
}

template <typename M, typename pin>
template <typename Array>
void HD44780_4004<M, pin>::new_extended_char(uint8_t c, const Array& glyph)
{
    LCD::set_cgram_address(c*8);

    for (uint8_t j  = 0; j < 8; ++j)
	LCD::write_data_to_CG_or_DDRAM(glyph[j]);

    LCD::set_ddram_address(0x00); 
}



// Alias
// -----
template <typename Micro, typename pin>
using HD44780_1602 = HD44780<Micro, pin>;

template <typename Micro, typename pin>
using HD44780_2004 = HD44780<Micro, pin>;

}// namespace



#endif


