#include "../include/I2c.hpp"

int main()
{
	std::cout << "inicialaze the i2c dispositive" << std::endl;
	I2c::All_init();
	std::cout << "-------------------------------" << std::endl;

	std::cout << "set servo in 150 angle and the motors in 50% the power " << std::endl;	
	I2c::set_servo_angle(150);
	I2c::motor(0,50,1);	
	I2c::print(); // print batry status
	sleep(3);
	std::cout << "set servo in 90 angle and the motor 1 in 50% the power " << std::endl;	
	I2c::set_servo_angle(90);
	I2c::stop_motors();
	I2c::motor(1,50,1);	
	I2c::print(); // print batry status
	sleep(3);
	std::cout << "the motor 2 in 50% the power " << std::endl;	
	I2c::stop_motors();
	I2c::motor(2,50,1);	
	I2c::print(); // print batry status
	std::cout << "the motors in 50% the power and invert rotacion " << std::endl;	
	I2c::motor(0,100,0);	
	I2c::brake_motor();
	I2c::print(); // print batry status
	I2c::end_motor_use();

	
	
}
