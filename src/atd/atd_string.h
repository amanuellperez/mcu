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

#ifndef __ATD_STRING_H__
#define __ATD_STRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos para manejar cadenas.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    24/07/2021 to_int, to_char
 *    29/07/2021 int_to_string
 *    05/11/2022 binary_char_to
 *
 ****************************************************************************/

namespace atd{

/// Convierte el dígito c de char a int.
/// digito = del 0 al 9
template <typename Int>
inline Int digit_to_(char c) {return c - '0';}

/// Convierte el char c de int a char.
/// digito = del 0 al 9
template <typename Int>
inline char digit_to_char(Int x) {return x + '0';}


/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
/// Precondición: x > 0
/// Es el equivalente a sprintf sin overflow.
template <typename It, typename Int>
It __posint_to_string(Int x, It p0, It pe)
{
    It pc = pe;

    while (x != 0 and pc != p0){
	Int r = x % 10;
	x = (x -  r) / 10;
	
	--pc;
	*pc = digit_to_char(r);
    }

    return pc;
}



/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
///
/// Si Int es signed esta función falla para el caso en que valga
/// std::numeric_limits::min() ya que falla el operador -x para este caso.
/// Es el equivalente a sprintf, salvo que esta función no da overflow (cosa
/// que sprintf sí que puede generar).
// Le doy visibilidad a una función de implementación de std no estandar.
// Observar el convenio de los argumentos: si llamo a la función "to"
// estamos haciendo x --> [p0, pe), luego el orden tiene que ser primero x y
// luego [p0, pe).
// Con todo sería mejor: int(x).to_string(p0, pe); 
// o mejor:		     x .to_string(p0, pe);
template <typename It, typename Int>
It int_to_string(Int x, It p0, It pe)
{
    if (p0 == pe)
	return p0;

    It pc = pe; // = ultima posición donde insertamos una cifra
    
    if (x == 0){
	--pc;
	*pc = '0';
	return pc;
    }

    bool negativo  = false;
    if (x < 0){
	negativo = true;
	x = -x;	// Esta operación falla si x == std::numeric_limits::min()!!!
    }

    pc = __posint_to_string(x, p0, pe);

    if (negativo and pc != p0){
	--pc;
	*pc = '-';
    }

    return pc;
}


// Convierte un entero escrito en binario como cadena de C en entero.
// Ejemplo: *str = "10101010" ==> return = uint8_t x = 0b10101010;
// Observar que no le pasamos el "0b" en la cadena. DUDA: ¿que admita 0b?
//
// Precondition: *str es una cadena de C de 0 y 1.
template <typename Int>
Int binary_char_to(const char* str)
{
    Int res{0};

    for (; *str != '\0'; ++str){
	res *= Int{2};

	if (*str == '1')
	    res += Int{1};
    }

    return res;
}



}// namespace

#endif


