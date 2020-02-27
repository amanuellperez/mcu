// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// Ejemplo básico de uso del Timer como contador.
// Genera una señal de 1MHz en el pin OC1A (pin 15)
// y en el pin OC1B (pin 16). Conectar el osciloscopio a estos pines para
// comprobarlo.
#include "../../../avr_timer0_basic.h"
#include "../../../avr_time.h"

using namespace avr;

using Timer = Timer0_CTC_mode;

// Probar cada periodo con diferentes frecuencias: 1 MHz y 8 MHz.
// Medir la salida con el osciloscopio.
//
// Para 1 MHz:
constexpr uint16_t period_in_us = 1;
//constexpr uint16_t period_in_us = 8;
//constexpr uint16_t period_in_us = 64 ;  
//constexpr uint16_t period_in_us = 256;
//constexpr uint16_t period_in_us = 1024;
//constexpr uint16_t period_in_us = 10; // <-- no tiene que compilar


// Para 8 MHz
//constexpr uint16_t period_in_us = 1;
//constexpr uint16_t period_in_us = 8;
//constexpr uint16_t period_in_us = 32;
//constexpr uint16_t period_in_us = 128;
//constexpr uint16_t period_in_us = 1024;// <-- error! no tiene que compilar.


// A 1 MHz la salida del osciloscopio será: 2*top*period_in_us +- error
// El error es debido a la incertidumbre en la frecuencia del reloj del avr.
constexpr Timer::counter_type top = 100; 

// TODO: falta probar  con 8MHz
// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
//constexpr uint16_t top = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


int main()
{
    Timer::top_OCRA(top);
    Timer::pin_A_toggle_on_compare_match();
    Timer::pin_B_toggle_on_compare_match();

    Timer::on<period_in_us>();


    while(1){
	wait_ms(5000);
    }
}



