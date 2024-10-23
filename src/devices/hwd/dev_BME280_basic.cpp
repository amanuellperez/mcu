#include "dev_BME280_basic.h"

#include <atd_bit.h>

namespace dev{


// Output value of “5123” equals 51.23 DegC.  
// From datasheet  (4.2.3)
// This is magic!!! Copy it and hopes it works.
impl_of::BME280_calibration::Celsius
	impl_of::BME280_calibration::compensate_T(const uint32_t& adc_T)
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



// From datasheet (4.2.3)
// This is magic!!! Copy it and hopes it works.
int32_t impl_of::BME280_calibration::compensate_P_(const int32_t& adc_P) const
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


// From datasheet (4.2.3) (copiada de la biblioteca de Bosch)
// This is magic!!! Copy it and hopes it works.
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format 
// (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
impl_of::BME280_calibration::Relative_humidity
impl_of::BME280_calibration::compensate_H(const uint32_t& adc_H) const
{
//    uint32_t humidity_max = 102400;
    int32_t var1 = t_fine - ((int32_t)76800);
    int32_t var2 = (int32_t)(adc_H * 16384);
    int32_t var3 = (int32_t)(((int32_t)dig_H4) * 1048576);
    int32_t var4 = ((int32_t)dig_H5) * var1;
    int32_t var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)dig_H6)) / 1024;
    var3 = (var1 * ((int32_t)dig_H3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)dig_H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)dig_H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);

    uint32_t H = (uint32_t)(var5 / 4096);

    return Relative_humidity::significand(100*H/1024);

    // Prefiero que muestre un valor excesivo para que se sepa que no está
    // midiendo correctamente (???)
//    if (humidity > humidity_max)
//    {
//        humidity = humidity_max;
//    }

//    return humidity;
}




void impl_of::BME280_temp_and_press_and_hum::mem_to_struct(
    const std::array<uint8_t, size>& mem, impl_of::BME280_temp_and_press_and_hum& st)
{
    st.upressure = atd::concat_bytes<uint32_t>(mem[0], mem[1], mem[2]);
    st.upressure >>= 4;

    st.utemperature = atd::concat_bytes<int32_t>(mem[3], mem[4], mem[5]);
    st.utemperature >>= 4;

    st.uhumidity = atd::concat_bytes<uint32_t>(mem[6], mem[7]);

}


// decode according to table 16
void impl_of::BME280_calibration::mem_to_temp_and_press(const uint8_t* mem)
{
    // El BME280 almacena los parámetros de calibración en little-endian:
    // LSB,MSB. De ahí el orden definido (see table 17)
    dig_T1 = atd::concat_bytes<uint16_t>(mem[1], mem[0]);
    dig_T2 = atd::concat_bytes<int16_t>(mem[3], mem[2]);
    dig_T3 = atd::concat_bytes<int16_t>(mem[5], mem[4]);

    dig_P1 = atd::concat_bytes<uint16_t>(mem[7], mem[6]);
    dig_P2 = atd::concat_bytes<int16_t>(mem[9], mem[8]);
    dig_P3 = atd::concat_bytes<int16_t>(mem[11], mem[10]);
    dig_P4 = atd::concat_bytes<int16_t>(mem[13], mem[12]);
    dig_P5 = atd::concat_bytes<int16_t>(mem[15], mem[14]);
    dig_P6 = atd::concat_bytes<int16_t>(mem[17], mem[16]);
    dig_P7 = atd::concat_bytes<int16_t>(mem[19], mem[18]);
    dig_P8 = atd::concat_bytes<int16_t>(mem[21], mem[20]);
    dig_P9 = atd::concat_bytes<int16_t>(mem[23], mem[22]);

    dig_H1 = mem[25];
}


// decode according to table 16
void impl_of::BME280_calibration::mem_to_hum(const uint8_t* mem)
{
    dig_H2 = atd::concat_bytes<int16_t>(mem[1], mem[0]);
    dig_H3 = mem[2];

// NO CUADRA LA DATASHEET CON EL CÓDIGO DE BOSCH. Lo dejo como lo marca la
// biblioteca de Bosch.
// Según Bosch:
    int16_t msb_h4 = static_cast<int16_t>(mem[3]) << 4;
    int16_t lsb_h4 = static_cast<int16_t>(mem[4]) & 0x0F;
    dig_H4 = msb_h4 | lsb_h4;

    int16_t msb_h5 = static_cast<int16_t>(mem[5]) << 4;
    int16_t lsb_h5 = static_cast<int16_t>(mem[4]) >> 4;
    dig_H5 = msb_h5 | lsb_h5;

    dig_H6 = mem[6];
}

