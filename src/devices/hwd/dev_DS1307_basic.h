// Copyright (C) 2020 Manuel Perez 
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
#ifndef __DEV_DS1307_BASIC_H__
#define __DEV_DS1307_BASIC_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del RTC DS1307
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    08/04/2020 v0.0
 *
 ****************************************************************************/

#include <stdint.h> // uint8_t...
#include <atd_memory.h>	// Memory_type
#include <atd_bcd.h>

#include <atd_bit.h>
#include <mcu_TWI_master_ioxtream.h>

#include "dev_TWI_memory_type.h"

namespace dev{

// Table 2: only time (not RAM)
// ¿Por qué no usar time_t para guardar los datos?
// Porque este es un traductor. La idea es que se pueda escribir leyendo
// literalmente la datasheet, de esa forma habrá menos errores de traducción.
struct __DS1307_timekeeper{
// Data
    uint8_t seconds;// 00-59
    uint8_t minutes;// 00-59
    uint8_t hours;  // 00-23 or 01-12 + AM/PM
    uint8_t day;    // day of the week (01-07)
    uint8_t date;   // day of the month (01-31)
    uint8_t month;  // 01-12
    uint8_t year;   // 00-99 (from 2000 to 2099)

// Configuración del reloj
    bool clock_on = true;// = not CH; true = working; false = clock halted.

// Formato de la hora
    bool AMPM_format = false;    // false = 24 hours format; true = AM/PM format
    bool is_PM; // si AMPM_format == true, indica si es PM o no (en cuyo caso sería
                // AM). Si AMPM_format == false su valor no está definido.

// Memory
    static constexpr atd::Memory_type mem_type =
					      atd::Memory_type::read_and_write;
    static constexpr uint8_t address {0x00};
    static constexpr uint8_t size = 7;

    static constexpr bool use_struct_as_mem = true;

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __DS1307_timekeeper& st);

    template <typename Oxtream>
    friend Oxtream& operator<<(Oxtream& out, const __DS1307_timekeeper& st);



private:
    static constexpr atd::Range_bitmask<0, 6, uint8_t> mask_seconds{};
    static constexpr atd::Range_bitmask<7, 7, uint8_t> mask_CH{};
    static constexpr atd::Range_bitmask<6, 6, uint8_t> mask_AMPM_mode{};
    // Formato de las horas (AM/PM)
    static constexpr atd::Range_bitmask<0, 4, uint8_t> mask_AMPM_hours{};
    static constexpr atd::Range_bitmask<5, 5, uint8_t> mask_AMPM_PM{};


    static void mem_to_struct(__DS1307_timekeeper& st);
    static __DS1307_timekeeper struct_to_mem(__DS1307_timekeeper st0);
};



template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __DS1307_timekeeper& st)
{
    using T = __DS1307_timekeeper;

    in >> st.seconds;
    in >> st.minutes;
    in >> st.hours;
    in >> st.day;
    in >> st.date;
    in >> st.month;
    in >> st.year;

    T::mem_to_struct(st);

    return in;
}


template <typename Oxtream>
Oxtream& operator<<(Oxtream& out, const __DS1307_timekeeper& st0)
{
    using T = __DS1307_timekeeper;

    T st = T::struct_to_mem(st0);

    out << st.seconds;
    out << st.minutes;
    out << st.hours;
    out << st.day;
    out << st.date;
    out << st.month;
    out << st.year;

    return out;
}


// CUIDADO: Solo comparo la hora y la fecha
// Lo añado para poder comparar default_time con la hora que tiene el RTC.
inline bool operator==(const __DS1307_timekeeper& a, 
		       const __DS1307_timekeeper& b)
{
    return ((a.seconds == b.seconds) and (a.minutes == b.minutes) and
            (a.hours == b.hours) and (a.date == b.date) and
            (a.month == b.month) and (a.year == b.year));
}


inline bool operator!=(const __DS1307_timekeeper& a,
                       const __DS1307_timekeeper& b)
{return !(a == b);}



// Control register
struct __DS1307_control_register{
// Data
    bool output_control;
    bool square_wave_enable;
    uint8_t rate_select;

    __DS1307_control_register() {}
    __DS1307_control_register(bool out, bool square, uint8_t rate)
	: output_control{out}, square_wave_enable{square}, rate_select{rate}{}

// Memory
    static constexpr atd::Memory_type mem_type =
					      atd::Memory_type::read_and_write;
    static constexpr uint8_t address {0x07};
    static constexpr uint8_t size = 1;

    static constexpr bool use_struct_as_mem = true;

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __DS1307_control_register& st);

    template <typename Oxtream>
    friend Oxtream& operator<<(Oxtream& out, const __DS1307_control_register& st);


// Table control register
// Posible mejora de eficiencia (?): no inicializar los valores que no se
// usan. Mirar la tabla en la datasheet. Aunque complica el código y no parece
// que mejore mucho.
    static __DS1307_control_register output_high()
    {return __DS1307_control_register{true, false, 0};}

    static __DS1307_control_register output_low()
    {return __DS1307_control_register{false, false, 0};}

    static __DS1307_control_register output_square_wave_1Hz()
    {return __DS1307_control_register{false, true, 0x00};}

    static __DS1307_control_register output_square_wave_4kHz()
    {return __DS1307_control_register{false, true, 0x01};}

    static __DS1307_control_register output_square_wave_8kHz()
    {return __DS1307_control_register{false, true, 0x02};}

    static __DS1307_control_register output_square_wave_32kHz()
    {return __DS1307_control_register{false, true, 0x03};}


