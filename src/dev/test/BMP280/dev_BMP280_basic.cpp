#include "dev_BMP280_basic.h"

#include <avr_UART.h>	// TODO: borrame. Depurar

#include <atd_bit.h>
namespace dev{


// From datasheet  (3.11.3)
// This is magic!!! Copy it and hopes it works.
int32_t __BMP280_calibration::compensate_T(const int32_t& adc_T)
{
    int32_t var1 =
        ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;

    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
                      ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                    ((int32_t)dig_T3)) >> 14;

    t_fine    = var1 + var2;

    int32_t T = (t_fine * 5 + 128) >> 8;

    return T;
}

// From datasheet (3.11.3)
// This is magic!!! Copy it and hopes it works.
uint32_t __BMP280_calibration::compensate_P(const uint32_t& adc_P) const
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

    return (uint32_t)p;
}



// TODO: códigos de error --> state!!!
uint8_t BMP280_TWI::init()
{
    BMP280_base::init();

    __BMP280_id id;
    state_ = read_object(id);

    if (error()){
	return 1;
    }

    if (!id.is_valid()){
	return 2;		// otro dispositivo!!!
    }

    reset();	// Los de Bosch en el ejemplo hacen un reset antes de leer
		// los params.
    if (error())
	return 2;

    read_calibration_params();

    if (error()){
	return 3;   
    }

    return 0;
}

void __BMP280_temp_and_press::mem_to_struct(const std::byte* mem,
                                            __BMP280_temp_and_press& st)
{
    st.pressure = atd::concat_bytes<uint32_t>(mem[0], mem[1], mem[2]);
    st.pressure >>= 4;

    st.temperature = atd::concat_bytes<int32_t>(mem[3], mem[4], mem[5]);
    st.temperature >>= 4;
}


// decode according to table 17
void __BMP280_calibration::mem_to_struct(std::byte* mem,
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

}// namespace

