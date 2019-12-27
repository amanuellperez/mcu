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

#include "avr_TWI.h"

namespace avr{


bool TWI::receive_data_with_ACK(volatile std::byte& x)
{
    TWCR =  (1 << TWEN)|	// enable(); (ver datasheet table 26-2)
	    (1 << TWINT)| 	// start_next_operation();
	    (1 << TWEA);	// generate_ACK();

    if (answer_receive_data_with_ACK_fail())
	return false;

    x = std::byte{TWDR};
    return true;
}


bool TWI::receive_data_with_NACK(volatile std::byte& x)
{
    TWCR =	(1 << TWEN)|	// enable(); (ver datasheet table 26-2)
	    (1 << TWINT); 	// start_next_operation();

    if (answer_receive_data_with_NACK_fail())
	return false;

    x = std::byte{TWDR};

    return true;
}


uint8_t TWI::send(uint8_t address, std::byte x)
{
    send_byte(std::byte{address} << 1);    // address + write
    if (answer_send_address_NACK())
	return 0;

    send_byte(x);
    if (answer_send_data_NACK())
	return 0;

    return 1;
}


uint8_t TWI::receive(uint8_t address, volatile std::byte* data, uint8_t n)
{
    send_byte((std::byte{address} << 1) | std::byte{0x01}); // address + read 

    if (answer_receive_address_NACK())
	return 0;

    for (uint8_t i = 0; i < n - 1; ++i, ++data){
	if(!receive_data_with_ACK(*data))
	    return i;
    }

    if(!receive_data_with_NACK(*data))
	return n - 1;

    return n;
}


}// namespace

