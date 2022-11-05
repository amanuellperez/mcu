// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include <cstdint>
#include <iostream>
#include <atd_array.h>
#include "pulse.h"

// Un mensaje es un array de pulsos
bool is_NEC_protocol(const atd::CArray_view<Pulse>& pulses);
bool print_NEC_protocol(std::ostream& out, const atd::CArray_view<Pulse>& pulses);


// Devuelve true si a = b +- 20%
// Esto es, si b - 20% <= a <= b + 20%
// (cojo el 20% ya que:
//	1. El clock del avr tiene un 10% de incertidumbre
//	2. Doy un 10% más de margen de error
// )
template <typename Int1, typename Int2>
inline bool is_equal(const Int1& a, const Int2& b)
{ return (b*0.8 <= a and a <= b*1.2); }



