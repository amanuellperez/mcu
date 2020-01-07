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

// Conectar un led al pin 14, y un pulsador
// entre resistencias al pin 4

#include "../../avr_pin.h"
#include "../../avr_time.h"

using namespace avr;
constexpr uint8_t num_pin = 14;


void test_pin()
{
    Pin<num_pin> pin;
    pin.as_output();

    while(1){
	pin.write_one();
	_delay_ms(1000);

	pin.write_zero();
	_delay_ms(1000);
    }
}


void test_pin_de_salida()
{
    Output_pin<num_pin> pin;

    while(1){
	pin.write_one();
	_delay_ms(500);

	pin.write_zero();
	_delay_ms(500);
    }
}


int main()
{
    test_pin();
    // test_pin_de_salida();
}


