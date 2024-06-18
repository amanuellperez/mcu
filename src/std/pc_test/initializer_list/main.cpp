// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../std_initializer_list.h"

// TODO: ¿cómo probar esta clase usando gcc? NO COMPILA EN
// mtd::initializer_list ya que el compilador sabe quién es
// std::initializer_list!!! Es una clase especial para el compilador.

template <typename T>
int f(std::initializer_list<T> args)
{
    int s = 0;
    for (auto x: args)
	s += x;
    return s;
}



int main()
{
    int x = 0;
    while(1){
	x+= f({1,2,3});
    }
}





