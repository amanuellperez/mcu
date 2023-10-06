// Copyright (C) 2019-2022 Manuel Perez 
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
#ifndef __MCU_STD_ITERATOR_H__
#define __MCU_STD_ITERATOR_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Equivalente a <iterator>
 *
 *  HISTORIA
 *    Manuel Perez
 *	03/11/2019: iterator_traits, reverse_iterator
 *	04/02/2020: advance, next, prev, distance
 *	23/12/2022: begin()/end()/size()
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_type_traits.h"	// void_t
#include <stddef.h>		// size_t
#include "std_range_access.h"

namespace STD{
// primitives
// ----------
// Tipos de iteradores
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
struct contiguous_iterator_tag	  : public random_access_iterator_tag { };

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
    requires is_object_v<T>
struct iterator_traits<T*> {
    using iterator_concept  = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
};


template <typename T>
struct iterator_traits<const T*> {
    using iterator_concept  = contiguous_iterator_tag;
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;
};



// iterator operations
// -------------------
// advance
template <typename It>
constexpr bool __is_random_access_iterator
	= is_same_v<typename iterator_traits<It>::iterator_category,
		    random_access_iterator_tag>;

template <typename It>
constexpr bool __is_bidirectional_iterator
	= is_same_v<typename iterator_traits<It>::iterator_category,
		    bidirectional_iterator_tag>;



template <typename InputIterator, typename Distance>
inline constexpr void advance (InputIterator& i, Distance n)
{
    if constexpr (__is_random_access_iterator<InputIterator>){
	i += n;
    }

    else if constexpr (__is_bidirectional_iterator<InputIterator>){
    
	if (n > 0){
	    for (; n != 0; --n)
		++i;
	} else {
	    for (; n != 0; ++n)
		--i;
	}

    }

    else{ // default: InputIterator

	for (; n != 0; --n)
	    ++i;
    }

}


// next
template < typename InputIterator>
inline constexpr InputIterator next(InputIterator x, 
	    typename iterator_traits<InputIterator>::difference_type n = 1)
{ 
    advance(x, n);

    return x;
}


// prev
template < typename BidirectionalIterator>
inline constexpr BidirectionalIterator prev(BidirectionalIterator x, 
	typename iterator_traits<BidirectionalIterator>::difference_type n = 1)
{ 
    advance(x, -n);

    return x;
}


// distance
template <typename InputIterator>
inline constexpr typename iterator_traits<InputIterator>::difference_type
distance(InputIterator p0, InputIterator pe)
{
    if constexpr (__is_random_access_iterator<InputIterator>){
	return pe - p0;
    }

    else{ // default:
	typename iterator_traits<InputIterator>::difference_type n{0};
	while (p0 != pe){
	    ++p0;
	    ++n;
	}

	return n;
    }

}



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





// size()
// ------
template <typename Container>
constexpr auto size(const Container& c) -> decltype(c.size())
{ return c.size(); }

template <typename T, size_t N>
inline constexpr size_t size(T (&c)[N]) noexcept 
{ return N; }


}// namespace

#include "std_iterator_concepts.h"

#endif



