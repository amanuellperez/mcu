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

#include "prj_main.h"
#include "prj_songs.h"
#include "mus_sounds.h"

void Main::musical_scale_one_line()
{
    constexpr uint16_t t = 400;

    UART uart;
    uart << "\nOne line octave\n";
    musical_scale(Octave::one_line, t);

    uart << "Do\n"; 
    Musician::play(Octave::two_line, Step::Do, t);
}


void Main::musical_scale_all()
{
    constexpr uint16_t t = 400;

    UART uart;
    uart << "\nGreat octave\n";
    musical_scale(Octave::great, t);

    uart << "\nSmall octave\n";
    musical_scale(Octave::small, t);

    uart << "\nOne line octave\n";
    musical_scale(Octave::one_line, t);

    uart << "\nTwo line octave\n";
    musical_scale(Octave::two_line, t);

    uart << "\nThree line octave\n";
    musical_scale(Octave::three_line, t);
}


void Main::musical_scale(Octave octave, uint16_t t)
{
    UART uart;
    uart << "\tDo "; 
    Musician::play(octave, Step::Do, t);

    uart << "Re "; 
    Musician::play(octave, Step::Re, t);

    uart << "Mi "; 
    Musician::play(octave, Step::Mi, t);

    uart << "Fa "; 
    Musician::play(octave, Step::Fa, t);

    uart << "Sol "; 
    Musician::play(octave, Step::Sol, t);

    uart << "La "; 
    Musician::play(octave, Step::La, t);

    uart << "Si "; 
    Musician::play(octave, Step::Si, t);
}

bool Main::organ_toy_UART_are_data_unread(const uint16_t& counter)
{
    if (UART::are_there_unread_data())
	return true;
    
    if (counter > 1){
	Micro::wait_ms(50);
	return UART::are_there_unread_data();
    }

    for (uint8_t i = 0; i < 8; ++i){
	Micro::wait_ms(50);
	if (UART::are_there_unread_data())
		return true;
    }

    return false;
}

// (RRR) ¿por qué complicarlo con max_counter?
//       Resulta que mi teclado (? o es el sistema operativo? o es el terminal
//       que uso?) cuando presionas una tecla y la mantienes presionada hasta
//       envía a pulsos la tecla pulsada.
//       Esto es, si mantengo pulsada la tecla 's' en UART aparece:
//       t = 0: 's'
//       t = 500 ms más menos: 's'
//       t = ??? ms (menor que 500 ms) resto de 's'
//       El driver del teclado (o quien sea) va muestreando el teclado: si
//       presionas una vez, muestrea la siguiente vez por la misma tecla 500
//       ms después; si lo mantienes pulsado sin soltar la muestrea más
//       rápidamente (a velocidad constante).
//       Si se cambia y presiona otra tecla inmediatamente aparece en
//       pantalla.
//
//       No funciona del todo bien (cuando sueltas una tecla se queda sonando
//       500 ms, tal como está programado), pero funciona suficientemente
//       bien. Total esto es una prueba. 
//
//       No sirve para tocar "La cucaracha" :( por culpa de la permanencia de
//       los 500 ms; ni tampoco el "Oda a la alegría" por el mismo motivo. Es
//       un vulgar PoC :(
//
//       Con todo a partir de esto es muy facil añadirle teclas y crear un
//       órgano de juguete que funcione correctamente.
void Main::organ_toy()
{
    UART uart;
    uart << "Organ toy\n"
	    "(This layout is for a spanish keyboard)\n"
	    "One line octave:\n\n"
	    "     Do#(E)  Re#(R)        Fa#(Y)   Sol#(U)  La#(I)\n"
	    "Do(S)   Re(D)   Mi(F) Fa(G)   Sol(H)    La(J)   Si(K)\n\n"
	    "Press a note or '1' to return to menu\n";


    constexpr uint16_t max_counter = 500;
    uint16_t counter = 0;
    while (1){
	char note{};
	if (counter >= max_counter){
	    Musician::silence();
	    counter = 0;
	}

	if (!UART::are_there_unread_data()){
	    ++counter;
	    Micro::wait_ms(1);
	}
	else{
	    uart >> note;
	    counter = 0;

	    if (note == '1'){
		Musician::silence();
		return;
	    }

	    switch(note){
	    break; case 'S': case 's': Musician::play(Octave::one_line, Step::Do);
	    break; case 'E': case 'e': Musician::play(Octave::one_line, Step::DoS);
	    break; case 'D': case 'd': Musician::play(Octave::one_line, Step::Re);
	    break; case 'R': case 'r': Musician::play(Octave::one_line, Step::ReS);
	    break; case 'F': case 'f': Musician::play(Octave::one_line, Step::Mi);
	    break; case 'G': case 'g': Musician::play(Octave::one_line, Step::Fa);
	    break; case 'Y': case 'y': Musician::play(Octave::one_line, Step::FaS);
	    break; case 'H': case 'h': Musician::play(Octave::one_line, Step::Sol);
	    break; case 'U': case 'u': Musician::play(Octave::one_line, Step::SolS);
	    break; case 'J': case 'j': Musician::play(Octave::one_line, Step::La);
	    break; case 'I': case 'i': Musician::play(Octave::one_line, Step::LaS);
	    break; case 'K': case 'k': Musician::play(Octave::one_line, Step::Si);
	    }
	}

    }

}


void Main::play_song()
{
    UART uart;
    uart << "Play song\n";

    music::Mononote_song song{25, ode_to_joy};
    Musician::play(song);

// depurar:
//    for (size_t i = 0; i < song.size(); ++i){
//	auto note = song.note(i); 
//	uart << (int) note.octave << ", " 
//	     <<  (int) note.note << ", " << song.duration_of(note) << '\n';
//    }
    
}


void Main::play_siren()
{
    UART uart;
    uart << "\n\nPlay siren\n"
	        "----------\n"
	    "Some of the sounds sounds better with a speaker instead of a buzzer\n"
	    "1. Siren 1 (speaker)\n"
	    "2. Siren 2.1\n"
	    "3. Siren 2.2\n"
	    "4. Alarm 1 (speaker)\n";

    char ans{};
    uart >> ans;

    switch(ans){
	break; case '1': 
		for (uint8_t i = 0; i < 2; ++i)
		    music::siren01<Musician>(2000, 4000, 10);

	break; case '2': 
		for (uint8_t i = 0; i < 5; ++i)
		    music::siren02<Musician>(400, 800, 500);

	break; case '3': 
		for (uint8_t i = 0; i < 10; ++i)
		    music::siren02<Musician>(2000, 4000, 10);

	break; case '4':
		for (uint8_t i = 0; i < 10; ++i)
		    music::alarm01<Musician>(3000, 50);
		
    }
  
}
