// Copyright (C) 2019-2023 Manuel Perez 
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

// Este microcontrolador dialoga con el test/TWI/master
#include "../../../mcu_TWI_slave.h"
#include <avr_atmega.h>


// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;


// Devices
// -------
constexpr uint8_t TWI_buffer_size = 100; // voy a enviarle un tipo de cada: int8, int16, ...
					 
using TWI_slave_cfg = mcu::TWI_slave_cfg<Micro, 
                                         my_mcu::TWI_basic,
					 TWI_buffer_size>;

using TWI = mcu::TWI_slave<TWI_slave_cfg>;




inline void traza(const char* fname)
{
    auto tmp = TWCR;

    my_mcu::UART_iostream uart;
    uart << ">>> " << fname << ": TWCR = " << static_cast<uint16_t>(tmp) << "; TWINT = ";

    if (atd::is_one_bit<TWINT>::of(TWCR))
        uart << "1\n";
    else
        uart << "0\n";
}

inline void traza_twcr()
{
    auto tmp = TWCR;
    my_mcu::UART_iostream uart;
    uart << "\tTWCR = " << static_cast<uint16_t>(tmp) << "\n";
}




constexpr uint8_t slave_address = 0x10;

enum class Service{
    error   ,
    unknown ,
    service1, 
    service2
};

constexpr uint8_t service1_name {0x34};
constexpr uint8_t service2_name {0x87};

// nread [out]: parametros leidos
Service read_service_name(std::array<uint8_t, TWI_buffer_size>& params_in, TWI::streamsize& nread)
{
    my_mcu::UART_iostream uart;
    uart << "\n\n=================\n";
    uart << "read_service_name\n";

    if (TWI::ok()){
	uart << "ERROR: state = ok. Devolvemos control\n";
	return Service::error;
    }

    while (!TWI::eor()){ // TODO: ojo: qué pasa si se envia algo diferente?
	uart << "Esperando a TWI::eor\n";
    } // while


    if (TWI::is_busy())
	uart << "ERROR: TWI::is_busy!!!\n";

    nread = TWI::read_buffer(params_in.data(), params_in.size());
    uart << "Leidos " << (int)nread << " bytes, a saber: ";
    for (uint8_t i = 0; i < nread; ++i)
	uart << (int) params_in[i] << ' ';
    uart << '\n';
    
    if (TWI::rec_bf())
	uart << "ERROR: estado == rec_bf\n";

    if (TWI::is_listening())
	uart << "TWI esta listening!\n";

    if (nread == 0) {
	uart << "No se ha leído nada!\n";
	TWI::stop_transmission();
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

void print_TWI_state()
{
    my_mcu::UART_iostream uart;
    uart << "state = ";

    if (TWI::state() == TWI::iostate::listening)
	uart << "listening\n";

    else if (TWI::state() == TWI::iostate::eor_data_nack)
	uart << "data_nack\n";

    else if (TWI::state() == TWI::iostate::bus_error)
	uart << "bus_error\n";

    else if (TWI::state() == TWI::iostate::rec_bf)
	uart << "rec_bf\n";

    else if (TWI::state() == TWI::iostate::eor)
	uart << "eor\n";

    else if (TWI::state() == TWI::iostate::wrt_be)
	uart << "wrt_be\n";

    else if (TWI::state() == TWI::iostate::eow)
	uart << "eow\n";

    else if (TWI::state() == TWI::iostate::eow_too_many_data)
	uart << "eow_too_many_data\n";

    else if (TWI::state() == TWI::iostate::eow_more_data)
	uart << "eow_more_data\n";

    else
	uart << "desconocido\n";

}

// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service1(const std::array<uint8_t, TWI_buffer_size>& params_in,
		    std::array<uint8_t, TWI_buffer_size>& params_out)
{
    my_mcu::UART_iostream uart;
    uart << "--------- Ejecutando service1\n";
    uart << "params_in: "
	 << static_cast<uint16_t>(params_in[1]) << ", "
	 << static_cast<uint16_t>(params_in[2]) << ", "
	 << static_cast<uint16_t>(params_in[3]) << '\n';

    // respondemos 3 byte con 45!!!
    params_out[0] = uint8_t{45};
    params_out[1] = uint8_t{50};
    params_out[2] = uint8_t{87};

    print_TWI_state();
    uart << "Esperamos a que esté wrt_be\n";
    while (!TWI::wrt_be())
    { 
	Micro::wait_ms(100);
	print_TWI_state();
	
    }


    uint8_t n = TWI::write_buffer(params_out.data(), 3);
    if (n != 3){
        uart << "ERROR: write_buffer != 3 (devuelve "
             << static_cast<uint16_t>(n) << "\n";
        return;
    }

    uart << "devolvemos los bytes de salida ...";
    while (TWI::is_busy())
    { ; }

    if (TWI::eow())
	uart << "OK\n";
    else{
	uart << "ERROR: ";
	print_TWI_state();
    }
    
}


// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service2(const std::array<uint8_t, TWI_buffer_size>& params_in)
{
    my_mcu::UART_iostream uart;
    uart << "-------------- Recibido service2\n";
}


// params_in[0,n) = parametros de entrada
void service_unknown(const std::array<uint8_t, TWI_buffer_size>& params_in,
                     TWI::streamsize n)
{
    my_mcu::UART_iostream uart;
    uart << "--------- Service unknown\n"
	 << "Devolvemos lo recibido\n";

    print_TWI_state();
    uart << "Esperamos a que esté wrt_be\n";
    while (!TWI::wrt_be())
    { 
	Micro::wait_ms(100);
	uart << "Tendría que estar en state == wrt_be, pero está en: ";
	print_TWI_state();
	
    }


    uint8_t nres = TWI::write_buffer(params_in.data(), n);
    if (nres != n){
        uart << "ERROR: write_buffer != " << n<< " (devuelve "
             << static_cast<uint16_t>(nres) << "\n";
        return;
    }

    uart << "devolvemos los bytes de salida ...";
    TWI::wait_till_no_busy();

    if (TWI::eow())
	uart << "OK\n";
    else{
	uart << "ERROR: ";
	print_TWI_state();
    }
    
}



void test_servidor()
{
    my_mcu::UART_iostream uart;
    uart << "test_servidor\n"
         << "-------------\n"
         << "Elegir buffer_size de manera que se garantice que todo el mensaje "
            "entre en el buffer!!!\n\n";

    while (1){
	uart << "listening ... ";
	while (!TWI::eor()){
	    asm("nop");
	}
	uart << "FIN\n";

	std::array<uint8_t, TWI_buffer_size> params_in;
	std::array<uint8_t, TWI_buffer_size> params_out;

	// TODO: read_service_name miente! Lee tambien params_in!!! CAMBIARLO!
	TWI::streamsize n;
	Service srv_name = read_service_name(params_in, n);
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
		TWI::stop_transmission();
		break;

	    case Service::unknown:
                service_unknown(params_in, n);
		TWI::stop_transmission();
                break;

	    case Service::error:
		TWI::stop_transmission();
		uart << "Transmission error\n";
		break;
	}

	if (!TWI::ok())
	    uart << "ERROR: state != ok\n";
    }
}




