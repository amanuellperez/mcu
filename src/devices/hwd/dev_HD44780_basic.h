// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __DEV_LCD_HD4780_BASIC_H__
#define __DEV_LCD_HD4780_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del display HD44780
 *  
 *  - COMENTARIOS: Aunque al principio implementé el acceso al LCD usando los
 *    8 bits en la práctica nunca lo he usado. Al estar todas las funciones de
 *    8 bits sin probar optó por comentarlas. Si se quieren usar
 *    descomentarlas y probarlas. NO BORRARLAS, dejarlas comentadas por si en
 *    el futuro se quiere usar el interfaz de 8 bits.
 *
 *  - HISTORIA:
 *     A. Manuel L.Perez
 *	    11/07/2017 v0.0
 *	    29/07/2019 v0.1: Creo traductor.
 *	    26/09/2019 v0.2: LCD_ostream y cambios menores.
 *	    14/10/2019 v0.3: Reestructuración de ficheros.
 *	    06/01/2020 v0.4: Elimino DPin a favor de Pin.
 *	    07/01/2021       set_cgram_address
 *	    08/01/2021 v0.5: Reestructuro para poder usar LCD 40 x 04.
 *	    06/02/2022       alias LCD_HD44780_1602 y LCD_HD44780_2004
 *
 ****************************************************************************/
#include <avr_pin.h>
#include "not_generic.h"

namespace dev{

/*!
 *  \brief  Traductor del LCD HD44780.
 *  
 *  Como es un traductor se limita a suministar las funciones que suministra
 *  el hardware añadiendo lo menos posible.
 *
 *  Dos formas de conectarlo:
 *	1.- Conectarlo a los pines D0-D7. Necesitamos 8 pines del MCU.
 *	2.- Conectarlo a los pines D4-D7. Necesitamos 4 pines del MCU.
 *
 *  El primer caso será más rápido que el segundo, aunque la diferencia
 *  en tiempo en la mayoría de las aplicaciones será irrelevante (???).
 *
 *  El MCU también hay que conectarlo a los pines RS, RW y E.
 *
 *  El resto de los pines es para alimentarlo, por ejemplo:
 *	    VSS = 5V
 *	    VDD = 0V
 *	    V0  = un potencial entre VDD y VSS. Usamos un potenciómetro de 10K
 *	    A   = conectado a 5V a través de una resistencia de 330 ohms.
 *	    K   = 0V
 */
template <uint8_t pin_RS>
struct LCD_HD44780_RS{
    static constexpr uint8_t RS = pin_RS;
};

template <uint8_t pin_RW>
struct LCD_HD44780_RW{
    static constexpr uint8_t RW = pin_RW;
};



template <uint8_t pin_D4,
	  uint8_t pin_D5,
	  uint8_t pin_D6,
	  uint8_t pin_D7>
struct LCD_HD44780_D4{
    static constexpr uint8_t D4 = pin_D4;
    static constexpr uint8_t D5 = pin_D5;
    static constexpr uint8_t D6 = pin_D6;
    static constexpr uint8_t D7 = pin_D7;
};


template <typename pin_RS, 
	  typename pin_RW,
	  typename pins_D4>
struct LCD_HD44780_base_pins4{
    static constexpr uint8_t num_D_pins = 4;

    static constexpr uint8_t RS = pin_RS::RS;
    static constexpr uint8_t RW = pin_RW::RW;

    static constexpr uint8_t D0 = 0;
    static constexpr uint8_t D1 = 0;
    static constexpr uint8_t D2 = 0;
    static constexpr uint8_t D3 = 0;
    static constexpr uint8_t D4 = pins_D4::D4;
    static constexpr uint8_t D5 = pins_D4::D5;
    static constexpr uint8_t D6 = pins_D4::D6;
    static constexpr uint8_t D7 = pins_D4::D7;
};






/*!
 *  \brief  Traductor del HD44780
 *
 *  (RRR) Los LCDs de 16 x 02, 20 x 04 y 40 x 04 usan las mismas funciones.
 *        Esta clase contiene todas estas funciones. Cada LCD particular
 *        concretará los pines usados (ahora es decidir qué pin E se usa, ya
 *        que el LCD 40 x 04 tiene 2 pines E).
 */
// TODO: pasar el Micro como parámetro y sustituir not_generic::Pin por
// Micro::Pin.
template <typename pin>
// requires: std::is_same_v<pin, LCD_HD44780_pins>;
class LCD_HD44780_base{
public:

protected:
//    LCD_HD44780_base(); <-- llamar a setup_pins() para inicializarlo.
    static void setup_pins();

