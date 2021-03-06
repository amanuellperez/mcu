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

#include "../../dev_counter_CD4017B.h"

using namespace dev;


int main() 
{
    auto counter = 
	counter_CD4017B(Counter_CD4017B_pin_clock<14>{},
		        Counter_CD4017B_pin_reset<13>{},
			Counter_CD4017B_pin_clock_inhibit<12>{});
//    auto counter = 
//	counter_CD4017B(Counter_CD4017B_pin_clock<14>{});
    while(1){
	// probamos que cuente
	for (uint8_t i = 0; i < 15; ++i){
	    counter.next();
	    wait_ms(100);
	}

	counter.disable();
	for (uint8_t i = 0; i < 15; ++i){
	    counter.next();
	    wait_ms(200);
	}

	counter.enable();
	for (uint8_t i = 0; i < 10; ++i){
	    counter.next();
	    wait_ms(200);
	}

	for (uint8_t i = 0; i < 10; ++i){
	    counter.reset();
	    wait_ms(200);
	    counter.next();
	    wait_ms(200);
	    counter.next();
	    wait_ms(200);
	}

    }

}


