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

#ifndef __DEV_LCD_HD4780_TR_H__
#define __DEV_LCD_HD4780_TR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del display HD44780
 *  
 *  - HISTORIA:
 *     A. Manuel López:
 *	    11/07/2017 v0.0
 *	    29/07/2019 v0.1: Creo traductor.
 *	    26/09/2019 v0.2: LCD_ostream y cambios menores.
 *	    14/10/2019 v0.3: Reestructuración de ficheros.
 *	    06/01/2020 v0.4: Elimino DPin a favor de Pin.
 *
 * - TODO: Falta probar conectarlo a 8 pines (pero en la práctica me es
 *   mucho más cómodo conectarlo solo a 4, no necesito 8 de momento).
 *
 ****************************************************************************/
#include <avr_pin.h>

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
template <uint8_t num_pin_RS>
struct LCD_HD44780_RS{
    static constexpr uint8_t RS = num_pin_RS;
};

template <uint8_t num_pin_RW>
struct LCD_HD44780_RW{
    static constexpr uint8_t RW = num_pin_RW;
};


template <uint8_t num_pin_E>
struct LCD_HD44780_E{
    static constexpr uint8_t E = num_pin_E;
};

template <uint8_t num_pin_D4,
	  uint8_t num_pin_D5,
	  uint8_t num_pin_D6,
	  uint8_t num_pin_D7>
struct LCD_HD44780_D4{
    static constexpr uint8_t D4 = num_pin_D4;
    static constexpr uint8_t D5 = num_pin_D5;
    static constexpr uint8_t D6 = num_pin_D6;
    static constexpr uint8_t D7 = num_pin_D7;
};


template <typename num_pin_RS, 
	  typename num_pin_RW,
	  typename num_pin_E,
	  typename num_pins_D4>
struct LCD_HD44780_pins4{
    static constexpr uint8_t num_pins_D = 4;

    static constexpr uint8_t RS = num_pin_RS::RS;
    static constexpr uint8_t RW = num_pin_RW::RW;
    static constexpr uint8_t E  = num_pin_E::E;

    static constexpr uint8_t D0 = 0;
    static constexpr uint8_t D1 = 0;
    static constexpr uint8_t D2 = 0;
    static constexpr uint8_t D3 = 0;
    static constexpr uint8_t D4 = num_pins_D4::D4;
    static constexpr uint8_t D5 = num_pins_D4::D5;
    static constexpr uint8_t D6 = num_pins_D4::D6;
    static constexpr uint8_t D7 = num_pins_D4::D7;
};


// TODO: no están probadas las funciones de 8 bits, así que
// no escribo de momento esta estructura struct LCD_HD44780_pins8{...}
// de esta forma es imposible usarla sin probar.

/*!
 *  \brief  Traductor del HD44780
 *
 * La forma de definir un LCD es de la siguiente forma:
 *
 * // Si lo conectamos solo a 4 pins de datos
 * using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
 *					   dev::LCD_HD44780_RW<5>,
 *					   dev::LCD_HD44780_E<6>,
 *					   dev::LCD_HD44780_D4<11,12,13,14>
 *					   >;
 *					   
 * using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;
 *
 * En caso de que se pasen en orden equivocado los pines (por ejemplo, 
 * se pase primero RW en vez de RS) dará un error (misterioso) de compilación
 * impidiendo que se definan mal los pines.
 *
 * DUDA: ¿merece la pena modificar LCD_HD44780_pins4 para que de un error
 * inteligible? De momento creo que no.
 *
 */
template <typename num_pin>
// requires: std::is_same_v<num_pin, LCD_HD44780_pins>;
class LCD_HD44780{
public:
    LCD_HD44780();


// INSTRUCCIONES: TABLE 6, PAG. 24
    /// Clears entire display and sets DDRAM address 0 in address counter.
    void clear_display();

    /// Sets DDRAM address 0 in address counter. Also returns display from 
    /// being shifted to original position. DDRAM contents remain unchanged.
    void return_home();

    /// Set cursor move direction and specifies display shift.
    /// If incrementa_cursor = true increments the DDRAM address by 1 when 
    /// a character code is written into or read from DDRAM.
    /// If incrementa_cursor = false, decrements DDRAM address.
    ///
    /// If shift_display = true shifts the entire display to the right (if
    /// incrementa_cursor = true) or to the left (if incrementa_cursor = false).
    /// If shift_display = false the display does not shift.
    void entry_mode(bool incrementa_cursor, bool shift_display);


