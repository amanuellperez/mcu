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
#include "../mcu_speaker.h"


// Configuración de las conexiones
constexpr uint8_t num_pin_speaker = 15;

using namespace avr;


int main()
{
    auto speaker = mcu::Speaker::nuevo<num_pin_speaker>();

    while(1){
//	sirena01(speaker, 2000, 4000, 10);	// esta es una sirena
//	sirena01(speaker, 2500, 3500, 10);
//	sirena01(speaker, 440, 1000, 5);    // raro. ¿es culpa de mi altavoz?
//	sirena01(speaker, 200, 1000, 1);    // raro. ¿es culpa de mi altavoz?

   // sirena02(speaker, 400, 800, 500);   // suena bien
	// sirena02(speaker, 400, 800, 65);   // parece una alarma
	
//	sirena03(speaker, 400, 800, 500, 1000);

	//sonido02(speaker);
	// alarma01(speaker, 3000, 50);	// esta bien
	// alarma01(speaker, 1000, 50);
	// alarma01(speaker, 200, 50);
//	alarma01(speaker, 1400, 25);	// ¿podría ser una alarma?
	alarma01(speaker, 2000, 100);	// ¿podría ser una alarma?

    }
}