    /// Inicializamos el IC
    template <typename pin_E>
    static void init();	

// INSTRUCCIONES: TABLE 6, PAG. 24
    /// Clears entire display and sets DDRAM address 0 in address counter.
    template <typename pin_E>
    static void clear_display();

    /// Sets DDRAM address 0 in address counter. Also returns display from 
    /// being shifted to original position. DDRAM contents remain unchanged.
    template <typename pin_E>
    static void return_home();


    /// Set cursor move direction and specifies display shift.
    /// If incrementa_cursor = true increments the DDRAM address by 1 when 
    /// a character code is written into or read from DDRAM.
    /// If incrementa_cursor = false, decrements DDRAM address.
    ///
    /// If shift_display = true shifts the entire display to the right (if
    /// incrementa_cursor = true) or to the left (if incrementa_cursor = false).
    /// If shift_display = false the display does not shift.
    template <typename pin_E>
    static void entry_mode(bool incrementa_cursor, bool shift_display);

    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    // display_on = false apaga el display. Esto lo único que hace es no
    // mostrar en pantalla el mensaje correspondiente. El LCD sigue
    // funcionando.
    template <typename pin_E>
    static void display_control(bool display_on, bool cursor_on, bool cursor_blink);


    /// Moves cursor or shift display.
    /// If display_no_cursor = true shift display, else move cursor.
    /// If to_the_right = true move to the right, else to the left.
    template <typename pin_E>
    static void cursor_or_display_shift(bool display_no_cursor, bool to_the_right);
 

    /// Sets interface data lenght (4 ó 8 bits), number of display lines
    /// (1 ó 2), and character font (5 x 8 or 5 x 10)
    template <typename pin_E>
    static void function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		    , bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		    , bool character_font_5x8); // ¿char de 5x8 ó 5x10?


    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    template <typename pin_E>
    static void write_data_to_CG_or_DDRAM(uint8_t data);

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    template <typename pin_E>
    static uint8_t read_data_from_CG_or_DDRAM();


    /// Sets DDRAM address: la dirección donde vamos a escribir el siguiente
    /// caracter.
    /// En el display de 2 filas, las direcciones en hexadecimal van:
    /// Fila 1: 0x00 ... 0x27
    /// Fila 2: 0x40 ... 0x67
    /// ¡¡¡Observar que no van contiguas!!!
    template <typename pin_E>
    static void set_ddram_address(uint8_t addr);


    /// Sets CGRAM address.
    /// CGRAM data is sent and received after this setting.
    template <typename pin_E>
    static void set_cgram_address(uint8_t addr);

    // TODO: uint8_t read_busy_flag_and_address(); (es la función
    // is_busy, reescribirla cambiando básicamente el nombre)
private:
// PINS
    static constexpr uint8_t num_D_pins = pin::num_D_pins;

    using RS = not_generic::Pin<pin::RS>;
    using RW = not_generic::Pin<pin::RW>;
//    using E = not_generic::Pin<pin::E>;

    // Si solo usamos 4 pines, serán D<4>-D<7>
    using D0 = not_generic::Pin<pin::D0>;
    using D1 = not_generic::Pin<pin::D1>;
    using D2 = not_generic::Pin<pin::D2>;
    using D3 = not_generic::Pin<pin::D3>;
    using D4 = not_generic::Pin<pin::D4>;
    using D5 = not_generic::Pin<pin::D5>;
    using D6 = not_generic::Pin<pin::D6>;
    using D7 = not_generic::Pin<pin::D7>;



    // Este display tiene 2 formas de funcionamiento:
    //	interfaz de 8 bits e interfaz de 4 bits. Cada interfaz
    //	tiene su propia forma de inicializarse.
//    void init8();
    template <typename pin_E>
    static void init4();

    /// Indica si el display está ocupado haciendo una operación interna.
    /// Mientras esté ocupado no se puede pedirle nada más.
    template <typename pin_E>
    static bool is_busy();

    // Indica si el display está ocupado haciendo una operación interna.
    // Mientras esté ocupado no se puede pedirle nada más.
//    bool is_busy8();
    template <typename pin_E>
    static bool is_busy4();
    
    // Funciones genéricas para escribir en el IC
    // Escribe (rs, rw, d7, d6, d5, d4, d3, d2, d1, d0) en el IC
    // Esta es la función clave a la hora de escribir. Internamente se 
    // encarga de mirar si usamos 4 ó 8 bit interface.
    template <typename pin_E>
    static void write_d(uint8_t rs, uint8_t rw, uint8_t d);

