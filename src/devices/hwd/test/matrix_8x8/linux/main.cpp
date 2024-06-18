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

// Probamos en linux el algoritmo para hacer el scroll horizontal
#include <iostream>
#include <cstring>

#include "glyphs.h"

void show_column(uint8_t g)
{
    for (uint8_t p = 1; p != 0; p <<= 1){
	if (g & p)
	    std::cout << 'x';
	else
	    std::cout << '-';

	std::cout << ' ';
    }
    std::cout << '\n';
}


void show(const Glyph& g)
{
    for (uint8_t i = 0; i < Glyph::cols(); ++i)
	show_column(g[i]);
}

void show_todos_glyphs()
{
    Glyphs glyph;
 
    for (uint8_t i = ' '; i != '~'; ++i){
	show(glyph[i]);
	std::cout << '\n';
    }

    show(glyph['~']);
    std::cout << '\n';

    // desconocido
    show(glyph[0]);
    std::cout << '\n';
}


/// El mensaje lo descomponemos en las columnas que tiene cada glyph.
class Mensaje{
public:
    static constexpr uint8_t max_size = 80;

    /// Copiamos internamente msg. Máximo max_size.
    Mensaje(const char* msg)
    {
	msg_[max_size - 1] = '\0';
	std::strncpy(msg_, msg, max_size); 
    }

    uint16_t cols() const {return cols_por_glyph * std::strlen(msg_);}

    /// Devuelve la columna k del mensaje
    uint8_t operator[](uint8_t k) const;

//private:
    char msg_[max_size];  

    uint8_t cols_por_glyph = Glyph::cols(); // + 2 spaces (???)
};

uint8_t Mensaje::operator[](uint8_t k) const
{
    uint8_t i = k / cols_por_glyph;
    uint8_t j = k % cols_por_glyph;

    if (j > cols())
	return 0;

    Glyph glyph = Glyphs::glyph(msg_[i]);
    return glyph[j];
}


void scroll()
{
    Mensaje msg("Hola, que tal?");

    uint8_t matrix_cols = 8;

    for (uint16_t j = 0; j < msg.cols(); ++j){
	std::cout << j << ":\n";
	
	for (uint8_t i = 0; i < matrix_cols; ++i)
	    show_column(msg[j + i]);

	std::cout << "\n\n";
    }
}

int main(int argc, char* argv[])
{
try{
    Mensaje msg{"Estamos emocionados de teneros aqui!!! Esperemos que lo "
                "paseis bien. HURRA!!!"};
    std::cout << msg.cols() << '\n';
    // show_todos_glyphs();
    // scroll();

}catch(std::exception& e){
    std::cerr << argv[0] << ": " << e.what() << '\n';
    return 1;
}


}

