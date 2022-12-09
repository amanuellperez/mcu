#include <iostream>

#include <stdlib.h>

int32_t t_fine;
uint16_t dig_T1 = 27317;
int16_t dig_T2 = 25808;
int16_t dig_T3 = 50;
uint16_t dig_P1 = 37912;
int16_t dig_P2 = -10589;
int16_t dig_P3 = 3024;
int16_t dig_P4 = 6601;
int16_t dig_P5 = -158;
int16_t dig_P6 = -7;
int16_t dig_P7 = 15500;
int16_t dig_P8 = -14600;
int16_t dig_P9 = 6000;


int32_t bosch_T(int32_t uncomp_temp)
{
    int32_t var1, var2;
    int32_t comp_temp;

        var1 =
(
 (
    ((uncomp_temp / 8) - ((int32_t) dig_T1 << 1))
 )
 * ((int32_t) dig_T2)) /
            2048;

        var2 =
((

(
    ((uncomp_temp / 16) - ((int32_t) dig_T1)) 
    *
    ((uncomp_temp / 16) - ((int32_t) dig_T1))
) / 4096) *
((int32_t) dig_T3)) / 16384;


    t_fine = var1 + var2;
    comp_temp = (t_fine * 5 + 128) / 256;

    return comp_temp;
}



uint32_t bosch_P(uint32_t uncomp_Pres)
{
    int64_t var1, var2, p;

    var1 = ((int64_t) (t_fine)) - 128000;
    var2 = var1 * var1 * (int64_t) dig_P6;

    var2 = var2 + ((var1 * (int64_t) dig_P5) * 131072);
    var2 = var2 + (((int64_t) dig_P4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t) dig_P3) / 256) +
	   ((var1 * (int64_t) dig_P2) * 4096);
    var1 = ((INT64_C(0x800000000000) + var1) * ((int64_t)dig_P1))
					/ 8589934592;
    if (var1 != 0)
    {
	p = 1048576 - uncomp_Pres;
	p = (((((p * 2147483648U)) - var2) * 3125) / var1);
	var1 = (((int64_t) dig_P9) * (p / 8192) * (p / 8192)) / 33554432;
	var2 = (((int64_t) dig_P8) * p) / 524288;
	p = ((p + var1 + var2) / 256) + (((int64_t)dig_P7) * 16);
	return (uint32_t)p;
    }

    else
	return 0;
}





int32_t compensate_T(const int32_t& adc_T)
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



uint32_t compensate_P(const uint32_t& adc_P) 
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

int main()
{
try{
    int32_t T = 512504;
    int32_t P = 523376;

    std::cout << "bosch_T: " << bosch_T(T) 
	      << "(t_fine = " << t_fine << ")\n"
	      << "mia_T  : " << compensate_T(T)
	      << "(t_fine = " << t_fine << ")\n";

    std::cout << "presion\n"
	      << "-------\n";

    std::cout << "bosch_P: " << bosch_P(P) 
	      << "(t_fine = " << t_fine << ")\n"
	      << "mia_P  : " << compensate_P(P)
	      << "(t_fine = " << t_fine << ")\n";


}catch(std::exception& e){
    std::cerr << "ERROR: " << e.what() << '\n';
    return 1;
}

}

