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

#include "../../../avr_signal_generator.h"
#include "../../../avr_timer1_generic.h"
#include "../../../avr_time.h"
#include "../../../avr_types.h"
#include "../../../avr_UART_iostream.h"


using SW_gen = avr::Square_wave_generator<avr::Timer1>;



void print(const avr::Microsecond& t)
{
    avr::UART_iostream uart;
    if (t < avr::Microsecond{1000})
	uart << t << " us";

    else if (t < avr::Millisecond{1000})
	uart << avr::Millisecond{t} << " ms";

    else 
	uart << avr::Second{t} << " s";

}

void print(const avr::Hertz& f)
{
    avr::UART_iostream uart;
    if (f < avr::Hertz{1000})
	uart << f << " Hz";

    else if (f < avr::KiloHertz{1000})
	uart << avr::KiloHertz{f} << " KHz";

    else 
	uart << avr::MegaHertz{f} << " MHz";
}


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
	    SW_gen::ch1_on();
	    ch1_on_ = true;
	    break;

	case 'f':
	    SW_gen::ch1_off();
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
	    SW_gen::ch2_on();
	    ch2_on_ = true;
	    break;

	case 'f':
	    SW_gen::ch2_off();
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
	case 1: SW_gen::on<1>(); break;
	case 8: SW_gen::on<8>(); break;
	case 64: SW_gen::on<64>(); break;
	case 256: SW_gen::on<256>(); break;
	case 1024: SW_gen::on<1024>(); break;
	default:
	    uart << "period_in_us [" << period_in_us << "] no válido\n";
	    break;
    }
}


void Main::on()
{
    timer_on_1MHz(period_in_us_);
}

void debug_freq(avr::Hertz freq_sq)
{
    avr::UART_iostream uart;
    uart << "\ndebug_freq: "
	    "\nTimer::clock_frequency() / 2*freq_sq - 1ul = ";
    uart << avr::Timer1::clock_frequency();
    uart << " / " << freq_sq << " - 1\n\t top = ";
    avr::Hertz::Scalar two = 2;
    auto top = (avr::Timer1::clock_frequency() / (two * freq_sq)) - 1ul;
    uart << top << '\n';

}

void Main::run()
{
    avr::UART_iostream uart;

    uart << "\n\nSquare_wave_generator\n"
                "---------------------\n"
            "Connect oscilloscope to pins:\n"
            "channel 1 = pin " << uint16_t{SW_gen::pin_channel1} << '\n'
         << "channel 2 = pin " << uint16_t{SW_gen::pin_channel2} 
	 << "\n\nPress a key to continue\n";

	char c{};
	uart >> c;

    while(1){
	uart << "\n\nState"
	          "\n-----"
		"\nTimer: period  = ";
	print(avr::Timer1::clock_period());

	uart << "; frequency  = ";
	print(avr::Timer1::clock_frequency());

	uart << "\n\nGenerated signal:"
	        "\n    frequency  = ";
	print(SW_gen::frequency());

	uart << "\n    ch1 " << (ch1_on_? "on": "off") 
	     << "\n    ch2 " << (ch2_on_? "on": "off") 
	     << '\n';

	uart << "\n\nMenu:\n"
	        "Timer [p]eriod\n"
	        "o[f]f\n"
		"[o]n\n"
		"[s]elect frequency\n"
		"ch[1] menu\n"
		"ch[2] menu\n";

	uint32_t num;
	uart >> c;
	switch(c){
	    case 'p':
		period_in_us_ = select_period_1MHz();
		SW_gen::off();
		timer_on_1MHz(period_in_us_);
		break;

	    case 'f':
		SW_gen::off();
		break;
	    case 'o':
		on();
		break;

	    case 's': 
		uart << "\nfreq (in Hz, máx " << SW_gen::max_frequency() << " no mas de 2^16 = 65500) = ";
		uart >> num;
		uart << num << '\n';
		freq_ = avr::Hertz{num};
		SW_gen::frequency(freq_);
		debug_freq(freq_);
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

