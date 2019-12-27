// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __AVR_PIN_H__
#define __AVR_PIN_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Clases para manejar pines del AVR
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 24/07/2017 Escrito
 *		    27/01/2019 Reescrito, paso a usar templates.
 *		    09/06/2019 Pin_de_salida<0> y pulso_1us. 
 *
 ****************************************************************************/
#include "avr_cfg.h"
#include "avr_time.h"

namespace avr{
/*!
 *  \brief  Clase para manejar pins
 *
 *  Este es un pin totalmente configurable. Lo podemos cambiar de entrada a
 *  salida y viceversa en medio del código.
 *
 *  Si se quiere una versión más reestringida usar Pin_de_salida/de_entrada
 *
 */
template <uint8_t n>
class Pin{
public:
    // Construcción
    constexpr Pin(){}	

    // No es posible copiar pins (aunque como es una template static no
    // debería de generar ningún problema)
    Pin(const Pin&)		= delete;
    Pin& operator=(const Pin&)	= delete;

    // -------------------
    // Configuramos el pin
    // -------------------
    /// Definimos el pin de salida
    static void de_salida() 
    {
	(*DDR[n]) |= BIT_MASK[n];
    }

    /// Definimos el pin como de entrada con la pull-up resistor
    static void de_entrada_con_pullup() 
    {
	(*DDR[n]) &= ~BIT_MASK[n];	// de entrada
	(*PORT[n]) |= BIT_MASK[n];	// enables pull-up resistor
    }

    /// Definimos el pin como de entrada sin la pull-up resistor
    static void de_entrada_sin_pullup() 
    {
	(*DDR[n]) &= ~BIT_MASK[n];	// de entrada
	(*PORT[n]) &= ~BIT_MASK[n];	// disabel pull-up resistor
    }
    // --------------------
    // Funciones de lectura
    // --------------------
    /// Leemos el valor del pin (0 ó 1)
    static uint8_t read()
    {return (*PIN[n]) & BIT_MASK[n];}

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() {return !read();}

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() {return read();}

    // ----------------------
    // Funciones de escritura
    // ----------------------
    /// Escribimos un 1
    static void write_one()	
    {(*PORT[n]) |= BIT_MASK[n];}

    /// Escribimos un 0
    static void write_zero()	
    {(*PORT[n]) &= ~BIT_MASK[n];}

//    /// Cambiamos el valor del pin (de 0 a 1 ó de 1 a 0).
//    TODO: 27/01/19 - borrarlo si no se usa
//    static void flip_bit() 
//    { (*PORT[n]) ^= BIT_MASK[n]; }

    /// Escribimos en el pin. Un valor distinto de cero es 1.
    static void write(uint8_t x)
    {
	if(x) write_one();
	else  write_zero();
    }
    
//    /// Accedemos a los registros directamente
//    TODO: 27/01/19 - borrarlo si no se usa
//    static constexpr volatile uint8_t* port(uint8_t num_pin) {return impl::PORT[num_pin];}
//    static constexpr volatile uint8_t* ddr(uint8_t num_pin) {return impl::DDR[num_pin];}
//    static constexpr volatile uint8_t* pin(uint8_t num_pin) {return impl::PIN[num_pin];}
//    static constexpr uint8_t bit_mask(uint8_t num_pin) {return impl::BIT_MASK[num_pin];}

    // Funciones de ayuda
    static void pulso_1us()
    {
	write_one();
	wait_us(1);  
	write_zero();
    }

    static void pulso_negativo_1us()
    {
	write_zero();
	wait_us(1);  
	write_one();
    }
};




/*!
 *  \brief  Creamos un pin que es de salida
 *  
 *  Observar que para que sea un pin de salida es necesario crear un objeto
 *  de este tipo.
 */
template<uint8_t n>
class Pin_de_salida{
public:
   constexpr Pin_de_salida()
   { Pin<n>::de_salida(); }

    Pin_de_salida& operator=(const Pin_de_salida&)	= delete;

    constexpr static void write_one()	{Pin<n>::write_one();}
    constexpr static void write_zero()	{Pin<n>::write_zero();}
    constexpr static void write(uint8_t x) {Pin<n>::write(x);}
    // static void flip_bit()	{Pin<n>::flip_bit();}


    // Funciones de ayuda
    constexpr static void pulso_1us() { Pin<n>::pulso_1us(); }
    constexpr static void pulso_negativo_1us() {Pin<n>::pulso_negativo_1us();}
};


template<>
class Pin_de_salida<0>{
public:
    constexpr Pin_de_salida(){}
    Pin_de_salida& operator=(const Pin_de_salida&)	= delete;

    constexpr static void write_one()	{}
    constexpr static void write_zero()	{}

    // Funciones de ayuda
    constexpr static void pulso_1us() {}
    constexpr static void pulso_negativo_1us() {}
};



/*!
 *  \brief  Creamos un pin de entrada con pull-up
 *
 */
template<uint8_t n>
class Pin_de_entrada_con_pullup{
public:
    Pin_de_entrada_con_pullup()
    {Pin<n>::de_entrada_con_pullup();}

    Pin_de_entrada_con_pullup& operator=(const Pin_de_entrada_con_pullup&)	= delete;

    /// Leemos el valor del pin (0 ó 1)
    static uint8_t read()
    {return Pin<n>::read();}

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() {return Pin<n>::is_zero();}

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() {return Pin<n>::is_one();}
};


/*!
 *  \brief  Creamos un pin de entrada sin pull-up
 *
 */
template<uint8_t n>
class Pin_de_entrada_sin_pullup{
public:
    Pin_de_entrada_sin_pullup()
    {Pin<n>::de_entrada_sin_pullup();}

    Pin_de_entrada_sin_pullup& operator=(const Pin_de_entrada_sin_pullup&)	= delete;

    /// Leemos el valor del pin (0 ó 1)
    static uint8_t read()
    {return Pin<n>::read();}

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() {return Pin<n>::is_zero();}

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() {return Pin<n>::is_one();}
};


}// namespace avr


#endif

