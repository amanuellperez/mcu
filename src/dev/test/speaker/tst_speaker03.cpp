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

// Probamos el speaker. Conectarlo al pin indicado.
// Con este programa probamos las frecuencias que soy capaz de oir.
// Conectar el cable FTDI. Si se pulsa + se incrementa la frecuencia, si -
// se decrementa.
#include "../mcu_speaker.h"
#include "../avr_USART.h"

// Configuración de las conexiones
constexpr uint8_t num_pin_speaker = 15;

using namespace avr;



int main()
{
    auto speaker = Speaker::nuevo<num_pin_speaker>();
    USART::init();
    
    uint16_t frecuencia = 20;	// empezamos con 20 herzios
    uint16_t incr = 10;		// vamos incrementando de incr en incr

    USART::print("\r\n---------\r\n");
    USART::print("EMPEZAMOS\r\n");
    USART::print("---------\r\n\r\n");
    while(1){
	auto nota = Speaker::nota(frecuencia);

	USART::print("frecuencia = ");
	USART::println(frecuencia);

	speaker.play_note(nota, 200);

	USART::print("Pulsa una tecla para continuar (+, -, * ó /)\r\n");
	char c = USART::receive();

	if (c == '+')
	    frecuencia += incr;
	else if (c == '-')
	    frecuencia -= incr;
	else if (c == '*')
	    frecuencia += 10*incr;
	else if (c == '/')
	    frecuencia -= 10*incr;

    }

}



