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

#ifndef __ATD_COORDINATES_H__
#define __ATD_COORDINATES_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Hay un montón (demasiadas!) formas de referirse a una posición:
 *
 *  + Coordenadas geométricas o xy: (x,y)
 *    ¿Cuál es el origen del sistema de referencia? Hay que especificarlo de
 *    alguna otra forma.
 *
 *  + Coordenadas matriciales o ij: (i,j)
 *    En una matriz se elige el punto superior-izdo como origen. Los ejes son: 
 *    j = x; i = -y
 *    Posibles valores para una matriz m x n (rows x cols):
 *		    i = [0...m-1], j = [0...n-1]
 *    Otra posible forma de definirlas, en lugar de usar (i,j), sería 
 *    (row, col) pero al ser más largo de escribir mejor (i,j) (además, que es
 *    la notación habitual en matemátias)
 *
 *  + Cooredandas por páginas (= bloques horizontales) o pagecol: (page, col)
 *    Algunos displays (SDD1306) lo que hacen es agrupar las filas por
 *    bloques, llamados pages (imaginar que coges el display y lo cortas en
 *    tiras horizontales de 8 filas cada una, cada una es una página donde
 *    poder dibujar). 
 *
 *
 *  Aunque en la práctica todo es lo mismo (un nombre de la posición de un
 *  punto) como sería muy confuso cuando programas matrices hablar de (x,y) o
 *  al hacer geometría hablar de (i, j), hay que definir una clase diferente.
 *
 *  TODO: Las coordenadas (x,y) están definidas en geo_2d. ¿Traerlas aquí?
 *
 * HISTORIA
 *    Manuel Perez
 *    21/08/2024 PageCol
 *    07/09/2024 Coord_ij
 *    21/09/2024 fix_Bounded_rectangle_ij
 *
 ****************************************************************************/
#include <ostream>

#include "atd_concepts.h"
#include "atd_names.h"

namespace atd{
/***************************************************************************
 *				PAGECOL
 ***************************************************************************/
// Descomponemos el plano en páginas horizontales
template <Type::Numeric Int>
struct PageCol{
    Int page;	
    Int col;	

    constexpr
    PageCol(const Int& page0, const Int& col0) : page{page0}, col{col0} { }

    constexpr PageCol& operator+=(const PageCol& x);
    constexpr PageCol& operator-=(const PageCol& x);
};


// operadores aritmeticos
// ----------------------
template <Type::Numeric Int>
constexpr PageCol<Int>& PageCol<Int>::operator+=(const PageCol& x)
{
    page += x.page;
    col  += x.col;

    return *this;
}

template <Type::Numeric Int>
constexpr PageCol<Int>& PageCol<Int>::operator-=(const PageCol& x)
{
    page -= x.page;
    col  -= x.col;

    return *this;
}

template <Type::Numeric Int>
inline constexpr PageCol<Int> 
    operator+(PageCol<Int> x, const PageCol<Int>& y)
{
    x += y;
    return x;
}

template <Type::Numeric Int>
inline constexpr PageCol<Int> 
    operator-(PageCol<Int> x, const PageCol<Int>& y)
{
    x -= y;
    return x;
}

// flujo de salida
// ---------------
template <Type::Numeric Int>
std::ostream& operator<<(std::ostream& out, const PageCol<Int>& p)
{ return out << '(' << p.page << ", " << p.col << ')'; }

// PageCol_rectangle
// -----------------
template <Type::Numeric Int>
struct PageCol_rectangle{
    using Point = PageCol<Int>;
    Point p0;	// extremo inferior-izdo
    Point p1;	// extremo superior-dcho

    // DUDA: ¿verificar que p1 > p0?
    constexpr
    PageCol_rectangle(const Point& p00, const Point& p10) 
		: p0{p00}, p1{p10} { }

    constexpr uint16_t size() const 
    { return (p1.col - p0.col + 1) * (p1.page - p0.page + 1); }
};


/***************************************************************************
 *				COORD_IJ
 ***************************************************************************/
// Coordenadas de matriz (i,j)
template <Type::Numeric Int>
struct Coord_ij{
    Int i;	
    Int j;	

    constexpr
    Coord_ij(const Int& i0, const Int& col0) : i{i0}, j{col0} { }

