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

#ifndef __DEV_KEYBOARD_H__
#define __DEV_KEYBOARD_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Teclado vulgar y corriente.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    07/01/2020 v0.0 Introduzco codificación de caracteres.
 *    20/02/2021      scan 
 *
 ****************************************************************************/
#include "dev_push_button.h"

#include <atd_static.h>

namespace dev{
/*!
\brief  Códificación de caracteres usadas en las Basic_keyboards
  
see: Basic_keyboard3 for rationale.
 */
struct Basic_keyboard_code{
    static constexpr uint8_t enter = '\n';
    static constexpr uint8_t up    = 200;
    static constexpr uint8_t down  = 201;
    static constexpr uint8_t right = 202;
    static constexpr uint8_t left  = 203;

    static constexpr uint8_t null  = 0;
};


// syntactic sugar
namespace Key_codes{
    static constexpr uint8_t NO_KEY = Basic_keyboard_code::null;
    static constexpr uint8_t OK_KEY = Basic_keyboard_code::enter;
    static constexpr uint8_t ENTER_KEY = OK_KEY;
    static constexpr uint8_t UP_KEY = Basic_keyboard_code::up;
    static constexpr uint8_t DOWN_KEY = Basic_keyboard_code::down;
}



template <uint8_t... args>
struct Keyboard_pins : public atd::static_array<uint8_t, args...>{ };

template <uint8_t... args>
struct Keyboard_codes : public atd::static_array<uint8_t, args...>{ };


/*!

\brief  Códificación del teclado de 3 teclas.

 */
// Observar que lo que realmente estamos implementando es un 
// static_map<uint8_t, uint8_t>!!!
template <typename Pins,
	  typename Codes>
struct Keyboard_code{
    static_assert(Pins::size == Codes::size, "Tienen que ser del mismo tamaño");

    // interfaz genérico necesario para poder hacer el init en Basic_keyboard.
    static constexpr uint8_t num_keys = Pins::size;
    // Se supone que en C++17 no es necesario inicializar un static constexpr
    // cuando tiene un constructor por defecto, pero parece ser que la versión
    // de compilador que uso (g++9.2.0) no lo debe de tener implementado.
    static constexpr Pins pin   = Pins{};
    static constexpr Codes code = Codes{};

    template <uint8_t c, uint8_t i = num_keys - 1>
    static constexpr uint8_t code2pin(){
	if constexpr (i != 255){
	    if constexpr (code[i] == c)
		return pin[i];
	    else
		return code2pin<c, static_cast<uint8_t>(i - 1)>();
	}
	else {
	    static_assert(atd::always_false_v<int>,
				    "El código no es un código válido");
	    return 0; // para quitar warning compilador
	}
    }

};



/*!
\brief  Teclado con n pulsadores.

  Esta es la primera versión (así que es posible que haya que reescribirla
  completamente en el futuro).


TIPOS

  En principio da la impresión de que puedo tener los siguientes tipos de
  teclados:

  * Básico:
  
    Teclado con pocos pulsadores. Los que de momento me parecen útiles son:
    - de 3 botones: up, down, ok.
    - de 5 botones: los 3 anteriores más left and right.

    En ambos casos podemos conectar directamente los pulsadores al avr,
    usando 3 ó 5 pines codificados en one-hot, o usar un decoder y convertir
    a binario los botones pulsados.


  * Matrix:

    Cuando necesitamos más pulsadores lo más práctico es conectarlos en
    matriz. En este caso el valor leído lo meteremos en un char (será un
    teclado como los de los PCs).
	

CODIFICACIÓN

  Para poder programar de forma genérica, necesito desvincular los pines a
  los que conectamos el teclado del significado que le atribuimos a la
  tecla. 

  A fin de cuentas esto es lo que hacemos en un teclado normal: al pulsar la
  tecla 'a' el código que devuelve el teclado es char c = 'a'; El usuario de
  la clase no sabe para nada en qué pin está conectada la tecla 'a'.

  ¿Cómo consigo esto? Con la codificación: al definir el teclado pasamos el
  código "ascii" que corresponde cada pulsador. 

	Ejemplo:

	pin pulsador	| char usado en el programa para referirnos a él
	----------------+-----------------------------------------------
	22		|	'+'
	23		|	'-'
	24		|	'\n'
  
	En el programa, para ver si se ha pulsado la tecla '\n' escribiremos
	el siguiente código:

		if (keyboard.key<'\n'>().is_pressed()) ...
  
	ó directamente:

		if (Keyboard::key<'\n'>().is_pressed()) ...
  
	La primera versión me gusta para aplicaciones finales, donde la
	aplicación tiene un teclado, un LCD, ... Además la ventaja de definir
	'keyboard' en la primera opción es que el constructor inicializa el
	teclado de la forma adecuada. Si no se crea un keyboard hay que llamar
	a la función Keyboard::init() para inicializarlo adecuadamente.

	La segunda es mejor para bibliotecas.


PROBLEMA CON LA CODIFICACIÓN

    Si la tecla significa "enter" lo lógico es codificarla como '\n', pero 
    ¿cómo codificar "up" and "down"? No hay códigos ascii y parece ser que hay 
    un montón de "asciis extendidos". 

    Solución 1: 
	crear mis propios códigos. Lo implemento como Basic_keyboard_code.
	De esta forma las funciones de biblioteca saben qué teclas tiene el 
	teclado (up, down and ok) sin necesidad de saber los pines donde están 
	conectados.

 */
template <typename Pins, typename Codes>
struct Basic_keyboard{
    using Code = Keyboard_code<Pins, Codes>;

    constexpr Basic_keyboard() {init();}

//  Se limita a hacer:
//  for(uint8_t i = 0; i < Code::num_keys; ++i)
//	key<i>().init();
    template <uint8_t i = Code::num_keys - 1>
    static constexpr void init()
    {
	Push_button<Code::pin[i]>::init();

	if constexpr (i >= 1)
	    init<i-1>();
    }

    template <uint8_t c>
    static constexpr auto key(){
	return Push_button<Code::template code2pin<c>()>{};
    }

    /// Scanea el teclado devolviendo el caracter asociado a la tecla pulsada.
    /// En caso de que no haya ninguna tecla pulsada devuelve NO_KEY.
    /// No bloquea la ejecución del programa.
    // for (uint8_t i = 0; i < Code::num_keys; ++i)
    //     if (key<i>().is_pressed())
    //		    return code[i];
    //
    //	return null;
    template <uint8_t i = Code::num_keys - 1>
    static constexpr uint8_t scan()
    {
	if (Push_button<Code::pin[i]>::is_pressed())
	    return Code::code[i];

	if constexpr (i >= 1)
            return scan<i-1>();

	return Key_codes::NO_KEY;
    }
};




}// namespace

#endif
