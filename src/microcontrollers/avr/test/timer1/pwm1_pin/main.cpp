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
#include "../../../avr_timer1_generic.h"
#include "../../../avr_time.h"
#include "../../../avr_UART_iostream.h"


// Microcontroller
// ---------------
namespace my_mcu = avr_;

					 
// Hwd devices
// -----------
using Timer    = my_mcu::Timer1;
// DONT_COMPILE(using PWM_pin1 = my_mcu::PWM1_pin<18>;)
using PWM_pin1 = my_mcu::PWM1_pin<15>;
using PWM_pin2 = my_mcu::PWM1_pin<16>;


// Cfg
// ---
using namespace my_mcu::literals;


// FUNCTIONS
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}


void hello()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nPWM1_pin test\n"
	        "-------------\n"
            "Connect oscilloscope to pins "
         << uint16_t{PWM_pin1::number} 
	 << " and " << uint16_t{PWM_pin2::number}
         << '\n';
}


void print(std::ostream& out, 
	   const my_mcu::timer1_::PWM_mode& pwm,
	   const my_mcu::Frequency& freq_clk)
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
    my_mcu::UART_iostream uart;
    uart << "\nPWM_mode test\n"
	    "-------------\n";

    my_mcu::Frequency freq_clk = 1_MHz;

    my_mcu::timer1_::PWM_mode pwm;

    for (my_mcu::Frequency fg = 20_Hz; fg < 100_Hz; fg += 1_Hz){
	pwm.calculate_cfg_method2(freq_clk, fg);
	uart << fg << ": ";
	print(uart, pwm, freq_clk);
	uart << '\n';
    }

}

void generate_pwm_signal_pin_A()
{
    my_mcu::UART_iostream uart;
    uart << "\nFrequency: ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    uart << "Duty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    my_mcu::PWM_signal pwm{freq, static_cast<uint8_t>(duty_cycle)};
    PWM_pin1::generate(pwm);
}


void pin_A_change_duty_cycle()
{
    my_mcu::UART_iostream uart;
    uart << "\nDuty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    PWM_pin1::duty_cycle(duty_cycle);
}

void test_pin_A()
{
    my_mcu::UART_iostream uart;
    uart << "\nTest pin A\n"
	    "-------------\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate PWM signal\n"
	    "3. Change duty cycle\n"
	    "4. Stop timer\n"
	    "5. Disconnect pin from timer\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': PWM_pin1::write_zero();
	break; case '1': PWM_pin1::write_one();
	break; case '2': generate_pwm_signal_pin_A();
	break; case '3': pin_A_change_duty_cycle();
	break; case '4': PWM_pin1::stop();
	break; case '5': PWM_pin1::disconnect();
	break; default: uart << "I don't understand.\n";
    }


}



void generate_pwm_signal_pin_B()
{
    my_mcu::UART_iostream uart;
    uart << "\nFrequency: ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    uart << "Duty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    my_mcu::PWM_signal pwm{freq, static_cast<uint8_t>(duty_cycle)};
    PWM_pin2::generate(pwm);
}


void pin_B_change_duty_cycle()
{
    my_mcu::UART_iostream uart;
    uart << "\nDuty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    PWM_pin2::duty_cycle(duty_cycle);
}

void test_pin_B()
{
    my_mcu::UART_iostream uart;
    uart << "\nTest pin B\n"
	    "-------------\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate PWM signal\n"
	    "3. Change duty cycle\n"
	    "4. Stop timer\n"
	    "5. Disconnect pin from timer\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': PWM_pin2::write_zero();
	break; case '1': PWM_pin2::write_one();
	break; case '2': generate_pwm_signal_pin_B();
	break; case '3': pin_B_change_duty_cycle();
	break; case '4': PWM_pin2::stop();
	break; case '5': PWM_pin2::disconnect();
	break; default: uart << "I don't understand.\n";
    }


}


int main()
{
    init_uart();
    
    hello();

    my_mcu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. PWM_mode: list of values\n"
		  "A. PWM_pin A\n"
		  "B. PWM_pin B\n";



        char c{};
	uart >> c;
	switch(c){
	    break; case '1': pwm_mode_test();

	    break; case 'a':
		   case 'A': test_pin_A();

	    break; case 'b':
		   case 'B': test_pin_B();

	    break; default:
		uart << "I don't understand.\n";
	}
    }
}



