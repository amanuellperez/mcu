// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *    08/04/2020 v0.0
 *
 ****************************************************************************/

#include <stdint.h> // uint8_t...
#include <atd_memory.h>	// Memory_type
#include <atd_bcd.h>
#include <avr_TWI.h>
#include <atd_bit.h>

namespace dev{

// Table 2: only time.
// ¿Por qué no usar time_t para guardar los datos?
// Porque este es un traductor. La idea es que se pueda escribir leyendo
// literalmente la datasheet, de esa forma habrá menos errores de traducción.
struct __DS1307_clock{
// Datos
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;

// Configuración del reloj
    bool clock_on = true;// = not CH; true = working; false = clock halted.

// Formato de la hora
    bool AMPM_format = false;    // false = 24 hours format; true = AM/PM format
    bool is_PM; // si AMPM_format == true, indica si es PM o no (en cuyo caso sería
                // AM). Si AMPM_format == false su valor no está definido.

    // Memory
    static constexpr atd::Memory_type mem_type =
					      atd::Memory_type::read_and_write;
    static constexpr std::byte address {0x00};
    static constexpr uint8_t size = 7;

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __DS1307_clock& st);

    template <typename Oxtream>
    friend Oxtream& operator<<(Oxtream& out, const __DS1307_clock& st);


private:
    static constexpr atd::Range_bitmask<0, 6, uint8_t> mask_seconds{};
    static constexpr atd::Range_bitmask<7, 7, uint8_t> mask_CH{};
    static constexpr atd::Range_bitmask<6, 6, uint8_t> mask_AMPM_mode{};
    // Formato de las horas (AM/PM)
    static constexpr atd::Range_bitmask<0, 4, uint8_t> mask_AMPM_hours{};
    static constexpr atd::Range_bitmask<5, 5, uint8_t> mask_AMPM_PM{};


    static void mem_to_struct(__DS1307_clock& st);
    static __DS1307_clock struct_to_mem(__DS1307_clock st0);
};



template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __DS1307_clock& st)
{
    using Clock = __DS1307_clock;

    in >> st.seconds;
    in >> st.minutes;
    in >> st.hours;
    in >> st.day;
    in >> st.date;
    in >> st.month;
    in >> st.year;

    Clock::mem_to_struct(st);

    return in;
}


template <typename Oxtream>
Oxtream& operator<<(Oxtream& out, const __DS1307_clock& st0)
{
    using Clock = __DS1307_clock;

    Clock st = Clock::struct_to_mem(st0);

    out << st.seconds;
    out << st.minutes;
    out << st.hours;
    out << st.day;
    out << st.date;
    out << st.month;
    out << st.year;

    return out;
}


/*!
 *  \brief  Driver básico del DS1307.
 *
 *  Errores: el reloj tiene un state donde se anotará el resultado de la
 *  última operación. 
 *
 */
template <typename TWI_master>
class DS1307{
public:
// Configuración de TWI
    using Address = typename TWI_master::Address;
    static constexpr Address slave_address = 0x68;	// es fija.
    static constexpr uint8_t TWI_buffer_size = 70; // tiene 64 bytes de memoria

    static_assert(TWI_master::buffer_size >= TWI_buffer_size);

// Types
    using TWI   = avr::TWI_master_memory_type<slave_address, TWI_master>;
    using State = TWI::iostate;
    using Clock  = __DS1307_clock;


// Operaciones
    /// Lectura del reloj
    void read(Clock& t) {state_ = TWI::read(t);}

    /// Escritura en el reloj.
    void write(const Clock& t) { state_ = TWI::write(t); }


// States
    bool good() const {return !error();}
    bool error() const {return TWI_master::error(state_);}

    bool no_response() const {return state_ == TWI::iostate::no_response;}

    // para depurar
    State state() const {return state_;}

private:
    State state_;

};

}// namespace

#endif


