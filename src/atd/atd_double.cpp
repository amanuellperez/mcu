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

#include "atd_double.h"


#include <cmath>

#include "atd_math.h"
#include "atd_string.h"


namespace atd{


std::pair<double, double> modf(double x)
{
    double integer_part{};
    double decimal_part = std::modf(x, &integer_part);

    return std::pair{integer_part, decimal_part};
}


//// No se trata de una función genérica. Sabé que [p0, pe) es un número, y lo que
//// hace es eliminar todos los ceros a la izquierda. Si el número fuera todo
//// ceros devuelve "0" y no "" que sería lo que haría una función genérica.
//// Devuelve [p0, qe) = cadena que contiene el número sin los ceros al final.
//std::pair<char*, char*> __remove_trailing_zeros(char* p0, char* pe)
//{
//    if (pe == p0)
//	return std::pair{p0, pe};
//
//    std::reverse_iterator r0{pe};
//    std::reverse_iterator re{p0};
//
//    auto p = std::find_if(r0, re,
//			 [](auto x){ return x != '0';});
//
//    if (p == re) // todo ceros
//	return std::pair{p0, std::next(p0)};
//
//    else if (p == r0)
//	return std::pair{p0, pe};
//
//    else
//	return std::pair{p0, p.base()};
//}
//
//
//char* mantissa_to_str_with_zeros(const double& x0, char* m0, char* me)
//{
//    int32_t x = static_cast<int32_t>(x0 * ten_to_the<double>(me - m0 + 1));
//    int32_t r{};
//    if ((x % 10) < 5)
//	r = 0;
//    else
//	r = 1;
//    x = x/10 + r; // redondeamos
//
//
//    auto p = std::reverse_iterator{me};
//    for (; p != std::reverse_iterator{m0}; ++p){
//	int y = x % 10;
//	*p = digit_to_char(y);
//	x /= 10;
//    }
//    
//    return p.base();
//}
//
//
//// Convierte el double 'x' en la cadena almacenándolo en [m0, me).
////  Ejemplo: si x = 0.123 y str es de 5 caracteres devuelve str = 12300
////  No añade '\0' al final, cuidado si se imprime.
//// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
//// El número de cifras que se usa es N = size[m0, me) = me - m0
//// Devuelve [p0, pe) = 'x' escrito como cadena de N cifras. En general 
////	m0 <= p0 <= pe <= me
//std::pair<char*, char*> mantissa_to_str(const double& x0, char* m0, char* me)
//{
//    if (m0 == me)
//	return std::pair{m0, me};
//
//    auto p = mantissa_to_str_with_zeros(x0, m0, me);
//
//    return __remove_trailing_zeros(p, me);
//    
//}
//


}// namespace

