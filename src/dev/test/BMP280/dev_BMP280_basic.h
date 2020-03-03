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
#ifndef __DEV_BMP280_BASIC_H__
#define __DEV_BMP280_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del BMP280
 *
 *  - COMENTARIOS: Tanto BOSCH como Adafruit suministran drivers de este
 *	dispositivo. Motivo principal de por qué lo implemento: aprender.
 *  
 *  - IDEA nueva:
 *	La datasheet suministra un montón de registros donde se almacenan
 *	distinta información. Crear estructuras basadas en esos registros. 
 *	Estas estructuras son copias de la datasheet.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    21/01/2020 v0.0
 *
 ****************************************************************************/

#include <stdint.h> // uint16_t ...
#include <avr_TWI.h>
#include <cstddef>  // std::byte
#include <atd_bit.h>


constexpr uint8_t TWI_buffer_size = 100;


namespace avr{


// TODO: quiero que sea
//	write(TWI, q, n); <-- es más estandard!!! TWI sería el flujo!!!
//  en lugar de 
//	write(TWI, slave_addres, q, n);
//  luego le tenemos que pasar el slave de alguna otra forma.
//  1. write(TWI<slave_address>, q, n);
//  2. write(TWI_slave{slave}, q, n);
//  3. ???
template <typename T, uint8_t bsz>
static TWI_master_ioxtream<T, bsz>::streamsize
    write(TWI_master_ioxtream<T, bsz>& twi,
	  typename TWI_master_ioxtream<T, bsz>::Address slave_address,
	  const std::byte* q,
	  typename TWI_master_ioxtream<T, bsz>::streamsize n)
{
    twi.open(slave_address);
     
    return twi.write(q, n); // twi.close() lo llama el destructor
}

}// namespace avr

namespace dev{

// TODO: genérico. Sacarlo fuera (atd_mem???)
enum class Memory_type{
    read = 1,
    write = 2,
    read_only = read,
    write_only = write, 
    read_and_write = read | write
};

inline constexpr bool is_readable(Memory_type mem)
{return mem == Memory_type::read or mem == Memory_type::read_and_write;}

inline constexpr bool is_writeable(Memory_type mem)
{return mem == Memory_type::write or mem == Memory_type::read_and_write;}


// ---------------------------------------------------------------------------
// datasheet 4.3.1
struct __BMP280_id{
// Data
    std::byte id;

    /// The value of this id is valid?
    bool is_valid() const;

// Memory
    static constexpr Memory_type mem_type = Memory_type::read_only;
    static constexpr std::byte address{0xD0};
    static constexpr uint8_t size = 1;	

private:
    // unico posible valor que puede tomar el id
    static constexpr std::byte unique_value{0x58};
};


template <typename Ixtream>
inline Ixtream& operator>>(Ixtream& in, __BMP280_id& st)
{ return in >> st.id; }


inline bool __BMP280_id::is_valid() const
{ return id == unique_value;}


// ---------------------------------------------------------------------------
// datasheet 4.3.2
struct __BMP280_reset{
    static constexpr std::byte reset_cmd {0xB6};

// Memoria donde guardamos el objeto
    static constexpr Memory_type mem_type = Memory_type::write_only;
    static constexpr std::byte address{0xE0};
    static constexpr uint8_t size = 1;

};



template <typename Oxtream>
inline Oxtream& operator<<(Oxtream& out, const __BMP280_reset& st)
{ 
    out << st.reset_cmd;

    return out;
}


// ---------------------------------------------------------------------------
// datasheet: 4.3.3
struct __BMP280_status{
// Data
    bool measuring; // Is there a conversion running?
    bool im_update;


// Memory
    static constexpr Memory_type mem_type = Memory_type::read_only;
    static constexpr std::byte address{0xF3};
    static constexpr uint8_t size = 1;	

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __BMP280_status& st);

private:
    static constexpr atd::Range_bitmask<3, 3, std::byte> mask_measuring{};
    static constexpr atd::Range_bitmask<0, 0, std::byte> mask_im_update{};

    // Codifica la región de memoria 'mem' en la estructura st
    static void mem_to_struct(std::byte* mem, __BMP280_status& st);
};


inline void __BMP280_status::mem_to_struct(std::byte* mem, __BMP280_status& st)
{
    st.measuring = (mask_measuring(mem[0]) == std::byte{0}? false: true);
    st.im_update = (mask_im_update(mem[0]) == std::byte{0}? false: true);
}


