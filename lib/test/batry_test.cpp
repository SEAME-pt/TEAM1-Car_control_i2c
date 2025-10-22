#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>
#include <cstdint>

#define INA219_ADDRESS 0x40

// Registradores principais do INA219
#define REG_CONFIG             0x00
#define REG_SHUNT_VOLTAGE      0x01
#define REG_BUS_VOLTAGE        0x02
#define REG_POWER              0x03
#define REG_CURRENT            0x04
#define REG_CALIBRATION        0x05

// Função para escrever 16 bits no INA219
void writeRegister(int fd, uint8_t reg, uint16_t value) {
    uint8_t buffer[3];
    buffer[0] = reg;
    buffer[1] = (value >> 8) & 0xFF;
    buffer[2] = value & 0xFF;
    if (write(fd, buffer, 3) != 3) {
        perror("Erro ao escrever no registrador");
        exit(1);
    }
}

// Função para ler 16 bits de um registrador
uint16_t readRegister(int fd, uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {
        perror("Erro ao selecionar registrador");
        exit(1);
    }
    uint8_t buffer[2];
    if (read(fd, buffer, 2) != 2) {
        perror("Erro ao ler registrador");
        exit(1);
    }
    return (buffer[0] << 8) | buffer[1];
}

int main() {
    const char *device = "/dev/i2c-1";
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Erro ao abrir barramento I2C");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, INA219_ADDRESS) < 0) {
        perror("Erro ao acessar dispositivo I2C");
        close(fd);
        return 1;
    }

    // ===== Configuração e calibração =====
    // Config padrão: PGA=±320mV, 12-bit ADC, modo contínuo
    uint16_t config = 0x019F;
    writeRegister(fd, REG_CONFIG, config);

    // Calibração (exemplo para shunt de 0.1Ω e corrente máx ≈ 3.2A)
    // Fator de calibração depende do resistor shunt
    uint16_t calibration = 4096;
    writeRegister(fd, REG_CALIBRATION, calibration);

    // ===== Leitura =====
    uint16_t shunt_raw = readRegister(fd, REG_SHUNT_VOLTAGE);
    uint16_t bus_raw = readRegister(fd, REG_BUS_VOLTAGE);
    uint16_t current_raw = readRegister(fd, REG_CURRENT);
    uint16_t power_raw = readRegister(fd, REG_POWER);

    // Conversões físicas
    _voltage = shunt_voltage = (int16_t)shunt_raw * 0.00001; // 10 µV/bit
    double bus_voltage = ((bus_raw >> 3) * 0.004);       // 4 mV/bit
    double current = (int16_t)current_raw * 0.1;         // 0.1 mA/bit (ajuste conforme calibração)
    double power = power_raw * 2.0;                      // 2 mW/bit (depende da calibração)

     
    std::cout << "==========================" << std::endl;
    std::cout << "INA219 - Leitura completa" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Tensão no barramento (Vbus): " << bus_voltage << " V" << std::endl;
    std::cout << "Tensão no shunt (Vshunt): " << shunt_voltage << " V" << std::endl;
    std::cout << "Corrente: " << current << " mA" << std::endl;
    std::cout << "Potência: " << power << " mW" << std::endl;
    std::cout << "==========================" << std::endl;

    close(fd);
    return 0;
}

