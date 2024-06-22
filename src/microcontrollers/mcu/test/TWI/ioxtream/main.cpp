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

// Este microcontrolador dialoga con el test/TWI/slave
#include "../../../mcu_TWI_master_ioxtream.h"
#include "../../../mcu_TWI_master.h"
#include <avr_atmega.h>

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;


// Devices
// -------
constexpr uint8_t TWI_buffer_size = 100;
using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           my_mcu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master  = mcu::TWI_master<TWI_master_cfg>;

using TWI = mcu::TWI_master_ioxtream<TWI_master>;

constexpr uint8_t slave_address = 0x10;



// Functions
// ---------
void twi_print_state()
{
    my_mcu::UART_iostream uart;

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

void twi_print_error()
{
    my_mcu::UART_iostream uart;

    if (TWI::no_response())
	uart << "Slave no responde.\n";

    else if (TWI::eow_data_nack())
	uart << "Error: eow_data_nack\n";

    else if (TWI::prog_error())
	uart << "Error de programación\n";

    else if (TWI::error_buffer_size())
	uart << "Error, buffer pequeño\n";

    else if (TWI::bus_error())
	uart << "bus_error!!!\n";

    else if (TWI::unknown_error())
	uart << "unknown_error\n";

    else
	uart << "Error desconocido\n";
    
}



void send_service1()
{
    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
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
    uart << "Writing ... ";
    for (uint8_t i = 0; i < N; ++i){
	twi << msg[i];
    }

    if (twi.error()){
	twi_print_error();

	return;
    }

    uart << "OK\n";

    uart << "Reading: ";
    constexpr uint8_t out_nbytes = 3;
//    twi.prepare_to_read(out_nbytes);
    for (uint8_t i = 0; i < out_nbytes; ++i){
	uint8_t x;
	twi >> x;
	uart << (int) x << ' ';
    }
    uart << '\n';

//    twi.close();

    if (twi.error()){
	twi_print_error();

	return;
    }
}


void send_service2()
{
    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Service2: ";


    uint8_t msg[1] = {uint8_t{0x87}};

    TWI twi(slave_address);

    twi << msg[0];

//    twi.close();

    if (twi.error())
	twi_print_error();
    
    else
	uart << "OK\n";
}

template <typename T>
void uart_print(const T& x)
{
    my_mcu::UART_iostream uart;
    uart << x;
}

void uart_print(uint8_t b)
{
    my_mcu::UART_iostream uart;
    uart << (int) b;
}

template <typename Int>
void send_type(const Int& x0, const char* tname)
{
    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Sending a " << tname << ": ";

    TWI twi(slave_address);

    twi << x0;

    if (twi.error()){
	uart << "twi ERROR!!!";
	twi_print_error();
	return;
    }


//    twi.prepare_to_read(sizeof(Int));

    Int y0{};
    twi >> y0;

    if (y0 != x0){
	uart << "ERROR: no se han recibido el dato enviado\n";
	uart << "\tEnviado: "; uart_print(x0); uart << '\n';
        uart << "\tRecibido: ";
	uart_print(y0);
	uart << '\n';
    }
    else
	uart << "OK\n";

//    twi.close();

    if (twi.error())
	twi_print_error();
    
}


void send_service4()
{
    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Service4: \n";

    uint8_t x0 = 34;
    uint16_t x1 = 320; // bytes (decimal): 64 1
    uint32_t x2 = 876;
    int8_t x3 = -4;
    int16_t x4 = -300; // bytes (decimal): 64 1
    int32_t x5 = -10000;

    TWI twi(slave_address);

    twi << x0 << x1 << x2
	<< x3 << x4 << x5;

    if (twi.error()){
	uart << "twi ERROR!!!";
	twi_print_error();
	return;
    }


    uart << "\tLeyendo datos enviados... ";
//    twi.prepare_to_read(sizeof(x0) + sizeof(x1) + sizeof(x2)
//	    +sizeof(x3) + sizeof(x4) + sizeof(x5));

    uint8_t y0;
    uint16_t y1;
    uint32_t y2;
    int8_t y3;
    int16_t y4;
    int32_t y5;

    twi >> y0 >> y1 >> y2 
	>> y3 >> y4 >> y5;

    if (!(y0 == x0 and y1 == x1 and y2 == x2 and
	  y3 == x3 and y4 == x4 and y5 == x5)){
	uart << "ERROR: no se han recibido los datos enviados\n";
        uart << "\tRecibido: " << (int)y0 << ", " << (unsigned int)y1 << ", "
             << (unsigned int)y2 << '\n';
    }
    else
	uart << "OK\n";

//    twi.close();

    if (twi.error())
	twi_print_error();
}

void test_all_send_type()
{
    send_type<char>('a', "char");
    send_type<signed char>('c', "signed char");
    send_type<unsigned char>('r', "unsigned char");
    send_type<uint8_t>(uint8_t{0x02}, "uint8_t");
    send_type<uint8_t>(34, "uint8_t");
    send_type<uint16_t>(340, "uint16_t");
    send_type<uint32_t>(10000, "uint32_t");
    send_type<uint64_t>(1000000, "uint64_t");
    send_type<int8_t>(-34, "int8_t");
    send_type<int16_t>(-340, "int16_t");
    send_type<int32_t>(-10000, "int32_t");
    send_type<int64_t>(-1000000, "int64_t");
}

struct Data{
    uint8_t x;
    int16_t y;
    char z;

    static constexpr uint8_t size()
    {return sizeof(uint8_t) + sizeof(int16_t) + sizeof(char);}
};

template <typename Istream>
Istream& operator>>(Istream& in, Data& d)
{
    return in >> d.x >> d.y >> d.z; 
}

template <typename Ostream>
Ostream& operator<<(Ostream& out, const Data& d)
{ 
   return out << d.x << d.y << d.z;
}

void service(const Data& in, Data& out)
{
    my_mcu::UART_iostream uart;

    TWI twi(slave_address);
    
    twi << in;

    if (twi.error()){
	uart << "Error: ";
	twi_print_error();
	return;
    }

//    twi.prepare_to_read(Data::size());
    twi >> out;

//    twi.close();

    if (twi.error())
	twi_print_error();
}


void test_typical_service()
{
    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "typical_service: ";


    Data d0{3, 800, 'x'};
    Data d1;
    service(d0, d1);

    if (d0.x != d1.x or d0.y != d1.y or d0.z != d1.z)
	uart << "ERROR: recibido d1 diferente al enviado!!!\n";
    else
	uart << "OK\n";
}

void test_n()
{
    constexpr int data_size = 3;
    uint8_t data[3] = {uint8_t{10}, uint8_t{20}, uint8_t{30}};

    my_mcu::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Probando read(q,n) y write(q, n): ";

    TWI twi(slave_address);

    if (twi.write(data, data_size) != data_size){
	uart << "ERROR en write: data_size recibido erroneo!\n";
	return;
    }


    uint8_t q[data_size];
//    twi.prepare_to_read(data_size);
    if (twi.read(q, data_size) != data_size){
	uart << "ERROR en read: data_size recibido erroneo!\n";
	return;
    }

    for (int i = 0; i < data_size; ++i){
	if (data[i] != q[i])
	    uart << "ERROR: q[" << i << "] = " << (int) q[i] 
		 << " != data[" << i << "] = " << (int) data[i] << '\n';
    }

//    twi.close();

    if (twi.error())
	twi_print_error();

    else 
	uart << "OK\n";

}



void test_master()
{
    while (1) {
	send_service1();
	Micro::wait_ms(500);
	send_service2();
	Micro::wait_ms(500);
	test_all_send_type();
	Micro::wait_ms(500);
	send_service4();
	test_typical_service();
	Micro::wait_ms(200);
	test_n();
    }
}


int main() 
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
    uart << "MASTER ioxtream Test\n";
    uart << "* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";

    TWI_master::turn_on<50>();

    uart << "TWI enable\n";
 
    test_master();
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}