void test_read()
{
    my_mcu::UART_iostream uart;
    uart << "test_read\n"
	 << "---------\n"
	 << "Probar con distintos buffer_size\n\n";

    while (1){
	uint8_t buffer[TWI_buffer_size];
	
	// El caso más habitual es que el master pida algo al slave, por ello,
	// lo normal será que el master siempre envía primero SLA+W.
	while (!TWI::eor()){

            if (TWI::rec_bf()) {
                uint8_t n = TWI::read_buffer(buffer, TWI_buffer_size);
                uart << "\nXXX Recibidos " << static_cast<uint16_t>(n)
                     << " datos:\n";
                for (uint8_t i = 0; i < n; ++i)
                    uart << '\t' << (uint16_t) buffer[i]
                         << '\n';
            }
        } // while

        while (TWI::eor()){
	if (uint8_t n = TWI::read_buffer(buffer, TWI_buffer_size)){
	    uart << "\nFFF Recibidos " << static_cast<uint16_t>(n)  << " datos:\n";
	    for (uint8_t i = 0; i < n; ++i)
		uart << '\t' << (uint16_t)(buffer[i]) << '\n';
	}
	}

	uart << "\n\n------------------------\n";
	
	Micro::wait_ms(100);

    }// while(1)

}



int main() 
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

    uart << "TWI slave\n"
	 << "---------\n";

    TWI::turn_on<slave_address>();

    uart << "TWI enable\n";
    
//    test_read();
    test_servidor();
}



ISR(TWI_vect)
{
    TWI::handle_interrupt();
}



