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
// y en el pin OC1B (pin 16)
#include "../../../avr_timer1_tr.h"
#include "../../../avr_time.h"

using namespace avr;

using Timer = Timer1_CTC_mode;

// Probar cada periodo con diferentes frecuencias: 1 MHz y 8 MHz.
// Para los 8 MHz hay que definir el fuse correspondiente y F_CPU en el
// makefile.
// Medir la salida con el osciloscopio.
//constexpr uint16_t period_in_us = 1;
// constexpr uint16_t period_in_us = 8;
//constexpr uint16_t period_in_us = 32;	// solo para 8 MHz
//constexpr uint16_t period_in_us = 64; // solo para 1 MHz
//constexpr uint16_t period_in_us = 128;  // solo para 8 MHz
constexpr uint16_t period_in_us = 256;// solo para 1 MHz
//constexpr uint16_t period_in_us = 1024;//solo para 1 MHz

constexpr uint16_t ocr1a = 1000; // 15625 miniticks * 64 us/minitick = 1 seg

// Si se quiere que generar una señal de 1 segundo, usar period_in_us = 64 y:
// constexpr uint16_t ocr1a = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


int main()
{
    // Elegimos OCR1A para generar una señal de 1 Hz
    // El osciloscopio la marca de 996ms. Hay que calibrar el número o usar un
    // cristal externo.

    Timer::top_OCRA(ocr1a);
    Timer::pin_A_toggle_on_compare_match();
    Timer::pin_B_toggle_on_compare_match();

    Timer::on<period_in_us>();


    while(1){
	wait_ms(5000);
    }
}



