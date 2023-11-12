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

#ifndef __DEV_BME280_BASIC_H__
#define __DEV_BME280_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del BME280
 *
 *  - COMENTARIOS: Tanto BOSCH como Adafruit suministran drivers de este
 *	dispositivo. Motivo principal de por qué lo implemento: aprender.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    10/12/2020 v0.0
 *
 ****************************************************************************/
#include <atd_bit.h>
#include <atd_decimal.h>
#include <atd_magnitude.h>
#include "dev_TWI_memory_type.h"

namespace dev{

namespace impl_of{
// ---------------------------------------------------------------------------
// datasheet 5.4.1
struct BME280_id{
// Data
    uint8_t id;

    /// The value of this id is valid?
    bool is_valid() const;

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address{0xD0};
    static constexpr uint8_t size = 1;	

    static constexpr bool use_struct_as_mem = true;

private:
    // unico posible valor que puede tomar el id
    static constexpr uint8_t chip_identification_number{0x60};
};


template <typename Ixtream>
inline Ixtream& operator>>(Ixtream& in, BME280_id& st)
{ return in >> st.id; }


inline bool BME280_id::is_valid() const
{ return id == chip_identification_number;}



// ---------------------------------------------------------------------------
// datasheet 5.4.2
struct BME280_reset{
    static constexpr uint8_t reset_cmd {0xB6};

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::write_only;
    static constexpr uint8_t address{0xE0};
    static constexpr uint8_t size = 1;

    static constexpr bool use_struct_as_mem = true;
};



template <typename Oxtream>
inline Oxtream& operator<<(Oxtream& out, const BME280_reset& st)
{ 
    out << st.reset_cmd;

    return out;
}



// ---------------------------------------------------------------------------
// datasheet: 5.4.4
struct BME280_status{
// Data
    bool measuring; // Is there a conversion running?
    bool im_update;

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address{0xF3};
    static constexpr uint8_t size = 1;	

// mem <-> struct
    static void mem_to_struct(const std::array<uint8_t, size>& mem
				, BME280_status& st);

private:
    static constexpr atd::Range_bitmask<3, 3, uint8_t> mask_measuring{};
    static constexpr atd::Range_bitmask<0, 0, uint8_t> mask_im_update{};

};


inline void
BME280_status::mem_to_struct(const std::array<uint8_t, size>& mem,
                               BME280_status& st)
{
    st.measuring = atd::to_bool(mask_measuring(mem[0]));
    st.im_update = atd::to_bool(mask_im_update(mem[0]));
}



// ---------------------------------------------------------------------------
// datasheet 5.4.5/6
struct BME280_config{
// Data
    // ctrl_hum
    uint8_t osrs_h;	// Controls oversampling of humidity.

    // ctrl_meas
    uint8_t osrs_t;	// Controls oversampling temperature.
    uint8_t osrs_p;	// Controls oversampling pressure.
    uint8_t mode;	// Controls the power mode of the device.

    // config
    uint8_t t_sb;	// Controls inactive duration (t_standby)
    uint8_t filter;	// Controls the time constant of IIR filter.
    uint8_t spi3w_en;	// Enables 3-wire SPI interface when set to '1'.


// 3.5: recommended modes of operation
    static void weather_monitoring(BME280_config& cfg);
    static void humidity_sensing(BME280_config& cfg);
    static void indoor_navigation(BME280_config& cfg);
    static void gaming(BME280_config& cfg);


// Memory. Al no ser continua la memoria (en medio está el registro 'status'
// que es de solo lectura y no pertenece a la configuración) no podemos usar 
// las funciones por defecto. Usamos twi_write particular.
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_and_write;
    static constexpr uint8_t address{0xF2}; 
    static constexpr uint8_t size = 4;

    // Este registro tiene forma específica de ser escrito
    static constexpr uint8_t ctrl_hum_address	{0xF2};
    static constexpr uint8_t ctrl_meas_address{0xF4};
    static constexpr uint8_t config_address	{0xF5};

    // posiciones dentro de la memoria de cada registro
    static constexpr uint8_t i_ctrl_hum   = 0;
    static constexpr uint8_t i_status     = 1;
    static constexpr uint8_t i_ctrl_meas  = 2;
    static constexpr uint8_t i_config     = 3;


