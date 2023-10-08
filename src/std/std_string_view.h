// Copyright (C) 2023 Manuel Perez 
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

#ifndef __MCU_STD_STRING_VIEW_H__
#define __MCU_STD_STRING_VIEW_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	El correspondiente a <string_view>
 * 
 * HISTORIA
 *    Manuel Perez
 *    25/09/2023 Versión mínima.
 *               Voy a probar a seguir la estructura del estandar dejando
 *               comentados aquellos apartados que no implemento, para saber
 *               qué hay que es lo que falta implementar.
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_char_traits.h"
#include "std_iterator.h"
#include "std_memory.h"
#include "std_range_access.h"
#include "std_stdexcept.h"  // out_of_range
#include "std_algorithm.h"  // min

namespace STD{

/***************************************************************************
 *			    basic_string_view
 ***************************************************************************/
template <typename charT, typename traits = char_traits<charT>>
class basic_string_view{
public:
// Types
    using traits_type	    = traits;
    using value_type	    = charT;
    using pointer	    = value_type*;
    using const_pointer	    = const value_type*;
    using reference	    = value_type&;
    using const_reference   = const value_type&;
    using const_iterator    = const value_type*;
    using const_reverse_iterator = STD::reverse_iterator<const_iterator>;
    using reverse_iterator  = const_reverse_iterator;
    using size_type	    = size_t;
    using difference_type   = ptrdiff_t;
    
    static constexpr size_type npos = size_type(-1);

// Construction and assignment
    constexpr basic_string_view() noexcept;
    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&)
							noexcept = default;
    constexpr basic_string_view(const charT* str);
    constexpr basic_string_view(const charT* str, size_type len);
    template <contiguous_iterator It, sized_sentinel_for<It> End>
	requires same_as<iter_value_t<It>, charT> and
		 (!convertible_to<End, size_type>)
	constexpr basic_string_view(It begin, End end)
	: data_{STD::to_address(begin)}, size_(end - begin)
	//size_{end - begin}	// este da warning: narrowing (???)
	{}

// Iterator support
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

// Capacity
    constexpr size_type size() const noexcept;
    constexpr size_type length() const noexcept;
    // TODO: constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr bool empty() const noexcept;

// Element access 
    constexpr const_reference operator[](size_type pos) const;
    constexpr const_reference at(size_type pos) const;
    constexpr const_reference front() const;
    constexpr const_reference back() const;
    constexpr const_pointer data() const noexcept;

// Modifiers
    constexpr void remove_prefix(size_type n);
    constexpr void remove_suffix(size_type n);
    constexpr void swap(basic_string_view& s) noexcept;

// String operations
    constexpr size_type copy(charT* s, size_type n, size_type pos = 0) const;

    constexpr basic_string_view 
			substr(size_type pos = 0, size_type n = npos) const;

    constexpr int compare(basic_string_view s) const noexcept;
    constexpr int compare(size_type pos1, size_type n1, basic_string_view s) const;
    constexpr int compare(size_type pos1, size_type n1, basic_string_view s,
			  size_type pos2, size_type n2) const;
    constexpr int compare(const charT* s) const;
    constexpr int compare(size_type pos1, size_type n1, const charT* s) const;
    constexpr int compare(size_type pos1, size_type n1, const charT* s,
				          size_type n2) const;

    constexpr bool starts_with(basic_string_view x) const noexcept;
    constexpr bool starts_with(charT x) const noexcept;
    constexpr bool starts_with(const charT* x) const;

    constexpr bool ends_with(basic_string_view x) const noexcept;
    constexpr bool ends_with(charT x) const noexcept;
    constexpr bool ends_with(const charT* x) const;

// Searching
    constexpr size_type find(basic_string_view s, size_type pos = 0) const noexcept;
    constexpr size_type find(charT c, size_type pos = 0) const noexcept;
    constexpr size_type find(const charT* s, size_type pos, size_type n) const;
    constexpr size_type find(const charT* s, size_type pos = 0) const;

    constexpr size_type rfind(basic_string_view s, size_type pos = npos) const noexcept;
    constexpr size_type rfind(charT c, size_type pos = npos) const noexcept;
    constexpr size_type rfind(const charT* s, size_type pos, size_type n) const;
    constexpr size_type rfind(const charT* s, size_type pos = npos) const;

