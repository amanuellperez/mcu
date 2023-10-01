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

// Probamos la matriz de leds 8x8.
// Funciona también con la matriz de leds comprada 1088 (total, es lo mismo).
#include <avr_port.h>
#include <avr_time.h>

#include <string.h>

#include "../../dev_cols_glyphs.h"

using namespace avr;
using dev::Glyph;

// Los positivos los conectamos al puerto B, mientras que los negativos al
// puerto D.
// Elijo: el eje x son los negativos, el eje y los positivos.
class MatrixLEDs_8x8{
public:
    MatrixLEDs_8x8();


    // Muetra un glyph
    void show(const Glyph& glyph);

    // Muestra un glyph n tics (donde tics es... ¿cuántos microsegundos???)
    void show(const Glyph& glyph, uint16_t n);
    
    /// Muestra la columna glyph en la columna col de la matriz
    /// OJO: col está en binario. Esto es, si se quiere mostrar la columna
    /// 3, hay que pasar col = 0b00000100; <--- observar también que la
    /// columna 3 corresponde a las "unidades de mil" (en base 2, ¿unidades de
    /// 8? @_@)
    void show_column(uint8_t glyph, uint8_t col);

    // Filas y columnas de esta matriz
    constexpr static uint8_t rows() {return 8;}
    constexpr static uint8_t cols() {return 8;}

    // Funciones para probar la matriz de leds
    // ---------------------------------------
    // Esta función solo es para probar la intensidad obtenida al encenderlos
    // todos los leds. En la práctica no creo que la use.
    void todos_on();

    // Para probar que todos los leds funcionen
    void barre_todos_los_leds(uint8_t time);

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

// Muestra la columna glyph en la columna col de la matriz
void MatrixLEDs_8x8::show_column(uint8_t glyph, uint8_t col)
{
    pos = 0;
    neg =~col;

    // Opción 3: enciendo cada led por separado.
    for (uint8_t num_led = 1; num_led != 0; num_led <<= 1){
	pos = (glyph & num_led);
	wait_us(20);	// mantenemos iluminado el led un tiempo
    }
}

void MatrixLEDs_8x8::show(const Glyph& glyph)
{
    uint8_t col = 1;
    for (uint8_t i = 0; i < Glyph::cols() ; ++i, col <<= 1)
	show_column(glyph[i], col);
}


// OBSOLETO: esta función hace el show por filas. Al final como quiero hacer
// un scrolll de izda a dcha es más eficiente mostrarlos por columnas.
// Con todo en el ascensor el scroll es de arriba a abajo, en cuyo caso sería
// mejor esta función.
// Incluyo 3 opciones, la tercera la ganadora, ya que el planteamiento natural
// da problemas con el primer led: apenas lo enciende. ¿Por qué? (???)
//void MatrixLEDs_8x8::show(const Glyph& glyph)
//{
//    uint8_t p = 1;
//    for (uint8_t i = 0; i < num_pins_; ++i, p <<= 1){
//	neg = 0xFF;
//	pos = p;
//	// Opción 3: enciendo cada led por separado.
//	for (uint8_t num_led = 1; num_led != 0; num_led <<= 1){
//	    neg = ~(glyph[i] & num_led);
//	  //  wait_us(10);  // No es necesario el wait. El bucle for ya 
//			    // introduce el tiempo de espera.
//	}
//	// Opción 1: Con esto debería de funcionar.
//	// Sin embargo tengo el problema de que apenas se enciende el primer
//	// led. ¿Por qué?
//	// neg = ~(glyph[i]);
//	// wait_us(10);	// tiempo que mantenemos los leds encendidos
//	// Opción 2: Encendemos el primer led por separado y luego el resto.
//	// Como el primer led está más tiempo encendido que los demás, brilla
//	// un poco más que el resto.
//	// neg = ~(glyph[i] & 1);
//	// wait_us(10); 
//	// neg = ~(glyph[i]);
//	// wait_us(10);
//    }
//}

void MatrixLEDs_8x8::show(const Glyph& glyph, uint16_t n)
{
    while (n > 0){
	show(glyph);
	--n;
    }
}



/// Para hacer que una cadena de texto haga un scroll concebimos la cadena
/// como formada por las columnas de los glyphs de cada caracter.
/// Ejemplo:
///	La cadena "hola" está formada por los glyphs h, o, l, a, cada uno
/// de los cuales está tiene 5 columnas. 
/// Al iterar por las columnas de este mensaje iteramos realmente por las
/// columnas de cada glyph que lo forma. De esta forma es muy sencillo  hacer 
/// el scroll.
class MatrixLEDs_8x8_mensaje{
public:
    // Longitud máxima del mensaje.
    static constexpr uint8_t max_size = 80;

    /// Copiamos internamente msg. Máximo max_size.
    MatrixLEDs_8x8_mensaje(const char* msg)
    {
	msg_[max_size - 1] = '\0';
	strncpy(msg_, msg, max_size); 
	size_ = strlen(msg_);
    }

    uint16_t cols() const {return cols_por_glyph * size_;}

    /// Devuelve la columna k del mensaje
    uint8_t operator[](uint16_t k) const;


private:
    char msg_[max_size];  
    uint8_t size_;  // guardo el size_ por eficiencia en tiempo y ahorrar
		    // llamar a strlen continuamente.
		    // OJO: uint8_t ===> max_size < 256

    uint8_t cols_por_glyph = Glyph::cols(); // + 2 spaces (???)
};




// k = índice unidimensional del contenedor.
// (i,j) = índices bidimensionales.
//	   i = número de caracter al que apunta k
//	   j = columna dentro del caracter al que apunta k
// Recordar que: k = cols_por_glyph*i +  j
uint8_t MatrixLEDs_8x8_mensaje::operator[](uint16_t k) const
{
    uint16_t i = k / cols_por_glyph;
//    uint8_t j = k % cols_por_glyph; // DUDA: realmente es más eficiente restar?
    uint16_t j = k - cols_por_glyph*i;

    if (i >= size_)
	return 0;

    Glyph glyph = dev::Glyphs::glyph(msg_[i]);
    return glyph[j];
}



int main()
{
    MatrixLEDs_8x8 mat;
//    Glyphs glyph;
//    char msg[] = "Hola, que tal estas? ";
    // MatrixLEDs_8x8_mensaje msg{"Hola, que tal estas?"};
    MatrixLEDs_8x8_mensaje msg{
        "Estamos emocionados de teneros aqui!!! Esperemos que lo "
        "paseis bien. HURRA!!!"};

    while(1){
//	mat.barre_todos_los_leds(100);
//	mat.todos_on();
    
	// barremos todos los glyphs a ver si están bien
//	for (uint8_t i = ' '; i < '~'; ++i)
//	    mat.show(glyph[i], 800);

	// Mostramos un mensaje letra por letra
//	for (char* p = &msg[0]; *p != '\0'; ++p)
//	    mat.show(glyph[*p], 800);

	// Hacemos un scroll del mensaje
	// Esta opción hace que las letras se vean muy apagadas
	for (uint16_t j = 0; j < msg.cols(); ++j){
	    for (uint16_t rep = 0; rep < 30; ++rep){
		uint8_t col = 1;    // OJO: esto solo admite 8 columnas!!!
		for (uint8_t i = 0; i < MatrixLEDs_8x8::cols(); ++i, col <<= 1)
		    mat.show_column(msg[j + i], col);
	    }
	}

    }
}
