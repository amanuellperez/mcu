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


#include "print.h"
#include "strings.h"


void print_bool_as_yes_no(std::ostream& out, bool x)
{
    if (x)
        atd::print(out, msg_yes);
    else
        atd::print(out, msg_no);

    out << '\n';
}

void print(std::ostream& out, const SDCard::R1& r1)
{
    atd::print(out, msg_r1_response);
    atd::print_int_as_hex(out, r1.data);
    out << '\n';

    if (!r1.is_valid()){
	atd::print(out, msg_invalid_r1);
	return;
    }

    if (r1.is_ready()){
	atd::print(out, msg_card_ready);
	return;
    }

    if (r1.in_idle_state_flag())
	atd::print(out, msg_in_idle_state);

    if (r1.erase_reset_flag())
	atd::print(out, msg_erase_reset_error);

    if (r1.illegal_command_flag())
	atd::print(out, msg_illegal_command);

    if (r1.communication_CRC_error_flag())
	atd::print(out, msg_crc_error);

    if (r1.erase_sequence_error_flag())
	atd::print(out, msg_erase_sequence_error);

    if (r1.address_error_flag())
	atd::print(out, msg_address_error);

    if (r1.parameter_error_flag())
	atd::print(out, msg_parameter_error);
}

void print_if_error_r1(std::ostream& out, const SDCard::R1& r1)
{
    if (!(r1.is_valid() or r1.is_ready() or r1.is_in_idle_state())
	or r1.is_an_error()){
	atd::print(out, msg_r1);
	atd::print_int_as_hex(out, r1.data);
	atd::print(out, msg_error1);
	print(out, r1);
	return;
    }
}


void print_raw_R3(std::ostream& out, const SDCard::R3& r3)
{
    atd::print(out, msg_r3);
    atd::print_int_as_hex(out, r3.OCR[0]);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[1], false);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[2], false);
    out << ' '; atd::print_int_as_hex(out, r3.OCR[3], false);
    out << '\n';
}

void print_type_card(std::ostream& out, const SDCard::R3& r3)
{
    using R3 = SDCard::R3;

    atd::print(out, msg_r3_response);
    
    print_if_error_r1(out, r3.r1);
    print_raw_R3(out, r3);

    atd::print(out, msg_card_has_finished_power_up);
    if (r3.card_has_finished_power_up()){
	atd::print(out, msg_yes);
	out << '\n';

	atd::print(out, msg_card_type);
	if (r3.card_type() == R3::Type::SDSC)
	    atd::print(out, msg_SDSC);

	else
	    atd::print(out, msg_SDHC_or_SDXC);
	out << '\n';
    }
    else
        atd::print(out, msg_no);
}



void print(std::ostream& out, const SDCard::R3& r3)
{
//    atd::print(out, msg_r3_response);
//    
//    print_if_error_r1(out, r3.r1);
//    print_raw_R3(out, r3);
//
//    out << "\tCard has finished power up? ";
//    if (r3.card_has_finished_power_up()){
//	out << "yes\n";
//
//	out << "\tCard type : ";
//	if (r3.card_type() == R3::Type::SDSC)
//	    out << "SDSC\n";
//
//	else
//	    out << "SDSC\n";
//    }
//    else
//	out << "no\n";

    print_type_card(out, r3);

    out << '\n';
    print_question(out, msg_support_3_5_3_6V);
    print_bool_as_yes_no(out, r3.support_3_5_3_6V());

    print_question(out, msg_support_3_4_3_5V);
    print_bool_as_yes_no(out, r3.support_3_4_3_5V());

    print_question(out, msg_support_3_3_3_4V);
    print_bool_as_yes_no(out, r3.support_3_3_3_4V());

    print_question(out, msg_support_3_2_3_3V);
    print_bool_as_yes_no(out, r3.support_3_2_3_3V());

    print_question(out, msg_support_3_1_3_2V);
    print_bool_as_yes_no(out, r3.support_3_1_3_2V());

    print_question(out, msg_support_3_0_3_1V);
    print_bool_as_yes_no(out, r3.support_3_0_3_1V());

    print_question(out, msg_support_2_9_3_0V);
    print_bool_as_yes_no(out, r3.support_2_9_3_0V());

    print_question(out, msg_support_2_8_2_9V);
    print_bool_as_yes_no(out, r3.support_2_8_2_9V());

    print_question(out, msg_support_2_7_2_8V);
    print_bool_as_yes_no(out, r3.support_2_7_2_8V());

}


