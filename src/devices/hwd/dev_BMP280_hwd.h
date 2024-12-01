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
#ifndef __DEV_BMP280_HWD_H__
#define __DEV_BMP280_HWD_H__
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
 *    Manuel Perez
 *    21/01/2020 v0.0
 *    07/04/2020 v0.1
 *    12/12/2020 v0.2
 *		 TODO (básico):
 *		 - Crear BMP280_SPI: se conecta vía SPI al sensor.
 *
 ****************************************************************************/
#include <atd_bit.h>
#include <atd_decimal.h>
#include <atd_magnitude.h>
#include <mcu_TWI_memory_type.h>



namespace dev{

// ---------------------------------------------------------------------------
// datasheet 4.3.1
struct __BMP280_id{
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
    // Según la datasheet del BME280 (5.2) puede tener los siguientes valores:
    //	0x56/0x57 = samples
    //	0x58      = mass production
    static constexpr uint8_t unique_value{0x58};
};


template <typename Ixtream>
inline Ixtream& operator>>(Ixtream& in, __BMP280_id& st)
{ return in >> st.id; }


inline bool __BMP280_id::is_valid() const
{ return id == unique_value;}


// ---------------------------------------------------------------------------
// datasheet 4.3.2
struct __BMP280_reset{
    static constexpr uint8_t reset_cmd {0xB6};

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::write_only;
    static constexpr uint8_t address{0xE0};
    static constexpr uint8_t size = 1;

    static constexpr bool use_struct_as_mem = true;
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
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address{0xF3};
    static constexpr uint8_t size = 1;	

// mem <-> struct
    static void mem_to_struct(const std::array<uint8_t, size>& mem
				, __BMP280_status& st);

private:
    static constexpr atd::Range_bitmask<3, 3, uint8_t> mask_measuring{};
    static constexpr atd::Range_bitmask<0, 0, uint8_t> mask_im_update{};

};


inline void
__BMP280_status::mem_to_struct(const std::array<uint8_t, size>& mem,
                               __BMP280_status& st)
{
    st.measuring = atd::to_bool(mask_measuring(mem[0]));
    st.im_update = atd::to_bool(mask_im_update(mem[0]));
}




// ---------------------------------------------------------------------------
// datasheet 4.3.4/5
struct __BMP280_config{
// Data
    // Ctrl_meas
    uint8_t osrs_t;	// Controls oversampling temperature.
    uint8_t osrs_p;	// Controls oversampling pressure.
    uint8_t mode;	// Controls the power mode of the device.

    // Config
    uint8_t t_sb;	// Controls inactive duration (t_standby)
    uint8_t filter;	// Controls the time constant of IIR filter.
    uint8_t spi3w_en;	// Enables 3-wire SPI interface when set to '1'.


// Table 7: recommended filter settings use cases
    static void handheld_device_low_power(__BMP280_config& cfg);
    static void handheld_device_dynamic(__BMP280_config& cfg);
    static void weather_monitoring(__BMP280_config& cfg);
    static void elevator_floor_change_detector(__BMP280_config& cfg);
    static void drop_detection(__BMP280_config& cfg);
    static void indoor_navigation(__BMP280_config& cfg);


// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_and_write;
    static constexpr uint8_t address{0xF4};
    static constexpr uint8_t size = 2;	

//  mem <-> struct
    static void mem_to_struct(const std::array<uint8_t,size>& mem, __BMP280_config& st);
    static void struct_to_mem(const __BMP280_config& st, uint8_t* mem);

    // Este registro tiene forma específica de ser escrito
    static constexpr uint8_t ctrl_meas_address{0xF4};
    static constexpr uint8_t config_address{0xF5};


    // Escribe toda la configuración. La configuración hay que escribirla
    // de una determinada forma: hay que hacer reset, leer... luego no vale
    // un simple write. Usar esta función para ello.
    template <typename TWI, typename TWI::Address slave_address>
    void twi_write() const;


// Options
// -------
    // Table 21 y 22 tienen los mismos valores para osrs_t y osrs_p
    static constexpr uint8_t oversampling_none{0x00};
    static constexpr uint8_t oversampling_x1	{0x01};
    static constexpr uint8_t oversampling_x2	{0x02};
    static constexpr uint8_t oversampling_x4	{0x03};
    static constexpr uint8_t oversampling_x8	{0x04};
    static constexpr uint8_t oversampling_x16	{0x05};

    // Table 23
    static constexpr uint8_t spi3w_disable{0x00};
    static constexpr uint8_t spi3w_enable{0x01};

    // Table 10: mode settings
    static constexpr uint8_t sleep_mode {0x00};
    static constexpr uint8_t force_mode {0x01}; // also 0x10 
    static constexpr uint8_t normal_mode{0x03};

    // Table 11: t_sb settings
    // Esto lo llama también la datasheet como ODR = Output Data Rate
    static constexpr uint8_t t_sb_0_5_ms	{0x00};
    static constexpr uint8_t t_sb_62_5_ms	{0x01};
    static constexpr uint8_t t_sb_125_ms	{0x02};
    static constexpr uint8_t t_sb_250_ms	{0x03};
    static constexpr uint8_t t_sb_500_ms	{0x04};
    static constexpr uint8_t t_sb_1000_ms	{0x05};
    static constexpr uint8_t t_sb_2000_ms	{0x06};
    static constexpr uint8_t t_sb_4000_ms	{0x07};
    
