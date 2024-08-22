// Copyright (C) 2019-2021 Manuel Perez 
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

// 10/06/2019 
// Probamos el interfaz del SPI (como master). 
// Para ello, uso un register 74HC595.
// Conectar:
//	pin 17 (MOSI) del micro al pin 14 (SER) del register
//	pin 19 (SCK)                   11 (SRCLK)
//	pin 12			       12 (RCLK) (= latch)
//
//  Otros pines del register:
//	conectar pin 10 (no_clear) a 5V, y pin 13 (no_oe) a 0V.
//
//  La salida del register conectarla a 8 leds.
//
//  Para este ejemplo no es necesario conectar el pin SS del SPI. Sin embargo,
//  en la datasheet pone claramente que este pin tiene que estar a 0 para que
//  haya bit trade. 
#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_SPI.h"
#include "../../../avr_pin.h"
#include "../../../avr_UART_iostream.h"

// TODO: revisar. No se está seleccionando explícitamente el slave!!!

using SPI = avr_::SPI_master;

constexpr uint8_t npin_RCLK = 12;
constexpr uint16_t periodo_en_us = 2;	// 2 microsegundos!!!
constexpr uint8_t npin_no_chip_select = 16;


template <uint8_t n>
void pulse_of_1us(avr_::Output_pin<n>&)
{
    using Pin = avr_::Output_pin<n>;
    Pin::write_one();
    avr_::wait_us(1);  
    Pin::write_zero();
}

void test_basic()
{
    avr_::Output_pin<npin_RCLK> pin_buffer_flush;

    while (1) {
	for (uint8_t p{1}; p != uint8_t{0}; p <<= 1){
	    SPI::write(p);
	    // pin_buffer_flush.pulse_of_1us();
	    pulse_of_1us(pin_buffer_flush);
	    avr_::wait_ms(100);
	}
  } // while(1)
}

void test_count()
{
    avr_::UART_iostream uart;
    avr_::Output_pin<npin_RCLK> pin_buffer_flush;

    uart << "Se irán mostrando en los leds todos los números del 0 al 255\n";

    while (1) {
	for (uint8_t i = 0; i <= 255; ++i){
	    uart << static_cast<uint16_t>(i) << '\n';
	    SPI::write(uint8_t{i});
	    //pin_buffer_flush.pulse_of_1us();
	    pulse_of_1us(pin_buffer_flush);
	    avr_::wait_ms(500);
	}

    }
}
void test_choose()
{
    avr_::UART_iostream uart;
    avr_::Output_pin<npin_RCLK> pin_buffer_flush;

    while (1) {
        uart << "Write a number (less than 256): ";
	uint16_t c{};
	uart >> c;
	uint8_t res = static_cast<uint8_t>(c);
	uart << (uint16_t)(c) << '\n';
	SPI::write(uint8_t{res});
	//pin_buffer_flush.pulse_of_1us();
	pulse_of_1us(pin_buffer_flush);

    }
}


int main() 
{
// init_uart()
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.turn_on();

// init_SPI()
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::clock_period_in_us<periodo_en_us>();
    SPI::on();

    uart << "\n\nSPI test\n"
	        "--------\n";

    while (1) {
        uart << "\nMenu:\n"
                "1. basic\n"
		"2. count\n"
                "3. choose number\n";

        char c{};
        uart >> c;
        switch (c) {
            break; case '1': test_basic();
	    break; case '2': test_count();
	    break; case '3': test_choose();
        }
    }
}



