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

#include "../../../mega_registers.h"
#include "../../../mega_timer0_hal.h"
#include "../../../mega_timer1_hal.h"
#include "../../../mega_import_avr.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>

#include <atd_test.h>
using namespace test;

// El comportamiento del Timer0 varia dependiendo de si usamos
// los dos pines o solo el B. Con esta variable selecciono la opción.
constexpr bool timer0_both_pins = false;

// Microcontroller
// ---------------
namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

					 
// Hwd devices
// -----------
// DONT_COMPILE(using PWM_pin1 = hwd::PWM0_pin<10>;)

using PWM_pin1 = myu::hal::PWM0_pin<11>;
using PWM_pin2 = myu::hal::PWM0_pin<12>;
// Este static assert es un comentario para el programador, para recordarle
// que el pin A de este micro es el número 12
static_assert(PWM_pin1::pin_number_A() == 12);

// DONT_COMPILE(using PWM_pin3 = myu::PWM1_pin<18>;)
using PWM_pin3 = myu::hal::PWM1_pin<15>;
using PWM_pin4 = myu::hal::PWM1_pin<16>;

// Cfg
// ---
using namespace myu::literals;

// Global vbles
// ------------
// Variables necesarias en las interrupciones
struct PWG{ // Pulse Wave Generation
inline static volatile uint16_t ocr1 = 0;
inline static volatile uint16_t ocr2 = 0;
inline static volatile uint8_t npin = 0;
inline static volatile uint8_t i = 0; // núm. pulso actual
constexpr static uint8_t nmax_pulses = 8; 
};

// FUNCTIONS
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
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

// esto es `abort` pero ya hay una función abort 
bool user_press_key()
{
    UART_iostream uart;

    if (uart.is_there_something_to_read()){
	char c{};
	uart >> c; // uart.clear(); que nombre estandar tiene esto?
	return true;
    }

    return false;
}

