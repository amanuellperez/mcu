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
 *   - DESCRIPCION: Traductor del display HD44780
 *  
 *   - TODO: 
 *	    Eliminar los pines dinámicos. La forma de conectar el LCD se
 *	    conoce en tiempo de compilación.
 *
 *   - DUDA: Tal como está implementado es genérico: admite conectar a 4 u 8
 *	pines. Sin embargo eso trae el pago de que si lo conectas a 4 estas
 *	enlazando también el código de 8 (son 2-3 funciones solo) aumentando
 *	el código. Se está pagando por algo que no se está usando. ¿Dividir la
 *	clase en dos, una con interfaz para 4 y otra para 8 para evitar esto?
 *
 *   - HISTORIA:
 *     A. Manuel López:
 *	    11/07/2017 v0.0
 *	    29/07/2019 v0.1: Creo traductor.
 *	    26/09/2019 v0.2: LCD_ostream y cambios menores.
 *	    14/10/2019 v0.3: Reestructuración de ficheros.
 *
 *
 ****************************************************************************/
#include <ostream>

#include <avr_dpin.h>

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
class LCD_HD44780{
public:
    // DPines que tiene el LCD. Para evitar errores en la construcción.
    struct DPin_RS {uint8_t v;};
    struct DPin_RW {uint8_t v;};
    struct DPin_E  {uint8_t v;};

    // cuando queremos usar 4-bit interface
    struct DPin_D4 {uint8_t D4, D5, D6, D7;};

    // cuando queremos usar 8-bit interface
    struct DPin_D8 {uint8_t D0, D1, D2, D3, D4, D5, D6, D7;};

    /// Definimos las conexiones hardware del display.
    /// Interface 8 bits.
    /// TODO: comento este constructor, ya que no lo he probado con el AVR.
    /// Funcionaba bien el interfaz 8 bits con arduino, pero no he hecho
    /// las pruebas correspondientes en AVR. Probarlo!!! Revisar la función
    /// init8, creo que está mal.
//    LCD_HD44780(DPin_RS rs, DPin_RW rw, DPin_E e, const DPin_D8& d);

    /// Interface 4 bits.
    LCD_HD44780(DPin_RS rs, DPin_RW rw, DPin_E e, const DPin_D4& d);


    // -------------------------------
    // Instrucciones: table 6, pag. 24
    // -------------------------------
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
    void set_ddram_address_1row() {set_ddram_address(0x00);}
    void set_ddram_address_2row() {set_ddram_address(0x40);}


    // TODO: uint8_t read_busy_flag_and_address(); (es la función
    // esta_ocupado, reescribirla cambiando básicamente el nombre)

    /// Write data to CG or DDRAM.
    /// To write into CG or DDRAM is determined by previous specification of
    /// the CGRAM or DDRAM address setting.
    void write_data_to_CG_or_DDRAM(char data);

    /// Read data from CG or DDRAM.
    /// Before entering this read instruction, either CGRAM or DDRAM address
    /// set instruction must be executed.
    uint8_t read_data_from_CG_or_DDRAM();


private:
    // Conexiones hardware
    avr::DPin RS;
    avr::DPin RW;
    avr::DPin E;
    avr::DPin D[8]; // Si solo usamos 4 pines, serán D[4]-D[7]

    // Configuración del IC
    uint8_t num_pins_d = 8; // Número de pines conectados ¿4 ó 8?
			    // TODO: ¿por qué no usar una enum?

    // syntactic sugar
    bool es_interface8_bits() {return num_pins_d == 8;}


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
    bool esta_ocupado();

    // Indica si el display está ocupado haciendo una operación interna.
    // Mientras esté ocupado no se puede pedirle nada más.
    bool esta_ocupado8();
    bool esta_ocupado4();
    
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
    void pulso_E();

    // Espera hasta que el LCD esta disponible para la siguiente instrucción
    // DUDA: añadirle un tiempo máximo de espera
    void espera_estar_disponible()
    { while(esta_ocupado()) ; }

};


}// namespace

#endif
