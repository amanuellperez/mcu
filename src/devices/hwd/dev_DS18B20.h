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

#ifndef __DEV_DS18B20_H__
#define __DEV_DS18B20_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Sensor de temperatura DS18B20, de Maxim
 *
 * REFERENCES
 *    DS18B20 datasheet
 *
 * TODO 
 *    No admite el parasite power mode. Hay que conectarlo a VCC
 *
 * HISTORIA
 *    Manuel Perez
 *    29/08/2023 Escrito
 *
 ****************************************************************************/
#include <stdint.h> 
#include <atd_crc.h>
#include <atd_minifloat.h>
#include <atd_magnitude.h>
#include <atd_bit.h>

namespace dev{

/***************************************************************************
 *			TRADUCTOR DEL DS18B20
 ***************************************************************************/
// La Scratchpad es:
//  + desde un punto de vista de hardware un array de bytes: data[8]
//  + desde el punto de vista del programador una estructura.
//
// Esta clase es responsable de pasar del array de bytes a la estructura y
// viceversa.
struct _Scratchpad{
    // Resolución: parte entera de 3 dígitos máximo (mide de -55ºC a +125ºC);
    // parte decimal 4 decimales (con 12 bits mide 0.0625). Total 7 dígitos.
    // Entran en int32_t.
    // TODO: al usar el DS18B20 en el datalogger, aumenta el datalogger 1KB de
    // pasar de Celsius<Float16> a Celsius<Float32>. ¡ES MUCHO!!!
    // Con todo Minifloat y Magnitude son experimentales. Si funciona
    // convendría revisarlas para ver si se pueden simplificar.
    using Celsius  = atd::Celsius<atd::Float32>;

    enum class Resolution{ bits_9, bits_10, bits_11, bits_12 };

// Data como struct
    Celsius temperature() const;
    int8_t TH() const {return static_cast<int8_t>(data[2]);}
    int8_t TL() const {return static_cast<int8_t>(data[3]);}
    Resolution resolution() const;

    uint8_t CRC() const {return data[8];}
    bool is_CRC_ok() const;

// Data como array
    uint8_t& operator[](uint8_t i) { return data[i]; }
    uint8_t operator[](uint8_t i) const { return data[i]; }
    static constexpr uint8_t size() { return size_;}

// Interfaz static
    static Celsius temperature(uint8_t T0, uint8_t T1, Resolution res);
    static uint8_t to_cfg_register(Resolution res);

private:
// Data
    static constexpr uint8_t size_ = 9;
    uint8_t data[size_];

// Configuration register
    static constexpr atd::Range_bitmask<5, 6, uint8_t> mask_cfg{};

// Calculo de T
    static int32_t decimal_part_of_T(uint8_t T0, Resolution res);
};


inline _Scratchpad::Celsius _Scratchpad::temperature() const 
{return temperature(data[0], data[1], resolution());}

inline bool _Scratchpad::is_CRC_ok() const
{ return (data[8] == atd::CRC8_Maxim(data, 8)); }



// SIGNIFICADO DE LA TEMPLATE
//     DS18B20<Atmega, One_wire> = Es el sensor DS18B20 conectado al 
//     micro Atmega a través de One_wire.
// 
// OBSERVACIONES
//    + Podríamos ahorrarnos pasar `Micro` como parámetro ya que la
//      implementación de `One_wire` lo contiene. Pero quiero hacer lo más
//      genérica posible esta clase. Que se pueda usar sin necesidad de usar
//      mi implementación de One_wire.
//
//    + one-wire es el medio de comunicación entre el micro y
//      el sensor. El medio podría ser otro: TWI, SPI, UART, ...
template <typename Micro0, typename One_wire0>
class DS18B20_basic{
public:
// Types
    using Micro    = Micro0;
    using One_wire = One_wire0;
    using Device   = typename One_wire::Device;

// CFG
    using Scratchpad = _Scratchpad;
    using Resolution = Scratchpad::Resolution;
    using Celsius    = Scratchpad::Celsius;

    enum class Result{
	ok = 0,	    // todo bien
	not_found,  // falla la llamada de One_wire::reset()
	time_out,
	wrong_CRC
    };


// One wire
// --------
    void bind(const Device& dev);

// Function commands
// -----------------
// La descripción de lo que hace cada comando se puede encontrar en la
// datasheet.
    // Para esperar a que acabe de realizar la conversión llamar a
    // `wait_until(time_out)
    Result convert_T() const;

    Result write_scratchpad(int8_t TH, int8_t TL, Resolution res) const;

    // Lee la scratchpad guardándola en el array pasado
    // DUDA: Necesito pasarlo como referencia? No debiera ser necesario ya que
    // Scratchpad es un array, luego es un puntero.
    Result read_scratchpad(Scratchpad& s) const;

    Result copy_scratchpad() const;

    // Para esperar a que acabe de realizar la operación llamar a
    // `wait_until(time_out)
    Result recall_e2() const;

    // CUIDADO: esta función no habla con este sensor concreto sino con todos
    // los DS18B20_basic, preguntándo si alguno de los sensores DS18B20_basic está
    // usando parasite power.
    // TODO
    // static Result read_power_supply();
    

