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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.
// Sacamos la salida por un LCD
#include "main.h"
#include "../interface.h"
#include "../buffer.h"

Main::Main():keyboard_{key_code}
{
// init data

// init_hardware():
    init_lcd();
    init_keyboard();

}


// En este caso voy a gestionar el LCD directamente
void Main::init_lcd()
{
    lcd_.terminal().stop_brcorner(true);// I'm not going to use it as a terminal
    lcd_.terminal().nowrap(); 
}


void Main::run()
{
    while(1){
	Interface interface{lcd_, keyboard_, Abb2str{}};
	interface.getline(buffer_);
	lcd_.cursor_pos(0,1);
	lcd_ << "Buffer = ";
	print(lcd_, buffer_);
	mcu::Micro::wait_ms(1000);
    }
}

         

int main()
{
    Main app;
    app.run();
}
