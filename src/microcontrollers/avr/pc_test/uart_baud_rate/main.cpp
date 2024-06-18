// Copyright (C) 2023 Manuel Perez 
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

#include <alp_test.h>
#include <alp_string.h>

#include <iostream> 

#include "../../avr_UART_baud_rate.h"

using namespace test;

void print_normal_mode(uint32_t f_clock, uint32_t baud_rate)
{
    auto [ubbr, error] =
	avr_::impl_of::UBBR_and_error_normal_mode(f_clock, baud_rate);
    std::cout << "UART_normal_mode<" << f_clock << ", " << baud_rate << ">:\n";
    std::cout << "\tUBBRn     = " << ubbr << "\n"
	         "\tbaud_rate = " << avr_::impl_of::baud_rate_normal_mode(f_clock, ubbr) << "\n"
		 "\terror     = " << (int) error << " %\n";

}

void print_double_speed_mode(uint32_t f_clock, uint32_t baud_rate)
{
    auto [ubbr, error] =
	avr_::impl_of::UBBR_and_error_double_speed_mode(f_clock, baud_rate);
    std::cout << "UART_double_speed_mode<" << f_clock << ", " << baud_rate << ">:\n";
    std::cout << "\tUBBRn     = " << ubbr << "\n"
	         "\tbaud_rate = " << avr_::impl_of::baud_rate_double_speed_mode(f_clock, ubbr) << "\n"
		 "\terror     = " << (int) error << " %\n";

}

template <uint32_t f_clock, uint32_t baud_rate, uint32_t max_error = 3>
void print_ubbrn()
{
    auto [mode, ubbrn] = avr_::UBBRn<f_clock, baud_rate, max_error>();
    std::cout << "UBBRn<" << f_clock << ", " << baud_rate << ">:\n";
    std::cout << "\tmode = ";

    if (mode == avr_::UART_mode::normal)
	std::cout << "normal\n";
    else
	std::cout << "double speed\n";

    std::cout << "\tUBBRn = " << ubbrn << '\n';

}

