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
    Timer::clock_frequency_prescaler(d); // esto enciende el Timer
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

void PWM_mode::calculate_cfg_top_OCRA(const Frequency::Rep& freq_clk,
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
// A partir de 2kHz el prescaler es 1. Si se quieren generar frecuencias
// menores es mejor elegir otros prescalers.
// TODO: (???) modificarlo? Qué frecuencias en la práctica se usan?
// Si lo modifico el código es mayor, ¿cuanto? merece la pena incrementar el
// código en TODOS los programas que usen PWM0_pin o no?
// Razones:
//  (1) por consistencia, sí: tiene que generar la frecuencia más proxima a la
//      pedida.
//  (2) por eficiencia, no lo se. Todo depende de cuanto aumente el código en
//      tamaño y ralentice al programa (no creo que ralentizarlo sea
//      problematico)
    prescaler = 1;
    if (ferr <= perr){
	top = fast.top;
	fast_mode = true;

    } else {
	top = phase.top;
	fast_mode = false;
    }

}



}// namespace timer0_
}// namespace avr_
