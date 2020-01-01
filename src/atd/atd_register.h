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

#ifndef __ATD_REGISTER_H__
#define __ATD_REGISTER_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Syntax sugar para manejar bits
 *
 *   - COMENTARIOS: Mirar atd_bit, puede que sea un poco más eficiente (todo
 *   depende de cómo optimice el compilador).
 *
 *   - HISTORIA:
 *           alp  - 10/02/2019 Escrito
 *
 ****************************************************************************/

namespace atd{
/*!
 *  \brief  Manejo de los registros del microcontrolador
 *
 *  En lugar de usar macros o escribir directamente:
 *	DDRB = 0x01;
 *
 *  escribimos
 *	Register{DDRB}.write_one_bit<1>();  
 *
 *  Al comparar el código en ensamblador que genera no observo diferencia.
 *
 *  ¿Qué gano con esto? Facilidad de lectura de código.
 */
template <typename Int>
class Register{
public:
    constexpr Register(Int& reg) noexcept
	:reg_{reg}{}


    /// Escribimos un 1 en las posiciones indicadas.
    /// Ejemplo: set_bit<1,3>(x); // escribe un 1 en los bits 1 y 3 de x
    template<int... pos>
    constexpr void write_one_bit() noexcept
    {
	using expand = int[];
	void(expand{0,
		((reg_ |= (Int{1} << pos)), 0)...});
    }

    /// Escribimos un 0 en las posiciones indicadas.
    /// Ejemplo: reset_bit<1,3>(x); // escribe un 0 en los bits 1 y 3 de x
    template<int... pos>
    constexpr void write_zero_bit() noexcept
    {
	using expand = int[];
	void(expand{0,
		((reg_ &= ~(Int{1} << pos)), 0)...});

    }

    /// Cambiamos los bits de las posiciones indicadas, de 0 -> 1 y de 1 -> 0.
    /// Ejemplo: flip<1,3>(x); // cambia los bits 1 y 3 de x.
    template<int... pos>
    constexpr void flip_bit() noexcept
    {
	using expand = int[];
	void(expand{0,
		((reg_ ^= (Int{1} << pos)), 0)...});
    }

    /// Devuelve el bit (0 ó 1) que se encuentra en la posición pos del número x
    /// Ejemplo: bit<3>();   // devuelve el bit 3 del registro reg
    template<int pos>
    constexpr int bit() noexcept
    // TODO: puede que sea más eficiente:
    //		    return reg_ & (Int{1} << pos);
    //	    ya que en tiempo de compilación conocemos fijo Int{1} << pos
    {return ((reg_ >> pos) & 0x01);}


    /// Devuelve true si el bit p de x es 0, y false si es 1.
    /// Ejemplo:
    ///	    int x = 10u;
    ///	    cout << Register{x}.is_zero_bit<3>();//¿es el bit 3 de x, cero?
    template<int p>
    constexpr bool is_zero_bit() noexcept
    {return bit<p>() == 0;}


    /// Devuelve true si el bit p de x es 1, y false si es 0.
    /// Ejemplo:
    ///	    int x = 10u;
    ///	    cout << Bit::is_one<3>();	// ¿es el bit 3 de x, uno?
    template<int p>
    constexpr bool is_one_bit() noexcept
    {return !is_zero_bit<p>();}

    // CUIDADO con estas dos funciones. 
    // Las funciones wait solo tienen sentido si le pasamos como parametro un
    // registro del controlador que se actualizara por su cuenta. Ya que x
    // se pasa como parámetro, si no es volatile dentro de la función no
    // cambia, quedándose colgada el micro!!!
    /// Espera hasta que el bit p de x es 1
    template<int p>
    constexpr void wait_until_is_one_bit() noexcept
    {
	while(is_zero_bit<p>()){;}
    }

    /// Espera hasta que el bit p de x es 1
    template<int p>
    constexpr void wait_until_is_zero_bit() noexcept
    {
	while(is_one_bit<p>()){;}
    }

private:
    Int& reg_;
};


}// namespace

#endif

