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

#pragma once
#ifndef __MCU_STD_IOS_BASE_H__
#define __MCU_STD_IOS_BASE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *
 *  - HISTORIA:
 *	Manuel Perez- 30/09/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

enum class _Ios_fmtflags {
//    boolalpha  = 1L << 0,
//    dec        = 1L << 1,
//    fixed      = 1L << 2,
//    hex        = 1L << 3,
    internal   = 1L << 4,
    left       = 1L << 5,
//    oct        = 1L << 6,
    right      = 1L << 7,
//    scientific = 1L << 8,
//    showbase   = 1L << 9,
//    showpoint  = 1L << 10,
//    showpos    = 1L << 11,
    skipws     = 1L << 12,
    unitbuf    = 1L << 13,
//    uppercase  = 1L << 14
    adjustfield = left | right | internal
};

inline constexpr _Ios_fmtflags operator&(_Ios_fmtflags a, _Ios_fmtflags b)
{return static_cast<_Ios_fmtflags>(static_cast<int>(a) & static_cast<int>(b));}

inline constexpr _Ios_fmtflags operator|(_Ios_fmtflags a, _Ios_fmtflags b)
{return static_cast<_Ios_fmtflags>(static_cast<int>(a) | static_cast<int>(b));}

inline constexpr _Ios_fmtflags operator^(_Ios_fmtflags a, _Ios_fmtflags b)
{return static_cast<_Ios_fmtflags>(static_cast<int>(a) ^ static_cast<int>(b));}

inline constexpr _Ios_fmtflags operator~(_Ios_fmtflags a)
{return static_cast<_Ios_fmtflags>(~static_cast<int>(a));}

inline constexpr _Ios_fmtflags& operator&=(_Ios_fmtflags& a, _Ios_fmtflags b)
{return a = a & b;}

inline constexpr _Ios_fmtflags& operator|=(_Ios_fmtflags& a, _Ios_fmtflags b)
{return a = a | b;}

inline constexpr _Ios_fmtflags& operator^=(_Ios_fmtflags& a, _Ios_fmtflags b)
{return a = a ^ b;}

inline constexpr bool operator==(_Ios_fmtflags a, int b)
{return static_cast<int>(a) == b;}

inline constexpr bool operator!=(_Ios_fmtflags a, int b)
{return !(a == b);}


enum class _Iostate{
    goodbit = 0, 
    badbit  = 1, 
    failbit = 2,
    eofbit  = 4
};

inline constexpr _Iostate operator&(_Iostate a, _Iostate b)
{return static_cast<_Iostate>(static_cast<int>(a) & static_cast<int>(b));}

inline constexpr _Iostate operator|(_Iostate a, _Iostate b)
{return static_cast<_Iostate>(static_cast<int>(a) | static_cast<int>(b));}

inline constexpr _Iostate operator^(_Iostate a, _Iostate b)
{return static_cast<_Iostate>(static_cast<int>(a) ^ static_cast<int>(b));}

inline constexpr _Iostate operator~(_Iostate a)
{return static_cast<_Iostate>(~static_cast<int>(a));}

inline constexpr _Iostate& operator&=(_Iostate& a, _Iostate b)
{return a = a & b;}

inline constexpr _Iostate& operator|=(_Iostate& a, _Iostate b)
{return a = a | b;}

inline constexpr _Iostate& operator^=(_Iostate& a, _Iostate b)
{return a = a ^ b;}

inline constexpr bool operator==(_Iostate a, int b)
{return static_cast<int>(a) == b;}

inline constexpr bool operator!=(_Iostate a, int b)
{return !(a == b);}


enum class _Ios_openmode {
    app    = 1L << 0,
    ate    = 1L << 1,
    binary = 1L << 2,
    in     = 1L << 3,
    out    = 1L << 4,
    trunc  = 1L << 5
};

inline constexpr _Ios_openmode operator&(_Ios_openmode a, _Ios_openmode b)
{return static_cast<_Ios_openmode>(static_cast<int>(a) & static_cast<int>(b));}

inline constexpr _Ios_openmode operator|(_Ios_openmode a, _Ios_openmode b)
{return static_cast<_Ios_openmode>(static_cast<int>(a) | static_cast<int>(b));}

inline constexpr _Ios_openmode operator^(_Ios_openmode a, _Ios_openmode b)
{return static_cast<_Ios_openmode>(static_cast<int>(a) ^ static_cast<int>(b));}

inline constexpr _Ios_openmode operator~(_Ios_openmode a)
{return static_cast<_Ios_openmode>(~static_cast<int>(a));}

inline constexpr _Ios_openmode& operator&=(_Ios_openmode& a, _Ios_openmode b)
{return a = a & b;}

inline constexpr _Ios_openmode& operator|=(_Ios_openmode& a, _Ios_openmode b)
{return a = a | b;}

inline constexpr _Ios_openmode& operator^=(_Ios_openmode& a, _Ios_openmode b)
{return a = a ^ b;}

inline constexpr bool operator==(_Ios_openmode a, int b)
{return static_cast<int>(a) == b;}

inline constexpr bool operator!=(_Ios_openmode a, int b)
{return !(a == b);}



class ios_base{
public:
    // fmtflags
    // --------
    using fmtflags = _Ios_fmtflags;

    /// Adds fill characters at a designated internal point in certain
    /// generated output, or identical to right if no such point is
    /// designated.
    static constexpr fmtflags internal = fmtflags::internal;

