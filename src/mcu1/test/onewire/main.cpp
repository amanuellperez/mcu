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

#include <avr_atmega.h>	// TODO: orden!!!
namespace mcu = atmega; // TODO: orden
#include <atd_ostream.h>
#include "../../dev_one_wire.h"

// Devices
// -------

constexpr uint8_t test_pin = 15;

using Pin = mcu::Pin<test_pin>;
using Cfg = dev::One_wire_cfg<mcu::Micro, Pin>;

using One_wire = dev::One_wire<Cfg>;
using Search = dev::One_wire_search<Cfg>;



// >>> BORRAME
unsigned char ROM_NO[8];
uint8_t LastDiscrepancy;
uint8_t LastFamilyDiscrepancy;
bool LastDeviceFlag;

bool tmp_search()
{
    mcu::UART_iostream uart;

   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number;
   bool    search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = false;

   // if the last call was not the last one
   if (!LastDeviceFlag) {
       uart << "\treset ... ";
      // 1-Wire reset
      if (!One_wire::reset()) {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = false;
         LastFamilyDiscrepancy = 0;
	 uart << "FAIL\n";
         return false;
      }
    uart << "OK\n";

      One_wire::write(0xF0);   // NORMAL SEARCH

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = One_wire::read_bit();
         cmp_id_bit = One_wire::read_bit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1)) {
	     uart << "\t0x11!!!!\n";
            break;
         } else {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit) {
               search_direction = id_bit;  // bit write value for search
            } else {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy) {
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               } else {
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);
               }
               // if 0 was picked then record its position in LastZero
               if (search_direction == 0) {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
	    One_wire::write_bit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0) {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65)) {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0) {
            LastDeviceFlag = true;
         }
         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0]) {
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      search_result = false;
   } else {
	for (uint8_t i = 0; i < Search::ROM_size; ++i){
	    atd::print_int_as_hex(uart, ROM_NO[i]);
	    uart << ' ';
	}
	uart << '\n';
   }
   return search_result;
}
// <<< BORRAME


// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}



void print_menu()
{
    mcu::UART_iostream uart;

    uart << "\n----------------\n";

    

}

void DS18B20_test()
{
    mcu::UART_iostream uart;
    
    uart << "\nDS18B20 basic test\n"
	    "------------------\n"
	    "Basic test: connect only one DS18B20\n"
	    "Press a key to continue\n";

    char ans{};
    uart >> ans;



    // Read the contents of the scratchpad
    static constexpr uint8_t scratchpad_size = 9;
    uint8_t scratchpad[scratchpad_size];

    if (!One_wire::reset()){
	uart << "NO devices found!!!\n";
	return;
    }
    else
	uart << "Device detected\n";

    One_wire::skip_rom();
    One_wire::write(0xBE);
    for (uint8_t i = 0; i < scratchpad_size; ++i){
	scratchpad[i] = One_wire::read();
	atd::print_int_as_hex(uart, scratchpad[i]);
	uart << ' ';
    }

    uart << '\n';

    
    One_wire::reset();
    One_wire::write(0xCC); // Skip ROM command to select a single device

    uart << "Convert T command ... ";
    One_wire::write(0x44);
    while (One_wire::read_bit() == 0)
	uart << '.';

    uart << "OK\n";


}


void basic_test()
{
    mcu::UART_iostream uart;

    uart << "\nBasic test\n"
	      "----------\n"
	    "Connect the oscilloscope to pin " << (int) test_pin << '\n' <<
	    "1. Write ones bit (at most 15 us low)\n"
	    "2. Write zero bit (at least 60 us low)\n";

    char ans{};
    uart.get(ans);

    if (ans == '2'){
	uart << "Writing zeros\n";
	while(1) One_wire::write_bit_zero();
    }

    else{
	uart << "Writing ones\n";
	while(1) One_wire::write_bit_one();
    }

}

void search_test()
{
    mcu::UART_iostream uart;

    uart << "\nSearch test\n"
	      "-----------\n";

    Search search;
    if (search.begin()){
	uart << "Device found\n";
	const uint8_t* ROM = search.value();

	for (uint8_t i = 0; i < Search::ROM_size; ++i){
	    atd::print_int_as_hex(uart, ROM[i]);
	    uart << ' ';
	}
	uart << '\n';
    }

    else
	uart << "No device found\n";

    mcu::Micro::wait_ms(3000);
}

void search_test2()
{
    mcu::UART_iostream uart;
    uart << "\n\ntmp_search()\n"
	    "------------\n";
    if (tmp_search())
	uart << "device found\n";

    else 
	uart << "No device found\n";

    mcu::Micro::wait_ms(1000);

}

int main()
{
    init_uart();

    mcu::UART_iostream uart;

    uart << "\n\nOne wire test\n"
	        "-------------\n"
		"1. Connect devices to pin" << (int) test_pin 
		<< " with a pull-up resistor of 4'7k\n"
		"2. Connect all devices to power supply\n"
		"   (don't use parasite power)\n";


    while(1){
//	print_menu();
	// basic_test();
	search_test();
//	search_test2();
	// DS18B20_test();
    }
}




