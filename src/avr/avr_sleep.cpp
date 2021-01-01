// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "avr_sleep.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>

namespace avr{

// De acuerdo al manual de avr-libc y a 
// https://onlinedocs.microchip.com/pr/GUID-78362176-487F-41B9-95C7-B478A9A186EB-en-US-2/index.html?GUID-5A2B94D6-356F-4107-A130-8F8F983903BC
bool sleep_if(volatile bool& a_dormir)
{
    bool recien_despertado = false;
    cli();
    if (a_dormir){
	a_dormir = false;
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	recien_despertado = true; 
    }
    sei();

    return recien_despertado;

}


/***************************************************************************
 *				POWER
 ***************************************************************************/
volatile bool Power::to_sleep_ = false;


}// namespace



