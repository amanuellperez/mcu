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

#ifndef __MCU_CONCEPTS_H__
#define __MCU_CONCEPTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Concepts asociados con el hardware usado en microcontroladores.
 *
 *  La implementación por capas que estoy haciendo se basa en pasar como
 *  parámetros de template Micros, Timers... A día de hoy no estoy validando
 *  de ninguna forma que lo pasado sea realmente un Micro.
 *
 *  Ejemplo:
 *	    template <typename Micro, ...>
 *	    class Driver{ ... };
 *
 *	    // Si instancio Driver con algo que no es un Micro 
 *	    using My_driver = Driver<No_soy_un_micro,...>;
 *
 *	    // el compilador dará un error clásico. Quedaría mejor:
 *	    template <mcu::Type::Micro Micro, ...>
 *	    class Driver{...};
 *
 *
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    11/12/2022 Unsafe_device
 *    22/06/2024 Escrito
 *
 ****************************************************************************/
namespace mcu{

// Timer1 tiene un contador de 16 bits que para acceder hay que bloquear las
// interrupciones. No hacerlo podría generar errores.
//
// Timer0 al tener un contador de 8 bits no padece de ese problema.
//
// Eso quiere decir que mientras el interfaz de Timer0 es `safe` en el sentido
// de que no es necesario bloquear las interrupciones, Timer1 suministra
// funciones unsafe_ para recordar que hay que hacer algo más si se quiere
// usar esa función sin problemas. 
//
// La implementación de Miniclock tiene que saber si el dispositivo es safe
// (para llamar funciones sin prefijo) o unsafe (para llamar las mismas
// funciones con prefijo safe_).
template <typename T>
concept Unsafe_device = requires(T a){
    {T::is_unsafe};	// prueba: usemos init como marcador
};

template <typename T>
concept Safe_device = (!Unsafe_device<T>);


}// namespace

#endif


