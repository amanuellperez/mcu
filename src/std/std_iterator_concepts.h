// Copyright (C) 2023 Manuel Perez 
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
#ifndef __MCU_STD_ITERATOR_CONCEPTS_H__
#define __MCU_STD_ITERATOR_CONCEPTS_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Definición de los concepts de los iterators
 *
 *  NOTAS
 *	Escribir todos los concepts lleva bastante tiempo, así que los voy a
 *	ir definiendo poco a poco. Primero los definiré de la forma más básica
 *	para poder usarlos y poco a poco los iré completándolo.
 *
 *  HISTORIA
 *    Manuel Perez
 *	22/08/2023: forward_iterator (v-1)
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

// versión del estandar
//template <typename It>
//concept forward_iterator = 
//    input_iterator<It> and
//    derived_from<???<It>, forward_iterator_tag> and
//    incrementable<It> and
//    sentinel_for<It, It>;

// Mi miniversión
template <typename It>
concept forward_iterator = 
    requires(It p){
	++p; p++;
	*p;
    };

}// namespace

#endif



