#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdint>
#include <stdexcept>

class PCA9685 {
public:
    PCA9685(const char* i2c_device, uint8_t address) : addr(address) {
        if ((file = open(i2c_device, O_RDWR)) < 0) {
            throw std::runtime_error("Failed to open I2C device");
        }
        if (ioctl(file, I2C_SLAVE, addr) < 0) {
            throw std::runtime_error("Failed to set I2C address");
        }
    }

    ~PCA9685() {
        close(file);
    }

    void write_byte(uint8_t reg, uint8_t val) {
        uint8_t buffer[2] = {reg, val};
        if (write(file, buffer, 2) != 2) {
            throw std::runtime_error("Failed to write I2C byte");
        }
    }

    void set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
        uint8_t reg_base = 0x06 + 4 * channel;
        write_byte(reg_base, on & 0xFF);
        write_byte(reg_base + 1, on >> 8);
        write_byte(reg_base + 2, off & 0xFF);
        write_byte(reg_base + 3, off >> 8);
    }

    
    void init(uint8_t prescaler) {
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
    }

    void stop_all() {
        for (uint8_t ch = 0; ch < 16; ++ch) {
            set_pwm(ch, 0, 0);
        }
    }

    void stop_motors() {
        for (uint8_t ch = 0; ch < 8; ++ch) {
            set_pwm_duty(ch, 0);
        }
    }

void set_pwm_duty(uint8_t channel, float duty_fraction) {
    if (duty_fraction <= 0.0f) {
        set_pwm(channel, 0, 0);
    } else if (duty_fraction >= 1.0f) {
        set_pwm(channel, 0, 4095);
    } else {
        uint16_t duty = static_cast<uint16_t>(duty_fraction * 4095);
        set_pwm(channel, 0, duty);
    }
}

void set_motor_throttle(float throttle, float gain) {
    float adjusted_throttle = throttle * gain;

    if (adjusted_throttle > 0.0f) {
        float duty = adjusted_throttle;
        set_pwm_duty(0, duty);  // Motor 1 speed
        set_pwm_duty(1, 1.0f);  // Direction 1
        set_pwm_duty(2, 0.0f);  // Direction 2
        set_pwm_duty(3, 0.0f);  // Motor 2 speed
        set_pwm_duty(4, duty);  // Motor 2 speed
        set_pwm_duty(5, 0.0f);  // Direction 2
        set_pwm_duty(6, 1.0f);  // Direction 1
        set_pwm_duty(7, duty);  // Motor 2 speed
    } else if (adjusted_throttle < 0.0f) {
        float duty = -adjusted_throttle;
        set_pwm_duty(0, duty);
        set_pwm_duty(1, 0.0f);
        set_pwm_duty(2, 1.0f);
        set_pwm_duty(3, duty);
        set_pwm_duty(4, 0.0f);
        set_pwm_duty(5, 1.0f);
        set_pwm_duty(6, 0.0f);
        set_pwm_duty(7, duty);
    } else {
        stop_motors();
    }
}

private:
    int file;
    uint8_t addr;
};

// Exemplo de uso
int main() {
    try {
        PCA9685 pca("/dev/i2c-1", 0x40);
        pca.init(121); // Prescaler exemplo para ~50Hz
        pca.set_motor_throttle(0.5f, 1.0f); // Motor para frente 50%
        sleep(2);
        pca.stop_all();
    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

