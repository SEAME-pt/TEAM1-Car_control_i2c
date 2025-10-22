#include "../include/I2c_PcA9685.hpp"
#include <stdint.h>

#include <cstdint>

int I2c_PcA9685::_fd_mot = 0;
int I2c_PcA9685::_fd_servo = 0;
int I2c_PcA9685::_fd_set = 0;
float I2c_PcA9685::_SERVO_MIN_PULSE_MS = 0.5f;
float I2c_PcA9685::_SERVO_MAX_PULSE_MS = 2.5f;
float I2c_PcA9685::_SERVO_FREQ = 50.0f;

void I2c_PcA9685::init(uint8_t addr_mot, uint8_t addr_servo,std::string i2c_device)
{
	int prescaler = 121;
	
	if ((_fd_mot = open(i2c_device.c_str(), O_RDWR)) < 0) {
            throw std::runtime_error("Failed to open I2C device");
        }
        if (ioctl(_fd_mot, I2C_SLAVE, addr_mot ) < 0) {
            throw std::runtime_error("Failed to set I2C address");
        }

	if ((_fd_servo = open(i2c_device.c_str(), O_RDWR)) < 0) {
            throw std::runtime_error("Failed to open I2C device");
        }
        if (ioctl(_fd_servo, I2C_SLAVE, addr_servo ) < 0) {
            throw std::runtime_error("Failed to set I2C address");
        }
	_fd_set = _fd_mot;
 	write_byte(0x00, 0x00); // MODE1 normal
        usleep(5000);
        write_byte(0x01, 0x04); // MODE2 totem pole
        usleep(5000);
        write_byte(0x00, 0x10); // MODE1 sleep
        usleep(5000);
        write_byte(0xFE, prescaler); // Set prescaler
        usleep(5000);
        write_byte(0x00, 0x80); // Exit sleep
        usleep(5000);
	_fd_set = _fd_servo;
 	write_byte(0x00, 0x00); // MODE1 normal
        usleep(5000);
        write_byte(0x01, 0x04); // MODE2 totem pole
        usleep(5000);
        write_byte(0x00, 0x10); // MODE1 sleep
        usleep(5000);
        write_byte(0xFE, prescaler); // Set prescaler
        usleep(5000);
        write_byte(0x00, 0x80); // Exit sleep
        usleep(5000);
	_fd_set = 0;
}


void I2c_PcA9685::write_byte(uint8_t reg, uint8_t val) {
        uint8_t buffer[2] = {reg, val};
        if (write(_fd_set, buffer, 2) != 2) {
            throw std::runtime_error("Failed to write I2C byte");
        }
    }

void I2c_PcA9685::set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
        uint8_t reg_base = 0x06 + 4 * channel;
        write_byte(reg_base, on & 0xFF);
        write_byte(reg_base + 1, on >> 8);
        write_byte(reg_base + 2, off & 0xFF);
        write_byte(reg_base + 3, off >> 8);
    }

void I2c_PcA9685::stop_all() {
	_fd_set = _fd_servo;
        for (uint8_t ch = 0; ch < 16; ++ch) {
            set_pwm(ch, 0, 0);
        }
	_fd_set = _fd_mot;
        for (uint8_t ch = 0; ch < 16; ++ch) {
            set_pwm(ch, 0, 0);
        }
    }

void I2c_PcA9685::stop_motors() {
	_fd_set = _fd_mot;
        for (uint8_t ch = 0; ch < 8; ++ch) {
            set_pwm_duty(ch, 0);
        }
    }

void I2c_PcA9685::set_pwm_duty(uint8_t channel, float duty_fraction) {
    if (duty_fraction <= 0.0f) {
        set_pwm(channel, 0, 0);
    } else if (duty_fraction >= 1.0f) {
        set_pwm(channel, 0, 4095);
    } else {
        uint16_t duty = static_cast<uint16_t>(duty_fraction * 4095);
        set_pwm(channel, 0, duty);
    }
}

uint16_t I2c_PcA9685::ms_to_pwm(float ms) {
        float pulse_length_us = 1000000.0f / _SERVO_FREQ / 4096.0f; // em us
        return static_cast<uint16_t>(ms * 1000.0f / pulse_length_us);
    }

    // Converte ângulo 0-180 para pulso em ms, depois para PWM
uint16_t I2c_PcA9685::angle_to_pwm(float angle) {
        if (angle < 0.0f) angle = 0.0f;
        if (angle > 180.0f) angle = 180.0f;
        float pulse_ms = _SERVO_MIN_PULSE_MS + (angle / 180.0f) * (_SERVO_MAX_PULSE_MS - _SERVO_MIN_PULSE_MS);
        return ms_to_pwm(pulse_ms);
    }

void I2c_PcA9685::set_servo_angle( float angle) {	
	uint8_t channel  = 0;
	_fd_set = _fd_mot;
        uint16_t pwm = angle_to_pwm(angle);
        set_pwm(channel, 0, pwm);
    }


void I2c_PcA9685::motor(int mot, int speed, bool dir)
{
    _fd_set = _fd_mot;

    // Limita speed entre 0 e 100
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;

    float duty = speed / 100.0f;

    if (mot == 0) { // ambos motores
        if (dir) { // forward
            set_pwm_duty(0, duty);  // motor 1 speed
            set_pwm_duty(1, 1.0f);  // direction 1
            set_pwm_duty(2, 0.0f);  // direction 2
            set_pwm_duty(3, duty);  // motor 2 speed
            set_pwm_duty(4, 1.0f);  // direction 1
            set_pwm_duty(5, 0.0f);  // direction 2
        } else { // reverse
            set_pwm_duty(0, duty);  // motor 1 speed
            set_pwm_duty(1, 0.0f);  // direction 1
            set_pwm_duty(2, 1.0f);  // direction 2
            set_pwm_duty(3, duty);  // motor 2 speed
            set_pwm_duty(4, 0.0f);  // direction 1
            set_pwm_duty(5, 1.0f);  // direction 2
        }
    }

    // Para mot == 1 ou mot == 2 podes fazer semelhante
}


void I2c_PcA9685::brake_motor()
{
	float intensity = 1;
        float duty = (intensity > 1.0f) ? 1.0f : (intensity < 0.0f ? 0.0f : intensity);

        // Ambos os lados “altos” (equivale a curto virtual no driver)
        set_pwm_duty(1, duty);
        set_pwm_duty(2, duty);
        set_pwm_duty(4, duty);
        set_pwm_duty(5, duty);

        usleep(100000); // 100 ms de frenagem ativa

        // Desliga tudo após frear
        stop_motors();

}

void I2c_PcA9685::end_motor_use()
{
	stop_motors();
	close(_fd_set);
	close(_fd_mot);
}



