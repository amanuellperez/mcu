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


constexpr uint8_t TWI_buffer_size = 100;
using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;

constexpr uint8_t slave_address = 0x10;



void twi_print_state()
{
    avr::UART_iostream uart;

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
    avr::UART_iostream uart;

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
    avr::UART_iostream uart;
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
    uart << "\n==============================\n";
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

template <typename T>
void uart_print(const T& x)
{
    avr::UART_iostream uart;
    uart << x;
}

void uart_print(std::byte b)
{
    avr::UART_iostream uart;
    uart << (int) b;
}

template <typename Int>
void send_type(const Int& x0, const char* tname)
{
    avr::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Enviando un " << tname << ": ";

    TWI twi;
    twi.open(slave_address);

    twi << x0;

    if (twi.error()){
	uart << "twi ERROR!!!";
	twi_print_error();
	return;
    }


    twi.read(sizeof(Int));

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

    twi.close();

    if (twi.error())
	twi_print_error();
    
}


void send_service4()
{
    avr::UART_iostream uart;
    uart << "\n==============================\n";
    uart << "Service4: \n";

    uint8_t x0 = 34;
    uint16_t x1 = 320; // bytes (decimal): 64 1
    uint32_t x2 = 876;
    int8_t x3 = -4;
    int16_t x4 = -300; // bytes (decimal): 64 1
    int32_t x5 = -10000;

    TWI twi;
    twi.open(slave_address);

    twi << x0 << x1 << x2
	<< x3 << x4 << x5;

    if (twi.error()){
	uart << "twi ERROR!!!";
	twi_print_error();
	return;
    }


    uart << "\tLeyendo datos enviados... ";
    twi.read(sizeof(x0) + sizeof(x1) + sizeof(x2)
	    +sizeof(x3) + sizeof(x4) + sizeof(x5));

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

    twi.close();

    if (twi.error())
	twi_print_error();
}

void test_all_send_type()
{
    send_type<char>('a', "char");
    send_type<signed char>('c', "signed char");
    send_type<unsigned char>('r', "unsigned char");
    send_type<std::byte>(std::byte{0x02}, "std::byte");
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
    in >> d.x >> d.y >> d.z; 
    return in;
}

template <typename Ostream>
Ostream& operator<<(Ostream& out, const Data& d)
{ 
   out << d.x << d.y << d.z;
   return out;
}

void service(const Data& in, Data& out)
{
    avr::UART_iostream uart;

    TWI twi;
    twi.open(slave_address);
    
    twi << in;

    if (twi.error()){
	uart << "Error: ";
	twi_print_error();
	return;
    }

    twi.read(Data::size());
    twi >> out;

    twi.close();

    if (twi.error())
	twi_print_error();
}


void test_typical_service()
{
    avr::UART_iostream uart;
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


void test_master()
{
    while (1) {
	send_service1();
	wait_ms(500);
	send_service2();
	wait_ms(500);
	test_all_send_type();
	wait_ms(500);
	send_service4();
	test_typical_service();
	wait_ms(500);
    }
}


int main() 
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    uart << "\n\n\n* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
    uart << "Empezando como MASTER ioxtream\n";
    uart << "* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";

    TWI::on<50>();

    uart << "TWI enable\n";
 
    test_master();
}



ISR(TWI_vect)
{
    TWI::handle_interrupt();
}

