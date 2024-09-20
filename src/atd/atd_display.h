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

#pragma once

#ifndef __ATD_DISPLAY_H__
#define __ATD_DISPLAY_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Displays genéricos.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    20/09/2024 Monochromatic_text_display (primeras pruebas)
 *
 ****************************************************************************/
#include "atd_coordinates.h"
#include <algorithm>

namespace atd{
/***************************************************************************
 *				TEXT_DISPLAY
 ***************************************************************************/
// struct Cfg {
//	using Display    = MAX7219_matrix...
//	using index_type = uint16_t;
//	static constexpr index_type text_rows = 2;
//	static constexpr index_type text_cols = 16;
//
// };
//
// primeras pruebas
// ----------------
// Tenemos:
//	1. Un buffer con el texto que queremos mostrar.
//	2. Un bitmatrix donde vamos a escribir los glyphs que queremos
//	   mostrar.
//	3. Un display, hardware real, donde mostraremos el bitmatrix anterior.
//	   El tamaño del bitmatrix será igual al del display. 
//	   (el diplay hace las mismas funciones que std::streambuffer en
//	   std::iostream)
//
//  Como quiero dar la posibilidad de hacer scroll en el display no vamos a
//  poder mostrar todo el buffer de texto. Por ello, introduzco un
//  "background display". En este display ficticio supondremos que tenemos
//  dibujados todos los glyphs del buffer de texto. El display real lo
//  moveremos sobre este background.
//  
template <typename Cfg>
class Monochromatic_text_display{
public:
// Types
    using size_type = Cfg::index_type; 
    using index_type= Cfg::index_type;
    using Coord_ij  = atd::Coord_ij<index_type>;
    using iterator  = char*;
    using const_iterator = const char*;

    using Font      = Cfg::Font;
    using Bitmatrix = Cfg::Bitmatrix;

    using Display   = Cfg::Display;

// Constructor
    Monochromatic_text_display(); 

// Text interface
    static constexpr size_type text_rows() { return Cfg::text_rows;}
    static constexpr size_type text_cols() { return Cfg::text_cols;}

    // Número de caracteres que entran 
    static constexpr size_type text_size() {return text_rows() * text_cols();}

    void write(const Coord_ij& p, char c);
    void write(const Coord_ij& p, std::string_view str);

    char read(const Coord_ij& p) const;

    iterator begin() { return &text_[0];}
    iterator end()   { return &text_[text_size()];}

    const_iterator begin() const { return &text_[0];}
    const_iterator end()   const { return &text_[text_size()];}


    // Rellena el buffer de texto con el caracter 'c'
    void fill(char c);

    // Borra todo el buffer de texto
    void clear();

// Background display
    static constexpr size_type bg_display_rows() 
	{ return text_rows() * Font::rows;}

    static constexpr size_type bg_display_cols() 
	{ return text_rows() * Font::cols;}
    
// Display
    static constexpr size_type display_rows() { return Bitmatrix::rows();}
    static constexpr size_type display_cols() { return Bitmatrix::cols();}
    
    static_assert(bg_display_rows() >= display_rows());
    static_assert(bg_display_cols() >= display_cols());

    // Volcamos el contenido del buffer en el display
    void flush();

private:
// Data
    // (RRR) Opto por definirlo unidimensional para que sea sencillo tanto
    // leer como escribir cadenas de texto (concatenadas entre lineas)
    // ¿mejor bidimensional? Que el uso lo diga.
    char text_[text_size()]; // buffer de texto
    Bitmatrix bm_;	     // Imagen con los glyphs a mostrar en el display
    
    Coord_ij bm_pos_;  // posicion del bm_ en el background

// Helpers
    index_type coord2index(const Coord_ij& p) const;
};

template <typename C>
inline Monochromatic_text_display<C>::Monochromatic_text_display()
	: bm_pos_{0, 0} 
{ 
    clear();
}

template <typename C>
inline auto Monochromatic_text_display<C>::coord2index(const Coord_ij& p) const -> index_type
{ return p.i * text_cols() + p.j; }

template <typename C>
void Monochromatic_text_display<C>::write(const Coord_ij& p, char c)
{
    index_type k = coord2index(p);
    if (k < text_size())
	text_[k] = c;

    else { // TODO: ¿qué hacer? Se podría configurar el comportamiento via
	   // Cfg:
	   //	1. Nada. No se escribe fuera.
	   //	2. Buffer circular: escribimos en (0,0)
	   //	3. Tiramos una línea hacia arriba y escribimos (como hace
	   //	   terminal)
	text_[0] = c; // de momento escribo en (0,0) para detectar errores
    }
}

template <typename C>
inline void Monochromatic_text_display<C>::write(const Coord_ij& p, std::string_view str)
{
    index_type k0 = coord2index(p);

    index_type ke = k0 + str.length();
    index_type ki = ke;
    if (ke > text_size()){
	ki = text_size();
	ke -= ki;
    }

    auto c = str.begin();
    for (; c != str.end() and k0 < ki; ++c, ++k0)
	text_[k0] = *c;

    if (ke != ki){
	// TODO: ¿qué hacer? Leer TODO de write(c)
	
	k0 = 0;
	for (; c != str.end() and k0 < ke; ++c, ++k0)
	    text_[k0] = *c;
    }
}

template <typename C>
inline char Monochromatic_text_display<C>::read(const Coord_ij& p) const
{
    index_type k = coord2index(p);
    if (k < text_size())
	return text_[k];

    else
	return '?';
}

template <typename C>
inline void Monochromatic_text_display<C>::fill(char c)
{ std::fill_n(&text_[0], text_size(), c); }

template <typename C>
inline void Monochromatic_text_display<C>::clear()
{ fill('\0'); }

}// atd

#endif


