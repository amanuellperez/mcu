#include "avr_TWI.h"

namespace avr{

uint8_t TWI::send_start()
{
    master_transmit_start();

    wait_until_finished_its_current_job();

    if (status() != avr::TWI_START){
	state_ = iostate::send_start_fail;
	return 0;
    }

    return 1;
}


uint8_t TWI::send_repeated_start()
{
    master_transmit_repeated_start();

    wait_until_finished_its_current_job();

    if (status() != avr::TWI_REPEATED_START){
	state_ = iostate::send_repeated_start_fail;
	return 0;
    }

    return 1;
}



uint8_t TWI::send_data(const std::byte* data, uint8_t n)
{
    uint8_t i = 0;
    for (; i < n; ++i){

	master_transmit_byte(data[i]);
	wait_until_finished_its_current_job();
	
	if (status() != avr::TWI_MTM_DATA_ACK){
	    state_ = iostate::send_data_fail;
	    break;
	}
    }

    return i;
}


uint8_t TWI::receive_data(std::byte* data, uint8_t n)
{
    uint8_t i = 0;
    for (; i < n - 1; ++i){
	start_receive_data_with_ACK();

	wait_until_finished_its_current_job();
	
	if (status() != avr::TWI_MRM_DATA_ACK){
	    state_ = iostate::receive_data_fail;
	    break;
	}
	data[i] = TWI_basic::data();
    }

    // último byte con NACK
    start_receive_data_with_NACK();

    wait_until_finished_its_current_job();
    
    if (status() != avr::TWI_MRM_DATA_NACK){
	state_ = iostate::receive_data_fail;
	return n-1;
    }
    data[i] = TWI_basic::data();

    return n;
}


}// namespace

