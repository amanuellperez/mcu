// Copyright (C) 2023 Manuel Perez 
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

#include <atd_ostream.h>

#include "dev.h"
#include "print.h"


void automatic_init()
{
    using Init = SDCard::Init_return;

    mcu::UART_iostream uart;
    uart << "\nSDCard::init() ...\n";
    switch (SDCard::init()){
	break; case Init::cmd0_fail	: 
		    uart << "CMD0 error\n";

	break; case Init::cmd8_crc_error: 
		    uart << "CMD8 CRC error\n";

	break; case Init::voltage_mismatch: 
		    uart << "ERROR: voltage mismatch\n";

	break; case Init::cmd8_echo_fail: 
		    uart << "CMD8 echo error\n";

	break; case Init::acmd41_in_idle_state: 
		    uart << "ACMD41 fail: card still in idle state\n";

	break; case Init::card_no_power_up: 
		    uart << "CMD58 fail: card still no power up\n";

	break; case Init::init_SDSC_card_ok:
		    uart << "Init OK: SDSC card type\n";

	break; case Init::init_SDHC_or_SDXC_ok:
		    uart << "Init OK: SDHC or SDXC card type\n";
    }

}

void step_by_step_init()
{
    mcu::UART_iostream uart;

    SDCard::SPI_cfg_init();
    uart << "go_idle_state (cmd0) ... = entering SPI mode\n";
    SDCard::R1 r1 = SDCard::go_idle_state();
    print(uart, r1);

    uart << "\nsend_if_cond (cmd8) ... \n";
    SDCard::R7 r7;
    SDCard::send_if_cond(r7);
    print(uart, r7);

    // Posibles respuestas con significado (ver 7.3.1.4)
    if (r7.r1.is_illegal_command())
	uart << "\tLegacy card\n";

    // TODO: si se pasa el supply_voltage y pattern a send_if_cond se puede
    // comprobar que todo sea correcto: hay que comprobar que en R7 haya hecho
    // el echo correspondiente (ver 7.3.1.4)
    else if (r7.r1.data == 0x09)
	uart << "\tCMD8 CRC error\n";

    else {
	if (r7.VCA() == 0)
	    uart << "\tVCA mismatch\n";

	else // TODO: esto es falso!!! No hemos comprobado que el pattern sea el correcto
	    uart << "\tCMD8 ok\n";
    }


    // Enviamos CMD58 (opcional) para averiguar qué potenciales es a los que
    // trabaja la tarjeta. Si nuestro hardware no trabaja con esos potenciales
    // tendríamos que considerarla no válida y acabar.
    // En este caso la tarjeta devolverá que está `busy`, sin inicializar. Es
    // normal (ver 7.2.1@physical_layer)
    uart << "\nread_ocr (cmd58) ... = supported voltages?\n";
    SDCard::R3 r3;
    SDCard::read_ocr(r3);
    print(uart, r3);

    uart << "\nsd_send_op_cond (acmd41) ... \n";
    r1 = SDCard::sd_send_op_cond();
    print(uart, r1);
    // TODO: si falla retry???

    uart << "\nread_ocr (cmd58) ... = is SDSC or SDHC/SDXC card? \n";
    SDCard::read_ocr(r3);
    print_type_card(uart, r3);
}



int main()
{
// init_UART();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

    Selector_SPI::init();

    uart << "----------------------------------------\n"
	 << "SDCard\n"
	 << "----------------------------------------\n\n";

    uart << "Menu\n"
	    "----\n"
	    "Choose type of initialization:\n"
	    "\t1. Step by step\n"
	    "\t2. Automatic\n";

    char ans{};
    uart >> ans;

    if (ans == '1')
	step_by_step_init();

    else
	automatic_init();

    uart << "Read:\n"
	    "-----\n";
    uart << "block_size = " << (int) SDCard::block_size << '\n';

    // CUIDADO!!! Ya está empezando a hacer cosas raras si le añado algún
    // `uart << ...`. El motivo es que el atmega32 solo tiene 2kB de RAM y el
    // block_size es 1/2 kB!!! Sin el array `data`, `make size` marca 1.4k de
    // RAM usados!!! Estoy al límite!!!
    uint8_t data[SDCard::block_size];
// DONT_HAVE_TO_COMPILE(uint8_t data[10]);

    SDCard::Address addr = 0;

    while(1){
	
	auto r = SDCard::read(addr, data);
	if (r.ok()){
	    uart << "read OK:\n";
	    uart << "-------------------------------\n";
	    for (uint16_t i = 0; i < SDCard::block_size; ++i){
		if (i % 15 == 0)
		    uart << '\n';
		atd::print_int_as_hex(uart, data[i], false);
		uart << ' ';
	    }
	    uart << "-------------------------------\n";
	}

	uart << "Press a key to continue\n";
	char c{};
	uart >> c;
	++addr;

    }
}