template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __BMP280_status& st)
{ 
    using T = __BMP280_status;

    std::byte mem[T::size];
    in.read(mem, T::size);

    T::mem_to_struct(mem, st);

    return in;
}



// ---------------------------------------------------------------------------
// datasheet 4.3.4/5
struct __BMP280_config{
// Data
    // Ctrl_meas
    std::byte osrs_t;	// Controls oversampling temperature.
    std::byte osrs_p;	// Controls oversampling pressure.
    std::byte mode;	// Controls the power mode of the device.

    // Config
    std::byte t_sb;	// Controls inactive duration (t_standby)
    std::byte filter;	// Controls the time constant of IIR filter.
    std::byte spi3w_en;	// Enables 3-wire SPI interface when set to '1'.


// Table 7: recommended filter settings use cases
    static constexpr void handheld_device_low_power(__BMP280_config& cfg);
    static constexpr void handheld_device_dynamic(__BMP280_config& cfg);
    static constexpr void weather_monitoring(__BMP280_config& cfg);
    static constexpr void elevator_floor_change_detector(__BMP280_config& cfg);
    static constexpr void drop_detection(__BMP280_config& cfg);
    static constexpr void indoor_navigation(__BMP280_config& cfg);


// Memory
    static constexpr Memory_type mem_type = Memory_type::read_and_write;
    static constexpr std::byte address{0xF4};
    static constexpr uint8_t size = 2;	

    // Este registro tiene forma específica de ser escrito
    static constexpr std::byte ctrl_meas_address{0xF4};
    static constexpr std::byte config_address{0xF5};

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __BMP280_config& st);

    // Escribe toda la configuración
    template <typename TWI>
    void twi_write(TWI& out, typename TWI::Address slave_address) const;


// Options
// -------
    // Table 21 y 22 tienen los mismos valores para osrs_t y osrs_p
    static constexpr std::byte oversampling_none{0x00};
    static constexpr std::byte oversampling_x1	{0x01};
    static constexpr std::byte oversampling_x2	{0x02};
    static constexpr std::byte oversampling_x4	{0x03};
    static constexpr std::byte oversampling_x8	{0x04};
    static constexpr std::byte oversampling_x16	{0x05};

    // Table 23
    static constexpr std::byte spi3w_disable{0x00};
    static constexpr std::byte spi3w_enable{0x01};

    // Table 10: mode settings
    static constexpr std::byte sleep_mode {0x00};
    static constexpr std::byte force_mode {0x01}; // also 0x10 
    static constexpr std::byte normal_mode{0x03};

    // Table 11: t_sb settings
    // Esto lo llama también la datasheet como ODR = Output Data Rate
    static constexpr std::byte t_sb_0_5_ms	{0x00};
    static constexpr std::byte t_sb_62_5_ms	{0x01};
    static constexpr std::byte t_sb_125_ms	{0x02};
    static constexpr std::byte t_sb_250_ms	{0x03};
    static constexpr std::byte t_sb_500_ms	{0x04};
    static constexpr std::byte t_sb_1000_ms	{0x05};
    static constexpr std::byte t_sb_2000_ms	{0x06};
    static constexpr std::byte t_sb_4000_ms	{0x07};
    
    // Table 6: filter settings <-- NO!!! (creo que no entiendo la tabla)
    // La tabla 6 no corresponde con el código de la biblioteca
    // de Bosch. Uso lo que aparece en el código. 
    static constexpr std::byte filter_off	{0x00};
    static constexpr std::byte filter_coeff_2	{0x01};
    static constexpr std::byte filter_coeff_4	{0x02};
    static constexpr std::byte filter_coeff_8	{0x03};
    static constexpr std::byte filter_coeff_16	{0x04};

private:
    static constexpr atd::Range_bitmask<5, 7, std::byte> mask_osrs_t{};
    static constexpr atd::Range_bitmask<2, 4, std::byte> mask_osrs_p{};
    static constexpr atd::Range_bitmask<0, 1, std::byte> mask_mode{};

    static constexpr atd::Range_bitmask<5, 7, std::byte> mask_t_sb{};
    static constexpr atd::Range_bitmask<2, 4, std::byte> mask_filter{};
    static constexpr atd::Range_bitmask<0, 0, std::byte> mask_spi3w_en{};