    // Implementación de write_d. No llamar a estas funciones NUNCA
    // directamente, sino siempre a través de write_d!!!
//    void write_d8(uint8_t rs, uint8_t rw, uint8_t d);
    template <typename pin_E>
    static void write_d4(uint8_t rs, uint8_t rw, uint8_t d);

    // Escribe solo los bits d4-d7 de 'd' (los más significativos)
    // Escribe (rs, rw, d7, d6, d5, d4) en el IC
    template <typename pin_E>
    static void write_d4_d7(uint8_t rs, uint8_t rw, uint8_t d);

    // Lee un byte usando solo los bits d4-d7 (esta función solo
    // se usa en read_data_from_CG_or_DDRAM).
    template <typename pin_E>
    static uint8_t read_d4();

    // Para realizar una operación en el LCD necesitamos activar E
    template <typename pin_E>
    static void pulse_E();

    // Espera hasta que el LCD esta disponible para la siguiente instrucción
    template <typename pin_E>
    static void wait_to_be_available()
    { while(is_busy<pin_E>()) ; }

// D PIN ACCESS
    static void D_pins_as_output();
    static void D_pins_as_input_without_pullup();

    static void write_d4(uint8_t d);
//    void write_d8(uint8_t d);
};


/***************************************************************************
 *		  LCD_HD44780: display 16 x 2 or 20 x 4
 ***************************************************************************/
template <uint8_t pin_E>
struct LCD_HD44780_E{
    static constexpr uint8_t E = pin_E;
};


template <typename pin_RS, 
	  typename pin_RW,
	  typename pin_E,
	  typename pins_D4>
struct LCD_HD44780_pins4{
    using Base_pins = LCD_HD44780_base_pins4<pin_RS, pin_RW, pins_D4>;

    static constexpr uint8_t E  = pin_E::E;
};

/*!
 * La forma de definir un LCD es de la siguiente forma:
 *
 * // Si lo conectamos solo a 4 pins de datos
 * using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
 *					   dev::LCD_HD44780_RW<5>,
 *					   dev::LCD_HD44780_E<6>,
 *					   dev::LCD_HD44780_D4<11,12,13,14>
 *					   >;
 *					   
 * using LCD_HD44780_base = dev::LCD_HD44780_base<LCD_pins>;
 *
 * En caso de que se pasen en orden equivocado los pines (por ejemplo, 
 * se pase primero RW en vez de RS) dará un error (misterioso) de compilación
 * impidiendo que se definan mal los pines.
 *
 * DUDA: ¿merece la pena modificar LCD_HD44780_pins4 para que de un error
 * inteligible? De momento creo que no.
 */

template <typename pin>
// requires: std::is_same_v<pin, LCD_HD44780_pins>;
class LCD_HD44780: public LCD_HD44780_base<typename pin::Base_pins>{
public:
    using Base = LCD_HD44780_base<typename pin::Base_pins>;

// Construcción:
//  Suministro 2 formas de usar el LCD:
//  1. Defines un objeto que se autoinicializa: LCD_HD44780 lcd;
//  2. No definir el objeto y usar todas las funciones static. No olvidar
//     llamar a init():
//	    LCD_HD44780::init();
//	    LCD_HD44780::clear_display(); 
//	    ...
//  En principio me gusta más 1, pero al implementar LCD_screen es más
//  práctico 2 ya que concibo el LCD como interfaz.
//		
    LCD_HD44780():Base() {init();}

    static void init();

// INSTRUCCIONES: TABLE 6, PAG. 24
    /// Clears entire display and sets DDRAM address 0 in address counter.
    static void clear_display()
    {Base:: template clear_display<E>();}

    /// Sets DDRAM address 0 in address counter. Also returns display from 
    /// being shifted to original position. DDRAM contents remain unchanged.
    static void return_home()
    {Base:: template return_home<E>();}

    /// Set cursor move direction and specifies display shift.
    /// If incrementa_cursor = true increments the DDRAM address by 1 when 
    /// a character code is written into or read from DDRAM.
    /// If incrementa_cursor = false, decrements DDRAM address.
    ///
    /// If shift_display = true shifts the entire display to the right (if
    /// incrementa_cursor = true) or to the left (if incrementa_cursor = false).
    /// If shift_display = false the display does not shift.
    static void entry_mode(bool incrementa_cursor, bool shift_display)
    {Base:: template entry_mode<E>(incrementa_cursor, shift_display);}


    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    // display_on = false apaga el display. Esto lo único que hace es no
    // mostrar en pantalla el mensaje correspondiente. El LCD sigue
    // funcionando.
    static void display_control(bool display_on, bool cursor_on, bool cursor_blink)
    {Base:: template display_control<E>(display_on, cursor_on, cursor_blink);}

