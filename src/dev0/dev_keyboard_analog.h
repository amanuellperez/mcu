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

#ifndef __KEYBOARD_ANALOG_H__
#define __KEYBOARD_ANALOG_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Keyboard conectado al ADC.
 *
 *  - COMENTARIOS: El teclado de una calculadora científica tiene mínimo 64
 *    teclas, siendo necesarios 8*2 = 16 pins para conectarlos al avr si se
 *    conecta como keypad. Demasiados pines. 
 *
 *    Otra forma de conectar el teclado es conectarlo directamente al ADC.
 *    Midiendo el número de arefs, obtenemos la tecla pulsada.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    25/08/2021 Keyboard_ADC0 <-- NO FUNCIONA BIEN. Problemas con dependencia
 *    de la temperatura!!! Modificarlo en el futuro o borrarlo.
 *
 ****************************************************************************/

#include <avr_ADC_basic.h>
#include "not_generic.h"

#include <tuple>    // std::tie


namespace dev{

// Arithmetic Bidimensional sequence:
// El potencial medido por el ADC vendrá dado por una sucesión V(row, col).
// El problema es el ruido introducido en el potencial. El potencial real
// medido será Vm = V(row, col) +- ruido. 
//
// Esta clase se encarga de calcular los índices (row, col) dado Vm, teniendo
// en cuenta un margen de error.
//
// La forma de gestionar el error es empezar la sucesión no en a0 (=V(0,0))
// sino en b0 (= a0 - nc/2). 
// Ejemplo: V(r, c) = 385 + 81r + 10c (aqui nr = 81 y nc = 10)
// Vamos a admitir un error de +-5 (= 10 / 2). Esto es, [380, 389] es a(0,0),
// [390, 399] es a(0,1) ...
//
// Observar que es una clase genérica. Si se ve que se puede usar en otros
// sitios llevarla a atd::math y cambiarle el nombre (pero ¿cuál?)
class __Bisequence{
public:
    using Int = uint16_t;

// Dos formas de inicializarlo:
    __Bisequence(Int a0, Int nc, Int nr)
        : b0_{a0 - nc / Int{2}}, nc_{nc}, nr_{nr} {}

// init: hay que definir a0 antes de poder usar la clase. 
//       Defino b0_ = 0 para poder detectar olvidos.
    __Bisequence(Int nc, Int nr) : b0_{0}, nc_{nc}, nr_{nr} {}
    void a0(Int a00) // a0 = primer elemento de la sucesión. Es V(0,0) ó a(0,0)
    { b0_ = a00 - nc_ / Int{2}; }


    // (row, col)
    std::pair<Int, Int> index(Int x)
    {
	x -= b0_;
	Int row = x / nr_;

	x -= row * nr_;

	Int col = x / nc_;

	return {row, col};

    }

    bool is_valid(Int x) { return x >= b0_; }
    bool is_not_valid(Int x) {return !is_valid(x); }

private:
    Int b0_;	// elemento inicial de la sucesión
    Int nc_;	// número a añadir para pasar de una columna a otra.
    Int nr_;	// número a añadir para pasar de una fila a otra.
		// precondition: nr_ > nc_ (de hecho nr_ > nc_*num_rows)
};


template <uint8_t npin0>
struct Keyboard_ADC0_npin{
    static constexpr uint8_t npin = npin0;
};

// ¿por qué uint16_t? Porque los arefs de not_generic::ADC son uint16_t
template <uint16_t aref00, uint16_t nrow0, uint16_t ncol0>
struct Keyboard_ADC0_voltages{
    static constexpr uint16_t aref0 = aref00;
    static constexpr uint16_t nrow  = nrow0;
    static constexpr uint16_t ncol  = ncol0;
};


/*!
 *  \brief  Keyboard para ser conectado al ADC, tipo 0.
 *
 *  Es una matriz de teclas conectada al ADC. Usa un único pin del AVR.
 *  Es un experimento. Por eso el ADC0, para poder usar versiones diferentes
 *  en el futuro.
 *
 */
template <typename Npin, typename Voltages>
class Keyboard_ADC0{
public:
    static constexpr uint8_t npin = Npin::npin;
    static constexpr uint16_t aref0 = Voltages::aref0;
    static constexpr uint16_t nrow = Voltages::nrow;
    static constexpr uint16_t ncol = Voltages::ncol;
   
    // Al hacer pruebas observo que el potencial inicial varia.
    // Sospecho (???) que es culpa de la temperatura. Al variar la temperatura
    // varía V_BE del transistor y la resistencia del teclado. 
    // De hecho basta con tocar el transistor con los dos dedos para ver cómo
    // al calentarse varía el potencial de salida, variando arefs. Este
    // teclado no funciona en la calle en invierno o en verano (???)
    // Para intentar paliar un poco el efecto de esta variación ajusto el
    // valor de aref0 en init. Si el usuario no pulsa ninguna tecla esto
    // debería de funcionar bien. El problema es ¿qué pasa si el usuario toca
    // una tecla al encender el teclado? El teclado deja de funcionar.
    // TODO: corregir esto. El teclado tiene que funcionar bien
    // independientemente de si el usuario pulsa una tecla o no al encender el
    // aparato. ¿No se puede 'autocalibrar'? ¿que mida aref0, nrow, ncol?
//    void init()
//    {
//	uint16_t arefs = arefs_();
//	seq_.a0(arefs + ncol);
//    }

    
    // Devuelve true si ha encontrado alguna tecla pulsada.
    // No bloquea la ejecución. Devuelve el control inmediatamente después
    // del scan.
    bool scan()
    {
	uint16_t arefs = arefs_();

	if (seq_.is_not_valid(arefs))
	    return false;

	std::tie(row_, col_) = seq_.index(arefs);
	return true;
    }

    // ultima tecla pulsada
    uint8_t row() const {return row_;}
    uint8_t col() const {return col_;}

private:
// last key pressed
    uint8_t row_ = null_rcol;
    uint8_t col_ = null_rcol;

    __Bisequence seq_{aref0, ncol, nrow};

    constexpr static uint8_t null_rcol = 255; // null row/col (= incorrect value)


    static uint16_t arefs_()
    {
	not_generic::ADC::select_pin<npin>();
	not_generic::ADC::start_conversion();
	not_generic::wait_until_conversion_is_complete();
	
	return not_generic::ADC::ADC_in_arefs();
    }
 
};





}// namespace



#endif