    /// Set entire display on/off, cursor on/off and blinking of cursor on/off
    void display_control(bool display_on, bool cursor_on, bool cursor_blink);

    /// Moves cursor or shift display.
    /// If display_no_cursor = true shift display, else move cursor.
    /// If to_the_right = true move to the right, else to the left.
    void cursor_or_display_shift(bool display_no_cursor, bool to_the_right);
   

    /// Sets interface data lenght (4 ó 8 bits), number of display lines
    /// (1 ó 2), and character font (5 x 8 or 5 x 10)
    void function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		    , bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		    , bool character_font_5x8); // ¿char de 5x8 ó 5x10?

    // TODO: set_cgram_address

    /// Sets DDRAM address: la dirección donde vamos a escribir el siguiente
    /// caracter.
    /// En el display de 2 filas, las direcciones en hexadecimal van:
    /// Fila 1: 0x00 ... 0x27
    /// Fila 2: 0x40 ... 0x67
    /// ¡¡¡Observar que no van contiguas!!!
    void set_ddram_address(uint8_t addr);

    // TODO: uint8_t read_busy_flag_and_address(); (es la función
    // is_busy, reescribirla cambiando básicamente el nombre)

    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    void write_data_to_CG_or_DDRAM(char data);

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    uint8_t read_data_from_CG_or_DDRAM();


private:
// PINS
    static constexpr uint8_t num_pins_D = num_pin::num_pins_D;

    using RS = avr::Pin<num_pin::RS>;
    using RW = avr::Pin<num_pin::RW>;
    using E = avr::Pin<num_pin::E>;

    // Si solo usamos 4 pines, serán D<4>-D<7>
    using D0 = avr::Pin<num_pin::D0>;
    using D1 = avr::Pin<num_pin::D1>;
    using D2 = avr::Pin<num_pin::D2>;
    using D3 = avr::Pin<num_pin::D3>;
    using D4 = avr::Pin<num_pin::D4>;
    using D5 = avr::Pin<num_pin::D5>;
    using D6 = avr::Pin<num_pin::D6>;
    using D7 = avr::Pin<num_pin::D7>;



    /// Inicializamos el display
    void setup();
    void setup_pins();	// hacemos el setup a todos los pins
    void init();	// inicializamos el IC

    // Este display tiene 2 formas de funcionamiento:
    //	interfaz de 8 bits e interfaz de 4 bits. Cada interfaz
    //	tiene su propia forma de inicializarse.
    void init8();
    void init4();

    /// Indica si el display está ocupado haciendo una operación interna.
    /// Mientras esté ocupado no se puede pedirle nada más.
    bool is_busy();

    // Indica si el display está ocupado haciendo una operación interna.
    // Mientras esté ocupado no se puede pedirle nada más.
    bool is_busy8();
    bool is_busy4();
    
    // Funciones genéricas para escribir en el IC
    // Escribe (rs, rw, d7, d6, d5, d4, d3, d2, d1, d0) en el IC
    // Esta es la función clave a la hora de escribir. Internamente se 
    // encarga de mirar si usamos 4 ó 8 bit interface.
    void write_d(uint8_t rs, uint8_t rw, uint8_t d);

    // Implementación de write_d. No llamar a estas funciones NUNCA
    // directamente, sino siempre a través de write_d!!!
    void write_d8(uint8_t rs, uint8_t rw, uint8_t d);
    void write_d4(uint8_t rs, uint8_t rw, uint8_t d);

    // Escribe solo los bits d4-d7 de 'd' (los más significativos)
    // Escribe (rs, rw, d7, d6, d5, d4) en el IC
    void write_d4_d7(uint8_t rs, uint8_t rw, uint8_t d);

    // Lee un byte usando solo los bits d4-d7 (esta función solo
    // se usa en read_data_from_CG_or_DDRAM).
    uint8_t read_d4();

    // Para realizar una operación en el LCD necesitamos activar E
    void pulse_E();

    // Espera hasta que el LCD esta disponible para la siguiente instrucción
    void wait_to_be_available()
    { while(is_busy()) ; }

// D PIN ACCESS
    void D_pins_as_output();
    void D_pins_as_input_without_pullup();

    void write_d4(uint8_t d);
    void write_d8(uint8_t d);
};

}// namespace

#include "dev_LCD_HD44780_tr.tcc"

#endif