    //  mem <-> struct
    static void mem_to_struct(const std::array<uint8_t, size>& mem,
					BME280_config& st);
    static void struct_to_mem(const BME280_config& st, uint8_t* mem);


    // Escribe toda la configuración. La configuración hay que escribirla
    // de una determinada forma: hay que hacer reset, leer... luego no vale
    // un simple write. Usar esta función para ello.
    template <typename TWI, typename TWI::Address slave_address>
    void twi_write() const;


// Options
// Tables 20, 23 and 24 have the same values for osrs_h, osrs_t and osrs_p.
    static constexpr uint8_t oversampling_none{0x00};
    static constexpr uint8_t oversampling_x1	{0x01};
    static constexpr uint8_t oversampling_x2	{0x02};
    static constexpr uint8_t oversampling_x4	{0x03};
    static constexpr uint8_t oversampling_x8	{0x04};
    static constexpr uint8_t oversampling_x16	{0x05};

// 6.3
    static constexpr uint8_t spi3w_disable{0x00};
    static constexpr uint8_t spi3w_enable{0x01};

// Table 25: mode settings
    static constexpr uint8_t sleep_mode {0x00};
    static constexpr uint8_t force_mode {0x01}; // also 0x10 
    static constexpr uint8_t normal_mode{0x03};

// Table 27: t_sb settings
    // Esto lo llama también la datasheet como ODR = Output Data Rate
    static constexpr uint8_t t_sb_0_5_ms	{0x00};
    static constexpr uint8_t t_sb_62_5_ms	{0x01};
    static constexpr uint8_t t_sb_125_ms	{0x02};
    static constexpr uint8_t t_sb_250_ms	{0x03};
    static constexpr uint8_t t_sb_500_ms	{0x04};
    static constexpr uint8_t t_sb_1000_ms	{0x05};
    static constexpr uint8_t t_sb_2000_ms	{0x06};
    static constexpr uint8_t t_sb_4000_ms	{0x07};
    
// Table 28: filter settings
    static constexpr uint8_t filter_off	{0x00};
    static constexpr uint8_t filter_coeff_2	{0x01};
    static constexpr uint8_t filter_coeff_4	{0x02};
    static constexpr uint8_t filter_coeff_8	{0x03};
    static constexpr uint8_t filter_coeff_16	{0x04};

private:
    static constexpr atd::Range_bitmask<0, 2, uint8_t> mask_osrs_h{};

    static constexpr atd::Range_bitmask<5, 7, uint8_t> mask_osrs_t{};
    static constexpr atd::Range_bitmask<2, 4, uint8_t> mask_osrs_p{};
    static constexpr atd::Range_bitmask<0, 1, uint8_t> mask_mode{};