    // TODO: nombre mal puesto. Esta función lo que hace es esperar a que el
    // DS18B20_basic haya acabado de ejecutar el comando solicitado. Si lo acaba
    // antes de `time_out_ms` milisegundos devuelve Result::ok. Si no lo acaba
    // devuelve Result::time_out. ¿cómo llamar a esta función?
    // Función a llamar despues de convert_T o recall_e2 para ver si el sensor
    // ha acabado la operación solicitada.
    Result wait_until(const uint16_t& time_out_ms) const;

private:
// Data
    Device dev_;

// Helper functions
    bool send_command(uint8_t cmd) const;

};

template <typename M, typename OW>
inline void DS18B20_basic<M,OW>::bind(const Device& dev)
{ dev_ = dev; }


template <typename M, typename OW>
bool DS18B20_basic<M,OW>::send_command(uint8_t cmd) const
{
    if (!One_wire::reset())
	return false;

    One_wire::match_rom(dev_);
    One_wire::write(cmd);

    return true;
}


template <typename M, typename OW>
DS18B20_basic<M,OW>::Result 
	    //DS18B20_basic<M,OW>::convert_T(const uint16_t& time_out_ms) const
	    DS18B20_basic<M,OW>::convert_T() const
{
    if (!send_command(0x44))
	return Result::not_found;
    
    return Result::ok;
//    return wait_until(time_out_ms);
}


template <typename M, typename OW>
DS18B20_basic<M,OW>::Result DS18B20_basic<M,OW>::
	write_scratchpad(int8_t TH, int8_t TL, Resolution res) const
{
    if (!send_command(0x4E))
	return Result::not_found;

    One_wire::write(static_cast<uint8_t>(TH));
    One_wire::write(static_cast<uint8_t>(TL));
    One_wire::write(Scratchpad::to_cfg_register(res));

    return Result::ok;
}

template <typename M, typename OW>
DS18B20_basic<M,OW>::Result
DS18B20_basic<M,OW>::read_scratchpad(Scratchpad& scratchpad) const
{
    if (!send_command(0xBE))
	return Result::not_found;

    for (uint8_t i = 0; i < Scratchpad::size(); ++i)
	scratchpad[i] = One_wire::read();

    return Result::ok;
}


template <typename M, typename OW>
DS18B20_basic<M,OW>::Result DS18B20_basic<M,OW>::copy_scratchpad() const
{
    if (!send_command(0x48))
	return Result::not_found;

    return Result::ok;
}


template <typename M, typename OW>
DS18B20_basic<M,OW>::Result 
DS18B20_basic<M,OW>::recall_e2() const
{
    if (!send_command(0xB8))
	return Result::not_found;

    return Result::ok;
//    return wait_until(time_out_ms);
}

template <typename M, typename OW>
DS18B20_basic<M,OW>::Result 
DS18B20_basic<M,OW>::wait_until(const uint16_t& time_out_ms) const
{
    if (time_out_ms == 0)
	return Result::time_out;

    uint16_t t = 0;

    // Esperamos a que haga la conversión antes de devolver control
    while (One_wire::read_bit() == 0){// == 0 means 'in progress'
	Micro::wait_ms(1);
	++t;
	if (t > time_out_ms)
	    return Result::time_out;
    }

    return Result::ok;
}

/***************************************************************************
 *			    DS18B20 DRIVER
 ***************************************************************************/
// (RRR) ¿por qué tengo que definir el tipo `Base`?
//       Si no lo defino al llamar a las funciones de Base dentro de esta
//       clase derivada el compilador no las va a encontrar, ya que lo que
//       busca son las funciones en el global namespace. 
//       Para verlo probar a quitar el `Base::` de las llamadas a función
//       dentro de read_temperature. 
//       Otra forma de hacerlo, para no tener que usar Base es llamar a las
//       funciones como `this->convert_T`. De esa forma el compilador sabe que
//       no nos referimos a una función global. 
//       Personalmente me gusta mas el `Base` (hoy, mañana...)
template <typename Micro0, typename One_wire0>
class DS18B20 : public DS18B20_basic<Micro0, One_wire0>{
public:
// Types
    using Base     = DS18B20_basic<Micro0, One_wire0>;

    using Micro    = Micro0;
    using One_wire = One_wire0;
    using Device   = typename One_wire::Device;
    using Result    = Base::Result;

// CFG
    using Scratchpad = _Scratchpad;
    using Resolution = Scratchpad::Resolution;
    using Celsius    = Scratchpad::Celsius;

// Interfaz unificado para sensores
    // Leemos la temperatura. Esta función no devuelve el control hasta que:
    //	1) El sensor devuelve una temperatura válida
    //	2) Han transcurrido time_out_ms milisegundos
    Celsius read_temperature(const uint16_t& time_out_ms);

// Gestión de errores
    // Devuelve el resulta_las_operation de la ultima función llamada 
    Result result_last_operation() const {return result_;}
    bool last_operation_is_ok() const { return result_ == Result::ok;}
    bool last_operation_fail() const { return !last_operation_is_ok();}

private:
    Result result_;
};


template <typename M, typename OW>
DS18B20<M, OW>::Celsius 
DS18B20<M, OW>::read_temperature(const uint16_t& time_out_ms)
{
    result_ = Base::convert_T();
    if (result_ != Result::ok)
	return Celsius{-1};

    result_ = Base::wait_until(time_out_ms);

    Scratchpad s;
    result_ = Base::read_scratchpad(s);
    if (result_ != Result::ok)
	return Celsius{-1};

    if (!s.is_CRC_ok()){
	result_ = Result::wrong_CRC;
	return Celsius{-1};
    }
    
    return s.temperature();
}



}// namespace

#endif


