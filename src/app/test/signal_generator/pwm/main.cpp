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
    using PW_gen = dev::PWM_generator<Timern>;

    Main();

    void run();

    void on();
    void select_mode();
    void ch1_menu();
    void ch2_menu();

    static uint16_t select_period_1MHz();
    static void timer_on_1MHz(uint16_t period_in_us);

    void print_mode();

// data
    avr::Frequency freq_ = 100_Hz;
    bool ch1_on_ = false;
    bool ch2_on_ = false;
    uint16_t period_in_us_ = 1;
    enum class Mode { fix_frequency, only_channel2, both_channels, none};
    Mode mode_ = Mode::none;
};

template <typename T>
void Main<T>::print_mode()
{
    avr::UART_iostream uart;
    switch(mode_){
	case Mode::fix_frequency:
	    uart << "fix_frequency";
	    break;

	case Mode::only_channel2:
	    uart << "only_channel2";
	    break;

	case Mode::both_channels:
	    uart << "both_channels";
	    break;

	case Mode::none:
	    uart << "none";
	    break;
    }
}

template <>
void Main<avr::Timer1>::select_mode()
{
    avr::UART_iostream uart;
    uart << "\n\nMode:\n"
	    "[0] - fix 0x00FF\n"
	    "[1] - fix 0x01FF\n"
	    "[3] - fix 0x03FF\n"
	    "[b]oth channels\n"
	    "[o]nly channel 2\n";

    char c{};
    uart >> c;
    switch(c){
	case '0':
	    PW_gen::template mode_fix_frequency<0x00FF>();
	    mode_ = Mode::fix_frequency;
	    break;

	case '1':
	    PW_gen::template mode_fix_frequency<0x01FF>();
	    mode_ = Mode::fix_frequency;
	    break;

	case '3':
	    PW_gen::template mode_fix_frequency<0x03FF>();
	    mode_ = Mode::fix_frequency;
	    break;

	case 'b':
	    PW_gen::mode_variable_pwm_both_channels();
	    mode_ = Mode::both_channels;
	    break;

	case 'o':
	    PW_gen::mode_variable_pwm_only_channel2();
	    mode_ = Mode::only_channel2;
	    break;
    }

}

template <>
void Main<avr::Timer0>::select_mode()
{
    avr::UART_iostream uart;
    uart << "\n\nMode:\n"
	    "[0] - fix 0xFF\n"
	    "[o]nly channel 2\n";

    char c{};
    uart >> c;
    switch(c){
	case '0':
	    PW_gen::template mode_fix_frequency<0xFF>();
	    mode_ = Mode::fix_frequency;
	    break;

	case 'o':
	    PW_gen::mode_variable_pwm_only_channel2();
	    mode_ = Mode::only_channel2;
	    break;
    }

}

template <typename T>
void Main<T>::ch1_menu()
{
    avr::UART_iostream uart;
    uart << "\n\nchannel 1:\n"
	    "[p] - duty cycle (as %)\n"
	    "[m] - duty cycle in microseconds\n"
	    "[i]nverting mode (turn on ch1)\n"
	    "[n]on inverting mode (turn on ch1)\n"
	    "o[f]f\n";

    uint16_t tmp{};
    char c{};
    uart >> c;
    switch(c){
	case 'p':
	    uart << "duty_cycle  (0 to 100) = ";
	    uart >> tmp;
	    PW_gen::ch1_duty_cycle(tmp);
	    break;

	case 'm':
	    uart << "duty_cycle  (in microseconds) = ";
	    uart >> tmp;
	    PW_gen::ch1_duty_cycle(typename avr::Time{tmp, -6});
	    break;


	case 'i':
	    PW_gen::ch1_inverting_mode();
	    ch1_on_ = true;
	    break;

	case 'n':
	    PW_gen::ch1_non_inverting_mode();
	    ch1_on_ = true;
	    break;

	case 'f':
	    PW_gen::ch1_off();
	    ch1_on_ = false;
	    break;

    }  

}


