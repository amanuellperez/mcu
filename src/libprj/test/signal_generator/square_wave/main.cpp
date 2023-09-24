// Copyright (C) 2021 Manuel Perez 
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

#include "../../../dev_signal_generator.h"
#include <avr_timer1_generic.h>
#include <avr_timer0_generic.h>
#include <avr_time.h>
#include <avr_types.h>
#include <avr_UART_iostream.h>



using namespace avr::literals;

template <typename Timern>
struct Main{
    using SW_gen = dev::Square_wave_generator<Timern>;

    Main();

    void run();

    void on();
    void ch1_menu();
    void ch2_menu();

    void info();
    static void print_info_detail(uint16_t period_in_us);

    static uint16_t select_period_1MHz();
    static void timer_on_1MHz(uint16_t period_in_us);

// data
    avr::Frequency freq_ = 100_Hz;
    bool ch1_on_ = false;
    bool ch2_on_ = false;
    uint16_t period_in_us_ = 1;
};

template <typename T>
void Main<T>::ch1_menu()
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



template <typename T>
void Main<T>::ch2_menu()
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
		
template <typename T>
uint16_t Main<T>::select_period_1MHz()
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

template <typename T>
void Main<T>::timer_on_1MHz(uint16_t period_in_us)
{
    avr::UART_iostream uart;

    switch(period_in_us){
	case 1: SW_gen::template on<1>(); break;
	case 8: SW_gen::template on<8>(); break;
	case 64: SW_gen::template on<64>(); break;
	case 256: SW_gen::template on<256>(); break;
	case 1024: SW_gen::template on<1024>(); break;
	default:
	    uart << "period_in_us [" << period_in_us << "] no válido\n";
	    break;
    }
}

template <typename Timer>
void Main<Timer>::print_info_detail(uint16_t period_in_us)
{
    avr::UART_iostream uart;

    uart << period_in_us << "\t\t|" << 
	SW_gen::clock_frequency() << "\t|" << 
	SW_gen::min_frequency() << "\t|" << 
	SW_gen::max_frequency() << '\n';
}

template <typename T>
void Main<T>::info()
{
    avr::UART_iostream uart;

    uart << "\n\nFrecuencias generadas (a 1MHz el micro)\n"
	        "---------------------------------------\n";

    SW_gen::ch1_off();
    SW_gen::ch2_off();


    uart << "period (in us)\t| freq\t\t| min freq\t| max freq\n";
    timer_on_1MHz(1);
    print_info_detail(1);
    timer_on_1MHz(8);
    print_info_detail(8);
    timer_on_1MHz(64);
    print_info_detail(64);
    timer_on_1MHz(256);
    print_info_detail(256);
    timer_on_1MHz(1024);
    print_info_detail(1024);

    char c{};
    uart << "\n\nPress a key to continue\n";
    uart >> c;

    // lo dejamos como estaba
    timer_on_1MHz(period_in_us_);
}

template <typename T>
void Main<T>::on()
{
    timer_on_1MHz(period_in_us_);
}

template <typename Timer>
void debug_freq(const avr::Frequency& freq_sq)
{
    using Rep = avr::Frequency::Rep;
    avr::UART_iostream uart;
    uart << "\ndebug_freq: "
	    "\ntop = Timer::clock_frequency() / 2*freq_sq - 1ul\n"
	      "    = ";
    uart << Timer::clock_frequency();
    uart << " / " << freq_sq << " - 1\n"
	    "   = ";
    auto top = (Timer::clock_frequency() / (2 * freq_sq)) - Rep{1};
    uart << top << '\n';

}

template <typename Timer>
void Main<Timer>::run()
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
                "\nTimer: period = "
             << Timer::clock_period();

        uart << "; frequency  = " << Timer::clock_frequency();

        uart << "\n\nGenerated signal:"
                "\n    frequency  = " << SW_gen::frequency();

        uart << "\n    ch1 " << (ch1_on_? "on": "off") 
	     << "\n    ch2 " << (ch2_on_? "on": "off") 
	     << '\n';

	uart << "\n\nMenu:\n"
	        "Timer [p]eriod\n"
	        "o[f]f\n"
		"[o]n\n"
		"[s]elect frequency\n"
		"ch[1] menu\n"
		"ch[2] menu\n"
		"[i]nfo\n"
		"[b]ack to main menu\n";

	uint32_t num;
	uart >> c;
	switch(c){
	    case 'i':
		info();
		break;

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
                uart << "\nfreq (in Hz, from " << SW_gen::min_frequency()
                     << " to " << SW_gen::max_frequency() << ") = ";
                uart >> num;
		uart << num << '\n';
		freq_ = avr::Frequency{num, 0};
		SW_gen::frequency(freq_);
		debug_freq<Timer>(freq_);
		break;

	    case '1':
		ch1_menu();
		break;

	    case '2':
		ch2_menu();
		break;

	    case 'b':
		return;

	}


    }
}

template <typename T>
Main<T>::Main()
{
// init_signal_generator()
    timer_on_1MHz(period_in_us_);
}


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\nSquare_wave_generator\n"
                "---------------------\n";

    while (1){
	uart << "\n\nSelect timer to test:\n"
		"[0] - Timer 0\n"
		"[1] - Timer 1\n";

	char c{};
	uart >> c;
	if (c == '0'){
	    Main<avr::Timer0> app;
	    app.run();
	}
	else {
	    Main<avr::Timer1> app;
	    app.run();
	}
    }
}


