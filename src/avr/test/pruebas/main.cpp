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
#include "../../avr_dpin.h"
#include "../../avr_time.h"

using namespace avr;
constexpr uint8_t num_pin = 14;

template <typename Int>
class Register{
public:
    constexpr Register(Int& reg):reg_{reg}{}

    /// Escribimos un 1 en las posiciones indicadas.
    /// Ejemplo: write_one<1,3>(x); // escribe un 1 en los bits 1 y 3 de x
    template<int... pos>
    constexpr void set_bit()
    {
	using expand = int[];
	void(expand{0,
		((reg_ |= (Int{1} << pos)), 0)...});
    }

    /// Escribimos un 0 en las posiciones indicadas.
    /// Ejemplo: write_zero<1,3>(x); // escribe un 0 en los bits 1 y 3 de x
    template<int... pos>
    constexpr void reset_bit()
    {
	using expand = int[];
	void(expand{0,
		((reg_ &= ~(Int{1} << pos)), 0)...});

    }


private:
    Int& reg_;
};




int main()
{
    Register{DDRB}.set_bit<1>();
    
    while (1){
	Register{PORTB}.set_bit<1>();
	_delay_ms(500);

	Register{PORTB}.reset_bit<1>();
	_delay_ms(500);

    }
//    DDRB = 0x01;
//
//    while (1){
//	PORTB = 0x01;
//	_delay_ms (500);
//
//	PORTB = 0x00;
//	_delay_ms (500);
//    }
}


