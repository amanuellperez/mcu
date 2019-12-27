// Copyright (C) 2019-2020 A.Manuel L.Perez
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
// Conectar 2 termómetros LM75 con direcciónes 1 y 2. Muestra
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
    constexpr static uint8_t temperature_reg = 0x00;
    constexpr static uint8_t configuration_reg = 0x01;
    constexpr static uint8_t T_hyst_reg = 0x02;
    constexpr static uint8_t T_os_reg = 0x03;
    constexpr static uint8_t product_id_reg = 0x07;

};


using LM75_1 = Thermometer_LM75<1>;
using LM75_2 = Thermometer_LM75<2>;


template <typename LM75>
void lee_temperatura()
{
    UART_iostream uart;

    TWI::Start start;
    if (!start){
	uart << "start fail\n";
        uart << "status_bits = [" << static_cast<uint16_t>(TWI::status_bits())
             << "]\n";
        return;
    }

    // Seleccionamos el registro de temperatura
    if (TWI::send(LM75::address, std::byte{LM75::temperature_reg}) != 1){
	uart << "Error al enviar temperature_reg\n";
	return;
    }

    // Reenviamos otro start (vamos a leer)
    TWI::send_repeated_start();
    if (TWI::send_repeated_start_fail()){
	uart << "repeated start fail\n";
	return;
    }

    uint8_t temp[2];
    temp[0] = 255; temp[1] = 255;

    if (TWI::receive(LM75::address, reinterpret_cast<std::byte*>(temp), 2) != 2u){
	uart << "No se han recibido los 2 bytes esperados!!!\n";
    }
    
    uart << static_cast<uint16_t>(temp[0]);    // high byte temperature

    if (temp[1] & (1 << 7)) 
      uart << ".5";
    else
      uart << ".0";

}



int main() 
{
    UART_iostream uart;
    cfg_basica(uart);
    uart.on();

    uart << "Empezando\n";

    // OJO: para que el TWI funcione a 100kHz es necesario operar
    // a 8 MHz!!
    // según make: 8MHz/(16+2*TWBR*1) ~= 100kHz
    // TODO: mejor: TWI::speed<100>();
    // Ver table 1-1 application note avr315
    TWI::enable_and_set_SCL_frequency_in_kHz<100>();

    uart << "TWI enable\n";
    
    while (1) {
	uart << "T1 = ";
	lee_temperatura<LM75_1>();
	uart << ";  T2 = ";
	lee_temperatura<LM75_2>();
	uart << " +- 1ºC (a 25ºC)";
	uart << "\n";

	wait_ms(1000);
    } // while(1)
}



