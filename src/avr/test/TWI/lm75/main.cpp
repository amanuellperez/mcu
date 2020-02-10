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

// OJO: para que el TWI funcione a 100kHz es necesario operar
// a 8 MHz!!
// Conectar 1 termómetros LM75 con direcciónes 1. Muestra
// por USART la temperatura que marcan.
#include "../../avr_TWI.h"
#include "../../avr_USART.h"
#include "../../avr_time.h"

using namespace avr;

// Esta clase es temporal para probar el TWI. La buena está en dev.
template <uint8_t addr>
class Thermometer_LM75{
public:
    
//private:
    // La dirección del LM75_1 es 1001|address, siendo address de 3 bits.
    constexpr static uint8_t address = (0b1001000 | addr);

    // Pointer register (datasheet, pag. 13)
    constexpr static std::byte temperature_reg {0x00};
    constexpr static uint8_t configuration_reg = 0x01;
    constexpr static uint8_t T_hyst_reg = 0x02;
    constexpr static uint8_t T_os_reg = 0x03;
    constexpr static uint8_t product_id_reg = 0x07;

};


using LM75_1 = Thermometer_LM75<1>;


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


template <typename LM75>
void lee_temperatura()
{
    UART_iostream uart;

    std::byte temp[2];
    temp[0] = std::byte{255}; temp[1] = std::byte{255};

    if (TWI::read<LM75::address>(LM75::temperature_reg, temp, 2) != 2){
	uart << "Error: ";
	TWI_error(uart);
    }
    else{
	uart << static_cast<uint16_t>(temp[0]);    // high byte temperature

	if (atd::is_one_bit<7>::of(temp[1]))
	  uart << ".5";
	else
	  uart << ".0";
    }

}



int main() 
{
    UART_iostream uart;
    cfg_basica(uart);
    uart.on();

    uart << "Empezando\n";

    // 1.- Configuración de TWI:
    TWI::interrupt_disable();	// elegir siempre si generar o no interrupciones


    // 2.- Encendemos TWI, seleccionando la frecuencia de funcionamiento:
    // OJO: para que el TWI funcione a 100kHz es necesario operar
    // a 8 MHz!!
    // según make: 8MHz/(16+2*TWBR*1) ~= 100kHz
    // TODO: mejor: TWI::speed<100>();
    // Ver table 1-1 application note avr315
    // TWI::on<100>();
    TWI::on<50>::as_a_master();

    uart << "TWI enable\n";
    
    while (1) {
	uart << "T1 = ";
	lee_temperatura<LM75_1>();
	uart << "\n";

	wait_ms(1000);
    } // while(1)
}



