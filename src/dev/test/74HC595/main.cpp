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

#include "../../dev_register_74HC595.h"

using namespace dev;



template <uint8_t p2, uint8_t p3>
void barre_encendiendolos_todos(Register_74HC595<p2,p3>& reg)
{
    for (uint8_t p = 1; p != 0; p <<= 1){
	reg.write(p);
	wait_ms(100);
    }

    wait_ms(100);
    reg.clear();
}

//template <uint8_t p2, uint8_t p3>
//void que_se_mueva_un_led_encendido(Register_74HC595<p2,p3>& reg)
//{
//    reg.buffer_write(1);
//    reg.buffer_shift();
//    reg.buffer_vuelca();
//    wait_ms(100);
//
//    reg.buffer_write(0);
//
//    for (uint8_t i = 0; i < 7; ++i){
//	wait_ms(100);
//	reg.buffer_shift();
//	reg.buffer_vuelca();
//    }
//
//    wait_ms(100);
//    reg.clear();
//}


template <uint8_t p2, uint8_t p3>
void contador(Register_74HC595<p2,p3>& reg)
{
    for (uint8_t i = 0; i < 255; ++i){
	reg.write(i);
	wait_ms(500);
    }

    reg.write(255);
    wait_ms(500);

}

constexpr uint16_t periodo_en_us = 2;	// 2 microsegundos!!!

using SPI = avr::SPI_master;

int main() 
{
    SPI::init<periodo_en_us>(0, 0);

    SPI::select_slave();
    
    auto reg = 
	register_74HC595(Pin_register_74HC595_RCLK<12>{}
			,Pin_register_74HC595_NO_SRCLR<0>{});

    reg.buffer_clear();

    while(1){
	// barre_encendiendolos_todos(reg);
	// que_se_mueva_un_led_encendido(reg);
	contador(reg);
    }
}


