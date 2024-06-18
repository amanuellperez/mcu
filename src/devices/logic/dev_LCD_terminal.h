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

#ifndef __DEV_LCD_TERMINAL_H__
#define __DEV_LCD_TERMINAL_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Terminal
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
 *	28/02/2022       Divido Screen en dos clases: Screen y Terminal
 *			 La clase Terminal realmente es un ostream!!!
 *			 TODO: este Terminal no es más que la implementación
 *			 de LCD_streambuf!!! Fusionarlas eliminando esta clase
 *			 Terminal, dejando solo LCD_ostream.
 *
 *
 ****************************************************************************/
#include <stdint.h>

#include <avr_atmega.h>	// Progmem TODO: remove. Not generic
#include "not_generic.h"
#include <atd_memory.h>

namespace dev{

// Screen flags
// ------------
enum class _LCD_terminal_flags {
    stop_brcorner_bit	  = 1L << 1,	// stop bottom-right corner
    wrap_bit		  = 1L << 2
};

inline constexpr _LCD_terminal_flags operator&(_LCD_terminal_flags a, _LCD_terminal_flags b)
{return static_cast<_LCD_terminal_flags>(static_cast<int>(a) & static_cast<int>(b));}

inline constexpr _LCD_terminal_flags operator|(_LCD_terminal_flags a, _LCD_terminal_flags b)
{return static_cast<_LCD_terminal_flags>(static_cast<int>(a) | static_cast<int>(b));}

inline constexpr _LCD_terminal_flags operator^(_LCD_terminal_flags a, _LCD_terminal_flags b)
{return static_cast<_LCD_terminal_flags>(static_cast<int>(a) ^ static_cast<int>(b));}

inline constexpr _LCD_terminal_flags operator~(_LCD_terminal_flags a)
{return static_cast<_LCD_terminal_flags>(~static_cast<int>(a));}

inline constexpr _LCD_terminal_flags& operator&=(_LCD_terminal_flags& a, _LCD_terminal_flags b)
{return a = a & b;}

inline constexpr _LCD_terminal_flags& operator|=(_LCD_terminal_flags& a, _LCD_terminal_flags b)
{return a = a | b;}

inline constexpr _LCD_terminal_flags& operator^=(_LCD_terminal_flags& a, _LCD_terminal_flags b)
{return a = a ^ b;}

inline constexpr bool operator==(_LCD_terminal_flags a, int b)
{return static_cast<int>(a) == b;}

inline constexpr bool operator!=(_LCD_terminal_flags a, int b)
{return !(a == b);}


/*!
 *  \brief  Concebimos el LCD como una terminal normal y corriente.
 *
 *  Esta clase nos permite concebir el LCD como un terminal o pantalla: vamos
 *  escribiendo, si llegamos al final de una línea introduce un retorno de
 *  carro, mientras que si llegamos al final del LCD sube todo lo escrito
 *  hacia arriba.
 *
 *  El LCD en el que se basa es un LCD genérico, con dos páginas de
 *  caracteres: la normal, que trae por defecto, y la definida por el usuario,
 *  que llamaré 'la extendida'. 
 *
 *  Además suministra funciones de acceso aleatorio para escribir en cualquier
 *  posición.
 *
 *  Notación: Personalmente me resulta más natural hablar como en las matrices
 *  (num_rows x num_cols), sin embargo, en internet siempre escriben primero
 *  num_cols x num_rows (hablan de 1602 y 2004).
 *
 *  La forma de definir una terminal es:
 *  using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
 *					dev::LCD_HD44780_RW<5>,
 *					dev::LCD_HD44780_E<6>,
 *					dev::LCD_HD44780_D4<11,12,13,14>
 *					>;
 *
 *  using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;
 *  using LCD_HD44780_1602_terminal = dev::LCD_HD44780_1602_terminal<LCD_HD44780>;
 *  using LCD_HD44780_2004_terminal = dev::LCD_HD44780_2004_terminal<LCD_HD44780>;
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
class LCD_terminal{
public:
// Init
    /// Conectamos el HD44780 con el interface 4 bits.
    LCD_terminal()
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
    /// Por defecto, wrap() == true, con lo que si llega al final de una línea
    /// continua en la línea siguiente. 
    /// Si nowrap() == true, imprime todo lo que puede de la primera línea en
    /// la primera fila del lcd, luego la segunda línea en la segunda fila...
    /// haciendo un wrap de las líneas en caso de que su longitud exceda el
    /// ancho del lcd.
    void print(const char* c);


    /// Imprime la cadena [p0, pe), pero en lugar de escribirla de izda a dcha, 
    /// como hace print, lo hace de dcha a izda. 
    /// En principio está pensada para cadenas cortas que entren en una línea
    /// en el LCD. 
    void print_align_to_the_right(const char* p0, const char* pe);

    // void print_align_to_the_right(uint8_t x); uint8_t == char!!!!
    void print_align_to_the_right(uint16_t x);
    void print_align_to_the_right(const uint32_t& x);
    void print_align_to_the_right(const uint64_t& x);

    // void print_align_to_the_right(uint8_t x); uint8_t == char!!!!
    void print_align_to_the_right(int16_t x);
    void print_align_to_the_right(const int32_t& x);
    void print_align_to_the_right(const int64_t& x);

    template <int ndigits = 8>
    void print_align_to_the_right(const double& x);

    /// Imprime la línea [p, '\n') escribiendo un máximo de num_max_char
    /// caracteres. Si la línea no entrase en el LCD no hace el wrap sino que la
    /// deja cortada (es el comportamiento del parámetro por defecto).
    /// Returns: un puntero a la primera letra de la siguiente línea, en caso
    /// de que la haya, o a '\0' en caso de que llegue al final de la cadena de
    /// C.
    /// El cursor queda colocado fuera del LCD (cursor_pos_x() == cols()!!!).
    const char* print_line_nowrap(const char* p, uint8_t num_max_char = cols());


// MOVIMIENTO DEL CURSOR
    /// Define la posición del cursor. En caso de pasarle una fila no 
    /// válida coloca el cursor en el (0,0).
    void cursor_pos(uint8_t col, uint8_t row);

    /// Devuelve la posición x del cursor.
    uint8_t cursor_pos_x() const {return x_;}

    /// Devuelve la posición y del cursor.
    uint8_t cursor_pos_y() const {return y_;}


//    // TODO: estas funciones no pertenecen a un terminal, más bien pertenecen a
//    // un cartel publicitario.
//    /// Mueve el display 1 posición a la derecha.
//    void cursor_shift_right() {LCD::cursor_or_display_shift(false, true);}
//
//    /// Mueve el display 1 posición a la izquierda.
//    void cursor_shift_left() {LCD::cursor_or_display_shift(false, false);}

    /// Flag que indica si al llegar a la esquina inferior derecha se deja
    /// de hacer el scrol o no.
    bool stop_brcorner() const {return flag(stop_brcorner_bit);}

    /// Al llegar a la esquina inferior derecha ¿dejamos de mover el cursor?
    /// Esto afecta siempre al comportamiento de print(char c).
    void stop_brcorner(bool stop)
    {
        if (stop)
            set_flag(stop_brcorner_bit);
        else
            unset_flag(stop_brcorner_bit);
    }

    /// Si la longitud de una línea es mayor que el ancho del LCD,
    /// ¿continuamos escribiéndola en la siguiente línea?
    // TODO: ¿qué nombre? Me gusta wrap() y nowrap() para el set y unset.
    bool is_wrap() const {return flag(wrap_bit);}

    /// Si la longitud de una línea es mayor que el ancho del LCD,
    /// ¿continuamos escribiéndola en la siguiente línea?
    /// Por defecto, wrap() == true.
    /// Returns: el estado del flag wrap antes de llamar a wrap().
    bool wrap() 
    { 
	bool res = flag(wrap_bit);
	set_flag(wrap_bit);
	return res;
    }

    bool nowrap() 
    { 
	bool res = flag(wrap_bit);
	unset_flag(wrap_bit); 
	return res;
    }



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
    
    template <typename ROM_read>
    void new_extended_char(uint8_t c,
                           const atd::ROM_array<uint8_t, 8, ROM_read>& glyph);
    void new_extended_char(uint8_t c,
                           const not_generic::Progmem_array_view<uint8_t, 8>& glyph);

// DATOS DEL LCD
    constexpr static uint8_t rows() {return num_rows;}
    constexpr static uint8_t cols() {return num_cols;}


// ACCESO AL LCD
    Generic_LCD_type& lcd() {return lcd_;}
    const Generic_LCD_type& lcd() const{return lcd_;}

private:
// Hardware al que está conectado
    Generic_LCD_type lcd_;

// Data
    // Invariante: el cursor siempre está en una posición válida de escritura.
    // (x_, y_) = posición del LCD real donde se puede escribir.
    uint8_t x_, y_; // (col, row) en la que se encuentra el cursor.


    // terminal_flags
    // ------------
    using scrflags = _LCD_terminal_flags;
    
    // Indica si cuando se intenta imprimir en el último caracter de la
    // terminal, el caracter de la última línea a la derecha, se imprime el
    // caracter y se hace el '\n', o si no se imprime el '\n' (stop = true).
    // De esta forma podemos escribir en todos los caracteres de un LCD, sino
    // es imposible escribir en el último caracter. Por defecto es false,
    // funcionando la terminal como un terminal normal y corriente.
    static constexpr scrflags stop_brcorner_bit = scrflags::stop_brcorner_bit;

    // Indica si cuando una linea es mayor que el ancho del terminal,
    // continuamos escribiendola en la siguiente línea o no.
    static constexpr scrflags wrap_bit = scrflags::wrap_bit;

    scrflags scrflags_ = scrflags::wrap_bit;

    // Añade los flags de flg a scrflags_
    void set_flag(scrflags flg) { scrflags_ |= flg; }

    // Borra mask de scrflags_
    void unset_flag(scrflags mask) { scrflags_ &= ~mask;}

    // El bit del flag flg ¿is set?
    bool flag(scrflags flg) const { return (scrflags_ & flg) != 0; }


    // Funciones de ayuda
    // ------------------
    // Enciende/apaga el cursor.
    bool cursor(bool on);

    // Hace que el cursor parpadee o deje de parpadear.
    void cursor_blink(bool yes);
    
    void cursor_move();
    void scroll_text_up();
    void copy_this_row_in_row_above(uint8_t i);
    void print_return();
    void print_printable_char(char c);

    // Imprimimos haciendo un wrap de las lineas (si exceden el ancho
    // del terminal las pasamos a la siguiente linea)
//    bool print_wrap(char c); ¿Qué diferencia de esta versión entre nowrap?
    void print_wrap(const char* c);

    // Imprimimos sin hacer un wrap de las líneas. Si una línea excede el
    // ancho del terminal la cortamos, mostrando solo lo que entre en el lcd.
    // OJO: si [p, '\0') esta formado por más líneas de las que tiene el lcd,
    // solo imprimimos las primeras lineas que entran en el display.
    // Esta forma de comportarse encaja muy bien para imprimir menus en el
    // lcd.
//   bool print_nowrap(char c);
    void print_nowrap(const char* p);

    // Rellena la línea actual del LCD con el caracter c. Escribe como máximo
    // n caracteres.
    // Postcondicion: cursor_pos_x() == cols() <-- observar que el cursor
    // queda fuera del LCD!!!
    void fill_line(uint8_t n, char c);

    // Implementación para Integers
    template <typename Int>
    void print_align_to_the_right_(const Int& x);
};

template <typename LCD>
using LCD_terminal_1602 = LCD_terminal<16,2, LCD>;

template <typename LCD>
using LCD_terminal_2004 = LCD_terminal<20,4, LCD>;

template <typename LCD>
using LCD_terminal_4004 = LCD_terminal<40,4, LCD>;

}// namespace


#include "dev_LCD_terminal.tcc"


#endif
