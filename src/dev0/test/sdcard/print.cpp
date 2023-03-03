#include "print.h"

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


