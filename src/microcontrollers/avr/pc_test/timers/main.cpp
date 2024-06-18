// Copyright (C) 2022 Manuel Perez 
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

// Pruebo las funciones para calcular las frecuencias.
// Lo ideal sería usar las funciones definidas en avr_tiemrn_basic
// pero tienen muchas dependencias con avr. Se podría hacer el programa en el
// avr directamente y probarlo via UART, pero este programa es más fácil de
// modificar y probar.

#include <alp_test.h>
#include <alp_string.h>

#include <array>
#include <iostream> 

using namespace test;

// Fórmula para calcular la frecuencia dados el divisor d (prescaler_factor) y
// el top M del counter. (pag. 132 datasheet)
template <uint32_t f_clock>
inline constexpr uint32_t freq(uint32_t d, uint32_t M)
{ return f_clock / (2 * d * (1 + M)); }

// Se podía cometer menos error si se redondeara dx1_M (= d*(1+M)).
// Pero eso llevaría más tiempo y como muestra el test no se comete un error
// de más del 10%.
template <uint32_t f_clock>
constexpr std::pair<uint32_t, uint32_t> calculate_prescaler_top(uint32_t freq_in_Hz)
{
    constexpr std::array<uint32_t, 5> prescaler_factor = {1, 8, 64, 256, 1024};
    constexpr uint8_t counter_top = 255;

    // f = f_clock / (2 * d * (1 + M)) ==> d*(1 + M) = f_clock / (2 * f);
    uint32_t dx1_M = f_clock / (2 * freq_in_Hz);

    for (size_t i = 0; i < prescaler_factor.size(); ++i){
	uint32_t M = dx1_M / prescaler_factor[i];
	if (M <= counter_top)
	    return {prescaler_factor[i], M - 1};
    }

    return {0, 0};

}

uint32_t abs_diff(uint32_t x, uint32_t y)
{
    if (x > y) return x - y;
    else return y - x;
}

void test_printing_freq()
{
    constexpr uint32_t f_clock = 1'000'000;
    std::cout << f_clock << '\n';

    std::cout << "freq\t[ d , M ]\n";
    for (uint32_t f = 20; f < 100; ++f){
	auto [d, M] = calculate_prescaler_top<f_clock>(f);

	std::cout << f << "\t";
	if (d != 0 and M != 0){
	    std::cout << '[' << d << ", " << M << "]\t= "
		      << freq<f_clock>(d, M) << " Hz\t"
		         "dif = " << abs_diff(freq<f_clock>(d, M), f) << '\n';
	}

	else 
	    std::cout << "ERROR\n";
    }
}

bool error_bigger_10percent(uint32_t x, uint32_t y)
{
    return !(0.9*x < y and y < 1.1*x);
}


void test_freq()
{
    std::cout << "Compruebo que la frecuencia generada está\n"
		 "dentro del 10% de error. Solo imprime algo en caso de error.\n";
    constexpr uint32_t f_clock = 1'000'000;

    for (uint32_t f = 20; f < 20'000; ++f){
	auto [d, M] = calculate_prescaler_top<f_clock>(f);
	if (d == 0 or M == 0)
	    std::cout << f << " ERROR: d == 0 or M == 0\n";
	
	else if (error_bigger_10percent(freq<f_clock>(d, M), f)){
	    std::cout << "ERRROR: " << f << "\t"
			"[" << d << ", " << M << "]\t= "
		      << freq<f_clock>(d, M) << " Hz\t"
		         "dif = " << abs_diff(freq<f_clock>(d, M), f) << '\n';
	}

    }
}


int main()
{
try{
    test::header("test");

//    test_printing_freq();
    test_freq();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





