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

// Este microcontrolador dialoga con el test/TWI/slave
#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_time.h"  // TODO: cambiar orden
#include "../../../avr_UART_iostream.h" // TODO: cambiar orden
#include "../../../avr_TWI_basic.h"
#include "../../../avr_TWI_master.h"


constexpr uint8_t TWI_buffer_size = 10;
using TWI = avr_::TWI_master<avr_::TWI_basic, TWI_buffer_size>;

constexpr uint8_t slave_address = 0x10;

inline void traza_twcr()
{
    auto tmp = TWCR;
    avr_::UART_iostream uart;
    uart << "TWCR = " << static_cast<uint16_t>(tmp) << '\n';
}


void twi_print_error()
{
    avr_::UART_iostream uart;

    if (TWI::no_response())
	uart << "Slave no responde.\n";

    else if (TWI::eow_data_nack())
	uart << "Error: eow_data_nack\n";

    else if (TWI::prog_error())
	uart << "Error de programación\n";

    else
	uart << "Error desconocido\n";
}

void twi_print_state()
{
    avr_::UART_iostream uart;

    if (TWI::error())
	uart << "state == error()\n";

    else if (TWI::no_response())
	uart << "state == no_response()\n";

    else if (TWI::prog_error())
	uart << "state == prog_error()\n";

    else if (TWI::eow())
	uart << "state == eow()\n";

    else if (TWI::eow_data_nack())
	uart << "state == eow_data_nack()\n";

    else if (TWI::eor())
	uart << "state == eor()\n";

    else if (TWI::eor_bf())
	uart << "state == eor_bf()\n";

    else if (TWI::ok())
	uart << "state == ok()\n";

    else
	uart << "state DESCONOCIDO!!!\n";
    
}

void twi_print_state(TWI::iostate st)
{
    avr_::UART_iostream uart;

    switch(st){
    case TWI::iostate::ok:
	uart << "ok\n";
	break;

    case TWI::iostate::read_or_write:
	uart << "read_or_write\n";
	break;

    case TWI::iostate::sla_w:
	uart << "sla_w\n";
	break;

    case TWI::iostate::sla_r:
	uart << "sla_r\n";
	break;

    case TWI::iostate::no_response:
	uart << "no_response\n";
	break;

    case TWI::iostate::transmitting:
	uart << "transmitting\n";
	break;

    case TWI::iostate::eow:
	uart << "eow\n";
	break;

    case TWI::iostate::eow_data_nack:
	uart << "eow_data_nack\n";
	break;

    case TWI::iostate::error_buffer_size:
	uart << "error_buffer_size\n";
	break;

    case TWI::iostate::receiving:
	uart << "receiving\n";
	break;

    case TWI::iostate::eor_bf:
	uart << "eor_bf\n";
	break;

    case TWI::iostate::eor:
	uart << "eor\n";
	break;

    case TWI::iostate::bus_error:
	uart << "bus_error\n";
	break;

    case TWI::iostate::unknown_error:
	uart << "unknown_error\n";
	break;

    case TWI::iostate::prog_error:
	uart << "prog_error\n";
	break;

    default:
	uart << "desconocido\n";
    }   
}