    static constexpr atd::Range_bitmask<5, 7, uint8_t> mask_t_sb{};
    static constexpr atd::Range_bitmask<2, 4, uint8_t> mask_filter{};
    static constexpr atd::Range_bitmask<0, 0, uint8_t> mask_spi3w_en{};

};



// Esta función se encarga de gestionar toda la transmisión:
// la abre, envia todos los datos, la cierra. Por eso la llamo twi_
// TODO: ¿cómo generalizarla a que funcione con SPI? Cuando tenga el interfaz
// de SPI lo miramos.
// En caso de error el error queda en el estado del flujo de salida out.
template <typename TWI_master, typename TWI_master::Address slave_address>
void BME280_config::twi_write() const
{ 
    TWI_memory_type<TWI_master, slave_address> twi_mem;

    uint8_t mem[size];
    struct_to_mem(*this, mem);

// 1. Poner el dispositivo en sleep mode. 
//    El ejemplo de Bosch dice que la forma más rápida de hacerlo es con
//    reset.
    twi_mem.write(BME280_reset{});

    if (TWI_master::error())
	return;

    
// 2. Leer. El ejemplo de Bosch dice que siempre hay que leer antes de
// escribir la cfg. ¿Es realmente necesario?
    uint8_t tmp[size];

    twi_mem.template mem_read<address, size>(tmp);

    if (TWI_master::error())
	return;

// 3. Enviar primero enviar config.
//    datasheet (4.3.5): In sleep mode writes are not ignored.
//    Esto es, este registro solo se puede escribir en sleep mode y hay que
//    escribirlo el solito, sin escribir ctrl_meas a la vez. (De hecho,
//    al principio intenté escribirlo a la vez que ctrl_meas y no funcionaba).
    twi_mem.template mem_write<config_address, 1>(&(mem[i_config]));

    if (TWI_master::error())
	return;

// 4. Enviar ctrl_hum. En 5.4.3 indica que para que el cambio sea efectivo hay
//    que escribir luego el ctrl_meas. 
    twi_mem.template mem_write<ctrl_hum_address, 1>(&(mem[i_ctrl_hum]));


// 5. Enviar luego ctrl_meas
    twi_mem.template mem_write<ctrl_meas_address, 1>(&(mem[i_ctrl_meas]));

}

// Para depurar
bool operator==(const BME280_config& a, const BME280_config& b);

inline 
bool operator!=(const BME280_config& a, const BME280_config& b)
{ return !(a == b); }



// ---------------------------------------------------------------------------
// datasheet 5.4.7-9
// According to 4: use a burst read to read from 0xF7 to 0xFC.
struct BME280_temp_and_press_and_hum{
    uint32_t upressure;	    // uncompensated pressure
    uint32_t utemperature;  // uncompensated temperature
    uint32_t uhumidity;	    // uncompensated humidity

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address {0xF7};
    static constexpr uint8_t size = 8;

// mem <-> struct
/// Devuelve los valores de temperatura, presión y humedad sin compensar.
// En caso de que los valores leídos no estén dentro de los límites
// devuelve todo 0.
    static void mem_to_struct(const std::array<uint8_t, size>& mem, 
						BME280_temp_and_press_and_hum& st);

};



// ---------------------------------------------------------------------------
struct BME280_calibration{
// Data. Types according table 16
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

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t  dig_H6;

// Memory. Método particular twi_read de lectura.
    static constexpr uint8_t temp_and_press_address {0x88};
    static constexpr uint8_t temp_and_press_size = 26;

    static constexpr uint8_t hum_address {0xE1};
    static constexpr uint8_t hum_size = 7;

    // Al ser discontinua la memoria tenemos que leer
    template <typename TWI_master, typename TWI_master::Address slave_address>
    void read();


// Unidades en las que medimos la temperatura y la presión.
    using Celsius	   = atd::Celsius<atd::Decimal<int32_t, 2>>;
    using Pascal           = atd::Pascal<atd::Decimal<int32_t, 2>>;
    using Hectopascal      = atd::Hectopascal<atd::Decimal<int32_t, 2>>;
    using Relative_humidity= atd::Decimal<uint32_t, 2>;

// Compensate functions
    /// Returns temperature in DegC, resolution is 0.01 DegC. 
    Celsius compensate_T(const uint32_t& adc_T);

    /// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24
    /// integer bits and 8 fractional bits).
    ///
    /// Output value of “24674867” represents 
    ///			24674867/256 = 96386.2 Pa = 963.862 hPa
    Pascal compensate_P(const uint32_t& adc_P) const{
	int32_t pc = compensate_P_(adc_P)/256;
	using Decimal = Pascal::Rep;
	return Pascal{Decimal::significand(pc)};
    }

    int32_t compensate_P_(const int32_t& adc_P) const;

    /// Returns humidity
    Relative_humidity compensate_H(const uint32_t& adc_H) const;

private:
    // Carries a fine resolution temperature value over to the pressure
    // compensation formula.
    int32_t t_fine;

    void mem_to_temp_and_press(const uint8_t* mem);
    void mem_to_hum(const uint8_t* mem);