void test_normal_mode(uint32_t f_clock, uint32_t baud_rate, uint16_t res)
{
    auto [ubbr, error] = avr_::impl_of::UBBR_and_error_normal_mode(f_clock, baud_rate);
    CHECK_TRUE(ubbr == res, 
	    alp::as_str() << "UBBR_and_error_normal_mode("
			  << f_clock << ", " << baud_rate << ")");
		    
}
void test_normal_mode()
{
    test::interface("UART_normal_mode");

    // Datos sacados de la app-note AVR306, table 3-2.
    test_normal_mode(1'000'000, 2'400, 25);
    test_normal_mode(1'000'000, 4'800, 12);
    test_normal_mode(1'000'000, 9'600, 6);
    test_normal_mode(1'000'000, 14'400, 3);
    test_normal_mode(1'000'000, 19'200, 2);
    test_normal_mode(1'000'000, 28'800, 1);

    test_normal_mode(4'000'000, 2'400, 103);
    test_normal_mode(4'000'000, 4'800, 51);
    test_normal_mode(4'000'000, 9'600, 25);
    test_normal_mode(4'000'000, 14'400, 16);
    test_normal_mode(4'000'000, 19'200, 12);
    test_normal_mode(4'000'000, 28'800, 8);
    test_normal_mode(4'000'000, 38'400, 6);
    test_normal_mode(4'000'000, 57'600, 3);
    test_normal_mode(4'000'000, 76'800, 2);
    test_normal_mode(4'000'000, 115'200, 1);

    test_normal_mode(8'000'000, 2'400, 207);
    test_normal_mode(8'000'000, 4'800, 103);
    test_normal_mode(8'000'000, 9'600, 51);
    test_normal_mode(8'000'000, 14'400, 34);
    test_normal_mode(8'000'000, 19'200, 25);
    test_normal_mode(8'000'000, 28'800, 16);
    test_normal_mode(8'000'000, 38'400, 12);
    test_normal_mode(8'000'000, 57'600, 8);
    test_normal_mode(8'000'000, 76'800, 6);
    test_normal_mode(8'000'000, 115'200, 3);
    test_normal_mode(8'000'000, 230'400, 1);
    test_normal_mode(8'000'000, 250'000, 1);
}

void test_double_speed_mode(uint32_t f_clock, uint32_t baud_rate, uint16_t res)
{
    auto [ubbr, error] = avr_::impl_of::UBBR_and_error_double_speed_mode(f_clock, baud_rate);
    CHECK_TRUE(ubbr == res, 
	    alp::as_str() << "UBBR_and_error_double_speed_mode("
			  << f_clock << ", " << baud_rate << ")");
		    
}
void test_double_speed_mode()
{
    test::interface("UART_double_speed_mode");

    // Datos sacados de la app-note AVR306, table 3-2.
    test_double_speed_mode(1'000'000, 2'400, 51);
    test_double_speed_mode(1'000'000, 4'800, 25);
    test_double_speed_mode(1'000'000, 9'600, 12);
    test_double_speed_mode(1'000'000, 14'400, 8);
    test_double_speed_mode(1'000'000, 19'200, 6);
    test_double_speed_mode(1'000'000, 28'800, 3);

    test_double_speed_mode(4'000'000, 2'400, 207);
    test_double_speed_mode(4'000'000, 4'800, 103);
    test_double_speed_mode(4'000'000, 9'600, 51);
    test_double_speed_mode(4'000'000, 14'400, 34);
    test_double_speed_mode(4'000'000, 19'200, 25);
    test_double_speed_mode(4'000'000, 28'800, 16);
    test_double_speed_mode(4'000'000, 38'400, 12);
    test_double_speed_mode(4'000'000, 57'600, 8);
    test_double_speed_mode(4'000'000, 76'800, 6);
    test_double_speed_mode(4'000'000, 115'200, 3);

    test_double_speed_mode(8'000'000, 2'400, 415);
    test_double_speed_mode(8'000'000, 4'800, 207);
    test_double_speed_mode(8'000'000, 9'600, 103);
    test_double_speed_mode(8'000'000, 14'400, 68);
    test_double_speed_mode(8'000'000, 19'200, 51);
    test_double_speed_mode(8'000'000, 28'800, 34);
    test_double_speed_mode(8'000'000, 38'400, 25);
    test_double_speed_mode(8'000'000, 57'600, 16);
    test_double_speed_mode(8'000'000, 76'800, 12);
    test_double_speed_mode(8'000'000, 115'200, 8);
    test_double_speed_mode(8'000'000, 230'400, 3);
    test_double_speed_mode(8'000'000, 250'000, 3);
}

template <uint32_t f_clock, uint32_t baud_rate, uint32_t max_error = 2>
void test_UBBRn(avr_::UART_mode res_mode,
		uint16_t res_ubbrn)
{
    auto [mode, ubbrn] = avr_::UBBRn<f_clock, baud_rate, max_error>();
    CHECK_TRUE(mode == res_mode and ubbrn == res_ubbrn,
	    alp::as_str() << "UBBRn<" << f_clock << ", " << baud_rate << ">");
}
void test_UBBRn()
{
    test::interface("UBBRn");

    using Mode = avr_::UART_mode;

    // Datos proceden de la app-note AVR306.
    // Observar que el error cometido va aumentando a medida que aumentamos el
    // baud_rate 
    test_UBBRn<1'000'000, 2'400>(Mode::normal, 25);
    test_UBBRn<1'000'000, 4'800>(Mode::normal, 12);
    test_UBBRn<1'000'000, 9'600>(Mode::double_speed, 12);
    test_UBBRn<1'000'000, 14'400, 4>(Mode::double_speed, 8);
    test_UBBRn<1'000'000, 19'200, 9>(Mode::double_speed, 6);
    test_UBBRn<1'000'000, 28'800, 9>(Mode::normal, 1);
    test_UBBRn<1'000'000, 38'400, 9>(Mode::double_speed, 2);


    test_UBBRn<4'000'000, 2'400>(Mode::normal, 103);
    test_UBBRn<4'000'000, 4'800>(Mode::normal, 51);
    test_UBBRn<4'000'000, 9'600>(Mode::normal, 25);
    test_UBBRn<4'000'000, 14'400>(Mode::double_speed, 34);
    test_UBBRn<4'000'000, 19'200>(Mode::normal, 12);
    test_UBBRn<4'000'000, 28'800, 3>(Mode::double_speed, 16);
    test_UBBRn<4'000'000, 38'400>(Mode::double_speed, 12);
    test_UBBRn<4'000'000, 57'600, 4>(Mode::double_speed, 8);
    test_UBBRn<4'000'000, 76'800, 7>(Mode::double_speed, 6);
    test_UBBRn<4'000'000, 115'200, 9>(Mode::normal, 1);

    test_UBBRn<8'000'000, 2'400>(Mode::normal, 207);
    test_UBBRn<8'000'000, 4'800>(Mode::normal, 103);
    test_UBBRn<8'000'000, 9'600>(Mode::normal, 51);
    test_UBBRn<8'000'000, 14'400>(Mode::normal, 34);
    test_UBBRn<8'000'000, 19'200>(Mode::normal, 25);
    test_UBBRn<8'000'000, 28'800>(Mode::double_speed, 34);
    test_UBBRn<8'000'000, 38'400>(Mode::normal, 12);
    test_UBBRn<8'000'000, 57'600>(Mode::double_speed, 16);
    test_UBBRn<8'000'000, 76'800>(Mode::double_speed, 12);
    test_UBBRn<8'000'000, 115'200, 4>(Mode::double_speed, 8);
    test_UBBRn<8'000'000, 230'400, 9>(Mode::normal, 1);


    print_normal_mode(16'000'000, 921'600);
    print_double_speed_mode(16'000'000, 921'600);
    print_ubbrn<16'000'000, 921'600, 9>();

    print_normal_mode(16'000'000, 1'000'000);
    print_double_speed_mode(16'000'000, 1'000'000);
    print_ubbrn<16'000'000, 1'000'000>();
}

int main()
{
try{
    test::header("uart_baud_rate");

    test_normal_mode();
    test_double_speed_mode();
    test_UBBRn();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