void send_service1()
{
    avr_::UART_iostream uart;
    uart << "\n\n=================\n";
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


    TWI::send_start();
    if (TWI::state() != TWI::iostate::read_or_write){
	uart << "Error: tendría que estar en read_or_write\n";
	return;
    }


    if (TWI::write_to<slave_address>(reinterpret_cast<std::byte*>(msg), 3) != 3){
	uart << "Error enviando servicio 1: ";
        uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    else
	uart << "\tEscribiendo ... ";

    for (uint8_t i = 3; i < N; ++i){
// 	wait_ms(100); <-- simula una transmisión lenta. Tal como está ahora
			// el slave no muetra los datos hasta que no recibe
			// todo. Esto se podría mejorar bastante. Para el
			// futuro si lo uso.
	TWI::write((std::byte*)(&msg[i]), 1);
    }

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
 
    if (TWI::prog_error()) // es el único posible error
	uart << "Error de programación\n";

    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (!TWI::eow()){
	uart << "FAIL\n";
	return;
    }

    uart << "OK\n";

    constexpr uint8_t out_nbytes = 3;
    static_assert(out_nbytes < TWI_buffer_size);

    uart << "\tLeemos respuesta: ";
    TWI::send_repeated_start();
    TWI::read_from<slave_address>(out_nbytes);

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

    if (TWI::prog_error()) // es el único posible error
	uart << "Error de programación\n";

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

    TWI::send_stop();
    
    if (!TWI::ok())
	uart << "ERROR: tendría que estar en 'ok'\n";

}


void send_service2()
{
    avr_::UART_iostream uart;
    uart << "\n\n=================\n";
    uart << "Service2:\n";


    std::byte msg[1] = {std::byte{0x87}};


    TWI::send_start();
    if (TWI::write_to<slave_address>(msg, 1) != 1){
	uart << "Error enviando servicio 2: ";
        uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    else
	uart << "\tEscribiendo ... ";

    TWI::wait_till_no_busy();
    
    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (TWI::eow()){
	uart << "OK\n";
	TWI::send_stop();
    }
    else
	uart << "FAIL\n";
}

void send_service3()
{
    avr_::UART_iostream uart;
    uart << "\n\n=================\n";
    uart << "Service3:\n";

    uint8_t x0 = 45;
    uint16_t x1 = 320; // bytes (decimal): 64 1
    uint16_t x2 = 876;	// bytes (decimal): 108 3

    TWI::reset();

    TWI::send_start();

    uart << "\tEscribiendo ... ";
    TWI::streamsize nres = TWI::write_to<slave_address>
	    (reinterpret_cast<std::byte*>(&x0), sizeof(x0));

    if (nres != sizeof(x0)){
	uart << "\nERROR: write_to devuelve 0!\n";
	if (TWI::prog_error())
	    uart << "prog_error()!!! Revisar código!!!\n";
	else
	    uart << "Se está intentando enviar demasiados datos de golpe, aumentar "
                "el buffer del TWI\n";
	return;
    }
    
    if (TWI::error()){
	twi_print_error();
	return;
    }

    nres = TWI::write(reinterpret_cast<std::byte*>(&x1), sizeof(x1));
    if (nres != sizeof(x1)){
	if (TWI::prog_error())
	    uart << "ERROR: write devuelve prog_error\n";
	else
	    uart << "\nERROR: write(x1) devuelve " << (int) nres << "\n";
	return;
    }


    nres = TWI::write(reinterpret_cast<std::byte*>(&x2), sizeof(x2));

    if (nres == 0){
	uart << "\nERROR: write(x2) devuelve 0\n";
	return;
    }

    TWI::wait_till_no_busy();

    if (TWI::no_response()) // es el único posible error
	uart << "dispostivo no responde\n";

    if (!TWI::eow()){
	uart << "FAIL\n";
	return;
    }

    uart << "OK\n";

    uart << "\tRecibiendo datos enviados ... ";
    TWI::send_repeated_start();

    TWI::read_from<slave_address>(sizeof(x0) + sizeof(x1) + sizeof(x2));

    TWI::wait_till_no_busy();

    uint8_t y0;
    uint16_t y1;
    uint16_t y2;
    if (TWI::read_buffer((std::byte*) &y0, sizeof(y0)) != sizeof(y0)){
	uart << "Error: no se ha recibido y1 enviado\n";
	TWI::send_stop();
	return;
    }

    if (TWI::read_buffer((std::byte*) &y1, sizeof(y1)) != sizeof(y1)){
	uart << "Error: no se ha recibido y1 enviado\n";
	TWI::send_stop();
	return;
    }

    if (TWI::error()){
	uart << "Error al recibir y1\n";
	twi_print_error();
	return;
    }

    if (TWI::read_buffer((std::byte*) &y2, sizeof(y2)) != sizeof(y2)){
	uart << "Error: no se ha recibido y2 enviado\n";
	TWI::send_stop();
	return;
    }

    if (TWI::error()){
	uart << "Error al recibir y2\n";
	twi_print_error();
	return;
    }

    uart << "Recibido "
	 << (int) y0 << ' ' << y1 << ' ' << y2 << '\n';

    if (x0 == y0 and x1 == y1 and x2 == y2)
	uart << "OK\n";
    else
	uart << "ERROR: recibido datos diferentes a los enviados\n";

    TWI::send_stop();
    
    if (!TWI::ok())
	uart << "ERROR: tendría que estar en 'ok'\n";

}



void test_write()
{
    send_service1();
    avr_::wait_ms(500);
    send_service2();
    avr_::wait_ms(500);
    send_service3();
    avr_::wait_ms(500);

}


void test_master()
{
    while (1) {
	test_write();
    }
}


int main() 
{
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.turn_on();

    uart << "Empezando como MASTER\n";

    TWI::on<50>();

    uart << "TWI enable\n";
 
    test_master();
}



ISR(TWI_vect)
{
    TWI::handle_interrupt();
}

