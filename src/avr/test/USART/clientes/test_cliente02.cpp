// Copyright (C) 2019-2020 A.Manuel L.Perez
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

// Este es un cliente de USART. Lo ejecuto en linux para comunicarme con el
// AVR vía USB-UART.
//
// Este programa va con el programa test_servidor02.cpp
// 
// Pruebo a ver que las funciones de escritura-lectura funcionen.


#include "alp_usb_usart.h"


#include <iostream>
using namespace std;
using namespace alp;


int main()
{
try{
    Termios usb{"/dev/ttyUSB0"};
    
    configura_por_defecto_AVR_USART(usb);

    unsigned char c[] = "abcdefghijklmnñopqrstuwxyz0123456789";

    unsigned char res;
    for (uint8_t i = 0; i < sizeof(c); ++i){
	usb << c[i];
	if (usb.read(res) != -1)
	    cout << res << flush;
	else 
	    cout << "error en " << c[i] << "\n" << flush;
    }

    cout << "\n\n";

}catch(Excepcion& e)
{
    std::cerr  << e.what() << std::endl;
    return -1;
}

return 0;

}

