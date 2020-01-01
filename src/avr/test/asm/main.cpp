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

#include <avr/io.h>
#include <util/delay.h>

extern "C" void square_wave();
extern "C" void square_wave_fastest();

extern "C" void square_wave1(uint8_t time, uint8_t num_ciclos);
extern "C" void square_wave2(uint8_t num_ciclos0, uint8_t num_ciclos1);
extern "C" void square_wave_ciclos(uint8_t num_ciclos0, uint8_t num_ciclos1
			    , uint8_t num_pulsos);

int main() 
{
    while(1){
	// square_wave_fastest();
	// square_wave();
	//square_wave1(255, 19);
	//square_wave2(40, 40);
	square_wave_ciclos(1, 1, 10);
	_delay_us(1000);
    }


    return 0;
}


