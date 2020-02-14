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

// Este microcontrolador dialoga con el test/TWI/master

#include "../../../avr_TWI_slave.h"
#include "../../../avr_TWI.h"
#include "../../../avr_USART.h"
#include "../../../avr_time.h"
#include "../../../avr_interrupt.h"

#include <atd_buffer.h>


constexpr uint8_t buffer_size = 10;
using TWI_slave = avr::TWI_slave<avr::TWI_basic, buffer_size>;

inline void traza(const char* fname)
{
    auto tmp = TWCR;

    avr::UART_iostream uart;
    uart << ">>> " << fname << ": TWCR = " << static_cast<uint16_t>(tmp) << "; TWINT = ";

    if (atd::is_one_bit<TWINT>::of(TWCR))
        uart << "1\n";
    else
        uart << "0\n";
}

inline void traza_twcr()
{
    auto tmp = TWCR;
    avr::UART_iostream uart;
    uart << "\tTWCR = " << static_cast<uint16_t>(tmp) << "\n";
}




constexpr uint8_t slave_address = 0x10;

enum class Service{
    error   ,
    unknown ,
    service1, 
    service2
};

constexpr std::byte service1_name {0x34};
constexpr std::byte service2_name {0x87};

Service read_service_name(std::array<std::byte, buffer_size>& params_in)
{
    avr::UART_iostream uart;

    while (!TWI_slave::eor()){ // TODO: ojo: qué pasa si se envia algo diferente?
	if (TWI_slave::get_area_full()) {
	    uart << "Error: get_area_full!!! buffer muy pequeño!!!\n\n";
	    TWI_slave::stop_transmission();
	    return Service::error;
	}
    } // while

    if (!TWI_slave::eor()){
            uart << "Tendría que estar en eor, pero esta en otro estado\n";
	    TWI_slave::stop_transmission();
	    return Service::error;
    }

    TWI_slave::streamsize n = TWI_slave::read_buffer(params_in.data(), params_in.size());

    if (n == 0) {
	uart << "No se ha leído nada!\n";
	TWI_slave::stop_transmission();
	return Service::error;
    }

    if (params_in[0] == service1_name)
	return Service::service1;

    else if (params_in[0] == service2_name)
	return Service::service2;

    uart << "Servicio desconocido [" << static_cast<uint16_t>(params_in[0])
         << "]\n";
    return Service::unknown;

}

// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service1(const std::array<std::byte, buffer_size>& params_in,
		    std::array<std::byte, buffer_size>& params_out)
{
    avr::UART_iostream uart;
    uart << "Ejecutando service1\n";
    uart << "params_in: "
	 << static_cast<uint16_t>(params_in[1]) << ", "
	 << static_cast<uint16_t>(params_in[2]) << ", "
	 << static_cast<uint16_t>(params_in[3]) << '\n';

    // respondemos 1 byte con 45!!!
    params_out[0] = std::byte{45};
}


// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service2(const std::array<std::byte, buffer_size>& params_in)
{
    avr::UART_iostream uart;
    uart << "Ejecutando service2\n";
}

void test_servidor()
{
    avr::UART_iostream uart;
    uart << "test_servidor\n"
         << "-------------\n"
         << "Elegir buffer_size de manera que se garantice que todo el mensaje "
            "entre en el buffer!!!\n\n";

    while (1){

	while (TWI_slave::listening()){
	    asm("nop");
	}

	std::array<std::byte, buffer_size> params_in;
	std::array<std::byte, buffer_size> params_out;

	// TODO: read_service_name miente! Lee tambien params_in!!! CAMBIARLO!
	Service srv_name = read_service_name(params_in);
	switch (srv_name){
	    case Service::service1:
		// Ejemplo: tiene parametros de entrada y salida
		service1(params_in, params_out);  // esta función 
			    // llama a: out = service1(in);
			    // transformando params_in y params_out en las
			    // structuras de datos correspondientes.
		// TODO: que params_out sea un "vector estatico". Esto es,
		// [begin, end) dentro de un array[begin, ...) <-- ¿esto es
		// std::span? De esa forma no tengo que pasarle la longitud!!!
//		write_params_out(params_out.data(), params_out.data() + 1);
		break;

	    case Service::service2:
		// Ejemplo: no tiene parametros de entrada, pero sí de salida
		service2(params_out); 
//		write_params_out(params_out);
		break;

	    case Service::unknown:
		TWI_slave::stop_transmission();
		break;

	    case Service::error:
		TWI_slave::stop_transmission();
		uart << "Transmission error\n";
		break;
	}

    }
}




void test_read()
{
    avr::UART_iostream uart;
    uart << "test_read\n"
	 << "---------\n"
	 << "Probar con distintos buffer_size\n\n";

    while (1){
	std::byte buffer[buffer_size];
	
	while (!TWI_slave::eor()){

            if (TWI_slave::get_area_full()) {
                uint8_t n = TWI_slave::read_buffer(buffer, buffer_size);
                uart << "\nXXX Recibidos " << static_cast<uint16_t>(n)
                     << " datos:\n";
                for (uint8_t i = 0; i < n; ++i)
                    uart << '\t' << std::to_integer<uint16_t>(buffer[i])
                         << '\n';
            }
        } // while

        while (TWI_slave::eor()){
	if (uint8_t n = TWI_slave::read_buffer(buffer, buffer_size)){
	    uart << "\nFFF Recibidos " << static_cast<uint16_t>(n)  << " datos:\n";
	    for (uint8_t i = 0; i < n; ++i)
		uart << '\t' << std::to_integer<uint16_t>(buffer[i]) << '\n';
	}
	}

	uart << "\n\n------------------------\n";
	
	wait_ms(100);

    }// while(1)

}



int main() 
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    uart << "Empezando\n";

    TWI_slave::on<50>::in<slave_address>();

    uart << "TWI enable\n";
    
//    test_read();
    test_servidor();
}



ISR(TWI_vect)
{
    TWI_slave::handle_interrupt();
}



