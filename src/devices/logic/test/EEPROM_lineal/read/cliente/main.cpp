// Copyright (C) 2019-2020 Manuel Perez 
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

// Este es un cliente de UART. Lo ejecuto en linux para comunicarme con el
// AVR vía USB-UART.
//
// Este programa le pide al usart que le envie una serie de números y lo 
// guardamos en un fichero.


#include <alp_termios.h>
#include <alp_exception.h>

#include <iostream>
using namespace std;
using namespace alp;

constexpr uint8_t ERR_READ_ERROR = -1;	// ha habido algún error al intentar leer
constexpr uint8_t ERR_READ_BAD_ID = -2;	// el id no es el correcto

int main()
{
try{
    Termios usb{"/dev/ttyUSB0"};
    
    configura_por_defecto_AVR_UART(usb);

    char res;
    
    usb << 'w';

    const char id[] = "Tlog";

    for (uint8_t i = 0; i < sizeof(id)-1; ++i){
	switch(usb.read(res)){
	    case -1:
		cerr << "Error al leer\n";
		return ERR_READ_ERROR;

	    case 0:
		cerr << "Timer expires\n";
		return ERR_READ_BAD_ID;
	}
	
	std::cout << res << std::flush;

//	cout << '[' << res << ']' << flush;
//	cout << " =? " << id[i] << '\n' << flush;

	if (id[i] != res){
	    cerr << "Id erroneo\n";
	    return ERR_READ_BAD_ID;
	}
    }

    cout << "\n\n";

    cout << "Id correcto\n\n";


    while(usb.read(res) > 0){
	if (res == ASCII::EOT){
	    cout << "\nRecibido EOT\n";
	    return 0;
	}
	cout << res;
	}



}catch(Excepcion& e)
{
    std::cerr  << e.what() << std::endl;
    return -1;
}

return 0;

}

