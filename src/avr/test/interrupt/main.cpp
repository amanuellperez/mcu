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

#include "../../avr_interrupt.h"
#include "../../avr_pin.h"
#include "../../avr_time.h"



// cfg
constexpr uint8_t push_button_pin = 4;
constexpr uint8_t led_pin = 14;

#define ISR_PCINT  ISR_PCINT_PIN4



ISR_PCINT {
    if (avr::Pin<push_button_pin>::is_zero())
	avr::Pin<led_pin>::write_one();
    else 
	avr::Pin<led_pin>::write_zero();
}

 
int main()
{
    avr::Pin<led_pin>::as_output();
    avr::Pin<push_button_pin>::as_input_with_pullup();
    avr::Interrupt::enable_pin<push_button_pin>();

    while(1){
	wait_ms(1000);
    }
}