    // Table 6: filter settings <-- NO!!! (creo que no entiendo la tabla)
    // La tabla 6 no corresponde con el código de la biblioteca
    // de Bosch. Uso lo que aparece en el código. 
    static constexpr uint8_t filter_off	{0x00};
    static constexpr uint8_t filter_coeff_2	{0x01};
    static constexpr uint8_t filter_coeff_4	{0x02};
    static constexpr uint8_t filter_coeff_8	{0x03};
    static constexpr uint8_t filter_coeff_16	{0x04};

private:
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
void __BMP280_config::twi_write() const
{ 
    mcu::TWI_memory_type<TWI_master, slave_address> twi_mem;

    uint8_t mem[size];
    struct_to_mem(*this, mem);

// 1. Poner el dispositivo en sleep mode. 
//    El ejemplo de Bosch dice que la forma más rápida de hacerlo es con
//    reset.

    twi_mem.write(__BMP280_reset{});

    if (TWI_master::error())
	return;

    
// 2. Leer. El ejemplo de Bosch dice que siempre hay que leer antes de
// escribir la cfg. ¿Es realmente necesario?
    uint8_t tmp[2];

    twi_mem.template mem_read<address, 2>(tmp);

    if (TWI_master::error())
	return;

// 3. Enviar primero enviar config.
//    datasheet (4.3.5): In sleep mode writes are not ignored.
//    Esto es, este registro solo se puede escribir en sleep mode y hay que
//    escribirlo el solito, sin escribir ctrl_meas a la vez. (De hecho,
//    al principio intenté escribirlo a la vez que ctrl_meas y no funcionaba).
    twi_mem.template mem_write<config_address, 1>(&(mem[1]));

    if (TWI_master::error())
	return;

// 4. Enviar primero ctrl_meas
    twi_mem.template mem_write<ctrl_meas_address, 1>(&(mem[0]));

}





// ---------------------------------------------------------------------------
// datasheet 4.3.6/7 y 3.9
// According to 3.9: use a burst read to read from 0xF7 to 0xFC.
struct __BMP280_temp_and_press{
    uint32_t upressure;	// uncompensated pressure
    int32_t utemperature; // uncompensated temperature

// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address {0xF7};
    static constexpr uint8_t size = 6;

// mem <-> struct
/// Devuelve los valores de temperatura y presión sin compensar.
// En caso de que los valores leídos no estén dentro de los límites
// devuelve 0.
    static void mem_to_struct(const std::array<uint8_t, size>& mem, 
						__BMP280_temp_and_press& st);

private:

    // Mira a ver si los valores de temperatura y presión están dentro de los
    // límites. Si no lo están, los pone a 0 (de esa forma el usuario puede 
    // detectar el error).
    static void make_bounded(__BMP280_temp_and_press& st);

// Límites (estos no los he encontrado en la datasheet, sino en el ejemplo 
// de Bosch)
    static constexpr int32_t utemp_min = 0x00000;
    static constexpr int32_t utemp_max = 0xFFFF0;

    static constexpr int32_t upress_min = 0x00000;
    static constexpr int32_t upress_max = 0xFFFF0;
};



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

// Unidades en las que medimos la temperatura y la presión.
    using Celsius	   = atd::Celsius<atd::Decimal<int32_t, 2>>;
    using Pascal           = atd::Pascal<atd::Decimal<int32_t, 2>>;
    using Hectopascal      = atd::Hectopascal<atd::Decimal<int32_t, 2>>;

// Compensate functions
    /// Returns temperature in DegC, resolution is 0.01 DegC. 
    Celsius compensate_T(const int32_t& adc_T);

    /// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24
    /// integer bits and 8 fractional bits).
    ///
    /// Output value of “24674867” represents 
    ///			24674867/256 = 96386.2 Pa = 963.862 hPa
    Pascal compensate_P(const int32_t& adc_P) const{
	int32_t pc = compensate_P_(adc_P)/256;
	using Decimal = Pascal::Rep;
	return Pascal{Decimal::significand(pc)};
    }

    int32_t compensate_P_(const int32_t& adc_P) const;


// Memory
    static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
    static constexpr uint8_t address {0x88};
    static constexpr uint8_t size = 24;

// mem <-> struct
    static void mem_to_struct(const std::array<uint8_t, size>& mem, 
						    __BMP280_calibration& st);

private:

    // Carries a fine resolution temperature value over to the pressure
    // compensation formula.
    int32_t t_fine;
};



/*!
 *  \brief  Base para el sensor BMP280. Clase de implementación.
 *
 *  Nos podemos comunicar al sensor via TWI o SPI. Eso da lugar a las clases
 *  BMP280_TWI y BMP280_SPI que son las que el usuario usará.
 */
class BMP280_base{
public:
// Types
    using Id               = __BMP280_id;
    using Status           = __BMP280_status;
    using Config           = __BMP280_config;
    using Temp_and_press   = __BMP280_temp_and_press;
    using Calibration      = __BMP280_calibration;

