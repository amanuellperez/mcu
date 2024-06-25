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
#include "dev.h"
#include "strings.h"
#include "print.h"

void automatic_init()
{
    using Init = SDCard::Init_return;

    myu::UART_iostream uart;
    uart << "\nSDCard::init() ...\n";
    switch (SDCard::init()){
	break; case Init::cmd0_fail	: 
		    atd::print(uart, msg_cmd0_fail);

	break; case Init::cmd8_crc_error: 
		    atd::print(uart, msg_cmd8_crc_error);

	break; case Init::voltage_mismatch: 
		    atd::print(uart, msg_voltage_mismatch);

	break; case Init::cmd8_echo_fail: 
		    atd::print(uart, msg_cmd8_echo_fail);

	break; case Init::acmd41_in_idle_state: 
		    atd::print(uart, msg_acmd41_in_idle_state);

	break; case Init::card_no_power_up: 
		    atd::print(uart, msg_card_no_power_up);

	break; case Init::init_SDSC_card_ok:
		    atd::print(uart, msg_init_SDSC_card_ok);

	break; case Init::init_SDHC_or_SDXC_ok:
		    atd::print(uart, msg_init_SDHC_or_SDXC_ok);
    }

}

void step_by_step_init()
{
    myu::UART_iostream uart;

    SDCard::SPI_cfg_init();
    atd::print(uart, msg_go_idle_state);
    SDCard::R1 r1 = SDCard::go_idle_state();
    print(uart, r1);

    atd::print(uart, msg_send_if_cond);
    SDCard::R7 r7;
    SDCard::send_if_cond(r7);
    print(uart, r7);

    // Posibles respuestas con significado (ver 7.3.1.4)
    if (r7.r1.is_illegal_command())
	atd::print(uart, msg_legacy_card);

    // TODO: si se pasa el supply_voltage y pattern a send_if_cond se puede
    // comprobar que todo sea correcto: hay que comprobar que en R7 haya hecho
    // el echo correspondiente (ver 7.3.1.4)
    else if (r7.r1.data == 0x09){
	uart << '\t';
	atd::print(uart, msg_cmd8_crc_error);
    }

    else {
	if (r7.VCA() == 0)
	    atd::print(uart, msg_vca_mismatch);

	else // TODO: esto es falso!!! No hemos comprobado que el pattern sea el correcto
	    atd::print(uart, msg_cmd8_ok);
    }


    // Enviamos CMD58 (opcional) para averiguar qué potenciales es a los que
    // trabaja la tarjeta. Si nuestro hardware no trabaja con esos potenciales
    // tendríamos que considerarla no válida y acabar.
    // En este caso la tarjeta devolverá que está `busy`, sin inicializar. Es
    // normal (ver 7.2.1@physical_layer)
    atd::print(uart, msg_read_ocr1);
    SDCard::R3 r3;
    SDCard::read_ocr(r3);
    print(uart, r3);

    atd::print(uart, msg_send_op_cond);
    r1 = SDCard::sd_send_op_cond();
    print(uart, r1);
    // TODO: si falla retry???

    atd::print(uart, msg_read_ocr2);
    SDCard::read_ocr(r3);
    print_type_card(uart, r3);
}
