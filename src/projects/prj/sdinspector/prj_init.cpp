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
#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"

bool sdcard_init()
{
    using Init = SDCard::Init_return;

    mcu::UART_iostream uart;
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
		    return true;

	break; case Init::init_SDHC_or_SDXC_ok:
		    atd::print(uart, msg_init_SDHC_or_SDXC_ok);
		    return true;
    }

    return false;
}

