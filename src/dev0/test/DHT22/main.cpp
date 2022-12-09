// Copyright (C) 2019-2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../mcu_DHT22.h"
#include "../../avr_UART.h"
#include "../../avr_time.h"

// No consigo programar bien este sensor.
// Si uso mi clase Pin introduce demasiado ineficiencia. Hay que programarlo
// directamente a bajo nivel, con lo que hay que saber de antemano a qué pin
// se conecta, no pudiendo reutilizar el código.
//
// Para depurar uso el pin 13. El data del sensor queda conectado al pin 26.
// Tal como ahora está programado funciona de vez en cuando (???)
// Usar el osciloscopio para ver la señal.



using namespace avr;


void lee(DHT22& sensor)
{
    auto usart = UART::as_a_stream();

    switch(sensor.read()){
	case DHT22::Error::NONE:
	    usart << "T = " << sensor.T_parte_entera
		  << "." << sensor.T_parte_decimal
		  << "; RH(%) = " << sensor.RH_parte_entera
		  << "." << sensor.RH_parte_decimal << "\n\r";
	    break;

	case DHT22::Error::BUS_HUNG:
	    usart << "ERROR: BUS_HUNG\n\r";
	    break;

	case DHT22::Error::NOT_PRESENT:
	    usart << "ERROR: NOT_PRESENT\r\n";
	    break;

	case DHT22::Error::ACK_TOO_LONG:
	    usart << "ERROR: ACK_TOO_LONG\r\n";
	    break;

	case DHT22::Error::SYNC_TIMEOUT:
	    usart << "ERROR: SYNC_TIMEOUT\r\n";
	    break;

	case DHT22::Error::DATA_TIMEOUT:
	    usart << "ERROR: DATA_TIMEOUT\r\n";
	    break;

	case DHT22::Error::CHECKSUM:
	    usart << "ERROR: CHECKSUM\r\n";
	    break;
    }
}


int main() 
{
    auto usart = UART::init();

    DHT22 sensor1{12};
    DHT22 sensor2{13};
    DHT22 sensor3{14};
   
    usart << "\r\n\nIniciando DTH22\r\n";
    usart << "---------------\r\n";
    while (1) {
	usart << "sensor pin 12: ";
	lee(sensor1);

	usart << "sensor pin 13: ";
	lee(sensor2);

	usart << "sensor pin 14: ";
	lee(sensor3);

	usart << "-------------\n\r\n";

	wait_ms(2000u);	// minimo 2 segundos entre medida y medida
    
  } // while(1)

}