void hello()
{
    UART_iostream uart;
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


template <typename PWM_cfg>
void print(std::ostream& out, 
	   const PWM_cfg& pwm,
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



template <typename PWM_pin>
void print_debug()
{
    UART_iostream uart;
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


	hwd::timer_::PWM_cfg<hwd::Timer1> pwm;
	pwm.prescaler = 1;
	pwm.top = 999;
	auto res2 = pwm.frequency_fast_mode(freq_clk);
	uart << "res2 = " << res2 << '\n';
	auto res3 = pwm.frequency_phase_mode(freq_clk);
	uart << "res3 = " << res3 << '\n';

    }

}

template <Type::Integer Int>
Int ask(std::iostream& out, const char* msg)
{
    // out << "From (freq. in Hz): ";
    out << msg;
    Int x;
    out >> x;
    return x;
}

template <typename PWM_pin>
void automatic_duty_cycle_test()
{
    UART_iostream uart;

    uart << "\n\nAutomatic generation of a range of duty cycles\n"
	        "----------------------------------------------\n";


    auto freq = ask<uint32_t>(uart, "Frequency to generate (in Hz): ");
    if (freq == 0) return;
    
    // Aquí todavia no hemos llamado a PWM_pin::generate con lo que 
    // PWM_pin::frequency es 0!!! Si se quiere imprimir ponerlo despues.
//    hwd::Frequency freq_gen = PWM_pin::frequency();
//    uart << "Generating frequency " << freq_gen << '\n';

    Test test{uart};
    test.silent_mode();

    for (uint8_t duty_cycle = 0; duty_cycle <= 100; duty_cycle += 10){
	myu::PWM_signal pwm{freq, atd::Percentage{duty_cycle}};
	PWM_pin::generate(pwm);

	uart << "duty cycle = " << (int) duty_cycle << " % ... ";
	
	auto dt = PWM_pin::duty_cycle();
	if (dt == atd::Percentage{duty_cycle})
	    uart << "OK\n";

	else
	    uart << "ERROR? real duty cycle = " << dt << '\n';
	
	if (duty_cycle != 0 and duty_cycle != 100) { 
	    // en 0, escribe un 0 ==> PWM_pin::is_zero() == true
	    // en 100, escribe 1 ==> PWM_pin::is_one() == true
	    CHECK_TRUE(test, PWM_pin::is_zero() == false, "!PWM_pin::is_zero()");
	    CHECK_TRUE(test, PWM_pin::is_one() == false, "!PWM_pin::is_one()");
	}

	myu::wait_ms(1000);
	if (user_press_key())
	    break;
    }

    test.silent_mode(false);

    PWM_pin::write_zero();
    myu::wait_ms(1);
    CHECK_TRUE(test, PWM_pin::is_zero() == true, "write_zero");
    CHECK_TRUE(test, PWM_pin::is_one() == false, "write_zero");

    PWM_pin::write_one();
    CHECK_TRUE(test, PWM_pin::is_zero() == false, "write_one");
    CHECK_TRUE(test, PWM_pin::is_one() == true, "write_one");

    PWM_pin::stop();

}
template <typename PWM_pin>
void automatic_frequency_test()
{
    UART_iostream uart;
    uart << "\n\nAutomatic generation of a range of frequencies\n"
	    "----------------------------------------------\n";

    auto freq0 = ask<uint32_t>(uart, "From (freq. in Hz): ");
    if (freq0 == 0) return;
    
    auto freq1 = ask<uint32_t>(uart, "To (freq. in Hz): ");
    if (freq1 == 0) return;

    auto incr = ask<uint32_t>(uart, "Increment (in Hz): ");
    if (incr == 0) return;

    uint8_t duty_cycle = 50;
    for (; freq0 <= freq1; freq0 += incr){
	myu::PWM_signal pwm{freq0, atd::Percentage{duty_cycle}};
	PWM_pin::generate(pwm);

	myu::Frequency freq_gen = PWM_pin::frequency();
	uart << "Trying to generate ... " << freq0 <<
	        " but generating " << freq_gen << '\n';

	myu::wait_ms(1000);
	if (user_press_key())
	    break;
    }

    PWM_pin::stop();

}

template <typename PWM_pin>
void generate_pwm_signal()
{
    UART_iostream uart;
    uart << "\nFrequency (in Hz): ";
    uint32_t freq{0};
    uart >> freq;

    if (freq == 0) return;

    uart << "Duty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    // if (duty_cycle == 0) return; // se puede probar duty cycle 0%

    myu::PWM_signal pwm{freq, atd::Percentage{static_cast<uint8_t>(duty_cycle)}};
    if (PWM_pin::generate(pwm) == nm::fail){
	uart << "\n\n**** ERROR ****\n"
	        "Can't generate that frequency\n"
		"Change frequency or choose other F_CPU\n";
	return;
    }

    myu::Frequency freq_gen = PWM_pin::frequency();
    uart << "Generating " << freq_gen << " signal\n";

//    print_debug<PWM_pin>();

}



template <typename PWM_pin>
void change_duty_cycle()
{
    UART_iostream uart;
    uart << "\nDuty cycle (as number): ";
    uint16_t duty_cycle{0};
    uart >> duty_cycle;
    if (duty_cycle == 0) return;

    auto dc = atd::Percentage{static_cast<uint8_t>(duty_cycle)};
    PWM_pin::duty_cycle(dc);
}

template <typename PWM_pin>
void pulse_wave_generation()
{
    // TODO: eliminar esta reestricción
    if constexpr (PWM_pin::number == 15 or
		  PWM_pin::number == 16) {

    UART_iostream uart;
    auto freq = ask<uint32_t>(uart, "\n\nFrequency in Hz: ");
    if (freq == 0) return;


    auto dc1 = ask<uint16_t>(uart, "First duty cycle (percentage as number): ");
    if (dc1 == 0) return;

    auto dc2 = ask<uint16_t>(uart, "First duty cycle (percentage as number): ");
    if (dc2 == 0) return;

    typename PWM_pin::Timer_cfg cfg;
    if (PWM_pin::cfg_to_generate(freq, cfg) == nm::fail){
	uart << "\n\n**** ERROR ****\n"
	        "Can't generate that frequency\n"
		"Change frequency or choose other F_CPU\n";
	return;
    }


    PWM_pin::cfg(cfg);

    PWG::ocr1 = PWM_pin::duty_cycle(cfg, atd::Percentage{static_cast<uint8_t>(dc1)});
    PWG::ocr2 = PWM_pin::duty_cycle(cfg, atd::Percentage{static_cast<uint8_t>(dc2)});

    PWM_pin::duty_cycle(PWG::ocr1);
    PWM_pin::connect();

    PWG::npin = PWM_pin::number;
    PWG::i    = PWG::nmax_pulses;

    PWM_pin::enable_interrupt();
    myu::enable_interrupts();
    PWM_pin::turn_on(cfg.prescaler);

    while (PWG::i){
	myu::wait_ms(1);
    }

    PWM_pin::turn_off();
    PWM_pin::disable_interrupt();
    myu::disable_interrupts();
    }

}

template <typename PWM_pin>
void connect_pin()
{
    UART_iostream uart;
    uart << "\n\nConnect or disconnect? (c/d) ";
    char ans{};
    uart >> ans;

    if (ans == 'd' or ans == 'D')
	PWM_pin::disconnect();

    else
	PWM_pin::connect();

}

template <typename PWM_pin>
void test_pin()
{
    UART_iostream uart;
    uart << "\nTest pin " << uint16_t{PWM_pin::number} << "\n"
	    "-------------\n"
	    "Connect oscilloscope to pin number " 
					<< uint16_t{PWM_pin::number} << "\n"
	    "0. Write 0\n"
	    "1. Write 1\n"
	    "2. Generate PWM signal\n"
	    "3. Change duty cycle\n"
	    "4. Stop timer\n"
	    "5. Connect/disconnect pin from timer\n"
	    "6. Automatic frequency test\n"
	    "7. Automatic duty cycle test\n"
	    "8. Pulse wave generation\n";

    char opt{};
    uart >> opt;

    switch(opt){
	break; case '0': PWM_pin::write_zero();
	break; case '1': PWM_pin::write_one();
	break; case '2': generate_pwm_signal<PWM_pin>();
	break; case '3': change_duty_cycle<PWM_pin>();
	break; case '4': PWM_pin::stop();
	break; case '5': connect_pin<PWM_pin>();
	break; case '6': automatic_frequency_test<PWM_pin>();
	break; case '7': automatic_duty_cycle_test<PWM_pin>();
	break; case '8': pulse_wave_generation<PWM_pin>();
	break; default: uart << "I don't understand.\n";
    }


}

int main()
{
    init_uart();
    init_timers();

    hello();

    UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Timer0::PWM_pin " << uint16_t{PWM_pin1::number} << "\n";

	if constexpr (timer0_both_pins)
	    uart << "2. Timer0::PWM_pin " << uint16_t{PWM_pin2::number} << "\n";

	uart << "3. Timer1::PWM_pin " << uint16_t{PWM_pin3::number} << "\n"
		"4. Timer1::PWM_pin " << uint16_t{PWM_pin4::number} << "\n";



        char c{};
	uart >> c;
	switch(c){
	    break; case '1': test_pin<PWM_pin1>();
	    break; case '2': if constexpr (timer0_both_pins)
				    test_pin<PWM_pin2>();
	    break; case '3': test_pin<PWM_pin3>();
	    break; case '4': test_pin<PWM_pin4>();

	    break; default:
		uart << "I don't understand.\n";
	}
    }
}



// Interrupts
// ----------
ISR_TIMER1_COMPA{
    if (PWG::i) {
	PWG::i = PWG::i - 1;
	if (PWG::i % 2) {
	    if (PWG::npin == PWM_pin3::number)
		PWM_pin3::unsafe_duty_cycle(PWG::ocr2);
	    else
		PWM_pin4::unsafe_duty_cycle(PWG::ocr2);
	} else {
	    if (PWG::npin == PWM_pin3::number)
		PWM_pin3::unsafe_duty_cycle(PWG::ocr1);
	    else
		PWM_pin4::unsafe_duty_cycle(PWG::ocr1);
	}
    }
}

ISR_TIMER1_COMPB{
    if (PWG::i) {
	PWG::i = PWG::i - 1;
	if (PWG::i % 2) {
	    if (PWG::npin == PWM_pin3::number)
		PWM_pin3::unsafe_duty_cycle(PWG::ocr2);
	    else
		PWM_pin4::unsafe_duty_cycle(PWG::ocr2);
	} else {
	    if (PWG::npin == PWM_pin3::number)
		PWM_pin3::unsafe_duty_cycle(PWG::ocr1);
	    else
		PWM_pin4::unsafe_duty_cycle(PWG::ocr1);
	}
    }
}