    // Mira a ver si los valores de temperatura y presión están dentro de los
    // límites. Si no lo están, los pone a 0 (de esa forma el usuario puede 
    // detectar el error).
//    static void make_bounded(BME280_temp_and_press_and_hum& st);

// Límites (estos no los he encontrado en la datasheet, sino en el ejemplo 
// de Bosch). Valores ya compensados
//    static constexpr int32_t temp_min = -4000;	// -40.00
//    static constexpr int32_t temp_max = 8500;	// +85.00
//
//    static constexpr uint32_t press_min = 30000;
//    static constexpr uint32_t press_max = 110000;
//
//    static constexpr uint32_t humidity_min = 0;
//    static constexpr uint32_t humidity_max = 102400;

};



template <typename TWI_master, typename TWI_master::Address slave_address>
void BME280_calibration::read()
{
    TWI_memory_type<TWI_master, slave_address> twi_mem;

    uint8_t mem[std::max(temp_and_press_size, hum_size)];

// primer bloque: el código de Bosch lee también el registro 0xA0.
    twi_mem.template mem_read<temp_and_press_address, temp_and_press_size>(mem);

    if (TWI_master::error())
	return;
    
    mem_to_temp_and_press(mem);


// segundo bloque
    twi_mem.template mem_read<hum_address, hum_size>(mem);

    if (TWI_master::error())
	return;
    
    mem_to_hum(mem);

}

}// namespace impl_of


/*!
 *  \brief  Base para el sensor BME280. Clase de implementación.
 *
 *  Nos podemos comunicar al sensor via TWI o SPI. Eso da lugar a las clases
 *  BME280_TWI y BME280_SPI que son las que el usuario usará.
 */
class BME280_base{
public:
// Types
    using Id               = impl_of::BME280_id;
    using Status           = impl_of::BME280_status;
    using Config           = impl_of::BME280_config;
    using Temp_and_press_and_hum   = impl_of::BME280_temp_and_press_and_hum;
    using Calibration      = impl_of::BME280_calibration;

    using Celsius           = Calibration::Celsius;
    using Pascal            = Calibration::Pascal;
    using Hectopascal       = Calibration::Hectopascal;
    using Relative_humidity = Calibration::Relative_humidity;


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

    uint8_t dig_H1() const {return calibration_.dig_H1;}
    int16_t dig_H2() const {return calibration_.dig_H2;}
    uint8_t dig_H3() const {return calibration_.dig_H3;}
    int16_t dig_H4() const {return calibration_.dig_H4;}
    int16_t dig_H5() const {return calibration_.dig_H5;}
    int8_t  dig_H6() const {return calibration_.dig_H6;}

protected:
// Data
    Calibration calibration_;

// Constructor
    // Esta clase es de implementación.
    BME280_base() {}

    // No podemos inicializar el sensor hasta no haber configurado
    // SPI o TWI. Por eso no puedo hacer el init en el constructor.
    void init() { }

// Reading T and P
    // Returns temperature in Celsius.
    Celsius compensate_T(const uint32_t& adc_T)
    {return calibration_.compensate_T(adc_T);}

    // Returns pressure in Pascals.
    Pascal compensate_P(const uint32_t& adc_P) const
    {return calibration_.compensate_P(adc_P);}

    // Returns humidity
    Relative_humidity compensate_H(const uint32_t& adc_H) const
    {return calibration_.compensate_H(adc_H);}
};


// (RRR) Gestión de errores:
//	 Voy a usar el equivalente a errno local a través de un state.
//	 Todas las operaciones que puedan fallar modificarán el state,
//	 quedando almacenado ahí el exito/fracaso de la operación.
template <typename TWI_master0, typename TWI_master0::Address slave_address0>
class BME280_TWI : public BME280_base {
public:
    static constexpr uint8_t TWI_buffer_size = 100; // TODO: ajustarlo al mínimo?

    // Conexión TWI
    using TWI_master = TWI_master0; // conexión TWI

    static_assert(TWI_buffer_size <= TWI_master::buffer_size,
                  "Buffer too small!!! Choose a bigger one.");


    // Slave address: el último bit se puede elegir:
    // Connecting SDO to GND results in slave address 1110110 (0x76);
    // connection it to V_DDIO results in 1110111 (0x77).
    static constexpr TWI_master::Address slave_address = slave_address0;
    static_assert(slave_address == 0x76 or slave_address == 0x77,
	    "Wrong BME280 address. Available only: 0x76 and 0x77");

