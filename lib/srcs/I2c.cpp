#include "../include/I2c.hpp"
#include <cstdint>


void I2c::All_init()
{

	I2c::I2c_PcA9685::init(0x60,0x40,"/dev/i2c-1");
	I2c::I2c_INA219::init(0x41, "/dev/i2c-1");
	

}