//    // Codifica la región de memoria 'mem' en la estructura st
    static void mem_to_struct(std::byte* mem, __BMP280_config& st);
    static void struct_to_mem(const __BMP280_config& st, std::byte* mem);
    static void struct_to_ctrl_meas_only(const __BMP280_config& st,
							    std::byte* mem);
};

// mem -> st
inline void __BMP280_config::mem_to_struct(std::byte* mem, __BMP280_config& st)
{
    st.osrs_t   = mask_osrs_t(mem[0]);
    st.osrs_p   = mask_osrs_p(mem[0]);
    st.mode     = mask_mode(mem[0]);

    st.t_sb     = mask_t_sb(mem[1]);
    st.filter   = mask_filter(mem[1]);
    st.spi3w_en = mask_spi3w_en(mem[1]);
}


inline void 
__BMP280_config::struct_to_ctrl_meas_only(const __BMP280_config& st,
                                                      std::byte* mem)
{
    mask_osrs_t(mem[0])   = st.osrs_t;
    mask_osrs_p(mem[0])   = st.osrs_p;
    mask_mode(mem[0])     = st.mode;
}



// st -> mem
inline void __BMP280_config::struct_to_mem(const __BMP280_config& st, std::byte* mem)
{
    struct_to_ctrl_meas_only(st, mem);

    mask_t_sb(mem[1])     = st.t_sb;
    mask_filter(mem[1])   = st.filter;
    mask_spi3w_en(mem[1]) = st.spi3w_en;
}




template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __BMP280_config& st)
{ 
    using T = __BMP280_config;

    std::byte mem[T::size];
    in.read(mem, T::size);

    T::mem_to_struct(mem, st);

    return in;
}

// Esta función se encarga de gestionar toda la transmisión:
// la abre, envia todos los datos, la cierra. Por eso la llamo twi_
// TODO: ¿cómo generalizarla a que funcione con SPI? Cuando tenga el interfaz
// de SPI lo miramos.
// En caso de error el error queda en el estado del flujo de salida out.
template <typename TWI>
inline void __BMP280_config::twi_write(TWI& out, 
	typename TWI::Address slave_address) const
{ 
    std::byte mem[size];
    struct_to_mem(*this, mem);

// 1. Poner el dispositivo en sleep mode. 
//    El ejemplo de Bosch dice que la forma más rápida de hacerlo es con
//    reset.
    __BMP280_reset reset;
    out.open(slave_address);
    out << reset.address
	<< reset;

    out.close();

    if (out.error()){
	return;
    }

    
// 2. Leer. El ejemplo de Bosch dice que siempre hay que leer antes de
// escribir la cfg.
    std::byte tmp[2];
    out.open(slave_address);
    out << address;

    out.read(2);
    out >> tmp[0] >> tmp[1];

    out.close();

    if (out.error()){
	return;
    }

// 1. Enviar primero enviar config.
//    datasheet (4.3.5): In sleep mode writes are not ignored.
//    Esto es, este registro solo se puede escribir en sleep mode y hay que
//    escribirlo el solito, sin escribir ctrl_meas a la vez. (De hecho,
//    al principio intenté escribirlo a la vez que ctrl_meas y no funcionaba).
    out.open(slave_address);

    out << config_address
	<< mem[1];

    out.close();

    if (out.error())
	return;

// 2. Enviar primero enviar ctrl_meas
    out.open(slave_address);

    out << ctrl_meas_address
	<< mem[0];

    out.close();

}