private:
    static constexpr atd::Range_bitmask<7, 7, uint8_t> mask_output_control{};
    static constexpr atd::Range_bitmask<4, 4, uint8_t> mask_square_wave_enable{};
    static constexpr atd::Range_bitmask<0, 1, uint8_t> mask_rate_select{};

    static void mem_to_struct(const uint8_t& mem,
                              __DS1307_control_register& st);

    static void struct_to_mem(const __DS1307_control_register& st, 
				uint8_t& mem);
};


template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __DS1307_control_register& st)
{
    using T = __DS1307_control_register;

    uint8_t mem;
    in >> mem;

    T::mem_to_struct(mem, st);

    return in;
}


template <typename Oxtream>
Oxtream& operator<<(Oxtream& out, const __DS1307_control_register& st)
{
    using T = __DS1307_control_register;

    uint8_t mem;
    T::struct_to_mem(st, mem);

    out << mem;

    return out;
}



/*!
 *  \brief  Driver básico del DS1307.
 *
 *  Gestión de errores: el reloj tiene un state donde se anotará el resultado 
 *  de la última operación. 
 *
 */
template <typename TWI_master>
class DS1307_basic{
public:
// Configuración de TWI
    using Address = typename TWI_master::Address;
    static constexpr Address slave_address = 0x68; // es fija.
    static constexpr uint8_t TWI_buffer_size = 70; // tiene 64 bytes de memoria

    static_assert(TWI_master::buffer_size >= TWI_buffer_size);

// Types
    using TWI              = TWI_memory_type<TWI_master, slave_address>;
    using State            = TWI::iostate;
    using Time_point       = __DS1307_timekeeper;
    using Control_register = __DS1307_control_register;


// Clock reading & writing
    /// Lectura del reloj
    void read(Time_point& t) {state_ = TWI::read(t);}

    /// Escritura en el reloj.
    void write(const Time_point& t) { state_ = TWI::write(t); }

// RAM access
    /// Leemos ram[i... i + n) guardándolo en buf.
    /// Devuelve el número de bytes leidos.
    uint8_t ram_read(uint8_t* buf, uint8_t n, uint8_t i);

    /// Escribimos n bytes de buf en ram[i... i+n).
    /// Devuelve el número de bytes escritos.
    uint8_t ram_write(uint8_t* buf, uint8_t n, uint8_t i);


// Output
// Recordar encender el reloj para poder generar las square waves
    void output_high();	// out = 1
    void output_low();	// out = 0
    void output_square_wave_1Hz();   // set to       1Hz
    void output_square_wave_4kHz();  // set to  4.096kHz
    void output_square_wave_8kHz();  // set to  8.192kHz
    void output_square_wave_32kHz(); // set to 32.768kHz

    void disable_output() {output_low();}
    void disable_square_wave() {disable_output();}

    void read(Control_register& c) {state_ = TWI::read(c);}


// States
    bool good() const {return !error();}
    bool error() const {return TWI_master::error(state_);}

    bool no_response() const {return state_ == TWI::iostate::no_response;}

    // para depurar
    State state() const {return state_;}

// Valor inicial que toma el reloj cuando se inicializa
    static Time_point default_time();

private:
    State state_;

    static constexpr uint8_t ram_address {0x08};

};

template <typename TWI_master>
uint8_t DS1307_basic<TWI_master>::ram_read(uint8_t* buf, uint8_t n, uint8_t i)
{
    using TWI = mcu::TWI_master_ioxtream<TWI_master>;

    TWI twi(slave_address);
     
    uint8_t addr = ram_address + i;
    twi << uint8_t{addr};

    if (twi.error()){
	state_ = TWI::state();
	return 0;
    }

    uint8_t nread = twi.read(buf, n);

    state_ = TWI::state();

    return nread;

}

template <typename TWI_master>
uint8_t DS1307_basic<TWI_master>::ram_write(uint8_t* buf, uint8_t n, uint8_t i)
{
    using TWI = mcu::TWI_master_ioxtream<TWI_master>;

    TWI twi(slave_address);
 
    if (twi.error()){
	state_ = TWI::state();
	return 0;
    }

    uint8_t addr = ram_address + i;
    twi << uint8_t{addr};

    uint8_t nwrite = twi.write(buf, n);

    state_ = TWI::state();

    return nwrite;
}

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_high()
{ state_ = TWI::write(Control_register::output_high()); }

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_low()
{ state_ = TWI::write(Control_register::output_low()); }

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_square_wave_1Hz()
{ state_ = TWI::write(Control_register::output_square_wave_1Hz()); }

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_square_wave_4kHz()
{ state_ = TWI::write(Control_register::output_square_wave_4kHz()); }

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_square_wave_8kHz()
{ state_ = TWI::write(Control_register::output_square_wave_8kHz()); }

template <typename TWI_master>
inline void DS1307_basic<TWI_master>::output_square_wave_32kHz()
{ state_ = TWI::write(Control_register::output_square_wave_32kHz()); }
// see page 8, datasheet
template <typename TWI_master>
inline DS1307_basic<TWI_master>::Time_point DS1307_basic<TWI_master>::default_time()
{
    Time_point res;

    res.date    = 1;
    res.month   = 1;
    res.year    = 0;

    res.hours   = 0;
    res.minutes = 0;
    res.seconds = 0;

    return res;
}

}// namespace

#endif