    using TWI_mem = TWI_memory_type<TWI_master, slave_address>;

    using State = TWI_master::iostate;

// CONSTRUCTION
    BME280_TWI() {init();}


// Modos de funcionamiento propuestos por BOSCH
    void weather_monitoring()
    { write_cfg_(Config::weather_monitoring); }

    void humidity_sensing()
    { write_cfg_(Config::humidity_sensing); }

    void indoor_navigation()
    { write_cfg_(Config::indoor_navigation); }

    void gaming()
    { write_cfg_(Config::gaming); }

// Funciones de bajo nivel
// -----------------------
    /// Read the device id.
    void read(Id& id) {state_ = TWI_mem::read(id);}

    /// The device is reset using the complete power-on-reset procedure.
    void reset() { state_ = TWI_mem::write(impl_of::BME280_reset{});}

    /// Read the status register.
    void read(Status& res) {state_ = TWI_mem::read(res);}

    void read(Config& res) {state_ = TWI_mem::read(res);}
    void write(Config& cfg)
    {
	cfg.twi_write<TWI_master, slave_address>();
	state_ = TWI_master::state();
    }

    void read(Temp_and_press_and_hum& res) {state_ = TWI_mem::read(res);}


    /// Returns temperature and pressure.
    void T_and_P_and_H(Celsius& T, Pascal& P, Relative_humidity& H)
    {
	Temp_and_press_and_hum tp;
	read(tp);
	T = compensate_T(tp.utemperature);
	P = compensate_P(tp.upressure);
	H = compensate_H(tp.uhumidity);
    }

    /// Returns temperature and pressure (in hectopascals)
    std::pair<Celsius, Hectopascal> T_and_hP()
    {
	Celsius T;
	Pascal P;
	Relative_humidity H;

	T_and_P_and_H(T, P, H);
	return {T, Hectopascal{P}};
    }

    /// Returns only the temperature.
    Celsius T()
    {
	Celsius T;
	Pascal P;
	Relative_humidity H;

	T_and_P_and_H(T, P, H);

	return T;
    }

    // No suministro P().
    // (RRR) Para medir la presión es necesario medir siempre la temperatura
    // primero. Al no suministrar P() pero sí T_and_P() el cliente de la clase
    // está obligado a calcular la T siempre que quiera calcular P. De esta
    // forma evito código del tipo:
    //		T = sensor.T();
    //		P = sensor.P();	// Volvería a leer la T!!! Innecesario.
    // Pascal P();

// states
    bool good() const {return !error();}
    bool error() const {return TWI_master::error(state_);}

    bool no_response() const {return state_ == TWI_master::iostate::no_response;}

    // para depurar
    State state() const {return state_;}

private:
// Data
    State state_;

// Functions
    // Inicializa el sensor.
    // Verifica que haya comunicación con el sensor y que el sensor conectado
    // sea realmente un BME280. Para ello comprueba que el id sea el
    // correspondiente.
    void init();

    void read_calibration_params() 
    {
	calibration_.read<TWI_master, slave_address>();
	state_ = TWI_master::state();
    }

    void write_cfg_(void f(Config&));
};


template <typename TWI, typename TWI::Address sa>
void BME280_TWI<TWI,sa>::init()
{
    BME280_base::init();

    impl_of::BME280_id id;
    state_ = TWI_mem::read(id);

    if (error()){
	state_ = TWI::state();
	return;
    }

    if (!id.is_valid()){
	state_ = TWI::state();
	return;
    }

    reset();	// Los de Bosch en el ejemplo hacen un reset antes de leer
		// los params.
		
    if (error()){
	state_ = TWI::state();
	return;
    }

    read_calibration_params();

    if (error()){
	state_ = TWI::state();
	return;
    }
}


template <typename TWI, typename TWI::Address sa>
void BME280_TWI<TWI,sa>::write_cfg_(void f(Config&))
{
    Config cfg;
    f(cfg); // escribe la configuración buscada
    cfg.spi3w_en = Config::spi3w_disable; // es TWI
    write(cfg); 
}


}

#endif


