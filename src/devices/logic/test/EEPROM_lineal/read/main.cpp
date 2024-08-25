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

// Este es un ejemplo de descargar la eeprom en un cliente linux (en la
// carpeta cliente) via UART.
#include "../../../dev_25LC256.h"
#include "../../../dev_EEPROM_buffer.h"
#include <avr_UART.h>
#include <avr_time.h>


using namespace avr;

using SPI = SPI_master;

constexpr uint8_t num_pin_chip_select = 16;
using EEPROM = EEPROM_buffer<EEPROM_25LC256<num_pin_chip_select>>;

constexpr uint8_t sz = 100;
constexpr char APP_ID[] = "Tlog";   // id de la aplicación

void read_and_print(EEPROM& eeprom, uint16_t addr0)
{
    UART_ostream uart;

    char res = '\0';
    UART::receive(res);
    if (res != 'w'){
	uart << "Petición desconocida\r\nAbortamos\r\n";
	return;
    }

    uart << APP_ID;

    uint8_t buf[sz];
    auto leidos = eeprom.read(addr0, buf, sz);
    if (leidos != sz)
	uart << "ERROR: no se han podido leer " << sz << " bytes\r\n";

    for (uint8_t i = 0; i < sz; ){
	uart << addr0 + i << " | ";
	for (uint8_t j = 0; j < 10 and i < sz; ++j, ++i)
	    uart << buf[i] << "  ";
	uart << "\n\r";
    }
}

void test_eeprom() {
    UART_ostream uart;
    SPI::turn_on<num_pin_chip_select>();

    EEPROM eeprom;
    eeprom.cfg_SPI();

    while(1){
	read_and_print(eeprom, 0);

	wait_ms(1000);
    }// while
}



int main()
{
    test_eeprom();
}