    /// Moves cursor or shift display.
    /// If display_no_cursor = true shift display, else move cursor.
    /// If to_the_right = true move to the right, else to the left.
    static void cursor_or_display_shift(bool display_no_cursor, bool to_the_right)
    { Base::template cursor_or_display_shift<E>(display_no_cursor,
                                                  to_the_right); }


    /// Sets interface data lenght (4 ó 8 bits), number of display lines
    /// (1 ó 2), and character font (5 x 8 or 5 x 10)
    static void function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		    , bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		    , bool character_font_5x8) // ¿char de 5x8 ó 5x10?
    {
        Base::template function_set(
            interface_8_bits, tiene_2_filas, character_font_5x8);
    }


    /// Sets DDRAM address: la dirección donde vamos a escribir el siguiente
    /// caracter.
    /// En el display de 2 filas, las direcciones en hexadecimal van:
    /// Fila 1: 0x00 ... 0x27
    /// Fila 2: 0x40 ... 0x67
    /// ¡¡¡Observar que no van contiguas!!!
    static void set_ddram_address(uint8_t addr)
    {Base::template set_ddram_address<E>(addr);}


    /// Sets CGRAM address.
    /// CGRAM data is sent and received after this setting.
    static void set_cgram_address(uint8_t addr)
    {Base::template set_cgram_address<E>(addr);}


    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    static void write_data_to_CG_or_DDRAM(uint8_t data)
    {Base::template write_data_to_CG_or_DDRAM<E>(data);}

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    static uint8_t read_data_from_CG_or_DDRAM()
    {return Base::template read_data_from_CG_or_DDRAM<E>();}

private:
    using E = not_generic::Pin<pin::E>;


    static void setup_pins() { E::as_output(); }

};


template <typename pin>
void LCD_HD44780<pin>::init() 
{
    Base::setup_pins();
    setup_pins();

    Base:: template init<E>(); 
}



/***************************************************************************
 *		  LCD_HD44780_4004: display 40 x 04
 ***************************************************************************/
template <uint8_t pin_E1, uint8_t pin_E2>
struct LCD_HD44780_4004_E{
    static constexpr uint8_t E1 = pin_E1;
    static constexpr uint8_t E2 = pin_E2;
};


template <typename pin_RS, 
	  typename pin_RW,
	  typename pin_E,
	  typename pins_D4>
struct LCD_HD44780_4004_pins4{
    using Base_pins = LCD_HD44780_base_pins4<pin_RS, pin_RW, pins_D4>;

    static constexpr uint8_t E1  = pin_E::E1;
    static constexpr uint8_t E2  = pin_E::E2;
};

/*!
 *  \brief  LCD de 40 x 04 basado en el HD44780 driver.
 *
 *  (RRR) ¿Por qué definir dos funciones para cada pin E en lugar de pasarle
 *        como parámetro el pin E?
 *
 *        Podemos hacer:
 *        1. Dos funciones diferentes: 
 *		    entry_mode1(...);
 *		    entry_mode2(...);
 *
 *	  2. Una función template parametrizada por E:
 *		   entry_mode<E1>(...);
 *
 *	  3. Pasarle  el pin como parámetro:
 *	          entry_mode(E1, ...);
 *
 *       La idea del traductor es traducir la datasheet sin añadir
 *       funcionalidad. De esa forma resulta muy sencillo implementarlo y no
 *       se introduce ninguna ineficiencia. Por ello descarto la opción 3.
 *       Entre la 1 y la 2, parecen iguales. De momento no tengo criterio para
 *       poder elegir entre 1 ó 2. Me resulta más sencilla la 1, por eso lo
 *       implemento así.
 *
 */
template <typename pin>
// requires: std::is_same_v<pin, LCD_HD44780_4004_pins4>;
class LCD_HD44780_4004: public LCD_HD44780_base<typename pin::Base_pins>{
public:
    using Base = LCD_HD44780_base<typename pin::Base_pins>;

    LCD_HD44780_4004() {init();}

    static void init() 
    {
	Base::setup_pins();
	setup_pins();

	Base:: template init<E1>(); 
	Base:: template init<E2>(); 
    }

// INSTRUCCIONES: TABLE 6, PAG. 24
    /// Clears entire display and sets DDRAM address 0 in address counter.
    static void clear_display()
    {
        Base::template clear_display<E1>();
        Base::template clear_display<E2>();
    }

