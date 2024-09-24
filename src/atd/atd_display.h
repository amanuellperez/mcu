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
 *				TEXT_BLOCK	
 ***************************************************************************/
// struct Cfg {
//	using index_type = uint16_t;
//	static constexpr index_type text_rows = 2;
//	static constexpr index_type text_cols = 16;
//
// };
template <typename Cfg>
class Text_block{
public:
// Types
    using size_type = Cfg::index_type; 
    using index_type= Cfg::index_type;
    using Coord_ij  = atd::Coord_ij<index_type>;
    using iterator  = char*;
    using const_iterator = const char*;


// Constructor
    Text_block(); 

// Text interface
    static constexpr size_type rows() { return Cfg::text_rows;}
    static constexpr size_type cols() { return Cfg::text_cols;}

    // Número de caracteres que entran 
    static constexpr size_type size() {return rows() * cols();}

    void write(const Coord_ij& p, char c);
    void write(const Coord_ij& p, std::string_view str);

    char read(const Coord_ij& p) const;
    char read(index_type i, index_type j) const {return read({i,j});}

    iterator begin() { return &text_[0];}
    iterator end()   { return &text_[size()];}

    const_iterator begin() const { return &text_[0];}
    const_iterator end()   const { return &text_[size()];}


    // Rellena el buffer de texto con el caracter 'c'
    void fill(char c);

    // Borra todo el buffer de texto
    void clear();


private:
// Data
    // (RRR) Opto por definirlo unidimensional para que sea sencillo tanto
    // leer como escribir cadenas de texto (concatenadas entre lineas)
    // ¿mejor bidimensional? Que el uso lo diga.
    char text_[size()]; // buffer de texto

// Helpers
    index_type coord2index(const Coord_ij& p) const;
};

template <typename C>
inline Text_block<C>::Text_block()
{ clear(); }

template <typename C>
inline auto Text_block<C>::coord2index(const Coord_ij& p) const -> index_type
{ return p.i * cols() + p.j; }

