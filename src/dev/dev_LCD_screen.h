// Copyright (C) 2019-2022 A.Manuel L.Perez 
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

#ifndef __DEV_LCD_SCREEN_H__
#define __DEV_LCD_SCREEN_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz al display HD44780
 *  
 *  - HISTORIA:
 *    A. Manuel L.Perez
 *	11/07/2017 v0.0
 *	29/07/2019 v0.1: Creo traductor.
 *	26/09/2019 v0.2: LCD_ostream y cambios menores.
 *	02/11/2019 v0.3: Añado flag stop_brcorner y wrap. Reescribo los flags.
 *	06/01/2020 v0.4: Elimino DPin a favor de Pin.
 *	09/01/2021 v0.5: Reestructurado. Lo desvinculo del HD44780.
 *		         Uso Generic_LCD.
 *	30/07/2021       print_align_to_the_right
 *	06/02/2022 v0.6: Basado en el concept 'Generic_LCD'
 *	28/02/2022     : Divido Screen en dos clases, Screen y Terminal.
 *
 ****************************************************************************/
#include <stdint.h>
#include <avr_memory.h>	// Progmem
#include <atd_types.h>	// Width

namespace dev{
/*!
 *  \brief  Concebimos el LCD como un Screen.
 *
 *  El LCD en el que se basa es un LCD genérico, con dos páginas de
 *  caracteres: la normal, que trae por defecto, y la definida por el usuario,
 *  que llamaré 'la extendida'. 
 *
 *  Notación: Personalmente me resulta más natural hablar como en las matrices
 *  (num_rows x num_cols), sin embargo, en internet siempre escriben primero
 *  num_cols x num_rows (hablan de 1602 y 2004).
 *
 *  Notas
 *  -----
 *  (1) En principio da la impresión de que num_cols y num_rows es información 
 *      redundante, ya que esa información tenía que estar en
 *      Generic_LCD_type. Sin embargo, curiosamente los traductores de los
 *      HD44780 no necesitan (num_cols, num_rows) para nada, no teniendo dicha
 *      información. Por eso es necesario pasarla aquí como parámetro.
 */
template <uint8_t num_cols, uint8_t num_rows, typename Generic_LCD_type>
class LCD_screen{
public:
// Init
    /// Conectamos el HD44780 con el interface 4 bits.
    LCD_screen()
	:x_{0}, y_{0} { } 


// FUNCIONES DE IMPRESIÓN
    /// Borra la pantalla dejando el cursor al principio.
    void clear() 
    {
	lcd_.clear_display();
	x_ = 0; y_ = 0;
    }

    /// Borra la fila i.
    void clear_row(uint8_t i);

    /// Imprime un caracter donde estuviese el cursor colocado.
    /// Returns: true si imprime el caracter, false si no.
    // Esta es la función básica de impresión. Todas las demás funciones
    // se basan en esta.
    bool print(char c);

    /// Imprime un caracter extendido, un caracter definido por el usuario.
    /// Returns: true si imprime el caracter, false si no.
    // La idea es concebir la página extendida, con los caracteres definidos
    // por el usuario, como un array lineal: del 0-n. 'c' nos da el índice
    // dentro de este array.
    bool print_extended(char c);

    /// Imprime una cadena.
    // Si la cadena no entra en lo que queda de línea la corta.
    // Return: el número de caracteres escritos.
    uint8_t print(const char* c);

    /// Imprime el número indicado.
    /// return: true si lo ha impreso entero. 
    ///         false, si solo parcialmente o ha fallado.
    // (RRR) ¿por qué llamar print_number(uint8_t)? 
    //       Porque uint8_t es un tipo char y por tanto se confunde
    //       print(char) con print(uint8_t).
    bool print_number(uint8_t x);
    bool print(uint16_t x);
    bool print(const uint32_t& x);
    bool print(const uint64_t& x);

    bool print_number(int8_t x);
    bool print(int16_t x);
    bool print(const int32_t& x);
    bool print(const int64_t& x);

    // Impresión de números con diferentes fuentes.
    // Todas estas funciones escriben los números con ceros a la izquierda de
    // tamaño Width
    bool print(uint16_t x, const atd::Width<uint8_t>& w);

// MOVIMIENTO DEL CURSOR
    /// Define la posición del cursor. En caso de pasarle una fila no 
    /// válida coloca el cursor en el (0,0).
    void cursor_pos(uint8_t col, uint8_t row);

    /// Devuelve la posición x del cursor.
    uint8_t cursor_pos_x() const {return x_;}

    /// Devuelve la posición y del cursor.
    uint8_t cursor_pos_y() const {return y_;}


// ACCESO A LA INFORMACIÓN MOSTRADA EN EL DISPLAY
    /// Devuelve el caracter mostrado en la posición (x, y)
    uint8_t read(uint8_t x, uint8_t y); 


// CONFIGURACIÓN DEL LCD
    /// Enciende el LCD.
    void display_on() {lcd_.display_on();}

    /// Apaga el LCD.
    void display_off() {lcd_.display_off();}

    /// Muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_on()	{return lcd_.cursor_on();}

    /// No muestra el cursor.
    /// Returns: El estado anterior del cursor.
    bool cursor_off()	{return lcd_.cursor_off();}

    /// Hace que el cursor parpadee.
    void cursor_blink()	    {lcd_.cursor_blink();}

    /// Hace que el cursor no parpadee.
    void cursor_no_blink()  {lcd_.cursor_no_blink();}


// PÁGINA DE MEMORIA EXTENDIDA
    /// Crea un nuevo caracter 'c' de 8 filas en la página de memoria extendida.
    // TODO: limpiar. Dejar una sola de estas funciones
    void new_extended_char(uint8_t c, const uint8_t glyph[8]);
    void new_extended_char(uint8_t c,
                           const avr::Progmem_array<uint8_t, 8>& glyph);
    void new_extended_char(uint8_t c,
                           const avr::Progmem_array_view<uint8_t, 8>& glyph);

    // DATOS DEL LCD
    constexpr static uint8_t rows() {return num_rows;}
    constexpr static uint8_t cols() {return num_cols;}

    
private:
// Hardware al que está conectado
    Generic_LCD_type lcd_;

// Data
    // Invariante: el cursor siempre está en una posición válida de escritura.
    // (x_, y_) = posición del LCD real donde se puede escribir.
    uint8_t x_, y_; // (col, row) en la que se encuentra el cursor.


    // Funciones de ayuda
    // ------------------
    // Enciende/apaga el cursor.
    bool cursor(bool on);

    // Hace que el cursor parpadee o deje de parpadear.
    void cursor_blink(bool yes);
    
    void cursor_move();
    void print_return();
    void print_printable_char(char c);

    // return: true si lo ha impreso entero. 
    //         false, si solo parcialmente o ha fallado.
    template <typename Int>
    bool print_unsigned_number(const Int& x, int ndigits = 0);

    template <typename Int>
    bool print_signed_number(const Int& x, int ndigits = 0);

};

template <typename LCD>
using LCD_screen_1602 = LCD_screen<16,2, LCD>;

template <typename LCD>
using LCD_screen_2004 = LCD_screen<20,4, LCD>;

template <typename LCD>
using LCD_screen_4004 = LCD_screen<40,4, LCD>;

}// namespace


#include "dev_LCD_screen.tcc"


#endif
