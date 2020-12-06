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

// 10/06/2019 
// Probamos el interfaz del SPI. Para ello, conectamos un register 74HC595
// Conectar:
//	pin 17 del micro al pin 14 del register
//	pin 19                  11
//	pin 12                  12
//
//  Otros pines del register:
//	conectar pin 10 (no_clear) a 5V, y pin 13 (no_oe) a 0V.
//
//  La salida del register conectarla a 8 leds.
//
//  Para este ejemplo no es necesario conectar el pin SS del SPI. Sin embargo,
//  en la datasheet pone claramente que este pin tiene que estar a 0 para que
//  haya bit trade. 
#include "../../avr_SPI.h"
#include "../../avr_pin.h"


using SPI = avr::SPI;

constexpr uint8_t num_pin_RCLK = 12;
constexpr uint16_t periodo_en_us = 2;	// 2 microsegundos!!!
constexpr uint8_t num_pin_no_chip_select = 16;


int main() 
{
    avr::Output_pin<num_pin_no_chip_select> no_CS;

    SPI::on_as_a_master<periodo_en_us>();
    SPI::spi_mode(0,0);
    SPI::data_order_MSB();

    no_CS.write_one();

    avr::Output_pin<num_pin_RCLK> pin_vuelca_buffer;

    while (1) {
	for (std::byte p{1}; p != std::byte{0}; p <<= 1){
	    SPI::write(p);
	    pin_vuelca_buffer.pulse_of_1us();
	    wait_ms(100);
	}
  } // while(1)
}



