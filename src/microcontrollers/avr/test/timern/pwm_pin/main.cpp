// Copyright (C) 2024 Manuel Perez 
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_timer0.h"
#include "../../../avr_timer1.h"
#include "../../../avr_time.h"
#include "../../../avr_UART_iostream.h"


// El comportamiento del Timer0 varia dependiendo de si usamos
// los dos pines o solo el B. Con esta variable selecciono la opción.
constexpr bool timer0_both_pins = false;

// Microcontroller
// ---------------
namespace myu = avr_;

					 
// Hwd devices
// -----------
// DONT_COMPILE(using PWM_pin1 = myu::PWM0_pin<10>;)

using PWM_pin1 = myu::PWM0_pin<11>;
using PWM_pin2 = myu::PWM0_pin<12>;
// Este static assert es un comentario para el programador, para recordarle
// que el pin A de este micro es el número 12
static_assert(PWM_pin1::pin_number_A() == 12);

// DONT_COMPILE(using PWM_pin3 = myu::PWM1_pin<18>;)
using PWM_pin3 = myu::PWM1_pin<15>;
using PWM_pin4 = myu::PWM1_pin<16>;

// Cfg
// ---
using namespace myu::literals;


// FUNCTIONS
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg(uart);
    uart.turn_on();
}

void init_timers()
{
    PWM_pin1::init(); 

    if constexpr (timer0_both_pins) 
	PWM_pin2::init();
    PWM_pin3::init();
    PWM_pin4::init();
}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\n**************************************************\n"
	        "                  PWM_pin test\n"
                "**************************************************\n";
    if constexpr (timer0_both_pins){
	uart << "Timer0 both pins working. In this mode, both pins\n"
	        "work but you only can select a limit set of\n"
		"frequencies to generate. To test only pin B change\n"
		"variable `timer0_both_pins` and recompile.\n";
    }
    else {
	uart << "Timer0 only pin B works. In this mode, only pin\n"
		"B works, but you can select a lot of frequencies\n"
		"to generate. To test both pins, change variable\n"
		"`timer0_both_pins` and recompile.\n";
    }
    uart << "**************************************************\n";

    uart << "To test\n"
	    "\tTimer0 connect oscilloscope to pins "
         << uint16_t{PWM_pin1::number} 
	 << " and " << uint16_t{PWM_pin2::number}
	 << "\nand to test\n"
	     "\tTimer1 connect it to pins "
         << uint16_t{PWM_pin3::number} 
	 << " and " << uint16_t{PWM_pin4::number}
         << '\n';
}


template <typename PWM_mode>
void print(std::ostream& out, 
	   const PWM_mode& pwm,
	   const myu::Frequency& freq_clk)
{
    if (pwm.fast_mode){
	out << " -> " << pwm.frequency_fast_mode(freq_clk);
	out << "\tFAST";
	    
    }
    else{
	out << " -> " << pwm.frequency_phase_mode(freq_clk);
	out << "\tPHASE";
    }

    out << pwm.prescaler << '\t'
	<< pwm.top << '\t';

}


void pwm_mode_test()
{
    myu::UART_iostream uart;
    uart << "\nPWM_mode test\n"
	    "-------------\n";

    myu::Frequency freq_clk = 1_MHz;

    myu::timer1_::PWM_mode pwm;

    for (myu::Frequency fg = 20_Hz; fg < 100_Hz; fg += 1_Hz){
	pwm.calculate_cfg_method2(freq_clk, fg);
	uart << fg << ": ";
	print(uart, pwm, freq_clk);
	uart << '\n';
    }

}

template <typename PWM_pin>
void print_debug()
{
    myu::UART_iostream uart;
    using Timer = typename PWM_pin::Timer;

    if constexpr (PWM_pin::number == 11 or  PWM_pin::number == 12){

    }
    else if constexpr (PWM_pin::number == 15 or  PWM_pin::number == 16){
    // Timer 1
	myu::Disable_interrupts l;
	auto top = Timer::unsafe_input_capture_register();
	uart << "top = " << top << '\n';

	auto pres_top1 = 1 * (top + 1);
	auto freq_clk = myu::Frequency{1'000'000};
	auto res = freq_clk/ pres_top1;
	uart << "res = " << res << '\n';


	avr_::timer_::PWM_mode<avr_::Timer1> pwm;
	pwm.prescaler = 1;
	pwm.top = 999;
	auto res2 = pwm.frequency_fast_mode(freq_clk);
	uart << "res2 = " << res2 << '\n';
	auto res3 = pwm.frequency_phase_mode(freq_clk);
	uart << "res3 = " << res3 << '\n';

    }

}

template <typename PWM_pin>
void generate_pwm_signal()
{
    myu::UART_iostream uart;
    uart << "\nFrequency (in Hz): ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    uart << "Duty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    myu::PWM_signal pwm{freq, static_cast<uint8_t>(duty_cycle)};
    PWM_pin::generate(pwm);

    myu::Frequency freq_gen = PWM_pin::frequency();
    uart << "Generating " << freq_gen << " signal\n";

//    print_debug<PWM_pin>();

}



template <typename PWM_pin>
void change_duty_cycle()
{
    myu::UART_iostream uart;
    uart << "\nDuty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    PWM_pin::duty_cycle(duty_cycle);
}


template <typename PWM_pin>
void test_pin()
{
    myu::UART_iostream uart;
    uart << "\nTest pin " << uint16_t{PWM_pin::number} << "\n"
	    "-------------\n"
	    "Connect oscilloscope to pin number " 
					<< uint16_t{PWM_pin::number} << "\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate PWM signal\n"
	    "3. Change duty cycle\n"
	    "4. Stop timer\n"
	    "5. Disconnect pin from timer\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': PWM_pin::write_zero();
	break; case '1': PWM_pin::write_one();
	break; case '2': generate_pwm_signal<PWM_pin>();
	break; case '3': change_duty_cycle<PWM_pin>();
	break; case '4': PWM_pin::stop();
	break; case '5': PWM_pin::disconnect();
	break; default: uart << "I don't understand.\n";
    }


}

int main()
{
    init_uart();
    init_timers();

    hello();

    myu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. PWM_mode: list of values\n"
		  "2. Timer0::PWM_pin " << uint16_t{PWM_pin1::number} << "\n";

	if constexpr (timer0_both_pins)
	    uart << "3. Timer0::PWM_pin " << uint16_t{PWM_pin2::number} << "\n";

	uart << "4. Timer1::PWM_pin " << uint16_t{PWM_pin3::number} << "\n"
		"5. Timer1::PWM_pin " << uint16_t{PWM_pin4::number} << "\n";



        char c{};
	uart >> c;
	switch(c){
	    break; case '1': pwm_mode_test();

	    break; case '2': test_pin<PWM_pin1>();
	    break; case '3': if constexpr (timer0_both_pins)
				    test_pin<PWM_pin2>();
	    break; case '4': test_pin<PWM_pin3>();
	    break; case '5': test_pin<PWM_pin4>();

	    break; default:
		uart << "I don't understand.\n";
	}
    }
}



