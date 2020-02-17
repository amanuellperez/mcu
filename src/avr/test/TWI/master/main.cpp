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

// Este microcontrolador dialoga con el test/TWI/slave
#include "../../../avr_time.h"  // TODO: cambiar orden
#include "../../../avr_USART.h" // TODO: cambiar orden
#include "../../../avr_TWI.h"
#include "../../../avr_TWI_master.h"


constexpr uint8_t TWI_buffer_size = 10;
using TWI = avr::TWI_master<avr::TWI_basic, TWI_buffer_size>;

constexpr uint8_t slave_address = 0x10;

inline void traza_twcr()
{
    auto tmp = TWCR;
    avr::UART_iostream uart;
    uart << "TWCR = " << static_cast<uint16_t>(tmp) << '\n';
}


void send_service1()
{
    avr::UART_iostream uart;
    uart << " =========== Enviando service1\n";

    TWI::reset();

    uint8_t msg[4];
    msg[0] = 0x34;
    msg[1] = 37;
    msg[2] = 98;
    msg[3] = 125;


    if (TWI::write<slave_address>(reinterpret_cast<std::byte*>(msg), 4) != 4){
	uart << "Error enviando servicio 1: ";
        uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    else
	uart << "Escribiendo ... ";

    uint16_t i = 0;
    for (; TWI::is_busy() and i < 65000; ++i)
    { ; }
    
    if (i == 65000){
	uart << "No responde!\n";
	uart << "Parando transmisión ... ";
	TWI::reset();
	uart << "OK\n";
	return;
    }
    
    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (!TWI::eow()){
	uart << "FAIL\n";
	return;
    }

    uart << "OK\n";

    constexpr uint8_t out_nbytes = 3;
    static_assert(out_nbytes < TWI_buffer_size);

    uart << "Leemos respuesta: ";
    TWI::read(out_nbytes);

    for (; TWI::is_busy() and i < 65000; ++i)
    { ; }
    
    if (i == 65000){
	uart << "No responde!\n";
	uart << "Parando transmisión ... ";
	TWI::reset();	// ESTO no funciona. De hecho, mientras el slave
	// mantenga SCL bajo es imposible parar la conexión.
	uart << "OK\n";
	return;
    }
    if (TWI::no_response()){ // es el único posible error
	uart << "ERROR: dispostivo no responde\n";
	return;
    }

    if (!TWI::eor_bf()){
	uart << "FAIL\n";
	return;
    }

    std::array<std::byte, out_nbytes> bout;
    if (TWI::read_buffer(bout.data(), out_nbytes) != out_nbytes){
	uart << "ERROR: no se han leido " << out_nbytes << " bytes del buffer\n";
    }
    else{
	uart << "OK; datos recibidos: ";
	for (auto b: bout)
	    uart << std::to_integer<uint16_t>(b) << ' ';
	uart << '\n';
    }

    if (!TWI::eor())
	uart << "ERROR: tendría que estar en 'eor'\n";

    TWI::stop_transmission();
    
    if (!TWI::ok())
	uart << "ERROR: tendría que estar en 'ok'\n";

}


void send_service2()
{
    avr::UART_iostream uart;
    uart << " =========== Enviando service2\n";
    uart << "TWI_buffer_size = " << static_cast<uint16_t>(TWI_buffer_size)
         << '\n';


    std::byte msg[1] = {std::byte{0x87}};


    if (TWI::write<slave_address>(msg, 1) != 1){
	uart << "Error enviando servicio 1: ";
        uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    else
	uart << "Escribiendo ... ";

    while (TWI::is_busy())
    { ; }
    
    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (TWI::eow()){
	uart << "OK\n";
	TWI::stop_transmission();
    }
    else
	uart << "FAIL\n";
}

void send_service3()
{
    avr::UART_iostream uart;
    uart << " =========== Enviando service3\n";

    uint8_t msg[1];
    msg[0] = 0xAA;


    if (TWI::write<slave_address>(reinterpret_cast<std::byte*>(msg), 1) != 1){
	uart << "Error enviando servicio 3: ";
        uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    else
	uart << "Escribiendo ... ";

    while (TWI::is_busy())
    { ; }
    
    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (TWI::eow()){
	uart << "OK\n";
	TWI::stop_transmission();
    }
    else
	uart << "FAIL\n";

}



void test_write()
{
    send_service1();
    wait_ms(500);
    send_service2();
    wait_ms(500);
    send_service3();
    wait_ms(500);

}


void test_master()
{
    while (1) {
	test_write();
    }
}


int main() 
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    uart << "Empezando como MASTER\n";

    TWI::on<50>();

    uart << "TWI enable\n";
 
    test_master();
}



ISR(TWI_vect)
{
    TWI::handle_interrupt();
}

