// Copyright (C) 2022.-2024 Manuel Perez 
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

#include "avr_timer1.h"

namespace avr_{

// ¿es genérico o depende del Timer0?
// Por culpa de frequency_in_Hz_to_prescaler_top que conoce la implementación
// del Timer esta función no es genérica.
void Square_wave_generator1_g::generate(uint32_t freq_in_Hz, uint8_t npin)
{
    auto [d, t] = frequency_in_Hz_to_prescaler_top(freq_in_Hz);

    init();

    disconnect_all_pins();
    connect_pin(npin);

    top(t); 
    Timer::clock_frequency_prescaler(d); // esto enciende el Timer
}




namespace timer1_{
// Experimentalmente he encontrado que esté método calcula todo con preescaler
// = 1, mode fast or phase.
void PWM_mode::calculate_cfg_method2(const Frequency::Rep& freq_clk, 
				     const Frequency::Rep& freq_gen)
{
// Calculamos los dos modos para prescaler = 1
    PWM_mode fast;
    fast.prescaler = 1;

    PWM_mode phase;
    phase.prescaler = 1;

    auto ferr = fast.prescaler2top_fast_mode  (freq_clk, freq_gen);
    auto perr = phase.prescaler2top_phase_mode(freq_clk, freq_gen);


// Rellenamos datos
    prescaler = 1;
    if (ferr <= perr){
	top = fast.top;
	fast_mode = true;

    } else {
	top = phase.top;
	fast_mode = false;
    }

}

//
//// Devuelve el error (100 en caso de no poder calcular la cfg)
//// precondition: prescaler tiene un valor válido
////
////  Fast  PWM: freq_gen = freq_clk/(p * (top + 1);
//uint8_t PWM_mode::prescaler2top_fast_mode( const Frequency::Rep& freq_clk,
//				 const Frequency::Rep& freq_gen)
//{
//    using Rep = Frequency::Rep;
//
//    // Si se trunca se comete en algunas ocasiones mucho error
//    auto q = atd::divide_rounding(freq_clk, freq_gen);
//
//    if (prescaler > q){
//	top = 0;
//	return 100;
//    }
//
//    top = atd::divide_rounding(q, Rep{prescaler}) - 1;
//    
//    if (top > 3 and top < Timer1::max()){
//
//	Frequency::Rep freq_res = freq_clk / (prescaler * (top + 1));
//	return percentage_error(freq_res, freq_gen);
//    }
//
//    else
//	return 100;
//
//}


//// Devuelve el error (100 en caso de no poder calcular la cfg)
//// precondition: prescaler tiene un valor válido
////
////  Phase PWM: freq_gen = freq_clk/(2 * p * top);
//uint8_t PWM_mode::prescaler2top_phase_mode( const Frequency::Rep& freq_clk,
//				 const Frequency::Rep& freq_gen)
//{
//    using Rep = Frequency::Rep;
//
//    // Si se trunca se comete en algunas ocasiones mucho error
//    auto q = atd::divide_rounding(freq_clk, freq_gen);
//
//    if (prescaler > q){
//	top = 0;
//	return 100;
//    }
//
//    top = atd::divide_rounding(q, Rep{2 * prescaler});
//    
//    if (top > 4 and top < Timer1::max()){
//
////	Frequency::Rep freq_res = frequency_phase_mode(freq_clk).value();
//	Frequency::Rep freq_res = freq_clk / (2 * prescaler * top);
//	return percentage_error(freq_res, freq_gen);
//    }
//
//    else
//	return 100;
//
//}

} // namespace timer1_
}// namespace
