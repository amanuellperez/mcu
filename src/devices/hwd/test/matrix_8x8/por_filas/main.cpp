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

#include <avr_port.h>
#include <avr_time.h>

using namespace avr;

using Glyph = uint8_t[8];

// Los positivos los conectamos al puerto B, mientras que los negativos al
// puerto D.
// Elijo: el eje x son los negativos, el eje y los positivos.
class MatrixLEDs_8x8{
public:
    MatrixLEDs_8x8();

    void barre_todos_los_leds(uint8_t time);
    void show(const Glyph& glyph);

    void show(const Glyph& glyph, uint16_t n);
    
    // Esta función solo es para probar la intensidad obtenida al encenderlos
    // todos los leds. En la práctica no creo que la use.
    void todos_on();

private:
    PuertoB_de_salida pos;
    PuertoD_de_salida neg;

    static constexpr uint8_t num_pins_ = 8;
};

MatrixLEDs_8x8::MatrixLEDs_8x8()
{
    pos = 0x00;
    neg = 0xFF;
}


void MatrixLEDs_8x8::barre_todos_los_leds(uint8_t time)
{
    for (uint8_t p = 1; p != 0; p <<= 1){
	pos = p;

	for (uint8_t n = 1; n != 0; n <<= 1){
	    neg = ~n;
	    wait_ms(time);
	}
    }
}



void MatrixLEDs_8x8::todos_on()
{
    for (uint8_t p = 1; p != 0; p <<= 1){
	pos = p;

	for (uint8_t n = 1; n != 0; n <<= 1)
	    neg = ~n;
    }
}

// Incluyo 3 opciones, la tercera la ganadora, ya que el planteamiento natural
// da problemas con el primer led: apenas lo enciende. ¿Por qué? (???)
void MatrixLEDs_8x8::show(const Glyph& glyph)
{
    uint8_t p = 1;
    for (uint8_t i = 0; i < num_pins_; ++i, p <<= 1){
	neg = 0xFF;
	pos = p;
	// Opción 3: enciendo cada led por separado.
	for (uint8_t num_led = 1; num_led != 0; num_led <<= 1){
	    neg = ~(glyph[i] & num_led);
	  //  wait_us(10);  // No es necesario el wait. El bucle for ya 
			    // introduce el tiempo de espera.
	}
	// Opción 1: Con esto debería de funcionar.
	// Sin embargo tengo el problema de que apenas se enciende el primer
	// led. ¿Por qué?
	// neg = ~(glyph[i]);
	// wait_us(10);	// tiempo que mantenemos los leds encendidos
	// Opción 2: Encendemos el primer led por separado y luego el resto.
	// Como el primer led está más tiempo encendido que los demás, brilla
	// un poco más que el resto.
	// neg = ~(glyph[i] & 1);
	// wait_us(10); 
	// neg = ~(glyph[i]);
	// wait_us(10);
    }
}

void MatrixLEDs_8x8::show(const Glyph& glyph, uint16_t n)
{
    while (n > 0){
	show(glyph);
	--n;
    }
}


// Para hacer los glyps lo que he hecho ha sido dibujarlos y luego ejecutar
// sobre ellos el script letras.sh.
//
// Ejemplo:
//  La letra A es
//	xxx
//     x   x
//     x   x
//     xxxxx
//     x   x
//     x   x
//
//  Dibujo esto y luego ejecuto letras.sh

