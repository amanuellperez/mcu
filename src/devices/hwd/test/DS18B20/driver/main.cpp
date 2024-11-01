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

#include "../../../dev_DS18B20.h"

#include <avr_atmega.h>	
#include <mcu_one_wire.h>
#include <atd_ostream.h>

// Micro
// -----
namespace myu = atmega; 
using Micro = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART>;


// pins
constexpr uint8_t one_wire_pin = 15;

// One wire protocol
// -----------------
using Cfg = mcu::One_wire_cfg<myu::Micro, one_wire_pin>;
using One_wire = mcu::One_wire<Cfg>;
using Search = mcu::One_wire_search<Cfg>;


// Hwd Devices
// -----------
using Sensor = dev::DS18B20<Micro, One_wire>;



// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}


bool is_return_cmd_ok(Sensor::Result result)
{
    using Result = Sensor::Result;

    UART_iostream uart;
    switch(result){
	break; case Result::ok: 
			// uart << "Ok"; 
			return true;

	break; case Result::not_found:
			uart << "ERROR (device not found)\n";

	break; case Result::time_out:
			uart << "TIMEOUT\n";

	break; case Result::wrong_CRC:
			uart << "WRONG CRC\n";
    }

    return false;
}


void print(const Sensor::Scratchpad& s)
{
    UART_iostream uart;

    uart << "Scratchpad\n"
	    "\tTemperature (LSB MSB = 0x50 0x05)= ";

    atd::print_int_as_hex(uart, s[0]);
    uart << ' ';
    atd::print_int_as_hex(uart, s[1]);
    uart << " ---> ";
    auto T = s.temperature();
    uart << T << "ºC";
    

    uart << "\n\tTH = " << (int) s.TH() << "ºC (";
    atd::print_int_as_hex(uart, s[2]);
    uart << ")\n\tTL = " << (int) s.TL() << "ºC (";
    atd::print_int_as_hex(uart, s[3]);
    uart << ")\n\tConfiguration register(";
    atd::print_int_as_hex(uart, s[4]);
    uart << "): resolution of ";
    switch (s.resolution()){
	using Res = Sensor::Scratchpad::Resolution;
	break; case Res::bits_9: uart << "9";
	break; case Res::bits_10: uart << "10";
	break; case Res::bits_11: uart << "11";
	break; case Res::bits_12: uart << "12";
    }
    uart << " bits";

    uart << "\n\tReserved (0xFF 0x?? 0x10) = ";
    atd::print_int_as_hex(uart, s[5]);
    uart << ' ';
    atd::print_int_as_hex(uart, s[6]);
    uart << ' ';
    atd::print_int_as_hex(uart, s[7]);
    uart << "\n\tCRC = ";
    atd::print_int_as_hex(uart, s.CRC());
    uart << " (";
    if (s.is_CRC_ok())
	uart << "OK";
    else
	uart << "WRONG";

    uart << ")\n";

}


// main
// ----
class Main{
public:
    Main();
    void run();

private:
// Hardware
    Sensor sensor_;

// Functions
    void print_menu(bool all_options) const;
    void bind_device();
    void convert_T() const;
    void read_scratchpad() const;
    void write_scratchpad() const;
    void copy_scratchpad() const;
    void recall_e2() const;
    void read_temperature();

    static constexpr uint16_t time_out_max = 3000;
};



// Functions
// ---------
void Main::print_menu(bool all_options) const
{
    UART_iostream uart;

    uart << "\n\nMenu\n"
	      "----\n"
	    "0. bind sensor\n";
    if (all_options)
	uart << 
	    "1. convert T\n"
	    "2. write scratchpad\n"
	    "3. read scratchpad\n"
	    "4. copy scratchpad\n"
	    "5. recall scratchpad\n"
	    "6. read temperature\n";


    uart << "----------------\n";
}

void Main::bind_device()
{
    UART_iostream uart;
    uart << "Binding ... ";
    
    Search search;
    Sensor::Device dev;
    if(search(dev)){
	sensor_.bind(dev);
	uart << "OK\n"
	        "Devide ROM: ";
	mcu::print_rom_as_hex(uart, dev);
	uart << '\n';
	

    }

    else
	uart << "ERROR. No device found\n";

}


