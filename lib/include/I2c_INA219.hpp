
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>



#include <cstdint>

class I2c_INA219
{
	protected:
		static double _Voltage; // Volt 
		static double _Current; // Ampere
		static double _Power;    // watt
		static uint8_t _addr;
		static int fd;
		static int status;
		static std::string _i2c_device;
	 	static void writeRegister(int fd, uint8_t reg, uint16_t value);
		static uint16_t readRegister(int fd, uint8_t reg);
	public: 
		static void init( uint8_t addr_servo, std::string i2c_device );
		static void update_values();
		static void print();
};



