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

#ifndef __MCU_STD_CONFIG_H__
#define __MCU_STD_CONFIG_H__
/****************************************************************************
 *
 *   - DESCRIPCION: En principio no quiero tener que distinguir entre
 *	si uso la STL estandard o la mía. Por ello, voy a meter todo este
 *	código, que tiene que ser estandar dentro de std.
 *
 *	Problema:
 *	1.- Para probarlo entra en conflicto mi std con el std estandar.
 *	Necesito meterlo en un espacio de nombres diferente (mtd).
 *	2.- ¿Qué pasa si en el futuro se ve que es mala idea usar std?
 *	Cambiando esta macro, cambia todo el código (aunque no el código del
 *	cliente).
 *
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 16/07/2019 v0.0
 *
 ****************************************************************************/
#if defined __AVR__ || __AVR
#   define STD std

#elif defined(__GNUC__) || defined(__clang__)
#   define STD mtd

#else
# error "Compilador desconocido"
#endif


#endif


