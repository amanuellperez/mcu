// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../avr_signal_generator_sf.h"
#include "../../avr_time.h"
#include "../../avr_types.h"
#include "../../avr_UART_iostream.h"


using Signal_gen = avr::Signal_generator_sf;


struct Main{
    Main();

    void run();

    void on();
    void ch1_menu();
    void ch2_menu();

    static uint16_t select_period_1MHz();
    static void timer_on_1MHz(uint16_t period_in_us);

// data
    avr::Hertz freq_ {100};
    bool ch1_on_ = false;
    bool ch2_on_ = false;
    uint16_t period_in_us_ = 1;
};


void Main::ch1_menu()
{
    avr::UART_iostream uart;
    uart << "\n\nchannel 1:\n"
	    "[o]n\n"
	    "o[f]f\n";

    char c{};
    uart >> c;
    switch(c){
	case 'o':
	    Signal_gen::ch1_on();
	    ch1_on_ = true;
	    break;

	case 'f':
	    Signal_gen::ch1_off();
	    ch1_on_ = false;
	    break;

    }  
}



void Main::ch2_menu()
{
    avr::UART_iostream uart;
    uart << "\n\nchannel 2:\n"
	    "[o]n\n"
	    "o[f]f\n";

    char c{};
    uart >> c;
    switch(c){
	case 'o':
	    Signal_gen::ch2_on();
	    ch2_on_ = false;
	    break;

	case 'f':
	    Signal_gen::ch2_off();
	    ch2_on_ = false;
	    break;

    }  
}
		
uint16_t Main::select_period_1MHz()
{
    avr::UART_iostream uart;

    uart << "\n\nperiod_in_us (avr a 1MHz):\n"
	    "1\n"
	    "8\n"
	    "64\n"
	    "256\n"
	    "1024\n";

    uint16_t res{};
    uart >> res;
    switch(res){
	case 1:
	case 8:
	case 64:
	case 256:
	case 1024:
	    return res;
    }

    return 1;
}

void Main::timer_on_1MHz(uint16_t period_in_us)
{
    avr::UART_iostream uart;

    switch(period_in_us){
	case 1: Signal_gen::on<1>(); break;
	case 8: Signal_gen::on<8>(); break;
	case 64: Signal_gen::on<64>(); break;
	case 256: Signal_gen::on<256>(); break;
	case 1024: Signal_gen::on<1024>(); break;
	default:
	    uart << "period_in_us [" << period_in_us << "] no válido\n";
	    break;
    }
}


void Main::on()
{
    timer_on_1MHz(period_in_us_);
}

void Main::run()
{
    avr::UART_iostream uart;

    uart << "\n\nSignal_generator\n"
            "----------------\n"
            "Connect oscilloscope to pins:\n"
            "channel 1 = pin " << uint16_t{Signal_gen::pin_channel1} << '\n'
         << "channel 2 = pin " << uint16_t{Signal_gen::pin_channel2} << '\n';

    while(1){
	uart << "\n\nState"
	          "\n-----"
		"\ntimer period = " << period_in_us_ << " us"
	        "\nfrequency (wanted) = " << freq_ << " Hz"
	        "\nfrequency (real)   = " << Signal_gen::frequency() << " Hz"
		"\nch1 on = " << (ch1_on_? "yes": "no") 
	     << "\nch2 on = " << (ch2_on_? "yes": "no") 
	     << '\n';

	uart << "\n\nMenu:\n"
	        "signal generator [p]eriod\n"
	        "signal generator o[f]f\n"
		"signal generator [o]n\n"
		"[s]elect frequency\n"
		"ch[1] menu\n"
		"ch[2] menu\n";

	uint32_t num;
	char c{};
	uart >> c;
	switch(c){
	    case 'p':
		period_in_us_ = select_period_1MHz();
		Signal_gen::off();
		timer_on_1MHz(period_in_us_);
		Signal_gen::frequency(freq_); // actualizamos top con ese period
		break;

	    case 'f':
		Signal_gen::off();
		break;
	    case 'o':
		on();
		break;

	    case 's': 
		uart << "\nfreq (in Hz) = ";
		uart >> num;
		uart << num << '\n';
		freq_ = avr::Hertz{num};
		Signal_gen::frequency(freq_);
		break;

	    case '1':
		ch1_menu();
		break;

	    case '2':
		ch2_menu();
		break;

	}


    }
}

Main::Main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();


// init_signal_generator()
    timer_on_1MHz(period_in_us_);
}


int main()
{

    Main app;
    app.run();
}

