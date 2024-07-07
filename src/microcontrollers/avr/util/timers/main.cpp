// Copyright (C) 2024 Manuel Perez 
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

// Calculamos las frecuencias que podemos generar con el Timer1

#include <alp_test.h>
#include <alp_string.h>
#include <atd_math.h>

#include <array>
#include <iostream>


using namespace test;

constexpr std::array prescalers = {1,8, 64, 256, 1024};


// constexpr int freq_clock = 1000000;

void print_result(int freq_clock, int freq_generated, 
		  int prescaler, int top, const std::string& mode)
{
    std::cout << freq_generated << '\t' 
	      << prescaler	<< '\t'
	      << top << '\t'
	      << mode << '\t'
	      << freq_clock << '\n';
}

void print_top(int freq_clock, int prescaler, int top)
{
    int freq_generated = freq_clock / (prescaler * (top + 1));
    if (freq_generated == 0)
	return;

    print_result(freq_clock, freq_generated, prescaler, top, "FAST PWM");

    freq_generated = freq_clock / (2*prescaler * top);
    if (freq_generated == 0)
	return;

    print_result(freq_clock, freq_generated, prescaler, top, "PHASE PWM");
}

void print_top_ICR1(int freq_clock, int prescaler, int max_top)
{
    for (int top = 4; top <  max_top; ++top)
	print_top(freq_clock, prescaler, top);
}

	
void print_top_ICR1(int freq_clock, int max_top)
{
    std::cout << "ICR1 = variable\n"
	         "---------------\n";
    for (auto p: prescalers)
	print_top_ICR1(freq_clock, p, max_top);

    std::cout << '\n';
}


void print_top_0x00FF(int freq_clock)
{
    std::cout << "TOP = 0x00FF\n"
	         "------------\n";
    for (auto p: prescalers)
	print_top(freq_clock, p, 0x00ff);

    std::cout << '\n';
}

void print_top_0x01FF(int freq_clock)
{
    std::cout << "TOP = 0x01FF\n"
	         "------------\n";
    for (auto p: prescalers)
	print_top(freq_clock, p, 0x01ff);

    std::cout << '\n';
}

void print_top_0x03FF(int freq_clock)
{
    std::cout << "TOP = 0x03FF\n"
	         "------------\n";
    for (auto p: prescalers)
	print_top(freq_clock, p, 0x03ff);

    std::cout << '\n';
}

void print_tops_timer1()
{
    constexpr int freq_clock = 8000000;

    std::cout << "---------------------------------------------\n";
    std::cout << "Frequencies that can be generated with Timer1\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Micro frequency = " << freq_clock << " Hz\n\n";

//    print_top_ICR1(freq_clock);
    print_top_0x00FF(freq_clock);
    print_top_0x01FF(freq_clock);
    print_top_0x03FF(freq_clock);
}


// Método 1 para calcular el prescaler, top y mode
//  El método 1 cuando encuentra un posible resultado para (pres, top, mode)
//  se queda con ese resultado sin mirar más (no mira el error que comete).
// ¿Qué error cometemos si aplicamos ese método? 
// ¡Saquemos estadísticas!
//	El máximo error es 30%, lo cual es bastante.
//	Para las frecuencias bajas funciona bastante bien.
//
//	Freq	    max error
//	50  kHz		 8%
//	86  kHz		14%
//	100 kHz		17%
//	120 kHz		21%
void error_method1(int freq_clk, int freq_gen, int max_top)
{

    // Las fórmulas son:
    //	    Fast  PWM: freq_gen = freq_clk/(p * (top + 1);
    //	    Phase PWM: freq_gen = freq_clk/(2 * p * top);
    //
    // siendo p = prescaler.
    // Las dos fórmulas se pueden fusionar en:
    //		freq_gen = freq_clk / (p * top);    (3)
    //	entendiendo p ó 2p, ó (top + 1) o top dependiendo del modo aplicado.
    //	Como los preescalares son 1,8,64, 256 y 1024, el valor de p en la
    //	fórmula (3) será pres = {1, 2, 8,16, 64, 128 ,256, 512, 1024 y 2048},
    //	correspondiendo las posiciones pares pres[2n] a Fast PWM, y las
    //	impares pres[2n + 1] a Phase PWM.
    constexpr std::array prescaler
			    = {1, 2, 8, 16, 64, 128, 256, 512, 1024, 2048 };


    // Si se trunca se comete en algunas ocasiones mucho error
    int q = atd::divide_rounding(freq_clk, freq_gen);
    int freq_res = 0;
    for (size_t i = 0; i < prescaler.size(); ++i){
	if (prescaler[i] > q)
	    break;

	int top = q / prescaler[i];

	freq_res = freq_clk / (prescaler[i] * top);

	if ((i % 2) == 0)
	    --top;


	if (top > 4 and top < max_top){
	    freq_res = freq_clk / (prescaler[i] * top);
	    float error = (float) freq_res / (float) freq_gen - 1;
	    if (error >= 0.01)
		std::cout << std::fixed << std::setprecision(2) << error << '\t' ;
	    else
		std::cout << 0 << '\t';

	    std::cout << freq_gen << '\t' // << '(' << freq_res << ')' << '\t'
		      << prescaler[i] << '\t'
		      << top << '\t';

	    if (i % 2)
		std::cout << "\tPHASE\n";
	    else
		std::cout << "\tFAST\n";
	    return;
		      
	}
    }
    std::cout << -1 << '\t' << freq_gen << '\n';

}

void error_method1(int max_top)
{
    int freq_clk = 1'000'000;

    for (int freq_gen = 20; freq_gen < 1'000'000; ++freq_gen)
    // for (int freq_gen = 20; freq_gen < 100'000; ++freq_gen)
	error_method1(freq_clk, freq_gen, max_top);
}