    using Celsius          = Calibration::Celsius;
    using Pascal           = Calibration::Pascal;
    using Hectopascal      = Calibration::Hectopascal;

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


protected:
// Data
    Calibration calibration_;


// Constructor
    // Esta clase es de implementación.
    BMP280_base() {}

    // No podemos inicializar el sensor hasta no haber configurado
    // SPI o TWI. Por eso no puedo hacer el init en el constructor.
    void init() { }


// Reading T and P
    // Returns temperature in Celsius.
    Celsius compensate_T(const int32_t& adc_T)
    {return calibration_.compensate_T(adc_T);}

    // Returns pressure in Pascals.
    Pascal compensate_P(const uint32_t& adc_P) const
    {return calibration_.compensate_P(adc_P);}


    // Para depurar
//    int32_t compensate_P_(const int32_t& adc_P) const
//    {return calibration_.compensate_P_(adc_P);}


};



// (RRR) Gestión de errores:
//	 Voy a usar el equivalente a errno local a través de un state.
//	 Todas las operaciones que puedan fallar modificarán el state,
//	 quedando almacenado ahí el exito/fracaso de la operación.
template <typename TWI_master0, typename TWI_master0::Address slave_address0>
class BMP280_TWI : public BMP280_base {
public:
    static constexpr uint8_t TWI_buffer_size = 100; // TODO: ajustarlo al mínimo?

    // Conexión TWI
    using TWI_master   = TWI_master0;

    static_assert(TWI_buffer_size <= TWI_master::buffer_size,
                  "Buffer too small!!! Choose a bigger one.");


    // Slave address: el último bit se puede elegir:
    // Connecting SDO to GND results in slave address 1110110 (0x76);
    // connection it to V_DDIO results in 1110111 (0x77).
    static constexpr TWI_master::Address slave_address = slave_address0;
    static_assert(slave_address == 0x76 or slave_address == 0x77,
	    "Wrong BMP280 address. Available only: 0x76 and 0x77");

    using TWI_mem = mcu::TWI_memory_type<TWI_master, slave_address>;

    using State = TWI_master::iostate;

    // CONSTRUCTION
    /// Inicializa el sensor.
    /// Verifica que haya comunicación con el sensor y que el sensor conectado
    /// sea realmente un BMP280. Para ello comprueba que el id sea el
    /// correspondiente.
    void init();


// Modos de funcionamiento propuestos por BOSCH
    void handheld_device_low_power()
    { write_cfg_(Config::handheld_device_low_power); }

    void handheld_device_dynamic()
    { write_cfg_(Config::handheld_device_dynamic);}

    void weather_monitoring()
    { write_cfg_(Config::weather_monitoring);}

    void elevator_floor_change_detector()
    { write_cfg_(Config::elevator_floor_change_detector);}

    void drop_detection()
    { write_cfg_(Config::drop_detection);}

    void indoor_navigation()
    { write_cfg_(Config::indoor_navigation);}


// Funciones de bajo nivel
// -----------------------
    /// Read the device id.
    void read(Id& id) {state_ = TWI_mem::read(id);}

    /// The device is reset using the complete power-on-reset procedure.
    void reset() { state_ = TWI_mem::write(__BMP280_reset{});}

    /// Read the status register.
    void read(Status& res) {state_ = TWI_mem::read(res);}

    void read(Config& res) {state_ = TWI_mem::read(res);}
    void write(Config& cfg);

    void read(Temp_and_press& res) {state_ = TWI_mem::read(res);}
 

    /// Returns temperature and pressure.
    std::pair<Celsius, Pascal> T_and_P()
    {
	Temp_and_press tp;
	read(tp);
	Celsius T = compensate_T(tp.utemperature);
	Pascal P = compensate_P(tp.upressure);

	return {T, P};
    }

    /// Returns temperature and pressure (in hectopascals)
    std::pair<Celsius, Hectopascal> T_and_hP()
    {
	auto [T, P] = T_and_P();
	return {T, Hectopascal{P}};
    }

    /// Returns only the temperature.
    Celsius T()
    {
	auto [T, P] = T_and_P();
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
    State state_;


    void read_calibration_params() {state_ = TWI_mem::read(calibration_);}

    void write_cfg_(void f(Config&));
};


template <typename TWI_master, typename TWI_master::Address sa>
inline void BMP280_TWI<TWI_master, sa>::write(Config& cfg)
{
    cfg.twi_write<TWI_master, slave_address>();
}

template <typename TWI, typename TWI::Address sa>
void BMP280_TWI<TWI,sa>::write_cfg_(void f(Config&))
{
    Config cfg;
    f(cfg);
    cfg.spi3w_en = Config::spi3w_disable; // es TWI
    write(cfg); 
}


template <typename TWI, typename TWI::Address sa>
void BMP280_TWI<TWI,sa>::init()
{
    BMP280_base::init();

    __BMP280_id id;
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

}// namespace

#endif






