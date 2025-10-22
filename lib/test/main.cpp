#include "../include/I2c_PcA9685.hpp"

int main()
{
	I2c::init(0x60,0x40,"/dev/i2c-1");
	I2c::set_servo_angle(150);
	I2c::motor(0,100,1);	
	I2c::set_servo_angle(90);
	sleep(5);
	I2c::set_servo_angle(0);
	I2c::stop_motors();
	
}
