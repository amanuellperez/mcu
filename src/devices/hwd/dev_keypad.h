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

#ifndef __DEV_KEYPAD_H__
#define __DEV_KEYPAD_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Keyboard de rows x cols
 *    Se trata de una matriz de pulsadores de 'rows' filas y 'cols' columnas.
 *    Es un teclado pensado para pulsar una tecla cada vez, una después de
 *    otra. Si se pulsan varias no se produce corto pero se deja undefined el
 *    resultado de scan().
 * 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    31/07/2021 Keypad, Keyboard_keypad
 *
 ****************************************************************************/
#include "not_generic.h"

#include <atd_static.h>

#undef getchar // get rid of macros

namespace dev{

template <uint8_t r0, uint8_t... args>
struct Keypad_rows{
    using Pins = atd::static_array<uint8_t, r0, args...>;
};

template <uint8_t c0, uint8_t... args>
struct Keypad_cols{
    using Pins = atd::static_array<uint8_t, c0, args...>;
};

template <typename Rows0, typename Cols0>
class Keypad {
public:
    Keypad();

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
    static constexpr uint8_t nrows() {return Rows::size;}
    static constexpr uint8_t ncols() {return Cols::size;}

private:
    using Rows = Rows0::Pins;
    using Cols = Cols0::Pins;

    template <typename Pin, uint8_t i = 0>
    static constexpr void pin_as_inputs_without_pullup();

    template <typename Pin, uint8_t i = 0>
    static constexpr void pin_as_inputs_with_pullup();

    template <typename Pin, uint8_t i = 0>
    static constexpr void pin_as_output_and_write_zero();

    template <typename Pin, uint8_t i = 0>
    constexpr static int what_pin_is_zero();

// last key pressed
    uint8_t row_ = null_rcol;
    uint8_t col_ = null_rcol;


// Helper functions
    uint8_t scan_rows() const;
    uint8_t scan_cols() const;
    constexpr static uint8_t null_rcol = 100; // null row/col (= incorrect value)

};

template <typename R, typename C>
Keypad<R, C>::Keypad()
{
    pin_as_inputs_with_pullup<Rows>();
    pin_as_inputs_without_pullup<Cols>();
}


template <typename R, typename C>
uint8_t Keypad<R, C>::getkey()
{
    while (scan()  != true) { }

    return last_key();
}

// Devuelve como int la última tecla pulsada.
template <typename R, typename C>
inline uint8_t Keypad<R, C>::last_key() const
{
    return ncols() * row_ + col_;
}

template <typename R, typename C>
uint8_t Keypad<R, C>::scan_rows() const
{
    pin_as_inputs_with_pullup<Rows>();
    pin_as_output_and_write_zero<Cols>();

    return what_pin_is_zero<Rows>();
}


template <typename R, typename C>
uint8_t Keypad<R, C>::scan_cols() const
{
    pin_as_inputs_with_pullup<Cols>();

    pin_as_output_and_write_zero<Rows>();
    not_generic::wait_ms(5); // sin este delay falla la tecla (4,0)
		// es para esperar que alcance equilibrio los pines

    return what_pin_is_zero<Cols>();
}


template <typename R, typename C>
bool Keypad<R, C>::scan()
{
    row_ = scan_rows();

    if (row_ != null_rcol)
	col_ = scan_cols();

    return (row_ != null_rcol and col_ != null_rcol);
}


template <typename R, typename C>
template <typename Pin, uint8_t i>
constexpr void Keypad<R, C>::pin_as_inputs_without_pullup()
{
    if constexpr (i == Pin::size)
	return;

    if constexpr (i != Pin::size){
	constexpr int npin = Pin::template at<i>;
	not_generic::Pin<npin>::as_input_without_pullup();
	pin_as_inputs_without_pullup<Pin, i+1>();
    }
}

template <typename R, typename C>
template <typename Pin, uint8_t i>
constexpr void Keypad<R, C>::pin_as_inputs_with_pullup()
{
    if constexpr (i == Pin::size)
	return;

    if constexpr (i != Pin::size){
	constexpr int npin = Pin::template at<i>;
	not_generic::Pin<npin>::as_input_with_pullup();
	pin_as_inputs_with_pullup<Pin, i+1>();
    }
}

template <typename R, typename C>
template <typename Pin, uint8_t i>
constexpr void Keypad<R, C>::pin_as_output_and_write_zero()
{
    if constexpr (i == Pin::size)
	return;

    if constexpr (i != Pin::size){
	constexpr int npin = Pin::template at<i>;
	not_generic::Pin<npin>::as_output();
	not_generic::Pin<npin>::write_zero();
	pin_as_output_and_write_zero<Pin, i+1>();
    }
}


template <typename R, typename C>
template <typename Pin, uint8_t i>
constexpr int Keypad<R, C>::what_pin_is_zero()
{
    if constexpr (i == Pin::size)
	return null_rcol;

    if constexpr (i != Pin::size){
	constexpr int npin = Pin::template at<i>;
	if (not_generic::Pin<npin>::is_zero())
	    return i;

	return what_pin_is_zero<Pin, i+1>();
    }
}



/*!
 *  \brief Capa puesta sobre el keypad que permite traducir las teclas
 *	   pulsadas en el código kascii correspondiente.
 *
 *  key_code nos dice como mapear las teclas del Keypad. La tecla 0 le
 *  asocio el código el código 'a', la tecla 1 el código 'right'...
 *
 */
template <typename Keypad>
class Keyboard_keypad{
public:

    Keyboard_keypad(const uint8_t* key_code0): key_code_{key_code0} {}

    uint8_t getchar();


private:
    Keypad keypad_;
    const uint8_t* key_code_;

    // Traduce la tecla pulsada al código ascii correspondiente
    uint8_t key_to_code(uint8_t c) {return key_code_[c];}
};


template <typename K>
inline uint8_t Keyboard_keypad<K>::getchar()
{
    return key_to_code(keypad_.getkey());
}


}// namespace


#endif