    constexpr size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of(charT c, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of(const charT* s, size_type pos, size_type n) const;
    constexpr size_type find_first_of(const charT* s, size_type pos = 0) const;

    constexpr size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of(charT c, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of(const charT* s, size_type pos, size_type n) const;
    constexpr size_type find_last_of(const charT* s, size_type pos = npos) const;

    constexpr size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of(charT c, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of(const charT* s, size_type pos, 
							  size_type n) const;
    constexpr size_type find_first_not_of(const charT* s, 
						    size_type pos = 0) const;

    constexpr size_type find_last_not_of(basic_string_view s, 
					    size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of(charT c, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of(const charT* s, size_type pos, 
							    size_type n) const;
    constexpr size_type find_last_not_of(const charT* s, size_type pos = npos) const;

private:
    const_pointer data_;
    size_type size_;

// Helper
    constexpr bool equal_(const char* p) const noexcept;
    
};

// Deduction guide
template <typename It, typename End>
basic_string_view(It, End) -> basic_string_view<iter_value_t<It>>;


/***************************************************************************
 *		    IMPLEMENTATION basic_string_view
 ***************************************************************************/
// Construction and assignment
// ---------------------------
template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::basic_string_view() noexcept
	    : data_{nullptr}, size_{0}
{ }


template <typename charT, typename Tr>
inline
constexpr basic_string_view<charT, Tr>::basic_string_view(const charT* str)
	: data_{str}, size_{traits_type::length(str)}
{ }


template <typename charT, typename Tr>
inline 
constexpr 
basic_string_view<charT, Tr>::basic_string_view(const charT* str, size_type len)
    : data_{str}, size_{len}
{ }


//template <typename charT, typename Tr>
//    template <typename It, typename End>
//inline 
//constexpr basic_string_view<charT, Tr>::basic_string_view(It begin, End end)
//    : data_{STD::to_address(begin)}, 
//    //size_{end - begin}    <-- aunque lo correcto es esto, los de gcc tienen
//    size_(end - begin)	//  <-- este para evitar que les de warning.
//			//	Probar a compilar el test cambiando al
//			//	anterior. Se verá un warning.
//{ }


// Iterator support
// ----------------
template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_iterator basic_string_view<charT, Tr>::begin() const noexcept
{ return data_; }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_iterator basic_string_view<charT, Tr>::end() const noexcept
{ return begin() + size(); }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_iterator
basic_string_view<charT, Tr>::cbegin() const noexcept
{ return data_; }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_iterator
basic_string_view<charT, Tr>::cend() const noexcept
{ return cbegin() + size(); }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_reverse_iterator
basic_string_view<charT, Tr>::rbegin() const noexcept
{ return const_reverse_iterator(end()); }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_reverse_iterator
basic_string_view<charT, Tr>::rend() const noexcept
{ return const_reverse_iterator(begin()); }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_reverse_iterator 
basic_string_view<charT, Tr>::crbegin() const noexcept
{ return const_reverse_iterator(end()); }

template <typename charT, typename Tr>
inline 
constexpr basic_string_view<charT, Tr>::const_reverse_iterator 
basic_string_view<charT, Tr>::crend() const noexcept
{ return const_reverse_iterator(begin()); }


// Capacity
// --------
template <typename charT, typename Tr>
inline
constexpr basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::size() const noexcept
{ return size_;}

template <typename charT, typename Tr>
inline
constexpr basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::length() const noexcept
{ return size_;}

//template <typename charT, typename Tr>
//inline
//constexpr size_type basic_string_view<charT, Tr>::max_size() const noexcept
//{ ???  }

template <typename charT, typename Tr>
inline
constexpr bool basic_string_view<charT, Tr>::empty() const noexcept
{ return size_ == 0; }

// Element access
// --------------
template <typename charT, typename Tr>
inline
constexpr 
basic_string_view<charT, Tr>::const_reference 
    basic_string_view<charT, Tr>::operator[](size_type pos) const
{ return data_[pos]; }

template <typename charT, typename Tr>
inline
constexpr 
basic_string_view<charT, Tr>::const_reference 
    basic_string_view<charT, Tr>::at(size_type pos) const
{ 
    if (pos >= size())
	throw_exception<out_of_range>("TODO: write comment");

    return data_[pos]; 
}


template <typename charT, typename Tr>
inline
constexpr 
basic_string_view<charT, Tr>::const_reference 
    basic_string_view<charT, Tr>::front() const
{ return data_[0]; }

template <typename charT, typename Tr>
inline
constexpr 
basic_string_view<charT, Tr>::const_reference 
    basic_string_view<charT, Tr>::back() const
{ return data_[size() -1]; }

template <typename charT, typename Tr>
inline
constexpr 
basic_string_view<charT, Tr>::const_pointer 
    basic_string_view<charT, Tr>::data() const noexcept
{ return data_;}


// Modifiers
// ---------
template <typename charT, typename Tr>
inline
constexpr void basic_string_view<charT, Tr>::remove_prefix(size_type n)
{
    data_ += n;
    size_ -= n;
}

template <typename charT, typename Tr>
inline
constexpr void basic_string_view<charT, Tr>::remove_suffix(size_type n)
{ size_ -= n; }

template <typename charT, typename Tr>
inline
constexpr void basic_string_view<charT, Tr>::swap(basic_string_view& s) noexcept
{
//    swap(data_, s.data_); <-- así lo implementaría yo
//    swap(size_, s.size_);
    auto tmp = *this;	// <-- así lo implementan los de gcc, alguna ventaja?
    *this = s;
    s = tmp;
}


// String operations
// -----------------
// copy
template <typename charT, typename Tr>
constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::
			    copy(charT* s, size_type n, size_type pos) const
{
    if (pos > size())
	throw_exception<out_of_range>("TODO: write comment");

    size_type rlen = min(n, size() - pos);
    traits_type::copy(s, data() + pos, rlen);

    return rlen;
}


// substr
// ------
template <typename charT, typename Tr>
constexpr
basic_string_view<charT, Tr>
basic_string_view<charT, Tr>::substr(size_type pos, size_type n) const
{
    if (pos > size())
	throw_exception<out_of_range>("TODO: write comment");

    size_type rlen = min(n, size() - pos);
    return basic_string_view{data() + pos, rlen};

}

// compare
// -------
template <typename charT, typename Tr>
constexpr 
int basic_string_view<charT, Tr>::compare(basic_string_view s) const noexcept
{
    size_t rlen = min(size(), s.size());

    return traits_type::compare(data(), s.data(), rlen);
}


template <typename charT, typename Tr>
inline 
constexpr int 
basic_string_view<charT, Tr>::
		compare(size_type pos1, size_type n1, basic_string_view s) const
{ return substr(pos1, n1).compare(s); }

template <typename charT, typename Tr>
inline 
constexpr int 
basic_string_view<charT, Tr>::
	compare(size_type pos1, size_type n1, basic_string_view s,
					    size_type pos2, size_type n2) const
{ return substr(pos1, n1).compare(s.substr(pos2, n2)); }

template <typename charT, typename Tr>
inline 
constexpr int 
basic_string_view<charT, Tr>::compare(const charT* s) const
{ return compare(basic_string_view(s)); }

template <typename charT, typename Tr>
inline 
constexpr int 
basic_string_view<charT, Tr>::
		    compare(size_type pos1, size_type n1, const charT* s) const
{ return substr(pos1, n1).compare(basic_string_view(s)); }

template <typename charT, typename Tr>
inline 
constexpr int 
basic_string_view<charT, Tr>::
    compare(size_type pos1, size_type n1, const charT* s, size_type n2) const
{ return substr(pos1, n1).compare(basic_string_view(s, n2)); }


// starts_with
// ------------
template <typename charT, typename Tr>
inline 
constexpr bool 
basic_string_view<charT, Tr>::starts_with(basic_string_view x) const noexcept
{ return substr(0, x.size()) == x; }

template <typename charT, typename Tr>
inline 
constexpr bool
basic_string_view<charT, Tr>::starts_with(charT x) const noexcept
{ return !empty() and traits_type::eq(front(), x); }

template <typename charT, typename Tr>
inline constexpr bool 
basic_string_view<charT, Tr>::starts_with(const charT* x) const
{ return starts_with(basic_string_view(x)); }


// ends_with
// ----------

template <typename charT, typename Tr>
inline 
constexpr bool 
basic_string_view<charT, Tr>::ends_with(basic_string_view x) const noexcept
{
    return ((size() >= x.size()) and compare(size() - x.size(), npos, x)) == 0;
}

template <typename charT, typename Tr>
inline 
constexpr bool 
basic_string_view<charT, Tr>::ends_with(charT x) const noexcept
{ return !empty() and traits_type::eq(back(), x); }

template <typename charT, typename Tr>
inline 
constexpr bool 
basic_string_view<charT, Tr>::ends_with(const charT* x) const
{ return ends_with(basic_string_view(x)); }


// Searching
// ---------
// TODO: 
//	Se puede optimizar un poco la búsqueda. No es necesario que i llegue
//	hasta el final, pero depende de j.
// 
template <typename charT, typename Tr>
constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::find(basic_string_view s, size_type pos) const noexcept
{
    size_type xpos = npos;
    
    if (size() < s.size())
	return npos;

    size_type j = 0;	// posición en s
    for (size_type i = pos; i < size(); ++i){
	if (traits_type::eq(data_[i], s[j])){
	    if (j == 0)
		xpos = i;

	    ++j;
	    if (j == s.size())
		return xpos;

	}

	else {
	    if (j != 0){
		xpos = npos;
		j = 0;
	    }
	}

    }

    return xpos;
}


// DUDA: el standard define esta función tal como la implemento, pero
// ¿sería más eficiente hacer una implementación propia? 
template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::find(charT c, size_type pos) const noexcept
{ return find(basic_string_view{addressof(c), 1}, pos); }


template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::find(const charT* s, size_type pos, size_type n) const
{ return find(basic_string_view{s, n}, pos); }


template <typename charT, typename Tr>
inline constexpr
basic_string_view<charT,Tr>::size_type 
basic_string_view<charT, Tr>::find(const charT* s, size_type pos) const
{ return find(basic_string_view{s}, pos); }


template <typename charT, typename Tr>
constexpr bool 
basic_string_view<charT, Tr>::equal_(const char* p) const noexcept
{
    for (size_type i = 0; i < size(); ++i, ++p){
	if (!traits_type::eq(*p, data_[i]))
	    return false;
    }

    return true;
}


template <typename charT, typename Tr>
constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::rfind(basic_string_view s, size_type pos) const noexcept
{
    if (size() < s.size())
	return npos;

    size_type i = size() - s.size() + 1;
    if (pos != npos)
	i = pos;

    while (1) {
	if (i == 0) 
	    return npos;

	--i;

	if (traits_type::eq(data_[i], s[0])){
	    if (s.equal_(&(data_[i])))
		return i;
	}
    }

    // inalcanzable
}

template <typename charT, typename Tr>
inline constexpr
basic_string_view<charT, Tr>::size_type
basic_string_view<charT, Tr>::rfind(charT c, size_type pos) const noexcept
{ return rfind(basic_string_view{addressof(c), 1}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type 
basic_string_view<charT, Tr>::rfind(const charT* s, size_type pos, size_type n) const
{ return rfind(basic_string_view{s, n}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::rfind(const charT* s, size_type pos) const
{ return rfind(basic_string_view{s}, pos); }



template <typename charT, typename Tr>
constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_of(basic_string_view s, size_type pos) const noexcept
{
    for (size_type i = pos; i < size(); ++i){
	for (size_type j = 0; j < s.size(); ++j){
	    if (traits_type::eq(data_[i], s[j]))
		return i;
	}
    }

    return npos;
}

template <typename charT, typename Tr>
inline constexpr
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_of(charT c, size_type pos) const noexcept
{ return find_first_of(basic_string_view{addressof(c), 1}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_of(const charT* s, size_type pos, 
							    size_type n) const
{ return find_first_of(basic_string_view{s, n}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT, Tr>::size_type 
basic_string_view<charT, Tr>::find_first_of(const charT* s, size_type pos) const
{ return find_first_of(basic_string_view{s}, pos); }


template <typename charT, typename Tr>
constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_of(basic_string_view s, 
						size_type pos) const noexcept
{
    size_type i = size();
    if (pos != npos)
	i = pos + 1; // +1 porque lo primero que hago es --i

    while (i > 0){
	--i;
	for (size_type j = 0; j < s.size(); ++j){
	    if (traits_type::eq(data_[i], s[j]))
		return i;
	}
    }

    return npos;
}

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_of(charT c, size_type pos) const noexcept
{ return find_last_of(basic_string_view{addressof(c), 1}, pos); }


template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_of(const charT* s, size_type pos,
							    size_type n) const
{ return find_last_of(basic_string_view{s, n}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_of(const charT* s, size_type pos) const
{ return find_last_of(basic_string_view{s}, pos); }


template <typename charT, typename Tr>
constexpr
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_not_of(basic_string_view s,
						size_type pos) const noexcept
{
    for (size_type i = pos; i < size(); ++i){
	bool found_equal = false;

	for (size_type j = 0; j < s.size(); ++j){
	    if (traits_type::eq(data_[i], s[j])){
		found_equal = true;
		break;
	    }
	}

	if (!found_equal)
	    return i;
    }

    return npos;
}

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_not_of(charT c, 
						size_type pos) const noexcept
{ return find_first_not_of(basic_string_view{addressof(c), 1}, pos); }


template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_not_of(const charT* s, size_type pos,
							    size_type n) const
{ return find_first_not_of(basic_string_view{s, n}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_first_not_of(const charT* s, 
							size_type pos) const
{ return find_first_not_of(basic_string_view{s}, pos); }

template <typename charT, typename Tr>
constexpr 
basic_string_view<charT,Tr>::size_type 
basic_string_view<charT, Tr>::find_last_not_of(basic_string_view s, 
						size_type pos) const noexcept
{
    size_type i = size();
    if (pos != npos)
	i = pos + 1; // +1 porque lo primero que hago es --i

    while (i > 0){
	--i;
	bool found_equal = false;

	for (size_type j = 0; j < s.size(); ++j){
	    if (traits_type::eq(data_[i], s[j])){
		found_equal = true;
		break;
	    }
	}
	
	if (!found_equal)
	    return i;
    }

    return npos;

}

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_not_of(charT c, size_type pos) const noexcept
{ return find_last_not_of(basic_string_view{addressof(c), 1}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type
basic_string_view<charT, Tr>::find_last_not_of(const charT* s, size_type pos,
							    size_type n) const
{ return find_last_not_of(basic_string_view{s, n}, pos); }

template <typename charT, typename Tr>
inline constexpr 
basic_string_view<charT,Tr>::size_type 
basic_string_view<charT, Tr>::find_last_not_of(const charT* s, size_type pos) const
{ return find_last_not_of(basic_string_view{s}, pos); }



/***************************************************************************
 *			    NON MEMBER FUNCTIONS
 ***************************************************************************/
// Non-member comparison functions
template <typename C, typename T>
inline constexpr bool operator==(basic_string_view<C, T>& x,
			  basic_string_view<C, T>& y) noexcept
{
    return x.size() == y.size() and
	   x.compare(y) == 0;
}


template <typename C, typename T>
inline constexpr bool operator==(basic_string_view<C, T>& x,
			type_identity_t<basic_string_view<C, T>> y) noexcept
{
    return x.size() == y.size() and
	   x.compare(y) == 0;
}

//template <typename C, typename T>
//constexpr bool operator<=>(basic_string_view<C, T>& x,
//			  basic_string_view<C, T>& y) no except;
//

// Inserters and extractors
// TODO (ver 21.4.5 borrador standard 2020)


// Alias
using string_view = basic_string_view<char>;
// TODO: ¿necesito estas en microcontroladores?
//using u8string_view = basic_string_view<char8_t>;
//using u16string_view = basic_string_view<char16_t>;
//using u32string_view = basic_string_view<char32_t>;
//using wstring_view = basic_string_view<wchar_t>;

// Hash support
// TODO

//inline namespace literals {
//inline namespace string_view_literals {
//    constexpr string_view operator""sv(const char* str, size_t len) noexcept;
//    // TODO: resto para char8_t, char16_t, ...
//} // namespace string_view_literals
//}// namespace literals


}// namespace


#endif


