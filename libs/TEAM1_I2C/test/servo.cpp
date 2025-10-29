#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdint>
#include <stdexcept>
#include <cmath>
#include <thread>

// AJUSTE ESTES VALORES conforme o seu servo:
// Para a maioria dos servos: 0.5ms (0°) a 2.5ms (180°)
// Se não atingir o curso total, tente 0.6ms~2.4ms
constexpr float SERVO_MIN_PULSE_MS = 0.5f;  // ms (0°)
constexpr float SERVO_MAX_PULSE_MS = 2.5f;  // ms (180°)
constexpr float SERVO_FREQ = 50.0f;         // Hz

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
        write_byte(0x00, 0xA1); // Restart, auto-increment, allcall
        usleep(5000);
    }

    // Converte pulso em ms para valor PWM (0-4095)
    uint16_t ms_to_pwm(float ms) {
        float pulse_length_us = 1000000.0f / SERVO_FREQ / 4096.0f; // em us
return static_cast<uint16_t>(ms * 1000.0f / pulse_length_us);
    }

    // Converte ângulo 0-180 para pulso em ms, depois para PWM
    uint16_t angle_to_pwm(float angle) {
        if (angle < 0.0f) angle = 0.0f;
        if (angle > 180.0f) angle = 180.0f;
        float pulse_ms = SERVO_MIN_PULSE_MS + (angle / 180.0f) * (SERVO_MAX_PULSE_MS - SERVO_MIN_PULSE_MS);
        return ms_to_pwm(pulse_ms);
    }

    void set_servo_angle(uint8_t channel, float angle) {
        uint16_t pwm = angle_to_pwm(angle);
        set_pwm(channel, 0, pwm);
    }

    // Movimento suave entre dois ângulos, garantindo o fim exato
    void smooth_move(uint8_t channel, float from, float to, int steps = 50, int delay_ms = 20) {
        for (int i = 0; i <= steps; ++i) {
            float t = static_cast<float>(i) / steps;
            float angle = from + (to - from) * t;
            set_servo_angle(channel, angle);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
    }

    void stop_all() {
        for (uint8_t ch = 0; ch < 16; ++ch) {
            set_pwm(ch, 0, 0);
        }
    }

private:
    int file;
    uint8_t addr;
};

int main() {
    try {
        PCA9685 pca("/dev/i2c-1", 0x40);
        pca.init(121); // Prescaler para ~50Hz


        // Teste ângulos extremos (caso precise calibrar)
        std::cout << "Testando extremos...\n";
        pca.set_servo_angle(0, 0);   // 0°
        sleep(1);
        pca.set_servo_angle(0, 180); // 180°
        sleep(1);

        pca.stop_all();

    } catch (const std::exception &e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