    constexpr Coord_ij& operator+=(const Coord_ij& x);
    constexpr Coord_ij& operator-=(const Coord_ij& x);
};


// operadores aritmeticos
// ----------------------
template <Type::Numeric Int>
constexpr Coord_ij<Int>& Coord_ij<Int>::operator+=(const Coord_ij& x)
{
    i += x.i;
    j  += x.j;

    return *this;
}

template <Type::Numeric Int>
constexpr Coord_ij<Int>& Coord_ij<Int>::operator-=(const Coord_ij& x)
{
    i -= x.i;
    j  -= x.j;

    return *this;
}

template <Type::Numeric Int>
inline constexpr Coord_ij<Int> 
    operator+(Coord_ij<Int> x, const Coord_ij<Int>& y)
{
    x += y;
    return x;
}

template <Type::Numeric Int>
inline constexpr Coord_ij<Int> 
    operator-(Coord_ij<Int> x, const Coord_ij<Int>& y)
{
    x -= y;
    return x;
}

template <Type::Numeric Int>
inline constexpr 
bool operator==(const Coord_ij<Int>& a, const Coord_ij<Int>& b)
{ return a.i == b.i and a.j == b.j; }

template <Type::Numeric Int>
inline constexpr 
bool operator!=(const Coord_ij<Int>& a, const Coord_ij<Int>& b)
{ return !(a == b); }


// flujo de salida
// ---------------
template <Type::Numeric Int>
std::ostream& operator<<(std::ostream& out, const Coord_ij<Int>& p)
{ return out << '(' << p.i << ", " << p.j << ')'; }


/***************************************************************************
 *			    BOUNDED_RECTANGLE_IJ
 ***************************************************************************/
// Notación:
//      
// p0-> +--------------------+
//	|		     |
//	|		     |
//	|		     |
//	|		     |
//	+--------------------+ <- p1
//			     x <- pe
//	Uso la misma notación que con los intervalos:
//	[p0, p1] = [p0, pe)
//	p0 = primer punto
//	p1 = ultimo punto
//	pe = p1 + 1
//	Lo unico que en rectángulos son esquinas. 
//	Observar que los rectángulos tienen una orientación: el punto p0 es 
//	la esquina superior izquierda, mientras que p1 la esquina inferior
//	derecha. Este convenio conviene mantenerlo independientemente de que
//	el rectangulo sea de coordenadas ij o xy.
//	
//  Se trata de un rectángulo acotado por otro (que llamo background): 
//  podemos mover este rectángulo por el background, siempre y cuando todo el
//  rectángulo esté incluido en el background.
//
namespace impl_of{

/***************************************************************************
 *			FIX_BOUNDED_RECTANGLE_IJ_PLANE_TYPE
 ***************************************************************************/
// Plane type: el background es plano 
template <typename Cfg>
class fix_Bounded_rectangle_ij_plane_type{
public:
// Types
    using index_type = Cfg::index_type;
    using Coord_ij = atd::Coord_ij<index_type>;
    
// Cfg
    // Dimensiones del background
    static constexpr index_type bg_width  = Cfg::bg_width;
    static constexpr index_type bg_height = Cfg::bg_height;
    
    // Dimensiones del rectángulo
    static constexpr index_type width()  {return  Cfg::width;}
    static constexpr index_type height() { return Cfg::height; }

    static_assert(bg_width  >= width());
    static_assert(bg_height >= height());
    

// Constructor
    constexpr fix_Bounded_rectangle_ij_plane_type(const Coord_ij& p0 = {0,0});

// Info
    constexpr Coord_ij p0() const {return p0_; }
    constexpr Coord_ij p1() const {return p1(p0_);}
    constexpr Coord_ij pe() const {return pe(p0_);}

// Move
    // Movimiento absoluto: movemos el rectángulo de tal manera que su esquina
    // superior izquierda sea q0. 
    // DUDA: ¿qué hacer en caso de no poder mover el rectángulo a esa
    // posición?
    //	1) No hace nada.
    //	2) Intenta colocar el rectángulo lo más próximo a q0
    //	Al no tener criterio, de momento "no hagamos nada"
    nm::Result move_to(const Coord_ij& q0);

    // Movimiento relativo: añadimos q0 a la esquina
    // Equivalente a move_to(p0() + q0)
    // TODO: nombre? move_rel es muy feo @_@
    nm::Result move_rel(const Coord_ij& q0) {return move_to(p0() + q0); }

    nm::Result scroll_up  (index_type incr) {return move_rel({-incr, 0});}
    nm::Result scroll_down(index_type incr) {return move_rel({+incr, 0});}
    nm::Result scroll_left(index_type incr) {return move_rel({0, -incr});}
    nm::Result scroll_right(index_type incr){return move_rel({0, +incr});}

private:
// Data
    Coord_ij p0_; // esquina superior izda del rectángulo

// Helpers
    constexpr Coord_ij pe(const Coord_ij& p0) const
    {return p0 + Coord_ij{height(), width()}; }

