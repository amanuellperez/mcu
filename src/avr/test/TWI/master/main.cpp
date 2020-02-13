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

#include "../../../avr_TWI.h"
#include "../../../avr_USART.h"
#include "../../../avr_time.h"

using namespace avr;


void TWI_error(std::ostream& out)
{

    switch(TWI::state()){
	case TWI::iostate::good:
	    out << "good\n";
	    break;


	case TWI::iostate::send_start_fail:
	    out << "send_start_fail\n";
	    break;


	case TWI::iostate::send_repeated_start_fail:
	    out << "send_repeated_start_fail\n";
	    break;


	case TWI::iostate::send_address_fail:
	    out << "send_address_fail\n";
	    break;


	case TWI::iostate::send_data_fail:
	    out << "send_data_fail\n";
	    break;


	case TWI::iostate::receive_data_fail:
	    out << "receive_data_fail\n";
	    break;


	case TWI::iostate::read_send_command_fail:
	    out << "read_send_command_fail\n";
	    break;

    }
}


constexpr uint8_t slave_address = 0x10;


void send_service1()
{
    UART_iostream uart;

    uint8_t msg[4];
    msg[0] = 0x34;
    msg[1] = 37;
    msg[2] = 98;
    msg[3] = 125;


    if (TWI::write<slave_address>(reinterpret_cast<std::byte*>(msg), 4) != 4){
	uart << "Error enviando servicio 1: ";
	TWI_error(uart);
    }
    else
	uart << "Envio ok\n";
}


void send_service2()
{
    UART_iostream uart;

    uint8_t msg[1];
    msg[0] = 0x87;


    if (TWI::write<slave_address>(reinterpret_cast<std::byte*>(msg), 1) != 1){
	uart << "Error enviando servicio 2: ";
	TWI_error(uart);
    }
    else
	uart << "Envio ok\n";
}

void send_service3()
{
    UART_iostream uart;

    uint8_t msg[1];
    msg[0] = 0xAA;


    if (TWI::write<slave_address>(reinterpret_cast<std::byte*>(msg), 1) != 1){
	uart << "Error enviando servicio 3: ";
	TWI_error(uart);
    }
    else
	uart << "Envio ok\n";
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



int main() 
{
    UART_iostream uart;
    cfg_basica(uart);
    uart.on();

    uart << "MASTER\n";

    // 1.- Configuración de TWI:
    TWI::interrupt_disable();	// elegir siempre si generar o no interrupciones

    // 2.- Encendemos TWI, seleccionando la frecuencia de funcionamiento:
    TWI::on<50>::as_a_master();

    uart << "TWI enable\n";
    
    while (1) {
	test_write();
    } // while(1)
}