void print(std::ostream& out, const SDCard::R7& r7)
{
    using R7 = SDCard::R7;

    atd::print(out, msg_r7_response);

    print_if_error_r1(out, r7.r1);

    atd::print(out, msg_r7);
    atd::print_int_as_hex(out, r7.data[0]);
    out << ' '; atd::print_int_as_hex(out, r7.data[1], false);
    out << ' '; atd::print_int_as_hex(out, r7.data[2], false);
    out << ' '; atd::print_int_as_hex(out, r7.data[3], false);
    out << '\n';

    atd::print(out, msg_version);
    out << (uint16_t) r7.version() << '\n';
    atd::print(out, msg_voltage);

    switch (r7.voltage()){
	break; case R7::Voltage::not_defined: 
		    atd::print(out, msg_not_defined);

	break; case R7::Voltage::v2_7_to_3_6_V: 
		    atd::print(out, msg_from_2_7_to_3_6V);

	break; case R7::Voltage::low_voltage: 
		    atd::print(out, msg_low_voltage);

	break; case R7::Voltage::reserved: 
		    atd::print(out, msg_reserved);
    }

    out << '\n';

    atd::print(out, msg_pattern);
    atd::print_int_as_hex(out,  r7.pattern());
    out << '\n';
}


void print(std::ostream& out, const SDCard::Read_return& r)
{
    atd::print(out, msg_read_return_response);

    print_question(out, msg_ok);
    print_bool_as_yes_no(out, r.ok());

    print_question(out, msg_timeout);
    print_bool_as_yes_no(out, r.timeout());

    print_question(out, msg_r1_ok);
    print_bool_as_yes_no(out, !r.r1_fail());
    if (r.r1_fail()){
	print(out, r.r1());
	return;
    }

    print_question(out, msg_error);
    print_bool_as_yes_no(out, r.error());

    print_question(out, msg_CC_error);
    print_bool_as_yes_no(out, r.CC_error());

    print_question(out, msg_card_ECC_failed);
    print_bool_as_yes_no(out, r.card_ECC_failed());

    print_question(out, msg_out_of_range);
    print_bool_as_yes_no(out, r.out_of_range());
}


void print(std::ostream& out, const SDCard::R2& r)
{
    print(out, r.r1);

    atd::print(out, msg_r2_response);
    atd::print_int_as_hex(out, r.r2);
    out << '\n';

    print_question(out, msg_card_is_locked);
    print_bool_as_yes_no(out, r.card_is_locked());

    print_question(out, msg_write_protect_erase_skip);
    print_bool_as_yes_no(out, r.write_protect_erase_skip());

    print_question(out, msg_error);
    print_bool_as_yes_no(out, r.error());

    print_question(out, msg_CC_error);
    print_bool_as_yes_no(out, r.CC_error());

    print_question(out, msg_card_ECC_failed);
    print_bool_as_yes_no(out, r.card_ECC_failed());

    print_question(out, msg_write_protect_violation);
    print_bool_as_yes_no(out, r.write_protect_violation());

    print_question(out, msg_erase_param);
    print_bool_as_yes_no(out, r.erase_param());

    print_question(out, msg_out_of_range);
    print_bool_as_yes_no(out, r.out_of_range());

}

void print(std::ostream& out, const SDCard::Data_response_token& r)
{
    atd::print(out, msg_data_response_token);
    atd::print_int_as_hex(out, r.value());
    out << '\n';
    
    print_question(out, msg_is_valid);
    print_bool_as_yes_no(out, r.is_valid());

    print_question(out, msg_data_accepted);
    print_bool_as_yes_no(out, r.data_accepted());

    print_question(out, msg_data_rejected_CRC_error);
    print_bool_as_yes_no(out, r.data_rejected_CRC_error());

    print_question(out, msg_data_rejected_write_error);
    print_bool_as_yes_no(out, r.data_rejected_write_error());

    print_question(out, msg_timeout);
    print_bool_as_yes_no(out, r.timeout());
}


void print(std::ostream& out, const SDCard::Write_return& r)
{
    atd::print(out, msg_write_return_response);

    if (r.ok()){
	atd::print(out, msg_write_ok);
	return;
    }

    if (!r.r1().ok()){
	print(out, r.r1());
	return;
    }

    if (!r.data_response_token().ok()){
	print(out, r.data_response_token());
	return;
    }

// si llega aquí es porque R2 es el que ha fallado
    print(out, r.r2());
}

