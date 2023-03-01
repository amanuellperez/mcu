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

#include <avr_atmega.h>
#include <avr_SPI_generic.h>
namespace mcu = atmega; // TODO: cambiar orden. Temporal.
			
#include "../../dev_sdcard.h"


using Micro   = mcu::Micro;

// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivos SPI
using Selector_SPI = 
    dev::SDCard_select<mcu::Output_pin<mcu::SPI::CS_pin_number>, mcu::SPI_master>;
struct SDCard_cfg{};
using SDCard = dev::SDCard_basic<SDCard_cfg>;


void print(std::ostream& out, const SDCard::R1& r1)
{
    out << "R1 response\n";
    out << "\tR1 : ";
    atd::print_int_as_hex(out, r1.data);
    out << '\n';

    if (!r1.is_valid()){
	out << "\tError: invalid r1\n";
	return;
    }

    if (r1.is_ready()){
	out << "\tCard ready\n";
	return;
    }

    if (r1.in_idle_state_flag())
	out << "\tIn idle state\n";

    if (r1.erase_reset_flag())
	out << "\tErase reset error\n";

    if (r1.illegal_command_flag())
	out << "\tIllegal command\n";

    if (r1.communication_CRC_error_flag())
	out << "\tCRC error\n";

    if (r1.erase_sequence_error_flag())
	out << "\tErase sequence error\n";

    if (r1.address_error_flag())
	out << "\tAddress error\n";

    if (r1.parameter_error_flag())
	out << "\tParameter error\n";
}

void print_if_error_r1(std::ostream& out, const SDCard::R1& r1)
{
    if (!(r1.is_valid() or r1.is_ready() or r1.is_in_idle_state())
	or r1.is_an_error()){
	out << "\tr1[";
	atd::print_int_as_hex(out, r1.data);
	out << "] ERROR:\n";
	print(out, r1);
	return;
    }
}


void print_raw_R3(std::ostream& out, const SDCard::R3& r3)
{
    out << "\tR3     : ";
    atd::print_int_as_hex(out, r3.OCR[0]);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[1], false);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[2], false);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[3], false);
    out << '\n';
}

void print_type_card(std::ostream& out, const SDCard::R3& r3)
{
    using R3 = SDCard::R3;

    out << "R3 response\n";
    
    print_if_error_r1(out, r3.r1);
    print_raw_R3(out, r3);

    out << "\tCard has finished power up? ";
    if (r3.card_has_finished_power_up()){
	out << "yes\n";

	out << "\tCard type : ";
	if (r3.card_type() == R3::Type::SDSC)
	    out << "SDSC\n";

	else
	    out << "SDHC or SDXC\n";
    }
    else
	out << "no\n";
}


void print(std::ostream& out, const SDCard::R3& r3)
{
    using R3 = SDCard::R3;

    out << "R3 response\n";
    
    print_if_error_r1(out, r3.r1);
    print_raw_R3(out, r3);

    out << "\tCard has finished power up? ";
    if (r3.card_has_finished_power_up()){
	out << "yes\n";

	out << "\tCard type : ";
	if (r3.card_type() == R3::Type::SDSC)
	    out << "SDSC\n";

	else
	    out << "SDSC\n";
    }
    else
	out << "no\n";

    out << "\tsupport 3.5-3-6V()? ";
    if (r3.support_3_5_3_6V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 3.4-3-5V()? ";
    if (r3.support_3_4_3_5V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 3.3-3-4V()? ";
    if (r3.support_3_3_3_4V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 3.2-3-3V()? ";
    if (r3.support_3_2_3_3V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 3.1-3-2V()? ";
    if (r3.support_3_1_3_2V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 3.0-3-1V()? ";
    if (r3.support_3_0_3_1V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 2.9-3-0V()? ";
    if (r3.support_2_9_3_0V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 2.8-2-9V()? ";
    if (r3.support_2_8_2_9V())
	out << "yes\n";
    else
	out << "no\n";

    out << "\tsupport 2.7-2-8V()? ";
    if (r3.support_2_7_2_8V())
	out << "yes\n";
    else
	out << "no\n";

}


void print(std::ostream& out, const SDCard::R7& r7)
{
    using R7 = SDCard::R7;

    out << "R7 response\n";

    print_if_error_r1(out, r7.r1);

    out << "\tR7     : ";
    atd::print_int_as_hex(out, r7.data[0]);
    out << ' '; atd::print_int_as_hex(out, r7.data[1], false);
    out << ' '; atd::print_int_as_hex(out, r7.data[2], false);
    out << ' '; atd::print_int_as_hex(out, r7.data[3], false);
    out << '\n';

    out << "\tVersion: " << (uint16_t) r7.version() << '\n';
    out << "\tVoltage: ";

    switch (r7.voltage()){
	break; case R7::Voltage::not_defined: out << "not defined!\n";
	break; case R7::Voltage::v2_7_to_3_6_V: out << "from 2.7 to 3.6V\n";
	break; case R7::Voltage::low_voltage: out << "low voltage\n";
	break; case R7::Voltage::reserved: out << "reserved\n";
    }

    out << "\tPattern: ";
    atd::print_int_as_hex(out,  r7.pattern());
    out << '\n';
}

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
    // block_size es 1/2 kB!!!
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