void impl_of::BME280_config::mem_to_struct(const std::array<uint8_t, size>& mem,
                                    impl_of::BME280_config& st)
{
    st.osrs_h   = mask_osrs_h(mem[i_ctrl_hum]);

    st.osrs_t   = mask_osrs_t(mem[i_ctrl_meas]);
    st.osrs_p   = mask_osrs_p(mem[i_ctrl_meas]);
    st.mode     = mask_mode(mem[i_ctrl_meas]);

    st.t_sb     = mask_t_sb(mem[i_config]);
    st.filter   = mask_filter(mem[i_config]);
    st.spi3w_en = mask_spi3w_en(mem[i_config]);
}



// st -> mem
void impl_of::BME280_config::struct_to_mem(const impl_of::BME280_config& st, uint8_t* mem)
{
// FUNDAMENTAL: inicializar la memoria!!!
    std::fill(mem, mem + size, uint8_t{0});

    mask_osrs_h(mem[i_ctrl_hum])   = st.osrs_h;

    mask_osrs_t(mem[i_ctrl_meas])   = st.osrs_t;
    mask_osrs_p(mem[i_ctrl_meas])   = st.osrs_p;
    mask_mode(mem[i_ctrl_meas])     = st.mode;

    mask_t_sb(mem[i_config])     = st.t_sb;
    mask_filter(mem[i_config])   = st.filter;
    mask_spi3w_en(mem[i_config]) = st.spi3w_en;
}

// 3.5.1
void impl_of::BME280_config::weather_monitoring(impl_of::BME280_config& cfg)
{
    cfg.mode   = force_mode;
    cfg.osrs_p = oversampling_x1;
    cfg.osrs_t = oversampling_x1;
    cfg.osrs_h = oversampling_x1;
    cfg.filter = filter_off;
//    cfg.t_sb   = ; ¿qué valor en force mode? no se usa.
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// 3.5.2
void impl_of::BME280_config::humidity_sensing(impl_of::BME280_config& cfg)
{
    cfg.mode   = force_mode;
    cfg.osrs_p = oversampling_none;
    cfg.osrs_t = oversampling_x1;
    cfg.osrs_h = oversampling_x1;
    cfg.filter = filter_off;
//    cfg.t_sb   = ; ¿qué valor en force mode? no se usa.
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}


// 3.5.3
void impl_of::BME280_config::indoor_navigation(impl_of::BME280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.t_sb   = t_sb_0_5_ms; 
    cfg.osrs_p = oversampling_x16;
    cfg.osrs_t = oversampling_x2;
    cfg.osrs_h = oversampling_x1;
    cfg.filter = filter_coeff_16;
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}

// 3.5.4
void impl_of::BME280_config::gaming(impl_of::BME280_config& cfg)
{
    cfg.mode   = normal_mode;
    cfg.t_sb   = t_sb_0_5_ms; 
    cfg.osrs_p = oversampling_x4;
    cfg.osrs_t = oversampling_x1;
    cfg.osrs_h = oversampling_none;
    cfg.filter = filter_coeff_16;
    // cfg.spi3w_en = lo define la clase: 0 para TWI y SPI, 1 para SPI_3_WIRE
}



//void impl_of::BME280_temp_and_press_and_hum::make_bounded(impl_of::BME280_temp_and_press_and_hum& st)
//{
//    if (st.utemperature <= utemp_min || st.utemperature >= utemp_max)
//	st.utemperature = 0;
//
//    if (st.upressure <= upress_min || st.upressure >= upress_max)
//	st.upressure = 0;
//
//    if (st.uhumidity <= uhumidity_min || st.uhumidity >= uhumidity_max)
//	st.uhumidity = 0;
//}
//


namespace impl_of{
bool operator==(const BME280_config& a, const BME280_config& b)
{
    return ((a.osrs_h == b.osrs_h) and
    (a.osrs_t == b.osrs_t) and
    (a.osrs_p == b.osrs_p) and
    (a.mode == b.mode) and
    (a.t_sb == b.t_sb) and
    (a.filter == b.filter) and
    (a.spi3w_en == b.spi3w_en));
}
}// impl_of
}// namespace