struct PWM_mode{
    int top;
    int prescaler;
    bool fast_mode; // if false, then mode == phase_mode
};

// Devuelve el error para poder imprimirlo
float calculate_data2(int freq_clk, int freq_gen, PWM_mode& res, int max_top)
{
    res.prescaler = 0; // invalid data
    float last_error = 1;

    // Las fórmulas son:
    //	    Fast  PWM: freq_gen = freq_clk/(p * (top + 1);
    //	    Phase PWM: freq_gen = freq_clk/(2 * p * top);
    //
    // siendo p = prescaler.
    // Las dos fórmulas se pueden fusionar en:
    //		freq_gen = freq_clk / (p * top);    (3)
    //	entendiendo p ó 2p, ó (top + 1) o top dependiendo del modo aplicado.
    //	Como los preescalares son 1,8,64, 256 y 1024, el valor de p en la
    //	fórmula (3) será pres = {1, 2, 8,16, 64, 128 ,256, 512, 1024 y 2048},
    //	correspondiendo las posiciones pares pres[2n] a Fast PWM, y las
    //	impares pres[2n + 1] a Phase PWM.
    constexpr std::array prescaler
			    = {1, 2, 8, 16, 64, 128, 256, 512, 1024, 2048 };


    // Si se trunca se comete en algunas ocasiones mucho error
    int q = atd::divide_rounding(freq_clk, freq_gen);
    int freq_res = 0;
    for (size_t i = 0; i < prescaler.size(); ++i){

	if (prescaler[i] > q)
	    return last_error;

	int top = q / prescaler[i];

	freq_res = freq_clk / (prescaler[i] * top);

	if ((i % 2) == 0)
	    --top;

	if (top > 4 and top < max_top){
	    freq_res = freq_clk / (prescaler[i] * top);
	    float error = (float) freq_res / (float) freq_gen - 1;

	    if (error < last_error){
		last_error = error;
		res.top = top;

		if ((i % 2) == 0){
		    res.prescaler = prescaler[i];
		    res.fast_mode = true;
		}
		else {
		    res.prescaler = prescaler[i] / 2;
		    res.fast_mode = false;
		}
	    }
	}

    }

    return last_error;
}

// Método 2 para calcular el prescaler, top y mode
//  El método 2 va a mirar todas las posiblidiades que hay (10 en total)
//  quedándose con la que menos error genere.
// Estadísticas:
//	Genera mejores resultados que el método 1.
//  
//	Freq	    max error
//	50  kHz		 7%
//	86  kHz		12%
//	100 kHz		15%
//	120 kHz		21%
//	
//	hasta 1MHz:
//	    1) Genera 47 mil resultados correctos, con 0% de error
//	       frente a los 11 mil resultados del método 1.
//
//	Estos resultados son a primera vista, no tomarlos como definitivos.
//
//  Optimización:
//	Viendo los resultados generados, si freq_clk = 1MHz todos los
//	resultados tienen prescaler = 1, modo fast or phase. Luego basta con
//	mirar estos dos casos ignorando el resto.
//
void error_method2(int freq_clk, int freq_gen, int max_top)
{
    PWM_mode mode;
    float error = calculate_data2(freq_clk, freq_gen, mode, max_top);

    std::cout << std::right << std::setw(6);
    if (error >= 0.01)
	std::cout << std::fixed << std::setprecision(2) << error;
    else
	std::cout << 0;

    std::cout << std::setw(10) << freq_gen 
	      << std::setw(10) << mode.prescaler 
	      << std::setw(10) << mode.top;

    std::cout << std::setw(5) << ' '
	      << std::left << std::setw(5);

    if (mode.fast_mode)
	std::cout << "FAST\n";
    else
	std::cout << "PHASE\n";

}

void error_method2(int max_top)
{
    int freq_clk = 1'000'000;

    std::cout << std::setfill(' ') << std::right;
    std::cout << std::setw(6) << "Error" 
	      << std::setw(10) << "Freq. gen"
	      << std::setw(10) << "Prescaler"
	      << std::setw(10) << "Top"
	      << std::setw(5) << ' '
	      << std::setw(5) << "Mode"
	      << '\n';
    
    std::cout << std::setw(6) << "-----" 
	      << std::setw(10) << "---------"
	      << std::setw(10) << "---------"
	      << std::setw(10) << "---"
	      << std::setw(5) << ' '
	      << std::setw(5) << "----"
	      << '\n';

    // for (int freq_gen = 20; freq_gen < 1'000'000; ++freq_gen)
    //for (int freq_gen = 20; freq_gen < 120'000; ++freq_gen)
    // for (int freq_gen = 20; freq_gen < 120'000; freq_gen +=1'000)
    for (int freq_gen = 20; freq_gen < 10'000; freq_gen +=10)
    //for (int freq_gen = 20; freq_gen < 40; ++freq_gen)
	error_method2(freq_clk, freq_gen, max_top);
}

int main_menu()
{
    int max_top{};

    std::cout << "\n\nTimers test\n"
	             "-----------\n"
		     "0. Timer 0\n"
		     "1. Timer 1\n";

    char opt{};
    std::cin >> opt;
    switch (opt){
	break; case '0': max_top = 255;
	break; case '1': 
	break; default : max_top = 65535;
    }

    return max_top;
}


int main()
{
try{
//    int max_top = main_menu();
    int max_top = 255;		// Timer 0
//    int max_top = 65535;	// Timer 1

    std::cout << "max_top = " << max_top << '\n';

//    print_tops_timer1();
//    error_method1(max_top);
    error_method2(max_top);

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





