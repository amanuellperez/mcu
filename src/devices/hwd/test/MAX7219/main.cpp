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

#include "../../avr_matrix_led_max7219.h"


// Probamos una matriz de leds conectadas al MAX7219.
// En principio solo conecto una de estas matrices.
using namespace avr;


int main() 
{
    // Message to be displayed. No lo almaceno en EEPROM ya que quiero poder
    // modificarlo dinámicamente el mensaje que muestro (una pantalla que
    // muestra siempre lo mismo no es muy interesante).
    auto msg = "Good morning";

    Matrix_led::MAX7219<1> matrix;

    while (1) 
    {
	matrix.show(msg);
    }                                                 
}


