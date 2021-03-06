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

// Este es un cliente de UART. Lo ejecuto en linux para comunicarme con el
// AVR vía USB-UART.
//
// Este programa va con el programa tst_leds1.cpp


#include "alp_usb_usart.h"

//#include <alp_time.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <termios.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <strings.h>

#include <iostream>
using namespace std;
using namespace alp;


int main()
{
try{
    Termios usb{"/dev/ttyUSB0"};
    
    configura_por_defecto_AVR_UART(usb);

    unsigned char c = 'f';

    cin >> c;
    while(c != 'q'){

	cout << "Enviado " << c << endl;

	usb << c;

	if (usb.read(c) > 0)
	    cout << "Recibido: " << c << endl;
	else
	    cout << "Error\n" << endl;

	cin >> c;
    }
}catch(Excepcion& e)
{
    std::cerr  << e.what() << std::endl;
    return -1;
}

return 0;

}

