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

#include "../../avr_interrupt.h"
#include "../../mcu_led.h"
#include "../../mcu_pulsador.h"
#include "../../avr_time.h"

// Para ver una versión más básica ver main00.cpp

// DUDA: Las interrupciones necesitan acceso a variables globales. 
//	Una forma de evitar tener acceso a las variables globales es definir
//	una aplicacion app global. ¿Otra forma mejor?

using namespace avr;

// configuración de la aplicación
constexpr uint8_t num_pin_pulsador = 4;
#define ISR_PCINT  ISR_PCINT_PIN4

// Aplicación que ejecutamos
struct Aplicacion{
    Pulsador pulsador{num_pin_pulsador};
    LED led{13_pin};

    Aplicacion()
    {
	Interrupt::enable_pin<num_pin_pulsador>();
    }

    void run() {wait_ms(1000);}

    void ejecuta_cuando_interrupcion()
    {
	if (pulsador.esta_pulsado())
	    led.on();
	else 
	    led.off();
    }
};


Aplicacion app;

ISR_PCINT { app.ejecuta_cuando_interrupcion(); }

 
int main()
{
    while(1){
	app.run();
    }
}



