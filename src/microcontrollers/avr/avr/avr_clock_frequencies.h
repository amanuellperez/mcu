// Copyright (C) 2024 Manuel Perez 
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

#ifndef __AVR_CLOCK_FREQUENCIES_H__
#define __AVR_CLOCK_FREQUENCIES_H__

/****************************************************************************
 *
 * DESCRIPCION
 *	Frequencias que usa el avr	
 *
 * COMENTARIOS
 *    No quiero usar el preprocesador para elegir si la aplicación quiere usar
 *    F_CPU fija o no. Por ello impongo la siguiente disciplina:
 *
 *    1) Se tiene que definir siempre explícitamente CLK_MAIN y CLK_PER.
 *    2) Cuando el usuario quiera que se calculen dinámicamente pasará un
 *    valor de 0. Si pasa un valor diferente de 0, se entenderá que ese es el
 *    valor constante que tendrá durante toda la aplicación.
 *
 * HISTORIA
 *    Manuel Perez
 *    01/11/2024 Primer intento
 *
 ****************************************************************************/

namespace avr_{

inline constexpr uint32_t f_cpu()
{ return F_CPU; }


// frecuencia en Hz a la que funciona el reloj conectado al avr
inline constexpr uint32_t clk_main()
{ 
    if constexpr (CLK_MAIN != 0)
	return CLK_MAIN; 

 
    else {
	static_assert(true, "TODO: mirar los registros correspondientes para averiguar CLK_MAIN");
	// Hay que mirar:
	//  1) a qué reloj está conectado el avr (OSCM20, ...)
	//  2) Si es OSCM20 ¿a 16MHz o 20MHz? <-- esto es en un fuse, luego
	//  se necesita otra variable para el makefile (eso solo sería si se
	//  va a cambiar dinámicamente el reloj externo por interno y
	//  viceversa). Como de momento no tengo ni idea de cuándo puede ser
	//  útil eso, esta parte no implementarla hasta saber cómo se va a
	//  usar.
    }
}


// frecuencia en Hz a la que funcionan los perifericos
inline constexpr uint32_t clk_per()
{ 
    if constexpr (CLK_PER != 0){
	static_assert(F_CPU == CLK_PER);

	return CLK_PER; 
    }

    else {
	static_assert(true, "TODO");
    }
}


}// avr_

#endif