template <typename T>
void Main<T>::ch2_menu()
{
    avr::UART_iostream uart;
    uart << "\n\nchannel 2:\n"
	    "[p] - duty cycle (as %)\n"
	    "[m] - duty cycle in microseconds\n"
	    "[i]nverting mode\n"
	    "[n]on inverting mode\n"
	    "o[f]f\n";

    uint16_t tmp{};
    char c{};
    uart >> c;
    switch(c){
	case 'p':
	    uart << "duty_cycle  (0 to 100) = ";
	    uart >> tmp;
	    PW_gen::ch2_duty_cycle(tmp);
	    break;

	case 'm':
	    uart << "duty_cycle  (in microseconds) = ";
	    uart >> tmp;
	    PW_gen::ch2_duty_cycle(avr::Time{tmp, -6});
	    break;

	case 'i':
	    PW_gen::ch2_inverting_mode();
	    ch2_on_ = true;
	    break;

	case 'n':
	    PW_gen::ch2_non_inverting_mode();
	    ch2_on_ = true;
	    break;

	case 'f':
	    PW_gen::ch2_off();
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
	case 1: PW_gen::template on<1>(); break;
	case 8: PW_gen::template on<8>(); break;
	case 64: PW_gen::template on<64>(); break;
	case 256: PW_gen::template on<256>(); break;
	case 1024: PW_gen::template on<1024>(); break;
	default:
	    uart << "period_in_us [" << period_in_us << "] no válido\n";
	    break;
    }
}


template <typename T>
void Main<T>::on()
{
    timer_on_1MHz(period_in_us_);
}

void debug_freq(avr::Frequency freq_sq)
{
    avr::UART_iostream uart;
    uart << "\ndebug_freq: "
	    "\nTimer::clock_frequency() / freq_sq - 1ul = ";
    uart << avr::Timer1::clock_frequency();
    uart << " / " << freq_sq << " - 1\n\t top = ";
    auto top = (avr::Timer1::clock_frequency() / freq_sq) - 1ul;
    uart << top << '\n';


}


template <typename T>
void Main<T>::run()
{
    avr::UART_iostream uart;

    uart << "\n\nPWM generator\n"
                "-------------\n"
            "Connect oscilloscope to pins:\n"
            "channel 1 = pin " << uint16_t{PW_gen::pin_channel1} << '\n'
         << "channel 2 = pin " << uint16_t{PW_gen::pin_channel2} <<
	    "\n\nPress a key to continue\n";
    char c{};
    uart >> c;

    while(1){
	uart << "\n\nState"
	          "\n-----"
		"\nTimer: period = "
	<< PW_gen::clock_period();
	uart << "; frequency  = "
	<< PW_gen::clock_frequency();

	uart << "\n\nGenerated signal:"
		"\n    mode = ";
	print_mode();
	uart << "\n    frequency = "
		<< PW_gen::frequency();
	uart << "; period = "
	<< PW_gen::period();

	uart << "\n\n channel |    on/off    |    duty_cycle"
	          "\n---------+--------------+-----------------"
	          "\n   ch1   |     " << (ch1_on_? "on": "off") 
	     << "       |    "
	<< PW_gen::ch1_duty_cycle();

	uart << "\n   ch2   |     " << (ch2_on_? "on": "off") 
	     << "       |    "
	<< PW_gen::ch2_duty_cycle();

	uart << "\n\n\nMenu:\n"
	        "[t]imer period\n"
	        "o[f]f\n"
		"[o]n\n"
		"[m]ode\n"
		"[s]elect signal frequency (only mode != fix_frequency)\n"
		"select signal [p]eriod (only mode != fix_frequency)\n"
		"ch[1] menu\n"
		"ch[2] menu\n"
		"[b]ack main menu\n";

	uint32_t num;
	uart >> c;
	switch(c){
	    case 't':
		period_in_us_ = select_period_1MHz();
		PW_gen::off();
		timer_on_1MHz(period_in_us_);
		break;

	    case 'f':
		PW_gen::off();
		break;
	    case 'o':
		on();
		break;

	    case 'm':
		select_mode();
		break;

	    case 'p':
		uart << "\nperiod (in us) = ";
		uart >> num;
		uart << num << '\n';
		PW_gen::period(avr::Time{num, -6});
		freq_ = avr::Time::Rep{1} / avr::Time{num, -6};
		break;

	    case 's': 
		uart << "\nfreq (in Hz) = ";
		uart >> num;
		uart << num << '\n';
		freq_ = avr::Frequency{num, 0};
		PW_gen::frequency(freq_);
		//debug_freq(freq_);
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
    PW_gen::frequency(freq_);
}


int main()
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

    uart << "\n\nPWM generator\n"
                "-------------\n";

    while(1){
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