    /// Adds fill characters on the right (final positions) of certain
    /// generated output.
    static constexpr fmtflags left = fmtflags::left;

    /// Adss fill characters on the left (initial positions) of certain
    /// generated output.
    static constexpr fmtflags right = fmtflags::right;

    /// Skips leading whitespace before certain input operations.
    static constexpr fmtflags skipws  = fmtflags::skipws;

    /// Flushes ouput after each output operation.
    static constexpr fmtflags unitbuf = fmtflags::unitbuf;

    /// A mask of left|right|internal.
    static constexpr fmtflags adjustfield = fmtflags::adjustfield;


    // iostate
    // -------
    using iostate = _Iostate;

    /// no error.
    static constexpr iostate goodbit = iostate::goodbit;

    /// irrecoverable stream error.
    static constexpr iostate badbit  = iostate::badbit;	

    /// input/output operation failed (formatting or extraction error).
    static constexpr iostate failbit = iostate::failbit; 

    /// associated input sequence has reached end-of-file.
    static constexpr iostate eofbit  = iostate::eofbit;


    // openmode
    // --------
    using openmode = _Ios_openmode;

    /// Seek to end before each write.
    static constexpr openmode app    = openmode::app;

    /// Open and seek to end immediately after opening.
    static constexpr openmode ate    = openmode::ate;

    /// Perform input and output in binary mode (as opposed to text mode)
    // (vg: en text mode se traduce en algunas plataformas '\n' por '\r\n')
    static constexpr openmode binary = openmode::binary;

    /// Open for input.
    static constexpr openmode in     = openmode::in;

    /// Open for output.
    static constexpr openmode out    = openmode::out;
    
    /// Truncate an existing stream when opening.
    static constexpr openmode trunc  = openmode::trunc;


    // fmtflags state
    // --------------
    /// Return the format control information for both input and output.
    fmtflags flags() const {return flags_;}

    /// Sets flags0 in flags(). 
    /// Postcondition: flags() == flags0.
    fmtflags flags(fmtflags flags0);

    /// Añade los flags de flags0 a flags().
    fmtflags setf(fmtflags flags0);

    /// Clears mask in flags(), sets flags0 & mask in flags().
    fmtflags setf(fmtflags flags0, fmtflags mask);


    /// Clears mask in flags().
    void unsetf(fmtflags mask) { flags_ &= ~mask; }


    /// Return the minimun field width (number of characters) to generate on
    /// certain output conversions.
    streamsize width() const {return width_;}

    /// Postcondition: width() == w.
    streamsize width(streamsize w);

protected:
    fmtflags flags_;
    iostate streambuf_state_;
    streamsize width_;
    
    // Funciones de ayuda
    // ------------------
    // Construye ios_base.
    void init(streambuf* sb);

    using traits_type = char_traits<char>;

    static bool is_eof(traits_type::int_type d) 
    { return eq(d, traits_type::eof());}

    static bool eq(traits_type::int_type a, traits_type::int_type b)
    { return traits_type::eq_int_type(a, b);}

    static bool eq(traits_type::char_type a, traits_type::int_type b)
    { return eq(traits_type::to_int_type(a), b);}

    static bool eq(traits_type::int_type a, traits_type::char_type b)
    { return eq(b, a);}

    static bool eq(traits_type::char_type a, traits_type::char_type b)
    { return traits_type::eq(a, b);}

    // Acceso a los flags de formato
    bool adjust_to_left() const
    { return (flags_ & ios_base::adjustfield) == ios_base::left; }

};

// TODO: meter funciones en .cpp
inline void ios_base::init(streambuf* sb)
{
    if (sb)
	streambuf_state_ = goodbit;
    else 
	streambuf_state_ = badbit;

    flags_ = ios_base::skipws;// TODO: | ios_base::dec;

    width_ = 0;
}

inline ios_base::fmtflags ios_base::flags(fmtflags flags0)
{
    fmtflags old = flags_;
    flags_ = flags0;

    return old;
}


inline ios_base::fmtflags ios_base::setf(fmtflags flags0) 
{
    fmtflags old = flags_;
    flags_ |= flags0;

    return old;
}

inline ios_base::fmtflags ios_base::setf(fmtflags flags0, fmtflags mask)
{
    fmtflags old = flags_;
    flags_ &= ~mask;
    flags_ |= (flags0 & mask);

    return old;
}


inline streamsize ios_base::width(streamsize w) 
{
    streamsize old = width_;
    width_ = w;

    return old;
}


// fmtflags manipulators
// ---------------------
inline ios_base& skipws(ios_base& i)
{
    i.setf(ios_base::skipws);
    return i;
}


inline ios_base& noskipws(ios_base& i)
{
    i.unsetf(ios_base::skipws);
    return i;
}


inline ios_base& unitbuf(ios_base& i)
{
    i.setf(ios_base::unitbuf);
    return i;
}

inline ios_base& nounitbuf(ios_base& i)
{
    i.unsetf(ios_base::unitbuf);
    return i;
}


// Adjustfield manipulators
// ------------------------
inline ios_base& internal(ios_base& i)
{
    i.setf(ios_base::internal, ios_base::adjustfield);
    return i;
}


inline ios_base& left(ios_base& i)
{
    i.setf(ios_base::left, ios_base::adjustfield);
    return i;
}


inline ios_base& right(ios_base& i)
{
    i.setf(ios_base::right, ios_base::adjustfield);
    return i;
}



}// namespace

#endif

