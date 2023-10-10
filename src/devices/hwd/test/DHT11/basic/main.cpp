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

#include <atd_bit.h>
#include <avr_atmega.h>

// Micro
// -----
namespace mcu = atmega; 
using Micro   = mcu::Micro;

// pins
constexpr uint8_t sensor_pin = 15;


// Hwd Devices
// -----------
//using Sensor = dev::DS18B20_basic<Micro, One_wire>;
struct Protocol{
//    using Micro = 
    using Pin = Micro::Pin<sensor_pin>;
    static void pin_write_zero();
    static void pin_release_the_bus(); // = pin_write_one()
    static bool pin_is_one();
    static bool pin_is_zero() {return !pin_is_one();}

};

inline void Protocol::pin_release_the_bus()
{
//    using Pin = typename Cfg::Pin;

    Pin::as_input_without_pullup(); // release the bus
}


inline void Protocol::pin_write_zero()
{
//    using Pin = typename Cfg::Pin;

    Pin::write_zero();
    Pin::as_output();
}

inline bool Protocol::pin_is_one()
{
//   using Pin = typename Cfg::Pin;

    return Pin::is_one();
}


// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}

bool read()
{
    mcu::UART_iostream uart;
    // Datos a leer
    uint8_t data[5];
    data[0] = 0xFF;
    data[1] = 0xFF;
    data[2] = 0xFF;
    data[3] = 0xFF;
    data[4] = 0xFF;

    // Start signal: at least 18 ms
    Protocol::pin_write_zero();
    Micro::wait_ms(20);
    Protocol::pin_release_the_bus();


    // MENTIRA: wait for 20-40 us, and the DHT pull-low for 80 us
    // REALIDAD: al medir el DHT11 que tengo espera 10-11 us!!!
    // DHT ACK: pulso de 80 us:
    // El DHT mantiene LOW por 80 us. El intervalo más pequeño es
    // [20, 100], mientras que el más grande es [40, 120]. La parte que se
    // solapa de estos intervalos es [40, 100] ==> a los 70 us (punto medio)
    // tiene que estar LOW.
    // Lo que voy a considerar es que la mitad del LOW_80us empieza justo en
    // estos 70us despues. Esto da un margen de error de +-10 us (por culpa de
    // los 20-40us).
    Micro::wait_us(30 + 40);
    if (Protocol::pin_is_one()){
	uart << "Can't read ACK\n";
	return false;
    }

    // Esperamos a que envíe todo el pulso de 80 us que queda (40 us en LOW y
    // 80 us en HIGH)
    Micro::wait_us(40 + 80 - 10);

    // Aquí empieza el DHT a enviar la respuesta


    // Leemos el primer start bit
    Micro::wait_us(46);
    if (Protocol::pin_is_one()){
	uart << "Can't read first bit. DHT not responding???\n";
	return false;
    }

    bool wait_for_start_bit = false;

    for (uint8_t i = 0; i < 5; ++i){
	for (int8_t j = 7; j >= 0; --j){
	    if (wait_for_start_bit){
		Micro::wait_us(46);
		if (Protocol::pin_is_one()){
		    uart << "Can't read bit " << j << " of byte number " << (int) i << '\n';
		    return false;
		}
	    }

	    Micro::wait_us(74);
	    bool bit = Protocol::pin_is_one();
	    if (bit)
		atd::write_one(data[i], j);
	    else
		atd::write_zero(data[i], j);

	    // Solo tenemos que esperar el start bit cuando el bit leído es 1.
	    // Si es 0, ya estamos dentro del siguiente start bit.
	    wait_for_start_bit = bit;
	}
    }



    uart << "Recibido: " << (int) data[0]
	 << '.' << (int) data[1]
	 << "; " << (int) data[2]
	 << '.' << (int) data[3]
	 << "//" << (int) data[4]
	 << '\n';

// Comprobamos que el checksum es correcto
// TODO: aclarar este checksum
    uint8_t check = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (check != data[4]){
	uart << "Wrong check\n";
	return false;
    }

    return true;
}


void hello()
{
    mcu::UART_iostream uart;
    uart << "\n\nDHT11 test\n"
	        "----------\n"
		"Connect sensor to pin " << sensor_pin 
		<< " with a pull-up resistor of 4.7k\n";

    uart << "Waiting 2 seconds for DHT11 to start working ... ";
    Micro::wait_ms(2000);
    uart << "OK\n";
}


int main()
{
    init_uart();
    hello();

    mcu::UART_iostream uart;

    while(1){
	if (!read())
	    uart << "Reading error\n";

	uart << "Waiting 1 second";
	Micro::wait_ms(1000);
	uart << '.';
    }

}



