// Copyright (C) 2022 A.Manuel L.Perez 
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
#pragma once

#ifndef __IR_NEC_MESSAGE_H__
#define __IR_NEC_MESSAGE_H__

#include <cstdint>
			    
#include "dev_train_of_pulses.h"	// Train_of_pulses
#include <atd_cast.h>	// convert(x).into(y)


// Devuelve true si a = b +- 20%
// Esto es, si b - 20% <= a <= b + 20%
// (cojo el 20% ya que:
//	1. El clock del avr tiene un 10% de incertidumbre
//	2. Doy un 10% más de margen de error
// )
template <typename Int1, typename Int2>
inline bool is_equal(const Int1& a, const Int2& b)
{ return (b*0.8 <= a and a <= b*1.2); }


struct NEC_message{
    uint8_t address;
    uint8_t inv_address;
    uint8_t command;
    uint8_t inv_command;
};

//struct NEC_message_verbose{
//
//    uint8_t address;
//    uint8_t inv_address;
//    uint8_t command;
//    uint8_t inv_command;
//};
//

std::ostream& operator<<(std::ostream& out, const NEC_message& msg);

//class NEC_message_conversor{
//public:
//    template <size_t N>
//    static bool convert_x_into_y(const dev::Train_of_pulses<N>& pulse, 
//							NEC_message& msg);
//private:
//    
//};
//
//
//template <size_t N>
//bool 
//NEC_message_conversor::convert_x_into_y(const dev::Train_of_pulses<N>& pulse, 
//						    NEC_message& msg)
//{
//    auto pe = pulse.end();
//    auto p = look_for_start(pulse.begin(), pe);
//
//    if (p == pe)
//	return false;
//
//    out <<   "First pulse     : (" << p->time_low << ", " << p->time_high << ") us\n";
//    ++p;   
//
//    out <<   "Address         : ";
//    p = print_byte(out, p, pe);
//
//    if (p == pe){
//	out << '\n';
//	return true; // es NEC
//    }
//
//
//    out << "\nInverted address: ";
//    p = print_byte(out, p, pe);
//
//    if (p == pe){
//	out << '\n';
//	return true; // es NEC
//    }
//
//
//    out << "\nCommand         : ";
//    p = print_byte(out, p, pe);
//
//    if (p == pe){
//	out << '\n';
//	return true; // es NEC
//    }
//
//
//    out << "\nInverted command: ";
//    p = print_byte(out, p, pe);
//
//    if (p == pe){
//	out << '\n';
//	return true; // es NEC
//    }
//
//    out << '\n';
//
//// DUDA: miramos si quedan más bytes para imprimir?
//
//    return true;
//
//}
//
//
//
//
//
//// Usage: atd::convert(ptrain).into(NEC_msg);
//template <size_t N>
//inline 
//bool convert_x_into_y(const dev::Train_of_pulses<N>& ptrain, NEC_message& msg)
//{
//    return NEC_message_conversor::convert_x_into_y(ptrain, msg);
//}


#endif
