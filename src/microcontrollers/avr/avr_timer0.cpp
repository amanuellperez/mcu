// Copyright (C) 2022-2024 Manuel Perez 
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

#include "avr_timer0.h"

namespace avr_{

// ¿es genérico o depende del Timer0?
// Por culpa de frequency_in_Hz_to_prescaler_top que conoce la implementación
// del Timer esta función no es genérica.
void Square_wave_generator0_g::generate(uint32_t freq_in_Hz, uint8_t npin)
{
    auto [d, t] = frequency_in_Hz_to_prescaler_top(freq_in_Hz);

    init();

    disconnect_all_pins();
    connect_pin(npin);

    top(t); 
    Timer::prescaler(d); // esto enciende el Timer
}


Square_wave_burst_generator0_g::
		    Square_wave_burst_generator0_g(uint32_t freq_in_Hz)
{
    std::tie(prescaler_factor_, top_) = 
				frequency_in_Hz_to_prescaler_top(freq_in_Hz);
    init();
    disconnect_all_pins();
}


namespace timer0_{

// (???) De momento la implementación es lo más sencilla posible.
// Da por supuesto que la freq_gen es una de las frecuencias que realmente se
// pueden generar. A fin de cuentas, si se usa el top 0xFF se están limitando
// las frecuencias, teniendo que saberlo el programador.
void PWM_mode::calculate_cfg_top_0xFF(const Frequency::Rep& freq_clk,
			   const Frequency::Rep& freq_gen)
{
    top = 0xFF;
    fast_mode = true;

    // Si se trunca se comete en algunas ocasiones mucho error
    auto q = atd::divide_rounding(freq_clk, freq_gen);

    prescaler = q / 256u;

    if (prescaler > 1024u){
	prescaler = q / 510u;
	fast_mode = false;
    }
}

// Lo que hace es mirar cual de los dos modos comete menos error usando el
// prescaler0.
atd::Percentage
	PWM_mode::calculate_cfg_top_OCRA(const Frequency::Rep& freq_clk,
			   const Frequency::Rep& freq_gen, 
			   const uint16_t& prescaler0)
{
// Calculamos los dos modos para prescaler = 1
    PWM_mode fast{};
    fast.prescaler = prescaler0;

    PWM_mode phase{};
    phase.prescaler = prescaler0;

    auto ferr = fast.prescaler2top_fast_mode  (freq_clk, freq_gen);
    auto perr = phase.prescaler2top_phase_mode(freq_clk, freq_gen);

// Rellenamos datos
    prescaler = prescaler0;
    if (ferr <= perr){
	top = fast.top;
	fast_mode = true;
	return ferr;

    } else {
	top = phase.top;
	fast_mode = false;
	return perr;
    }
}

// Voy a iterar por todos los prescalers buscando el que genera menos error
// (son solo 5 prescalers, y la mayoria de las veces funcionara para prescaler
// = 1)
atd::Percentage PWM_mode::calculate_cfg_top_OCRA(const Frequency::Rep& freq_clk,
			   const Frequency::Rep& freq_gen)
{
    auto error = calculate_cfg_top_OCRA(freq_clk, freq_gen, 
						Timer0::prescaler_factor[0]);

    PWM_mode tmp;

    for (uint8_t i = 1; i < Timer0::prescaler_factor.size(); ++i){
	auto error2 = 
		tmp.calculate_cfg_top_OCRA(freq_clk, freq_gen, 
				    Timer0::prescaler_factor[i]);
	
	if (error2 < error){
	    *this = tmp;

	    if (error2 == 0)
		return error2;

	    error = error2;
	}

    }

    return error;
}


}// namespace timer0_
}// namespace avr_
