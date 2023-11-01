#include "dev_DS1307_basic.h"

#include <atd_cast.h>

namespace dev{

void __DS1307_timekeeper::mem_to_struct(__DS1307_timekeeper& st)
{
    if (mask_CH(st.seconds))
	st.clock_on = false;
    else
	st.clock_on = true;

    st.seconds = mask_seconds(st.seconds);

    st.seconds = atd::BCD2int(st.seconds);
    st.minutes = atd::BCD2int(st.minutes);

    if (mask_AMPM_mode(st.hours)){
	st.AMPM_format = true;
	st.is_PM = (mask_AMPM_PM(st.hours) == 1);
	st.hours = mask_AMPM_hours(st.hours);
    }
    else
	st.AMPM_format = false;

    st.hours   = atd::BCD2int(st.hours);


    st.date    = atd::BCD2int(st.date);
    st.month   = atd::BCD2int(st.month);
    st.year    = atd::BCD2int(st.year);

}


__DS1307_timekeeper __DS1307_timekeeper::struct_to_mem(__DS1307_timekeeper st)
{
    st.seconds = atd::int2BCD(st.seconds);
    st.minutes = atd::int2BCD(st.minutes);
    st.hours   = atd::int2BCD(st.hours);

    if (st.AMPM_format){
	mask_AMPM_mode(st.hours) = 1;

	if (st.is_PM)
	    mask_AMPM_PM(st.hours) = 1;
	else 
	    mask_AMPM_PM(st.hours) = 0;
    } 


    st.date    = atd::int2BCD(st.date);
    st.month   = atd::int2BCD(st.month);
    st.year    = atd::int2BCD(st.year);
    if (st.clock_on)
	mask_CH(st.seconds) = 0;
    else
	mask_CH(st.seconds) = 1;

    return st;
}


void __DS1307_control_register::mem_to_struct(const uint8_t& mem,
                              __DS1307_control_register& st)
{
    st.output_control     = (mask_output_control(mem) == uint8_t{1});
    st.square_wave_enable = (mask_square_wave_enable(mem) == uint8_t{1});
    st.rate_select        = mask_rate_select(mem);
}


void __DS1307_control_register::struct_to_mem(const __DS1307_control_register& st, 
				uint8_t& mem)
{
    mem = uint8_t{0};
    mask_output_control(mem)     = atd::bool_to<uint8_t>(st.output_control);
    mask_square_wave_enable(mem) = atd::bool_to<uint8_t>(st.square_wave_enable);
    mask_rate_select(mem)        = st.rate_select;
}


}// namespace