    /// Sets DDRAM address 0 in address counter. Also returns display from 
    /// being shifted to original position. DDRAM contents remain unchanged.
    static void return_home()
    {
        Base::template return_home<E1>();
        Base::template return_home<E2>();
    }

    /// Set cursor move direction and specifies display shift.
    /// If incrementa_cursor = true increments the DDRAM address by 1 when 
    /// a character code is written into or read from DDRAM.
    /// If incrementa_cursor = false, decrements DDRAM address.
    ///
    /// If shift_display = true shifts the entire display to the right (if
    /// incrementa_cursor = true) or to the left (if incrementa_cursor = false).
    /// If shift_display = false the display does not shift.
    static void entry_mode1(bool incrementa_cursor, bool shift_display)
    {Base:: template entry_mode<E1>(incrementa_cursor, shift_display);}

    static void entry_mode2(bool incrementa_cursor, bool shift_display)
    {Base:: template entry_mode<E2>(incrementa_cursor, shift_display);}


    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    // display_on = false apaga el display. Esto lo único que hace es no
    // mostrar en pantalla el mensaje correspondiente. El LCD sigue
    // funcionando.
    static void display_control1(bool display_on, bool cursor_on, bool cursor_blink)
    {Base:: template display_control<E1>(display_on, cursor_on, cursor_blink);}

    static void display_control2(bool display_on, bool cursor_on, bool cursor_blink)
    {Base:: template display_control<E2>(display_on, cursor_on, cursor_blink);}

    /// Moves cursor or shift display.
    /// If display_no_cursor = true shift display, else move cursor.
    /// If to_the_right = true move to the right, else to the left.
    static void cursor_or_display_shift1(bool display_no_cursor, bool to_the_right)
    { Base::template cursor_or_display_shift<E1>(display_no_cursor,
                                                  to_the_right); }

    static void cursor_or_display_shift2(bool display_no_cursor, bool to_the_right)
    { Base::template cursor_or_display_shift<E2>(display_no_cursor,
                                                  to_the_right); }

    /// Sets interface data lenght (4 ó 8 bits), number of display lines
    /// (1 ó 2), and character font (5 x 8 or 5 x 10)
    static void function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		    , bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		    , bool character_font_5x8) // ¿char de 5x8 ó 5x10?
    {
        Base::template function_set(
            interface_8_bits, tiene_2_filas, character_font_5x8);
    }


    /// Sets DDRAM address: la dirección donde vamos a escribir el siguiente
    /// caracter.
    /// En el display de 2 filas, las direcciones en hexadecimal van:
    /// Fila 1: 0x00 ... 0x27
    /// Fila 2: 0x40 ... 0x67
    /// ¡¡¡Observar que no van contiguas!!!
    static void set_ddram_address1(uint8_t addr)
    {Base::template set_ddram_address<E1>(addr);}

    static void set_ddram_address2(uint8_t addr)
    {Base::template set_ddram_address<E2>(addr);}

    /// Sets CGRAM address.
    /// CGRAM data is sent and received after this setting.
    static void set_cgram_address1(uint8_t addr)
    {Base::template set_cgram_address<E1>(addr);}

    static void set_cgram_address2(uint8_t addr)
    {Base::template set_cgram_address<E2>(addr);}

    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    static void write_data_to_CG_or_DDRAM1(char data)
    {Base::template write_data_to_CG_or_DDRAM<E1>(data);}

    static void write_data_to_CG_or_DDRAM2(char data)
    {Base::template write_data_to_CG_or_DDRAM<E2>(data);}

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    static uint8_t read_data_from_CG_or_DDRAM1()
    {return Base::template read_data_from_CG_or_DDRAM<E1>();}

    static uint8_t read_data_from_CG_or_DDRAM2()
    {return Base::template read_data_from_CG_or_DDRAM<E2>();}

private:
    using E1 = not_generic::Pin<pin::E1>;
    using E2 = not_generic::Pin<pin::E2>;

    static void setup_pins() 
    { 
	E1::as_output(); 
	E2::as_output(); 
    }

};


// Una de las características fundamentales de los LCDs son sus dimensiones.
// Al definir el hardware en un programa quiero que se pueda leer que estoy
// usando un LCD de 16x02, de 20x04 ó de 40x04. Por eso defino estos alias.
template <typename pin>
using LCD_HD44780_1602 = LCD_HD44780<pin>;

template <typename pin>
using LCD_HD44780_2004 = LCD_HD44780<pin>;

}// namespace

#include "dev_HD44780_basic.tcc"

#endif
