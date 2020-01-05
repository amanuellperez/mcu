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

// Hacemos el coche fantástico 
#include "../../../dev_led.h"
#include <avr_time.h>

using namespace dev;

struct LEDS{
    void on(uint8_t i){
	switch(i){
	    case 0: led0.on(); break;
	    case 1: led1.on(); break;
	    case 2: led2.on(); break;
	    case 3: led3.on(); break;
	    case 4: led4.on(); break;
	    case 5: led5.on(); break;
	    case 6: led6.on(); break;
	    case 7: led7.on(); break;
	}
    }

    void off(uint8_t i){
	switch(i){
	    case 0: led0.off(); break;
	    case 1: led1.off(); break;
	    case 2: led2.off(); break;
	    case 3: led3.off(); break;
	    case 4: led4.off(); break;
	    case 5: led5.off(); break;
	    case 6: led6.off(); break;
	    case 7: led7.off(); break;
	}
    }


    LED<16> led7;
    LED<15> led6;
    LED<14> led5;
    LED<13> led4;
    LED<12> led3;
    LED<11> led2;
    LED<10> led1;
    LED<9> led0;
};

struct Main_app{
    LEDS led;
    constexpr static uint8_t num_leds = 8;

    // Distintos efectos
    void coche_fantastico(uint8_t n);
    void cayendo(uint8_t n);
    void parpadea(uint8_t n);
    void rellena_rebotando(uint8_t n);

    void coche_fantastico();
    void cayendo();
    void parpadea();
    void rellena_rebotando();


    // bajo nivel
    void hacia_adelante();
    void hacia_atras();
    void cae_hasta_piso(uint8_t p);

    // Cae desde el piso superior s hasta el inferior i, dejando i encendido
    void cae_desde_hasta(uint8_t s, uint8_t i);
    void sube_desde_hasta(uint8_t i, uint8_t s);
    void rebota(uint8_t i, uint8_t s);

    void todos_on();
    void todos_off();

};

void Main_app::hacia_adelante()
{
    for (uint8_t i = 0; i < 8; ++i){
	led.on(i);
	wait_ms(80);
	led.off(i);
    }
}

void Main_app::hacia_atras()
{
    for (uint8_t i = 8; i > 0; --i){
	led.on(i-1);
	wait_ms(80);
	led.off(i-1);
    }
}

void Main_app::coche_fantastico(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i)
	coche_fantastico();
}


void Main_app::coche_fantastico()
{
    hacia_adelante();
    hacia_atras();
}

void Main_app::todos_on()
{
    for (uint8_t i = 0; i < 8; ++i)
	led.on(i);
}


void Main_app::todos_off()
{
    for (uint8_t i = 0; i < 8; ++i)
	led.off(i);
}


void Main_app::cayendo(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i){
	todos_off();
	wait_ms(200);
	cayendo();
	wait_ms(200);
    }

    todos_off();
}

void Main_app::cayendo()
{
    for (uint8_t i = 0; i < 8; ++i)
	cae_hasta_piso(i);
}

void Main_app::cae_hasta_piso(uint8_t p)
{
    for (uint8_t i = 7; i > p; --i){
	led.on(i);
	wait_ms(100);
	led.off(i);
    }

    led.on(p);
}

void Main_app::parpadea(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i)
	parpadea();
}

void Main_app::parpadea()
{
    todos_on();
    wait_ms(100);
    todos_off();
    wait_ms(100);
}

// Cae desde el piso superior s hasta el inferior i, dejando i encendido
// Postcondicion: i queda encendido
void Main_app::cae_desde_hasta(uint8_t s, uint8_t i)
{
    while (s > i){
	led.on(s);
	wait_ms(100);
	led.off(s);
	--s;
    }

    led.on(i);
}

// Sube desde el piso inferior i hasta el superior s, dejando s encendido
// Postcondicion: s queda encendido
void Main_app::sube_desde_hasta(uint8_t i, uint8_t s)
{
    while (i < s){
	led.on(i);
	wait_ms(100);
	led.off(i);
	++i;
    }

    led.on(s);
}

// Rebota: esto es, cae desde s hasta i, y rebota subiendo desde i hasta s
// Postcondicion: deja i y s encendidos
// [ expects: i <= s ]
void Main_app::rebota(uint8_t i, uint8_t s)
{
    cae_desde_hasta(s, i);
    ++i;

    if (i <= s)
	sube_desde_hasta(i, s);
}

void Main_app::rellena_rebotando(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i){
	rellena_rebotando();
	wait_ms(500);
    }
}

void Main_app::rellena_rebotando()
{
    uint8_t i = 0;
    uint8_t s = num_leds - 1;

    todos_off();

    while (s >= i){
	rebota(i, s);
	++i;
	--s;
    }

}


int main() 
{
    Main_app app;

    while(1){
	app.parpadea(4);
	app.coche_fantastico(3);
	app.cayendo(3);
	app.rellena_rebotando(3);
    }

}


