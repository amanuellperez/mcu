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

#pragma once
#ifndef __MCU_STD_ITERATOR_H__
#define __MCU_STD_ITERATOR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Equivalente a <iterator>
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	03/11/2019: iterator_traits, reverse_iterator
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_type_traits.h"	// void_t
#include <stddef.h>		// size_t

namespace STD{
// primitives
// ----------
// Tipos de iteradores
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

// El standard indica que si It no es un iterador, iterator_traits == vacia!
// Usamos void_t para seleccionar esto.
template <typename It, typename = void_t<>>
struct __iterator_traits { };

template <typename It>
    struct __iterator_traits < It,
			    void_t<typename It::iterator_category,
				   typename It::value_type,
				   typename It::difference_type,
				   typename It::pointer,
				   typename It::reference> >
{
    using iterator_category = typename It::iterator_category;
    using value_type        = typename It::value_type;
    using difference_type   = typename It::difference_type;
    using pointer           = typename It::pointer;
    using reference         = typename It::reference;
};

template <typename It>
    struct iterator_traits : public __iterator_traits<It> { };


template <typename T>
struct iterator_traits<T*> {
    using difference_type   = ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = random_access_iterator_tag;
};


template <typename T>
struct iterator_traits<const T*> {
    using difference_type   = ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = random_access_iterator_tag;
};



// iterator operations
// -------------------



// predefined iterators
// --------------------
/*!
 *  \brief  Adaptador reverse_iterator
 *
 */
template <typename It> 
class reverse_iterator{
public:
    using iterator_type     = It;
    using iterator_category = typename iterator_traits<It>::iterator_category;
    using value_type        = typename iterator_traits<It>::value_type;
    using difference_type   = typename iterator_traits<It>::difference_type;
    using pointer           = typename iterator_traits<It>::pointer;
    using reference         = typename iterator_traits<It>::reference;

    constexpr reverse_iterator() { }
    constexpr explicit reverse_iterator(It x) : current{x} { }

    template <typename U>
    constexpr reverse_iterator(const reverse_iterator<U>& u)
	    : current{u.base()} { }

    template <typename U>
    constexpr reverse_iterator& operator=(const reverse_iterator<U>& u)
    {
	current = u.base();
	return *this;
    }

    constexpr iterator_type base() const {return current;}
    constexpr reference operator*() const
    {
	iterator_type it = current;
	--it;
	return *it;
    }

    constexpr pointer operator->() const
    { return &(operator*()); }

    constexpr reverse_iterator& operator++()
    {
	--current;
	return *this;
    }

    constexpr reverse_iterator& operator++(int)
    {
	reverse_iterator tmp = *this;
	--current;
	return tmp;
    }

    constexpr reverse_iterator& operator--()
    {
	++current;
	return *this;
    }

    constexpr reverse_iterator& operator--(int)
    {
	reverse_iterator tmp = *this;
	++current;
	return tmp;
    }
    
    constexpr reverse_iterator operator+(difference_type n) const
    { return reverse_iterator{current - n}; }

    constexpr reverse_iterator& operator+=(difference_type n)
    {
	current -= n;
	return *this;
    }

    constexpr reverse_iterator operator-(difference_type n) const
    { return reverse_iterator{current + n}; }

    constexpr reverse_iterator& operator-=(difference_type n)
    {
	current += n;
	return *this;
    }

    constexpr reference operator[](difference_type n) const
    { return *(*this + n); }

protected:
    It current;	// El standard define esto así.
    
};

template <typename It1, typename It2>
inline constexpr bool operator==(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return x.base() == y.base(); }

template <typename It1, typename It2>
inline constexpr bool operator<(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return x.base() < y.base(); }


template <typename It1, typename It2>
inline constexpr bool operator!=(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return !(x == y);}

template <typename It1, typename It2>
inline constexpr bool operator>(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return y < x; }

template <typename It1, typename It2>
inline constexpr bool operator>=(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return !(x < y); }

template <typename It1, typename It2>
inline constexpr bool operator<=(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
{ return !(y < x); }

template <typename It1, typename It2>
inline constexpr auto operator-(const reverse_iterator<It1>& x,
                                 const reverse_iterator<It2>& y)
    -> decltype(y.base() - x.base())
{ return (y.base() - x.base()); }

template <typename It>
inline constexpr reverse_iterator<It>
    operator+(typename reverse_iterator<It>::difference_type n,
	      const reverse_iterator<It>& x)
{ return x + n;}

template <typename It>
inline constexpr reverse_iterator<It> make_reverse_iterator(It i)
{return reverse_iterator<It>{i};}

}// namespace

#endif



