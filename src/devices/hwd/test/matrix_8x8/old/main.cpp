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

// FUNCIONA pero baja mucho la intensidad de los leds si se quieren encender 
// todos los LED a la vez.  Al acceder directamente a los puertos mejora en
// intensidad.
// Esta es una matriz de leds 8x8
// Al ser este un dispositivo de bajo nivel, no uso LED, sino directamente
// accedo a los pins.

#include <avr_pin.h>
#include <avr_time.h>

using namespace avr;

// 
template
<uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3, 
uint8_t p4, uint8_t p5, uint8_t p6, uint8_t p7>
struct Ocho_pines{
    Pin_de_salida<p0> pin0;
    Pin_de_salida<p1> pin1;
    Pin_de_salida<p2> pin2;
    Pin_de_salida<p3> pin3;
    Pin_de_salida<p4> pin4;
    Pin_de_salida<p5> pin5;
    Pin_de_salida<p6> pin6;
    Pin_de_salida<p7> pin7;

    void write_one(uint8_t i){
	switch(i){
	    case 0: pin0.write_one(); return;
	    case 1: pin1.write_one(); return;
	    case 2: pin2.write_one(); return;
	    case 3: pin3.write_one(); return;
	    case 4: pin4.write_one(); return;
	    case 5: pin5.write_one(); return;
	    case 6: pin6.write_one(); return;
	    case 7: pin7.write_one(); return;
	}
    }


    void write_zero(uint8_t i){
	switch(i){
	    case 0: pin0.write_zero(); return;
	    case 1: pin1.write_zero(); return;
	    case 2: pin2.write_zero(); return;
	    case 3: pin3.write_zero(); return;
	    case 4: pin4.write_zero(); return;
	    case 5: pin5.write_zero(); return;
	    case 6: pin6.write_zero(); return;
	    case 7: pin7.write_zero(); return;
	}
    }

};


// Le pasamos los pines positivos p0-p7 y los negativos n0-n7 a los que
// conectamos la matriz de LEDs
// El eje x son los negativos, el eje y los positivos.
template
<typename Positivos, typename Negativos>
struct Matix_8x8{
    static constexpr uint8_t num_pins = 8;

    Positivos pos;
    Negativos neg;

    void write_one(uint8_t i, uint8_t j)
    {
	pos.write_one(i);
	neg.write_zero(j);
    }

    void write_zero(uint8_t i, uint8_t j)
    {
	pos.write_zero(i);
	neg.write_one(j);
    }

    void clear()
    {
	for (uint8_t i = 0; i < num_pins; ++i){
	    pos.write_zero(i);
	    neg.write_one(i);
	}

    }

    void barre_fila(uint8_t i)
    {
	pos.write_one(i);

	for (uint8_t j = 0; j < num_pins; ++j){
	    neg.write_zero(j);
	    wait_ms(100);
	    neg.write_one(j);
	}

	pos.write_zero(i);

    }

    void barre_todos_los_leds()
    {
	for (uint8_t i = 0; i < 8; ++i)
	    for (uint8_t j = 0; j < 8; ++j){
		write_one(i,j);
		wait_ms(100);
		write_zero(i,j);
	    }
    }


    void barre_todos_los_leds(uint8_t n)
    {
	for (uint8_t i = 0; i < n; ++i)
	    barre_todos_los_leds();
    }


    void todos_on()
    {
	for (uint8_t i = 0; i < 8; ++i){
	    pos.write_one(i);

	    for (uint8_t j = 0; j < 8; ++j){
		neg.write_zero(j);
		neg.write_one(j);
	    }

	    pos.write_zero(i);
	}
    }

};


int main() 
{
    Matix_8x8<Ocho_pines<14,15,16,17,18,19,9,10>,
	Ocho_pines<2,3,4,5,6,11,12,13>>mat;

    mat.clear();

    while(1){
	mat.todos_on();
//	mat.barre_todos_los_leds(4);

//	for (uint8_t i = 0; i < 8; ++i)
//	    mat.barre_fila(i);
    }
}


