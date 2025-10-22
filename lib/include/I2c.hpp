#include "I2c_PcA9685.hpp"
#include "I2c_INA219.hpp"

#include <cstdint>



class I2c: public I2c_PcA9685 , public I2c_INA219
{
	
	protected:
		static std::string  _i2c_device;
	public:
		static void All_init();


};
