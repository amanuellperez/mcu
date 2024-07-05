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
#include <avr_timer1.h>
#include <avr_timer0.h>
#include <avr_time.h>
#include <avr_types.h>
#include <avr_UART_iostream.h>



using namespace avr::literals;

template <typename Timern>
struct Main{
    using SW_gen = dev::Square_wave_generator<Timern>;

    void run();
    void debug();

    static void print_info_detail(uint16_t period_in_us);

    static uint16_t select_period_1MHz();
    static void timer_on_1MHz(uint16_t period_in_us);
};

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

template <typename Timer>
void Main<Timer>::run()
{
    avr::UART_iostream uart;

    uart << "\n\nFrecuencias generadas (a 1MHz el micro)\n"
	        "---------------------------------------\n";
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

    // debug();

    char c{};
    uart << "\n\nPress a key to continue\n";
    uart >> c;
}


template <typename Timer>
void Main<Timer>::debug()
{
    avr::UART_iostream uart;

    using GT           = gen::Generic_timer<Timer>;
    using counter_type = typename GT::counter_type;
    using c2 = atd::same_type_with_double_bits<counter_type>;
    //using Frequency = avr::Frequency;
    using Frequency = atd::ENG_frequency<atd::Decimal<uint64_t,3>>;

    timer_on_1MHz(8);

    // Frequency cf = GT::clock_frequency();
    Frequency cf{125,3};
//    typename GT::counter_type top = GT::square_wave_max_top();
    c2 top   = std::numeric_limits<counter_type>::max();
    c2 den           = c2{2 * (1u + top)};
//    c2 den = 131072u;
    // Frequency res = cf / (2 * (1 + top));
    // Frequency res = cf / den;
    Frequency::Rep kk = atd::to_integer<Frequency::Rep>(131072);
    Frequency rep = cf / Frequency::Rep{131072};

    Frequency sca = cf / 131072;

    uart << "\ndebug"
	    "\n-----\n"
	    "clock_frequency = " << cf << "\n\n"
	    "min = " << cf << " / (2 * (1u + "
         << top << ")) = "
         << rep << '\n';
    uart << "sca = " << sca << '\n';
    uart << "den = " << den << '\n';
    uart << "kk = " << kk << '\n';
}


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\nBasic\n"
                "-----\n";

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


