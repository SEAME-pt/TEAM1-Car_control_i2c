
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <cstdint>


class I2c
{
	private:
		static int _fd_mot;
		static int _fd_servo;
		static int _fd_set;
		static float _SERVO_MIN_PULSE_MS;  // ms (0°)
		static float _SERVO_MAX_PULSE_MS;  // ms (180°)
		static float _SERVO_FREQ;   
		static std::string  _i2c_device;
		static void write_byte(uint8_t reg, uint8_t val);
		static void set_pwm(uint8_t channel, uint16_t on, uint16_t off);
		static void set_pwm_duty(uint8_t channel, float duty_fraction);
		static uint16_t ms_to_pwm(float ms);
    		static uint16_t angle_to_pwm(float angle);

	public:
		static void init(uint8_t addr_mot, uint8_t addr_servo,std::string i2c_device);
		static void stop_all();
		static void stop_motors();
		static void motor(int mot,int speed,bool dir);
   		static void set_servo_angle( float angle);
};