    constexpr Coord_ij p1(const Coord_ij& p0) const
    {return pe(p0) - Coord_ij{1,1};}

    constexpr bool is_inside_bg(const Coord_ij& p) const;

    constexpr Coord_ij reduce_to_background(const Coord_ij& p) const
    { return Coord_ij{p.i % bg_height, p.j % bg_width};}
};


template <typename C>
inline 
constexpr fix_Bounded_rectangle_ij_plane_type<C>::
    fix_Bounded_rectangle_ij_plane_type(const Coord_ij& p0) 
    : p0_{reduce_to_background(p0)}
{ }

template <typename C>
nm::Result fix_Bounded_rectangle_ij_plane_type<C>::move_to(const Coord_ij& q0)
{
    if (!(is_inside_bg(q0) and is_inside_bg(p1(q0))))
	return nm::fail;

    p0_ = q0;

    return nm::ok;
}

template <typename C>
inline 
constexpr bool fix_Bounded_rectangle_ij_plane_type<C>::
				is_inside_bg(const Coord_ij& p) const
{
    return (0 <= p.i and p.i < bg_height)
	    and
	   (0 <= p.j and p.j < bg_width);
}





/***************************************************************************
 *		    FIX_BOUNDED_RECTANGLE_IJ_CYLINDER_TYPE
 ***************************************************************************/
// Cylinder type: el background es un "cilindro"
// Realmente NO es un cilindro, mas bien es un plano infinito donde hemos
// dibujado el background uno al lado del otro:
//  
//          |               |               |               |
//          |               |               |               |
// ---------+---------------+---------------+---------------+--------------
//          |               |       2       |               |
//          |               |               |               |
// ---------+---------------+---------------+---------------+--------------
//          |               |   background  |               |
//          |               |       0       |       1       |
// ---------+---------------+---------------+---------------+--------------
//          |               |               |               |
//          |               |               |               |
//	    
//  Cada uno de los rectángulos anteriores es una copia del background.
//  Nuestro rectángulo (= ventana movible sobre el background) lo podemos
//  mover por ese plano infinito. De esa forma, si lo movemos hacia la
//  derecha, al salirse del background 0 y entrar en el background 1 la
//  ventana mostrará el lado derecho del background 0 y el lado izdo del
//  background 1 (es el equivalente ha hacer un scroll sobre un cilindro).
//  Y algo parecido sucederá si subimos del background 0 al 2.
//  
//
// Esta clase surge para poder hacer banners que se muevan en pantallas tipo
// MAX7219: hacemos que el banner se vaya moviendo y cuando llega al final
// vuelve a empezar (tiene forma cilindrica horizontalmente).
// Pero podríamos hacer que el banner se mueva de arriba-abajo, en cuyo caso
// cuando llegue al final empezará por arriba de nuevo (tiene forma cilindrica
// pero esta vez verticalmente)
//
template <typename Cfg>
class fix_Bounded_rectangle_ij_cylinder_type{
public:
// Types
    using index_type = Cfg::index_type;
    using Coord_ij = atd::Coord_ij<index_type>;
    
// Cfg
    // Dimensiones del background
    static constexpr index_type bg_width  = Cfg::bg_width;
    static constexpr index_type bg_height = Cfg::bg_height;
    
    // Dimensiones del rectángulo
    static constexpr index_type width()  {return  Cfg::width;}
    static constexpr index_type height() { return Cfg::height; }

    static_assert(bg_width  >= width());
    static_assert(bg_height >= height());
    

// Constructor
    constexpr fix_Bounded_rectangle_ij_cylinder_type(const Coord_ij& p0 = {0,0});

// Info
    constexpr Coord_ij p0() const {return p0_; }
    constexpr Coord_ij p1() const {return p1(p0_);}
    constexpr Coord_ij pe() const {return pe(p0_);}

// Move
    // Movimiento absoluto: movemos el rectángulo de tal manera que su esquina
    // superior izquierda sea q0. 
    // DUDA: ¿qué hacer en caso de no poder mover el rectángulo a esa
    // posición?
    //	1) No hace nada.
    //	2) Intenta colocar el rectángulo lo más próximo a q0
    //	Al no tener criterio, de momento "no hagamos nada"
    nm::Result move_to(const Coord_ij& q0);

    // Movimiento relativo: añadimos q0 a la esquina
    // Equivalente a move_to(p0() + q0)
    // TODO: nombre? move_rel es muy feo @_@
    nm::Result move_rel(const Coord_ij& q0) {return move_to(p0() + q0); }

