#include "dev_BMP280_hwd.h"

#include <atd_bit.h>
namespace dev{


// Output value of “5123” equals 51.23 DegC.  
// From datasheet  (3.11.3)
// This is magic!!! Copy it and hopes it works.
__BMP280_calibration::Celsius
	__BMP280_calibration::compensate_T(const int32_t& adc_T)
{
    int32_t var1 =
        ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;

    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
                      ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                    ((int32_t)dig_T3)) >> 14;

    t_fine    = var1 + var2;

    int32_t T = (t_fine * 5 + 128) >> 8;

    using Decimal = Celsius::Rep;
    return Celsius{Decimal::significand(T)};
}



// From datasheet (3.11.3)
// This is magic!!! Copy it and hopes it works.
int32_t __BMP280_calibration::compensate_P_(const int32_t& adc_P) const
{
    // Observar que usa t_fine!!! Hay que calcular primero compensate_T!!!
    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)dig_P6;

    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) +
           ((var1 * (int64_t)dig_P2) << 12);
    var1 = ((int64_t{0x800000000000} + var1) * ((int64_t)dig_P1)) >> 33;

    if (var1 == 0)
	return 0; // avoid exception caused by division by zero

    int64_t p    = 1048576 - adc_P;
    p    = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p    = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

    return (int32_t)p;
}



// void __BMP280_temp_and_press::mem_to_struct(const uint8_t* mem,
void __BMP280_temp_and_press::mem_to_struct(
    const std::array<uint8_t, size>& mem, __BMP280_temp_and_press& st)
{
    st.upressure = atd::concat_bytes<uint32_t>(mem[0], mem[1], mem[2]);
    st.upressure >>= 4;

    st.utemperature = atd::concat_bytes<int32_t>(mem[3], mem[4], mem[5]);
    st.utemperature >>= 4;

    make_bounded(st);
}


// decode according to table 17
// void __BMP280_calibration::mem_to_struct(uint8_t* mem,
void __BMP280_calibration::mem_to_struct(const std::array<uint8_t,size>& mem,
                                         __BMP280_calibration& st)
{
    // El BMP280 almacena los parámetros de calibración en little-endian:
    // LSB,MSB. De ahí el orden definido (see table 17)
    st.dig_T1 = atd::concat_bytes<uint16_t>(mem[1], mem[0]);
    st.dig_T2 = atd::concat_bytes<int16_t>(mem[3], mem[2]);
    st.dig_T3 = atd::concat_bytes<int16_t>(mem[5], mem[4]);

    st.dig_P1 = atd::concat_bytes<uint16_t>(mem[7], mem[6]);
    st.dig_P2 = atd::concat_bytes<int16_t>(mem[9], mem[8]);
    st.dig_P3 = atd::concat_bytes<int16_t>(mem[11], mem[10]);
    st.dig_P4 = atd::concat_bytes<int16_t>(mem[13], mem[12]);
    st.dig_P5 = atd::concat_bytes<int16_t>(mem[15], mem[14]);
    st.dig_P6 = atd::concat_bytes<int16_t>(mem[17], mem[16]);
    st.dig_P7 = atd::concat_bytes<int16_t>(mem[19], mem[18]);
    st.dig_P8 = atd::concat_bytes<int16_t>(mem[21], mem[20]);
    st.dig_P9 = atd::concat_bytes<int16_t>(mem[23], mem[22]);
}



// mem -> st
// void __BMP280_config::mem_to_struct(uint8_t* mem, __BMP280_config& st)
void __BMP280_config::mem_to_struct(const std::array<uint8_t, size>& mem,
                                    __BMP280_config& st)
{
    st.osrs_t   = mask_osrs_t(mem[0]);
    st.osrs_p   = mask_osrs_p(mem[0]);
    st.mode     = mask_mode(mem[0]);

    st.t_sb     = mask_t_sb(mem[1]);
    st.filter   = mask_filter(mem[1]);
    st.spi3w_en = mask_spi3w_en(mem[1]);
}



// st -> mem
//void __BMP280_config::struct_to_mem(const __BMP280_config& st,
//                                    std::array<uint8_t, size>& mem)
void __BMP280_config::struct_to_mem(const __BMP280_config& st,
                                    uint8_t* mem)
{
//    static_assert(mem.size() == 2, "mem"); // voy a usar 2 bytes

// FUNDAMENTAL: inicializar la memoria!!!
    mem[0] = uint8_t{0};
    mem[1] = uint8_t{0};

    mask_osrs_t(mem[0])   = st.osrs_t;
    mask_osrs_p(mem[0])   = st.osrs_p;
    mask_mode(mem[0])     = st.mode;

    mask_t_sb(mem[1])     = st.t_sb;
    mask_filter(mem[1])   = st.filter;
    mask_spi3w_en(mem[1]) = st.spi3w_en;
}


// Table 7
void __BMP280_config::handheld_device_low_power(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x16;
    cfg.osrs_t = oversampling_x2;
    cfg.filter = filter_coeff_4;
    cfg.t_sb   = t_sb_62_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// Table 7
void __BMP280_config::handheld_device_dynamic(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x4;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_coeff_16;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
void __BMP280_config::weather_monitoring(__BMP280_config& cfg)
{
    cfg.mode =  force_mode;
    cfg.osrs_p = oversampling_x1;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_off;
    // cfg.t_sb = ¿Qué valor poner aqui en force_mode? Se supone que lo ignora
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
void __BMP280_config::elevator_floor_change_detector(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x4;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_coeff_4;
    cfg.t_sb   = t_sb_125_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// Table 7
void __BMP280_config::drop_detection(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x2;
    cfg.osrs_t = oversampling_x1;
    cfg.filter = filter_off;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// Table 7
 void __BMP280_config::indoor_navigation(__BMP280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.osrs_p = oversampling_x16;
    cfg.osrs_t = oversampling_x2;
    cfg.filter = filter_coeff_16;
    cfg.t_sb   = t_sb_0_5_ms; // ODR (see table 14)
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}



void __BMP280_temp_and_press::make_bounded(__BMP280_temp_and_press& st)
{
    if (st.utemperature <= utemp_min || st.utemperature >= utemp_max)
	st.utemperature = 0;

    if (st.upressure <= upress_min || st.upressure >= upress_max)
	st.upressure = 0;
}

}// namespace

