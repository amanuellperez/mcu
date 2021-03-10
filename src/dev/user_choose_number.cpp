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

#include "user_choose_number.h"

namespace dev{
namespace __user_choose_number{

void Counter::reset() 
{ 
    counter_.reset();
    counter_.max(d1_max[0], d0_max[0]); 

    i_ = 0;
}


void Counter::tick() 
{
    if (counter_.is_max())
	next_state();
    else
	counter_.tick();

}

void Counter::next_state()
{
    if (i_ < nstates - 1){
	++i_;
	counter_.max(d1_max[i_], d0_max[i_]);
	counter_.reset();
    }
}

}// namespace __user_choose_number

} // namespace dev