template <typename C>
void Text_block<C>::write(const Coord_ij& p, char c)
{
    index_type k = coord2index(p);
    if (k < size())
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
inline void Text_block<C>::write(const Coord_ij& p, std::string_view str)
{
    index_type k0 = coord2index(p);

    index_type ke = k0 + str.length();
    index_type ki = ke;
    if (ke > size()){
	ki = size();
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
inline char Text_block<C>::read(const Coord_ij& p) const
{
    index_type k = coord2index(p);
    if (k < size())
	return text_[k];

    else
	return '?';
}

template <typename C>
inline void Text_block<C>::fill(char c)
{ std::fill_n(&text_[0], size(), c); }

template <typename C>
inline void Text_block<C>::clear()
{ fill('\0'); }

/***************************************************************************
 *			    SUBTEXT_BLOCK
 ***************************************************************************/
namespace impl_of{

// (RRR) ¿por qué heredo?
//	 Para poder hacer visible la variable cylinder_type en
//	 Subtext_block_rectangle_cfg. Heredando es muy sencillo.
template <typename Cfg>
struct Subtext_block_rectangle_cfg : Cfg{
    using index_type = Cfg::index_type;

// Dimensiones del background
    static constexpr index_type bg_width  = Cfg::text_cols;
    static constexpr index_type bg_height = Cfg::text_rows;
    
// Dimensiones del rectángulo
    static constexpr index_type width = Cfg::subtext_cols;
    static constexpr index_type height= Cfg::subtext_rows;
};

}// namespace impl_of
 
// En lugar de llamar rows/cols opto por llamarlos subtext_rows/subtext_cols
// para que en una misma struct Cfg se pueda pasar la configuración del
// text_block y del subtext_block. 
//
// struct Cfg{
//	static constexpr uint16_t subtext_rows = 2;
//	static constexpr uint16_t subtext_cols = 8;
// };
template <typename Cfg>
class Subtext_block{
public:
// Types
    using Text_block= atd::Text_block<Cfg>;
    using size_type = typename Text_block::size_type;
    using index_type= typename Text_block::index_type;
    using Coord_ij  = typename Text_block::Coord_ij;
    using iterator  = typename Text_block::iterator;
    using const_iterator = typename Text_block::const_iterator;

// Constructor
    Subtext_block(Text_block& txt_block) : txt_block_{txt_block} { }
    
// Bidimensional container
    static constexpr size_type rows() { return Cfg::subtext_rows;}
    static constexpr size_type cols() { return Cfg::subtext_cols;}

    // Las coordenadas son relativas al subtext
    char read(const Coord_ij& p) const;
    char read(index_type i, index_type j) const {return read({i,j});}


// Unidimensional container
    static constexpr size_type size() { return rows() * cols(); }


// Movimiento
    nm::Result move_to(const Coord_ij& q0) {return rect_.move_to(q0);}

    nm::Result move_up  (index_type incr) {return rect_.move_up(incr);}
    nm::Result move_down(index_type incr) {return rect_.move_down(incr);}
    nm::Result move_left(index_type incr) {return rect_.move_left(incr);}
    nm::Result move_right(index_type incr){return rect_.move_right(incr);}

// Para depurar, no creo que se necesite en la práctica
    constexpr Coord_ij p0() const {return rect_.p0(); }
    constexpr Coord_ij p1() const {return rect_.p1(); }
    constexpr Coord_ij pm() const {return rect_.pm(); }
    constexpr Coord_ij pe() const {return rect_.pe();}

private:
// Data
    Text_block& txt_block_;

    using Rect_cfg = impl_of::Subtext_block_rectangle_cfg<Cfg>;
    Bounded_rectangle_ij<Rect_cfg> rect_;
};

template <typename Cfg>
inline 
char Subtext_block<Cfg>::read(const Coord_ij& p) const
{ return txt_block_.read(rect_.local_to_background(p)); }


/***************************************************************************
 *			TEXT_BLOCK_WITH_VIEW	
 ***************************************************************************/
//struct Text_block_cfg{
//// Bacground
//    using index_type = int;
//    //using index_type = unsigned int;
//    static constexpr index_type text_rows = 4;
//    static constexpr index_type text_cols = 10;
//
//// View
//    static constexpr bool cylinder_type{}; <-- ELEGIR EL TIPO!!!
//    static constexpr index_type subtext_rows = 2;
//    static constexpr index_type subtext_cols = 5;
//};
//
// Esta clase fusiona las dos clases anteriores
template <typename Cfg>
class Text_block_with_view {
public:
// Types
    using Bg   = Text_block<Cfg>;
    using View = Subtext_block<Cfg>;

    using size_type = typename Bg::size_type;
    using index_type= typename Bg::index_type;
    using Coord_ij  = typename Bg::Coord_ij;
    using iterator  = typename Bg::iterator;
    using const_iterator = typename Bg::const_iterator;

// Background interface
    static constexpr size_type bg_rows() { return Bg::rows();}
    static constexpr size_type bg_cols() { return Bg::cols();}

    // Número de caracteres que entran 
    static constexpr size_type bg_size() {return Bg::size();}

    // Coordenadas absolutas al background
    void bg_write(const Coord_ij& p, char c) { bg_.write(p, c); }
    void bg_write(const Coord_ij& p, std::string_view str)
    { bg_.write(p, str); }

    // Coordenadas absolutas al background
    char bg_read(const Coord_ij& p) const { return bg_.read(p);}
    char bg_read(index_type i, index_type j) const {return bg_.read({i,j});}

    iterator bg_begin() { return bg_.begin();}
    iterator bg_end()   { return bg_.end();}

    const_iterator bg_begin() const { return bg_.begin();}
    const_iterator bg_end()   const { return bg_.end();}


    // Rellena el buffer de texto con el caracter 'c'
    void bg_fill(char c) {return bg_.fill(c);}

    // Borra todo el buffer de texto
    void bg_clear() {return bg_.clear();}

// View interface
    static constexpr size_type view_rows() { return View::rows();}
    static constexpr size_type view_cols() { return View::cols();}

    // Las coordenadas son relativas a la view
    char view_read(const Coord_ij& p) const {return view_.read(p); }
    char view_read(index_type i, index_type j) const {return view_.read(i, j);}


// Unidimensional container
    static constexpr size_type view_size() { return View::size(); }


// Movimiento
// DUDA: las operaciones de movimiento solo son aplicables a la view, nunca al
// background. Sobraría view_ ¿quitarlo? (aunque se leera mejor el codigo si
// se pone txt.view_move_up(2), que no txt.move_up(2))
    nm::Result view_move_to(const Coord_ij& q0) {return view_.move_to(q0);}

    nm::Result view_move_up  (index_type incr) {return view_.move_up(incr);}
    nm::Result view_move_down(index_type incr) {return view_.move_down(incr);}
    nm::Result view_move_left(index_type incr) {return view_.move_left(incr);}
    nm::Result view_move_right(index_type incr){return view_.move_right(incr);}

// Para depurar, no creo que se necesite en la práctica
// (???) Necesitare p0()?
//    constexpr Coord_ij p0() const {return view_.p0(); }
//    constexpr Coord_ij p1() const {return view_.p1(); }
//    constexpr Coord_ij pm() const {return view_.pm(); }
//    constexpr Coord_ij pe() const {return view_.pe();}

private:
// Data
    Bg bg_;
    View view_{bg_};
};


/***************************************************************************
 *			MONOCROMATIC_TEXT_DISPLAY
 ***************************************************************************/
// struct Cfg {
//	using Display    = MAX7219_matrix...
//	using index_type = uint16_t;
//	static constexpr index_type bg_rows = 2;
//	static constexpr index_type bg_cols = 16;
//	static constexpr index_type view_rows = 2;
//	static constexpr index_type view_cols = 16;
//	using Font = ... // fuente que vamos a usar para escribir en el	display
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
class Monochromatic_text_display : public Text_block<Cfg>{
public:
// Types
    using TextBlock     = Text_block<Cfg>;

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

// Background display
    static constexpr size_type bg_display_rows() 
	{ return TextBlock::text_rows() * Font::rows;}

    static constexpr size_type bg_display_cols() 
	{ return TextBlock::text_rows() * Font::cols;}
    
// Display
    static constexpr size_type display_rows() { return Bitmatrix::rows();}
    static constexpr size_type display_cols() { return Bitmatrix::cols();}
    
    static_assert(bg_display_rows() >= display_rows());
    static_assert(bg_display_cols() >= display_cols());

    // Volcamos el contenido del buffer en el display
    void flush();

private:
// Data
//    Subtext_block subtxt_;  // Bloque de texto que mostramos en el display

    Bitmatrix bm_;	    // Es subtxt_ pero dibujado con la fuente Font
//  Display out_;	// Display donde mostramos bm_
};


}// atd
#endif


