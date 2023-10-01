// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../../avr_UART_baud_rate.h"

#include <iostream>
#include <type_traits>

#include <alp_test.h>

using namespace alp;
using namespace test;
using namespace avr;

std::ostream& operator<<(std::ostream& out, UART_mode m)
{
    switch(m){
	case UART_mode::normal:
	    std::cout << "normal mode";
	    break;

	case UART_mode::double_speed:
	    std::cout << "double speed mode";
	    break;

    }

    return out;
}

template <typename T, T v>
struct Wrapper{
//    constexpr Wrapper(T v):value{v}{}
    constexpr static T value = v;
};

template <uint32_t f_clock, uint32_t baud_rate>
void normal_mode(uint32_t UBBRn, bool dentro_tolerancia)
{
    std::cout << "normal_mode: " << f_clock << ", " << baud_rate << "\n";
    CHECK_TRUE((UART_normal_mode<f_clock, baud_rate>::UBBRn == UBBRn), "UBBRn");
    CHECK_TRUE((UART_normal_mode<f_clock, baud_rate>::dentro_tolerancia ==
                   dentro_tolerancia), "dentro_tolerancia");
}


template <uint32_t f_clock, uint32_t baud_rate>
void double_speed_mode(uint32_t UBBRn, bool dentro_tolerancia)
{
    std::cout << "double_speed_mode: " << f_clock << ", " << baud_rate << "\n";
    CHECK_TRUE((UART_double_speed_mode<f_clock, baud_rate>::UBBRn == UBBRn), "UBBRn");
    CHECK_TRUE((UART_double_speed_mode<f_clock, baud_rate>::dentro_tolerancia ==
                   dentro_tolerancia), "dentro_tolerancia");
}



template <uint32_t f_clock, uint32_t baud_rate>
void UBBRn_test(uint32_t UBBRn_res, UART_mode mode)
{
    std::cout << "UBBRn: " << f_clock << ", " << baud_rate << "\n";
    // Los meto dentro de Wrapper para garantizar que se está generando
    // todo en tiempo de compilación.
    CHECK_TRUE((Wrapper<uint32_t, UBBRn<f_clock, baud_rate>().second>::value ==
                UBBRn_res),
               "UBBRn");
    CHECK_TRUE((Wrapper<UART_mode, UBBRn<f_clock, baud_rate>().first>::value ==
                mode),
               "mode");
}


int main(int argc, const char* argv[])
{
try{
    // He calculado los resultados usando una hoja de calculo para comprobar
    // las bauds rates habituales.
    normal_mode<1000000, 110>(567, true);
    normal_mode<1000000, 150>(416, true);
    normal_mode<1000000, 300>(207, true);
    normal_mode<1000000, 1200>(51, true);
    normal_mode<1000000, 2400>(25, true);
    normal_mode<1000000, 4800>(12, true);
    normal_mode<1000000, 9600>(6, false);
    normal_mode<1000000, 19200>(2, false);
    normal_mode<1000000, 38400>(1, false);
    normal_mode<1000000, 57600>(0, false);
    normal_mode<1000000, 115200>(0, false);
    normal_mode<1000000, 230400>(0, false);
    normal_mode<1000000, 460800>(0, false);
    normal_mode<1000000, 921600>(0, false);

    double_speed_mode<1000000, 110>(1135, true);
    double_speed_mode<1000000, 150>(832, true);
    double_speed_mode<1000000, 300>(416, true);
    double_speed_mode<1000000, 1200>(103, true);
    double_speed_mode<1000000, 2400>(51, true);
    double_speed_mode<1000000, 4800>(25, true);
    double_speed_mode<1000000, 9600>(12, true);
    double_speed_mode<1000000, 19200>(6, false);
    double_speed_mode<1000000, 38400>(2, false);
    double_speed_mode<1000000, 57600>(1, false);
    double_speed_mode<1000000, 115200>(0, false);
    double_speed_mode<1000000, 230400>(0, false);
    double_speed_mode<1000000, 460800>(0, false);
    double_speed_mode<1000000, 921600>(0, false);


    normal_mode<8000000, 110>(4544, true);
    normal_mode<8000000, 150>(3332, true);
    normal_mode<8000000, 300>(1666, true);
    normal_mode<8000000, 1200>(416, true);
    normal_mode<8000000, 2400>(207, true);
    normal_mode<8000000, 4800>(103, true);
    normal_mode<8000000, 9600>(51, true);
    normal_mode<8000000, 19200>(25, true);
    normal_mode<8000000, 38400>(12, true);
    normal_mode<8000000, 57600>(8, false);
    normal_mode<8000000, 115200>(3, false);
    normal_mode<8000000, 230400>(1, false);
    normal_mode<8000000, 460800>(0, false);
    normal_mode<8000000, 921600>(0, false);

    double_speed_mode<8000000, 110>(9090, true);
    double_speed_mode<8000000, 150>(6666, true);
    double_speed_mode<8000000, 300>(3332, true);
    double_speed_mode<8000000, 1200>(832, true);
    double_speed_mode<8000000, 2400>(416, true);
    double_speed_mode<8000000, 4800>(207, true);
    double_speed_mode<8000000, 9600>(103, true);
    double_speed_mode<8000000, 19200>(51, true);
    double_speed_mode<8000000, 38400>(25, true);
    double_speed_mode<8000000, 57600>(16, false);
    double_speed_mode<8000000, 115200>(8, false);
    double_speed_mode<8000000, 230400>(3, false);
    double_speed_mode<8000000, 460800>(1, false);
    double_speed_mode<8000000, 921600>(0, false);


    UBBRn_test<1000000, 110>(567, UART_mode::normal);
    UBBRn_test<1000000, 150>(416, UART_mode::normal);
    UBBRn_test<1000000, 300>(207, UART_mode::normal);
    UBBRn_test<1000000, 1200>(51, UART_mode::normal);
    UBBRn_test<1000000, 2400>(25, UART_mode::normal);
    UBBRn_test<1000000, 4800>(12, UART_mode::normal);
    UBBRn_test<1000000, 9600>(12, UART_mode::double_speed);
    // El siguiente no tiene que compilar:
    // UBBRn_test<1000000, 19200>(6, UART_mode::double_speed);


    UBBRn_test<8000000, 110>(4544, UART_mode::normal);
    UBBRn_test<8000000, 150>(3332, UART_mode::normal);
    UBBRn_test<8000000, 300>(1666, UART_mode::normal);
    UBBRn_test<8000000, 1200>(416, UART_mode::normal);
    UBBRn_test<8000000, 2400>(207, UART_mode::normal);
    UBBRn_test<8000000, 4800>(103, UART_mode::normal);
    UBBRn_test<8000000, 9600>(51, UART_mode::normal);
    UBBRn_test<8000000, 19200>(25, UART_mode::normal);
    UBBRn_test<8000000, 38400>(12, UART_mode::normal);
    // El siguiente no tiene que compilar:
//    UBBRn_test<8000000, 57600>(16, UART_mode::normal);

}catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
}



