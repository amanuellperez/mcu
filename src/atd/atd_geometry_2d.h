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

#ifndef __ATD_GEOMETRY_2D_H__
#define __ATD_GEOMETRY_2D_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases para manejar conceptos geométricos en 2D	
 *
 * HISTORIA
 *    Manuel Perez
 *    16/06/2024 Point
 *
 ****************************************************************************/
#include <ostream>

#include "atd_concepts.h"

namespace atd{
// (RRR) Lo meto dentro del namespace geo2d ya que hay conceptos repetidos en
//       2D y 3D, como Point, cuya implementación es diferente (el punto 3D
//       tiene la coordenada z que no la tiene el 2D)
namespace geo2d{

// Point
// -----
// CUIDADO: un Point no tiene definida ningún tipo de estructura algebráica.
// En matemáticas no está definida la suma de puntos (sino la de vectores).
// Si se necesita el concepto de Vector revisar `alp`, allí hay cosas hechas.
template <Type::Numeric Int>
struct Point{
    Int x, y;
};


template <Type::Numeric Int>
std::ostream& operator<<(std::ostream& out, const Point<Int>& p)
{ return out << '(' << p.x << ", " << p.y << ')'; }

}// namespace geo2d
}// namespace atd


#endif