// Table 7
inline constexpr void __BMP280_config::handheld_device_low_power(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x16;
    cfg.osrs_t = oversampling_x2;
    cfg.filter = filter_coeff_4;
    cfg.t_sb   = t_sb_62_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// Table 7
inline constexpr void __BMP280_config::handheld_device_dynamic(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x4;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_coeff_16;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
inline constexpr void __BMP280_config::weather_monitoring(__BMP280_config& cfg)
{
    cfg.mode =  force_mode;
    cfg.osrs_p = oversampling_x1;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_off;
    // cfg.t_sb = ¿Qué valor poner aqui en force_mode? Se supone que lo ignora
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
inline constexpr void __BMP280_config::elevator_floor_change_detector(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x4;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_coeff_4;
    cfg.t_sb   = t_sb_125_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// Table 7
inline constexpr void __BMP280_config::drop_detection(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x2;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_off;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
inline constexpr void __BMP280_config::indoor_navigation(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x16;
    cfg.osrs_t = oversampling_x2;
    cfg.filter = filter_coeff_16;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}




// ---------------------------------------------------------------------------
// datasheet 4.3.6/7 y 3.9
// According to 3.9: use a burst read to read from 0xF7 to 0xFC.
struct __BMP280_temp_and_press{
    uint32_t pressure;
    int32_t temperature;

// Memory
    static constexpr Memory_type mem_type = Memory_type::read_and_write;
    static constexpr std::byte address {0xF7};
    static constexpr uint8_t size = 6;

    // En caso de que los valores leídos no estén dentro de los límites
    // devuelve 0.
    /// Devuelve los valores de temperatura y presión sin compensar.
    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __BMP280_temp_and_press& st);

private:
    // Codifica la región de memoria 'mem' en la estructura st
    static void mem_to_struct(const std::byte* mem, __BMP280_temp_and_press& st);

    // Mira a ver si los valores de temperatura y presión están dentro de los
    // límites. Si no lo están, los pone a 0 (de esa forma el usuario puede 
    // detectar el error).
    static void make_bounded(__BMP280_temp_and_press& st);

// Límites (estos no los he encontrado en la datasheet, sino en el ejemplo 
// de Bosch)
    static constexpr int32_t temp_min = 0x00000;
    static constexpr int32_t temp_max = 0xFFFF0;

    static constexpr int32_t press_min = 0x00000;
    static constexpr int32_t press_max = 0xFFFF0;
};

inline void __BMP280_temp_and_press::make_bounded(__BMP280_temp_and_press& st)
{
    if (st.temperature <= temp_min || st.temperature >= temp_max)
	st.temperature = 0;

    if (st.pressure <= press_min || st.pressure >= press_max)
	st.pressure = 0;
}


template <typename Ixtream>
Ixtream& operator>>(Ixtream& in, __BMP280_temp_and_press& st)
{ 
    using T = __BMP280_temp_and_press;

    std::byte mem[T::size];

    in.read(mem, T::size);

    T::mem_to_struct(mem, st);

    T::make_bounded(st);

    return in;
}



// ---------------------------------------------------------------------------
struct __BMP280_calibration{
// Types according table 17
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;


    /// Returns temperature in DegC, resolution is 0.01 DegC. 
    /// Output value of “5123” equals 51.23 DegC.  
    int32_t compensate_T(const int32_t& adc_T);

    /// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24
    /// integer bits and 8 fractional bits).
    ///
    /// Output value of “24674867” represents 
    ///			24674867/256 = 96386.2 Pa = 963.862 hPa
    uint32_t compensate_P(const uint32_t& adc_P) const;


// Memory
    static constexpr Memory_type mem_type = Memory_type::read_only;
    static constexpr std::byte address {0x88};
    static constexpr uint8_t size = 24;

    template <typename Ixtream>
    friend Ixtream& operator>>(Ixtream& in, __BMP280_calibration& st);

private:
    // Codifica la región de memoria 'mem' en la estructura st
    // mem -> st
    static void mem_to_struct(std::byte* mem, __BMP280_calibration& st);

    // Carries a fine resolution temperature value over to the pressure
    // compensation formula.
    int32_t t_fine;
};


template <typename Ixtream>
inline Ixtream& operator>>(Ixtream& in, __BMP280_calibration& st)
{ 
    using T = __BMP280_calibration;

    std::byte mem[T::size];

    in.read(mem, T::size);

    T::mem_to_struct(mem, st);

    return in;
}



/*!
 *  \brief  Base para el sensor BMP280. Clase de implementación.
 *
 *  Nos podemos comunicar al sensor via TWI o SPI. Eso da lugar a las clases
 *  BMP280_TWI y BMP280_SPI que son las que el usuario usará.
 */
class BMP280_base{
public:
// Types
    using Id	 = __BMP280_id;
    using Status = __BMP280_status;
    using Config = __BMP280_config;
    using Temp_and_press = __BMP280_temp_and_press;

// Calibration parameters (for debugging purpose)
    uint16_t dig_T1() const {return calibration_.dig_T1;}
    int16_t dig_T2() const {return calibration_.dig_T2;}
    int16_t dig_T3() const {return calibration_.dig_T3;}

    uint16_t dig_P1() const {return calibration_.dig_P1;}
    int16_t dig_P2() const {return calibration_.dig_P2;}
    int16_t dig_P3() const {return calibration_.dig_P3;}
    int16_t dig_P4() const {return calibration_.dig_P4;}
    int16_t dig_P5() const {return calibration_.dig_P5;}
    int16_t dig_P6() const {return calibration_.dig_P6;}
    int16_t dig_P7() const {return calibration_.dig_P7;}
    int16_t dig_P8() const {return calibration_.dig_P8;}
    int16_t dig_P9() const {return calibration_.dig_P9;}

    /// Returns temperature in DegC, resolution is 0.01 DegC. 
    /// Output value of “5123” equals 51.23 DegC.  
    int32_t compensate_T(const int32_t& adc_T)
    {return calibration_.compensate_T(adc_T);}

    /// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24
    /// integer bits and 8 fractional bits).
    ///
    /// Output value of “24674867” represents 
    ///			24674867/256 = 96386.2 Pa = 963.862 hPa
    /// Llamar primero a compensate_T y luego compensate_P <--- TODO: ordenar.
    uint32_t compensate_P(const uint32_t& adc_P) const
    {return calibration_.compensate_P(adc_P);}

protected:
// Types
    using Calibration = __BMP280_calibration;

// Construcción: esta clase es de implementación.
    BMP280_base() {}

    // No podemos inicializar el sensor hasta no haber configurado
    // SPI o TWI. Por eso no puedo hacer el init en el constructor.
    // Devuelve 0 si todo va bien.
    uint8_t init() { return 0;}

// Data
    Calibration calibration_;

};




// TODO: convertirla en template, para independizarlo del avr.
class BMP280_TWI : public BMP280_base {
public:
    using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;

    // TODO: esta es una función genérica: lee una zona de memoria en el
    // device conectado via TWI y la devuelve codificada en la estructura T
    // correspondiente. La clase T contiene toda la información de cómo es
    // almacenada en el device.
    // Params: [out] st: objeto leido.
    // Return value: On success 0, on error TWI::error.
    // TODO: funcion genérica, sacarla fuera (meterla en TWI???)
    template <typename T>
    static TWI::iostate read_object(T& st)
    {
	static_assert (is_readable(T::mem_type));
    
	TWI twi;
	twi.open(slave_address);
	 
	twi << T::address;

	if (twi.error())
	    return TWI::state();

	twi.read(T::size);

	twi >> st;

	twi.close();

	return TWI::state();
    }


    // TODO: funcion genérica, sacarla fuera (meterla en TWI???)
    template <typename T>
    static TWI::iostate write_object(const T& st)
    {
	static_assert (is_writeable(T::mem_type));

	TWI twi;
	twi.open(slave_address);
	

	twi << T::address << st;

	twi.close();

	return TWI::state();
	
    }



// CONSTRUCTION
    uint8_t init();

    /// Read the device id.
    // TODO: devolver el iostate o memorizarlo mejor y devolver el número
    // leido. Observar que si en la aplicación hay más dispositivos TWI si
    // alguien más llama a TWI se perderá el estado. d
    void read(Id& id) {state_ = read_object(id);}

    /// The device is reset using the complete power-on-reset procedure.
    void reset() { state_ = write_object(__BMP280_reset{});}

    /// Read the status register.
    /// out: Status&
    void read(Status& res) {state_ = read_object(res);}

    void read(Config& res) {state_ = read_object(res);}
    void write(Config& cfg);

    /// Reads temperature in deg
    //TODO: read_T_in_dC();

    void read(Temp_and_press& res) {state_ = read_object(res);}
    

// states
    bool good() const {return !error();}
    bool error() const {return TWI::error(state_);}

    bool no_response() const {return state_ == TWI::iostate::no_response;}

    // para depurar
    TWI::iostate state() const {return state_;}

private:
    TWI::iostate state_;

    // TODO: el último bit se puede elegir:
    // Connecting SDO to GND results in slave address 1110110 (0x76);
    // connection it to V_DDIO results in 1110111 (0x77).
    static constexpr TWI::Address slave_address = 0x76;


    // Returns: On success, 0.
    //		On error, != 0.
    void read_calibration_params() {state_ = read_object(calibration_);}

};


inline void BMP280_TWI::write(Config& cfg)
{
    TWI twi;
    cfg.twi_write(twi, slave_address);
}

}// namespace

#endif