void Main::convert_T() const
{
    UART_iostream uart;
    
    uart << "Sending convert_T cmd ... ";
    auto result = sensor_.convert_T();
    
    if (!is_return_cmd_ok(result))
	return;

    result = sensor_.wait_until(time_out_max);
    
    if (!is_return_cmd_ok(result))
	return;

    uart << "OK\n";
}


void Main::read_scratchpad() const
{
    UART_iostream uart;
    
    uart << "Sending `read scractpad` cmd ... ";
    Sensor::Scratchpad s;
    
    auto result = sensor_.read_scratchpad(s);

    if (!is_return_cmd_ok(result))
	return;

    if (s.is_CRC_ok()){
	uart << "OK\n";
	print(s);
    }
    else
	uart << "WRONG CRC\n";
}

void Main::write_scratchpad() const
{
    UART_iostream uart;
    
    uart << "Write to scratchpad:\n"
	    "\tTH = ";

    int16_t TH = 0;
    uart >> TH;

    uart << "\n\tTL = ";
    int16_t TL = 0;
    uart >> TL;

    uart << "\n\tResolution: choose 9, 10, 11 or 12 bits: ";
    uint16_t ures = 0;
    uart >> ures;

    using Resolution = Sensor::Resolution;
    Resolution res;

    switch(ures){
	break; case 9: res = Resolution::bits_9;
	break; case 10: res = Resolution::bits_10;
	break; case 11: res = Resolution::bits_11;
	break; case 12: res = Resolution::bits_12;
	break; default: return;
    }

    uart << "\nSending write_scratchpad(" << TH 
				  << ", " << TL 
				  << ", " << (int) ures << ") ... ";
    
    auto result = sensor_.write_scratchpad((uint8_t)TH, (uint8_t)TL, res);

    if (!is_return_cmd_ok(result))
	return;

    uart << "OK\n";
    
}


void Main::copy_scratchpad() const
{
    UART_iostream uart;
    
    uart << "Sending `copy scractpad` cmd ... ";

    auto result = sensor_.copy_scratchpad();

    if (!is_return_cmd_ok(result))
	return;

    uart << "OK\n";

}

void Main::recall_e2() const
{
    UART_iostream uart;
    
    uart << "Sending `recall2` cmd ... ";
    
    auto result = sensor_.recall_e2(); 

    if (!is_return_cmd_ok(result))
	return;

    result = sensor_.wait_until(time_out_max);
    
    if (!is_return_cmd_ok(result))
	return;

    uart << "OK\n";

}

void Main::read_temperature()
{
    UART_iostream uart;
    
    uart << "Reading temperature ... ";
    auto T = sensor_.read_temperature(time_out_max);

    if (!is_return_cmd_ok(sensor_.result_last_operation()))
	return;


    atd::Kelvin<atd::Float32> K = T;
    atd::Fahrenheit<atd::Float32> F = T;
    uart << "OK\n"
	    "T = " << T << "\n"
	    "    " << K << "\n"
	    "    " << F << '\n';



}

Main::Main()
{
    init_uart();
}



void Main::run()
{
    UART_iostream uart;

    uart << "\n\nDS18B20 driver test\n"
	        "-------------------\n"
		"1. Connect devices to pin" << (int) one_wire_pin 
		<< " with a pull-up resistor of 4'7k\n"
		"2. Connect all devices to power supply\n"
		"   (don't use parasite power)\n";


    bool all_options = false;

    while(1){
	print_menu(all_options);

	char ans{};
	uart >> ans;

	if (ans == '0') {
	    bind_device();
	    all_options = true;
	}

	else if (all_options){
	    switch(ans){
		break; case '1': convert_T();
		break; case '2': write_scratchpad();
		break; case '3': read_scratchpad();
		break; case '4': copy_scratchpad();
		break; case '5': recall_e2();
		break; case '6': read_temperature();

		break; default: uart << "Unknown option\n";
	    }
	}

    }
}



int main()
{
    Main app;
    app.run();
}


