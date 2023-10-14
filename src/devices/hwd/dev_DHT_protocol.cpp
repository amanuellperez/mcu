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

#include "dev_DHT11.h"

namespace dev{

DHT_protocol::Result
    DHT_protocol::is_pulse_corrupted(const Train_of_pulses& pulse)
{
    if (pulse.polarity() == 0)
	return Result::train_of_pulses_bad_polarity;

    if (pulse.size() != nmax_pulses)
	return Result::train_of_pulses_bad_size;

    return Result::ok;
}


void DHT_protocol::pulse2data(const Train_of_pulses& pulse, uint8_t data[5])
{
    // Datos a leer
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;

//  pulse[0] == pulse de 80us. Como no medimos tiempo, sino ticks, no se cómo
//  confirmar que se esté enviando realmente este pulso.

    // CUIDADO: los valores que contienen los datos son pulse[1..40], pero el
    // array es data[0...4]
    for (uint8_t i = 0; i < 40; ++i){
	data[i / 8] <<= 1;

	if (pulse[i + 1].time_high > pulse[i + 1].time_low)
	    //atd::write_bit<0>::to<1>::in(data[i / 8]); <-- en este caso esto
	    //lía más que aclara @_@
	    data[i / 8] |= 0x01;

    }
// pulse[41] == pulse end. El DHT_protocol mantiene por 50 us low y luego release the
// pin.

}

}// namespace dev

 