    nm::Result scroll_up  (index_type incr) {return move_rel({-incr, 0});}
    nm::Result scroll_down(index_type incr) {return move_rel({+incr, 0});}
    nm::Result scroll_left(index_type incr) {return move_rel({0, -incr});}
    nm::Result scroll_right(index_type incr){return move_rel({0, +incr});}

private:
// Data
    Coord_ij p0_; // esquina superior izda del rectángulo

// Helpers
    constexpr Coord_ij pe(const Coord_ij& p0) const
    {
	auto res = p0 + Coord_ij{height(), width()};
	if (res.i > bg_height)
	    res.i = res.i % bg_height;

	if (res.j > bg_width)
	    res.j = res.j % bg_width;

	return res;
    }

    constexpr Coord_ij p1(const Coord_ij& p0) const
    {return reduce_to_background(p0 + Coord_ij{height() - 1, width() - 1});}

    constexpr Coord_ij reduce_to_background(const Coord_ij& p) const
    { return Coord_ij{p.i % bg_height, p.j % bg_width};}
};



template <typename C>
inline 
constexpr fix_Bounded_rectangle_ij_cylinder_type<C>::
    fix_Bounded_rectangle_ij_cylinder_type(const Coord_ij& p0) 
    : p0_{reduce_to_background(p0)}
{ }

template <typename C>
nm::Result fix_Bounded_rectangle_ij_cylinder_type<C>::move_to(const Coord_ij& q0)
{
    p0_ = reduce_to_background(q0);

    return nm::ok;
}



// static_switch
// -------------
enum class Bounded_rectangle_type{
    fix_cylinder_type, fix_plane_type, 
//    cylinder_type, plane_type
    write_the_code // <-- falta cylinder_type and plane_type
};

template <typename T>
inline 
constexpr Bounded_rectangle_type bounded_rectangle_type()
{
// Para definir el fix_Bounded_rectangle_ij hay que pasar en la configuración
// width y height
    if constexpr ( requires { T::width and T::height; }) {
	if constexpr ( requires { T::cylinder_type; })
	    return Bounded_rectangle_type::fix_cylinder_type;
	else
	    return Bounded_rectangle_type::fix_plane_type;
    }

    return Bounded_rectangle_type::write_the_code;
}

}// namespace impl_of
 
// fix_Bounded_rectangle_ij
// ------------------------
// typename Bounded_rectangle_ij(typename Cfg)
// {
//	switch(Cfg::type){
//	    case plane_type   : return Bounded_rectangle_ij_plane_type;
//	    case cylinder_type: return Bounded_rectangle_ij_cylinder_type;
//	    case fix_plane_type   : return fix_Bounded_rectangle_ij_plane_type;
//	    case fix_cylinder_type: return fix_Bounded_rectangle_ij_cylinder_type;
//	}
// }
//
// (RRR) Tengo varias versiones de Bounded_rectangle_ij:
//	 (1) Podemos hacer que las dimensiones del rectángulo se pasen en
//	 tiempo de compilación o no.
//	 (2) El background es un rectángulo de tamaño fijo con lo que 
//	 la ventana solo se puede mover en ese background, o es un plano
//	 infinito donde el background se repite 
//
//	 Aunque tengo la misma idea Bounded_rectangle_ij, la puedo configurar
//	 de diferentes formas. Cada una de estas formas debería de dar lugar a
//	 un nombre:
//		Bounded_rectangle_ij_plane_type
//		Bounded_rectangle_ij_cylinder_type
//		fix_Bounded_rectangle_ij_plane_type
//		fix_Bounded_rectangle_ij_cylinder_type
//		
//	 El tener que recordar estos nombres complica la vida al usuario. Por
//	 ello, fusiono las 4 clases anteriores en una clase
//	 Bounded_rectangle_ij<Cfg> donde la Cfg configurará el comportamiento
//	 de la clase. Para el usuario es mucho más sencillo de manejar.
//
template <typename Cfg, 
	    impl_of::Bounded_rectangle_type type 
				= impl_of::bounded_rectangle_type<Cfg>()>
class Bounded_rectangle_ij;

template <typename Cfg>
class Bounded_rectangle_ij <Cfg, impl_of::Bounded_rectangle_type::fix_plane_type>
    : public impl_of::fix_Bounded_rectangle_ij_plane_type<Cfg>
{};

template <typename Cfg>
class Bounded_rectangle_ij <Cfg, impl_of::Bounded_rectangle_type::fix_cylinder_type>
    : public impl_of::fix_Bounded_rectangle_ij_cylinder_type<Cfg>
{};

} // namespace atd

#endif



