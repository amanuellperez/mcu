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

#include "../mcu_base.h"
#include "../avr_USART.h"

constexpr uint8_t pin_pulsador = 15;


using namespace avr;

int main()
{
    Pulsador pulsador{pin_pulsador};

    USART::init();

    auto usart = USART::as_a_stream();

    usart   << "Probemos el bouncing del pulsador\r\n"
	    << "Pulsa el pulsador y se incrementará el número\r\n";

    uint8_t n = 0u;
    bool presionado = false;

    while(1){
	usart << n << "\r\n";
	
	if(!presionado){
	    if(pulsador.esta_presionado()){
		presionado = true;
		++n;
	    }
	}

	if(presionado and !pulsador.esta_presionado())
	    presionado = false;
	
    }
}

