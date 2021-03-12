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

// Probamos el speaker. Conectarlo al pin indicado.
// Hacemos un órgano conectado al teclado.
// Conectar el cable FTDI para tocar las notas que se quieran.
#include "../dev_speaker.h"
#include <avr_UART.h>

// Configuración de las conexiones
constexpr uint8_t num_pin_speaker = 15;

using Speaker = dev::Speaker;



enum Escala5{
    DO  = 955, 
    DOS = 901, 
    RE  = 850, 
    RES = 802, 
    MI  = 757, 
    FA  = 715, 
    FAS = 675, 
    SOL = 637, 
    SOLS= 601, 
    LA  = 567, 
    LAS = 535, 
    SI  = 505};


//constexpr uint16_t melodia[] 
//= { 505, 802, 505, 802, 505
//  , 567, 505, 505, 505, 802
//  , 505, 505, 802, 802, 802
//  , 505, 802, 505, 802, 200};
//
//constexpr uint16_t duracion[] 
//= {500, 500, 500, 500, 500
// , 500, 500, 500, 500, 500
// , 500, 500, 500, 500, 500
// , 500, 500, 500, 500, 500};



void play_note(Speaker& speaker, char c, uint16_t duracion)
{
    switch(c){
	case 'a':
	    speaker.play_note(Escala5::DO, duracion);
	    break;

	case 's':
	    speaker.play_note(Escala5::RE, duracion);
	    break;

	case 'd':
	    speaker.play_note(Escala5::MI, duracion);
	    break;

	case 'f':
	    speaker.play_note(Escala5::FA, duracion);
	    break;

	case 'g':
	    speaker.play_note(Escala5::SOL, duracion);
	    break;

	case 'h':
	    speaker.play_note(Escala5::LA, duracion);
	    break;

	case 'j':
	    speaker.play_note(Escala5::SI, duracion);
	    break;
    }
}



int main()
{
    auto speaker = Speaker::nuevo<num_pin_speaker>();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();
    
    auto uart = UART::as_a_stream();

    uart << "\r\n--------------------------\r\n";
    uart << "ÓRGANO (usando el teclado)\r\n";
    uart << "--------------------------\r\n\r\n";

    uart << "Pulsa las teclas a, s, d, f, g, h, j, para tocar la escala "
	     "musical (en octava 5)\r\n";

    while(1){
	// si quieres tocar una canción automáticamente
//	for(uint8_t i = 0; i < 20; ++i)
//	    speaker.play_note(melodia[i], duracion[i]);

	char c = UART::receive();
	play_note(speaker, c, 200);

    }
}



