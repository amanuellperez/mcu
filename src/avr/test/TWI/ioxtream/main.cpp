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
#include "../../../avr_TWI_basic.h"
#include "../../../avr_TWI_master_ioxtream.h"


constexpr uint8_t TWI_buffer_size = 10;
using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;

constexpr uint8_t slave_address = 0x10;


void twi_print_error()
{
    avr::UART_iostream uart;

    if (TWI::no_response())
	uart << "Slave no responde.\n";

    else if (TWI::eow_data_nack())
	uart << "Error: eow_data_nack\n";

    else if (TWI::prog_error())
	uart << "Error de programación\n";

    else
	uart << "Error desconocido\n";
}


void send_service1()
{
    avr::UART_iostream uart;
    uart << "Service1:\n";

    TWI::reset();

    constexpr uint8_t N = 8;
    uint8_t msg[N];
    msg[0] = 0x34;
    msg[1] = 37;
    msg[2] = 98;
    msg[3] = 125;
    msg[4] = 15;
    msg[5] = 30;
    msg[6] = 45;
    msg[7] = 60;


    TWI twi{slave_address};
// twi.open(slave_address); // no necesario si se pasa en el constructor
    uart << "Escribimos ... ";
    for (uint8_t i = 0; i < N; ++i){
	twi << msg[i];
    }

    if (twi.error()){
	twi_print_error();

	return;
    }

    uart << "OK\n";

    uart << "Leemos: ";
    constexpr uint8_t out_nbytes = 3;
    twi.read(out_nbytes);
    for (uint8_t i = 0; i < out_nbytes; ++i){
	uint8_t x;
	twi >> x;
	uart << (int) x << ' ';
    }
    uart << '\n';

    twi.close();

    if (twi.error()){
	twi_print_error();

	return;
    }
}


void send_service2()
{
    avr::UART_iostream uart;
    uart << "Service2: ";


    std::byte msg[1] = {std::byte{0x87}};

    TWI twi;
    twi.open(slave_address);

    twi << msg[0];

    twi.close();

    if (twi.error())
	twi_print_error();
    
    else
	uart << "OK\n";
}

void send_service3()
{
    avr::UART_iostream uart;
    uart << "Service3: ";

    uint8_t msg[1];
    msg[0] = 0xAA;

    TWI twi;
    twi.open(slave_address);

    twi << msg[0];

    twi.close();

    if (twi.error())
	twi_print_error();
    
    else
	uart << "OK\n";
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

    uart << "Empezando como MASTER ioxtream\n";

    TWI::on<50>();

    uart << "TWI enable\n";
 
    test_master();
}



ISR(TWI_vect)
{
    TWI::handle_interrupt();
}