constexpr Glyph glyphs_letras[] = {
{0b0000000,0b0000000,0b0000000,0b0000000,0b0000000,0b0000000,0b0000000}, // SPACE
{0b1000100,0b1000100,0b1000100,0b1111100,0b1000100,0b1000100,0b0111000}, // A
{0b0111100,0b1000100,0b1000100,0b0111100,0b1000100,0b1000100,0b0111100}, // B
{0b0111000,0b1000100,0b0000100,0b0000100,0b0000100,0b1000100,0b0111000}, // C
{0b0111100,0b1000100,0b1000100,0b1000100,0b1000100,0b1000100,0b0111100}, // D
{0b1111100,0b0000100,0b0000100,0b0111100,0b0000100,0b0000100,0b1111100}, // E
{0b0000100,0b0000100,0b0000100,0b0111100,0b0000100,0b0000100,0b1111100}, // F
{0b0111000,0b1000100,0b1100100,0b0000100,0b0000100,0b1000100,0b0111000}, // G
{0b1000100,0b1000100,0b1000100,0b1111100,0b1000100,0b1000100,0b1000100}, // H
{0b0111000,0b0010000,0b0010000,0b0010000,0b0010000,0b0010000,0b0111000}, // I
{0b0011000,0b0100100,0b0100000,0b0100000,0b0100000,0b0100000,0b1110000}, // J
{0b1000100,0b0100100,0b0010100,0b0001100,0b0010100,0b0100100,0b1000100}, // K
{0b1111100,0b0000100,0b0000100,0b0000100,0b0000100,0b0000100,0b0000100}, // L
{0b1000100,0b1000100,0b1000100,0b1010100,0b1010100,0b1101100,0b1000100}, // M
{0b1000100,0b1000100,0b1100100,0b1010100,0b1001100,0b1000100,0b1000100}, // N
{0b0111000,0b1000100,0b1000100,0b1000100,0b1000100,0b1000100,0b0111000}, // O
{0b0000100,0b0000100,0b0000100,0b0111100,0b1000100,0b1000100,0b0111100}, // P
{0b1011000,0b0100100,0b1010100,0b1000100,0b1000100,0b1000100,0b0111000}, // Q
{0b1000100,0b0100100,0b0010100,0b0111100,0b1000100,0b1000100,0b0111100}, // R
{0b0111100,0b1000000,0b1000000,0b0111000,0b0000100,0b0000100,0b1111000}, // S
{0b0010000,0b0010000,0b0010000,0b0010000,0b0010000,0b0010000,0b1111100}, // T
{0b0111000,0b1000100,0b1000100,0b1000100,0b1000100,0b1000100,0b1000100}, // U
{0b0010000,0b0101000,0b0101000,0b1000100,0b1000100,0b1000100,0b1000100}, // V
{0b0101000,0b1010100,0b1010100,0b1010100,0b1000100,0b1000100,0b1000100}, // W
{0b1000100,0b1000100,0b0101000,0b0010000,0b0101000,0b1000100,0b1000100}, // X
{0b0010000,0b0010000,0b0010000,0b0010000,0b0101000,0b1000100,0b1000100}, // Y
{0b1111100,0b0000100,0b0001000,0b0010000,0b0100000,0b1000000,0b1111100}, // Z




{0b0111000,0b1000100,0b1001100,0b1010100,0b1100100,0b1000100,0b0111000}, // 0
{0b0111000,0b0010000,0b0010000,0b0010000,0b0010000,0b0011000,0b0010000}, // 1
{0b1111100,0b0001000,0b0010000,0b0100000,0b1000000,0b1000100,0b0111000}, // 2
{0b0111000,0b1000100,0b1000000,0b0100000,0b0010000,0b0100000,0b1111100}, // 3
{0b0100000,0b0100000,0b1111100,0b0100100,0b0101000,0b0110000,0b0100000}, // 4
{0b0111000,0b1000100,0b1000000,0b1000000,0b0111100,0b0000100,0b1111100}, // 5
{0b0111000,0b1000100,0b1000100,0b0111100,0b0000100,0b0001000,0b0110000}, // 6
{0b0001000,0b0001000,0b0001000,0b0010000,0b0100000,0b1000000,0b1111100}, // 7
{0b0111000,0b1000100,0b1000100,0b0111000,0b1000100,0b1000100,0b0111000}, // 8
{0b0011000,0b0100000,0b1000000,0b1111000,0b1000100,0b1000100,0b0111000}  // 9

};

Glyph glyph_caracter_desconocido{
0b01010101,
0b10101010,
0b01010101,
0b10101010,
0b01010101,
0b10101010,
0b01010101,
0b10101010
};

class Letras{
public:
    static constexpr uint8_t offset_glyph0 = 26;

    const Glyph& operator[](char c)
    {
	if (65 <= c and c <= 90)
	    return glyphs_letras[c - 65];
	else if (48 <= c and c <= 57)
	    return glyphs_letras[c - 48 + offset_glyph0];

	return glyph_caracter_desconocido;
    }
    
};

int main()
{
    MatrixLEDs_8x8 mat;
//    Letras letra;
//    char msg[] = "HOLA, QUE TAL ESTAS ";

    while(1){
//	mat.barre_todos_los_leds(100);
//	mat.todos_on();
    
//	for (char* p = &msg[0]; *p != '\0'; ++p)
//	    mat.show(letra[*p], 800);
	for (uint8_t i = 1; i < 4; ++i)
	    mat.show(glyphs_letras[3], 800);
    }
}
