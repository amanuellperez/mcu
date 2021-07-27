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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.

#pragma once

#ifndef __KEYBOARD_5X5_H__
#define __KEYBOARD_5X5_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Keyboard de 5 x 5
 *    Se trata de una matriz de pulsadores de 5 filas y 5 columnas.
 *    Es un teclado pensado para pulsar una tecla cada vez, una después de
 *    otra. Si se pulsan varias no se produce corto pero se deja undefined el
 *    resultado de scan().
 * 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    16/07/2021 v0.0
 *
 ****************************************************************************/
#include <avr_pin.h>


template <uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4>
struct Keyboard_5_rows{
    static constexpr uint8_t row0  = r0;
    static constexpr uint8_t row1  = r1;
    static constexpr uint8_t row2  = r2;
    static constexpr uint8_t row3  = r3;
    static constexpr uint8_t row4  = r4;
    static constexpr uint8_t nrows = 5;
};

template <uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4>
struct Keyboard_5_cols{
    static constexpr uint8_t col0  = c0;
    static constexpr uint8_t col1  = c1;
    static constexpr uint8_t col2  = c2;
    static constexpr uint8_t col3  = c3;
    static constexpr uint8_t col4  = c4;
    static constexpr uint8_t ncols = 5;
};

template <typename Rows, typename Cols>
class Keyboard_5x5 {
public:
    Keyboard_5x5();

    // Devuelve true si ha encontrado alguna tecla pulsada.
    // No bloquea la ejecución. Devuelve el control inmediatamente después
    // del scan.
    bool scan();

    // scanea el teclado hasta que se pulsa una tecla. 
    // No devuelve el control hasta que el usuario pulsa una tecla. Bloquea!
    uint8_t getkey();
    
    // Devuelve como int la última tecla pulsada.
    uint8_t last_key() const;

    // ultima tecla pulsada
    uint8_t row() const {return row_;}
    uint8_t col() const {return col_;}

// Configuration
    static constexpr uint8_t nrows() {return Rows::nrows;}
    static constexpr uint8_t ncols() {return Cols::ncols;}

private:
    avr::Pin<Cols::col0> col0;
    avr::Pin<Cols::col1> col1;
    avr::Pin<Cols::col2> col2;
    avr::Pin<Cols::col3> col3;
    avr::Pin<Cols::col4> col4;

    avr::Pin<Rows::row0> row0;
    avr::Pin<Rows::row1> row1;
    avr::Pin<Rows::row2> row2;
    avr::Pin<Rows::row3> row3;
    avr::Pin<Rows::row4> row4;

// last key pressed
    uint8_t row_ = null_rcol;
    uint8_t col_ = null_rcol;


// Helper functions
    uint8_t scan_rows() const;
    uint8_t scan_cols() const;
    constexpr static uint8_t null_rcol = 100; // null row/col (= incorrect value)

    void scan_row0();
    void scan_row1();
    void scan_row2();
    void scan_row3();
    void scan_row4();
};

template <typename R, typename C>
Keyboard_5x5<R, C>::Keyboard_5x5()
{
    row0.as_input_with_pullup();
    row1.as_input_with_pullup();
    row2.as_input_with_pullup();
    row3.as_input_with_pullup();
    row4.as_input_with_pullup();

    col0.as_input_without_pullup();
    col1.as_input_without_pullup();
    col2.as_input_without_pullup();
    col3.as_input_without_pullup();
    col4.as_input_without_pullup();
}


template <typename R, typename C>
uint8_t Keyboard_5x5<R, C>::getkey()
{
    while (scan()  != true) { }

    return last_key();
}

// Devuelve como int la última tecla pulsada.
template <typename R, typename C>
inline uint8_t Keyboard_5x5<R, C>::last_key() const
{
    return nrows() * row_ + col_;
}

template <typename R, typename C>
uint8_t Keyboard_5x5<R, C>::scan_rows() const
{
    row0.as_input_with_pullup();
    row1.as_input_with_pullup();
    row2.as_input_with_pullup();
    row3.as_input_with_pullup();
    row4.as_input_with_pullup();

    col0.as_output(); col0.write_zero();
    col1.as_output(); col1.write_zero();
    col2.as_output(); col2.write_zero();
    col3.as_output(); col3.write_zero();
    col4.as_output(); col4.write_zero();

    if (row0.is_zero()) return 0;
    if (row1.is_zero()) return 1;
    if (row2.is_zero()) return 2;
    if (row3.is_zero()) return 3;
    if (row4.is_zero()) return 4;

    return null_rcol;
}


template <typename R, typename C>
uint8_t Keyboard_5x5<R, C>::scan_cols() const
{
    col0.as_input_with_pullup();
    col1.as_input_with_pullup();
    col2.as_input_with_pullup();
    col3.as_input_with_pullup();
    col4.as_input_with_pullup();

    row0.as_output(); row0.write_zero();
    row1.as_output(); row1.write_zero();
    row2.as_output(); row2.write_zero();
    row3.as_output(); row3.write_zero();
    row4.as_output(); row4.write_zero();
    wait_ms(5); // sin este delay falla la tecla (4,0)
		// es para esperar que alcance equilibrio los pines

    if (col0.is_zero()) return 0;
    if (col1.is_zero()) return 1;
    if (col2.is_zero()) return 2;
    if (col3.is_zero()) return 3;
    if (col4.is_zero()) return 4;

    return null_rcol;
}


template <typename R, typename C>
bool Keyboard_5x5<R, C>::scan()
{
    row_ = scan_rows();

    if (row_ != null_rcol)
	col_ = scan_cols();

    return (row_ != null_rcol and col_ != null_rcol);
}




#endif

