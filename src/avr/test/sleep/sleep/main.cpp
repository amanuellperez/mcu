// Copyright (C) 2019-2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

// Hacemos que parpadee un led. Cuando se pulsa un pulsador se duerme el avr,
// con lo que el led deja de iluminarse. Al soltar el pulsador, despierta,
// volviendo a iluminarse.
// Para ver que cualquier pin puede despertar al avr, defino 2 interrupciones.
// Funciona con los 2 pines correctamente. Probar a descomentar el pin2.
#include "../../../avr_sleep.h"
#include "../../../avr_interrupt.h"
#include "../../../avr_pin.h"
#include "../../../avr_time.h"


// cfg
constexpr uint8_t push_button_pin1 = 4;
//constexpr uint8_t push_button_pin2 = 9;
constexpr uint8_t led_pin = 14;

#define ISR_PCINT1  ISR_PCINT_PIN4
//#define ISR_PCINT2  ISR_PCINT_PIN9


volatile bool a_dormir = false;


ISR_PCINT1 {
    a_dormir = avr_::Pin<push_button_pin1>::is_zero();
}

//ISR_PCINT2 {
//    a_dormir = avr_::Pin<push_button_pin2>::is_zero();
//}


int main()
{
    avr_::Pin<led_pin>::as_output();
    avr_::Pin<push_button_pin1>::as_input_with_pullup();
    avr_::Interrupt::enable_pin<push_button_pin1>();

//    avr_::Pin<push_button_pin2>::as_input_with_pullup();
//    avr_::Interrupt::enable_pin<push_button_pin2>();
    
    avr_::Sleep::set_mode_power_down();

    while(1){
	avr_::Pin<led_pin>::write_one();
	wait_ms(500);

	avr_::Pin<led_pin>::write_zero();
	wait_ms(500);

	avr_::sleep_if(a_dormir);

    }
}



