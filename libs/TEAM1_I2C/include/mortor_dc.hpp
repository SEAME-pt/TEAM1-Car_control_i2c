#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

#define I2C_ADDR 0x40  // Endereço I2C do driver PCA9685 (ajuste conforme seu módulo)
#define MOTOR_CHANNEL 0  // Canal PWM que controla o motor

// Função para configurar o PCA9685 (exemplo)
void setupPCA9685(int fd) {
    wiringPiI2CWriteReg8(fd, 0x00, 0x00); // Modo normal
    usleep(5000);
}

// Função para ajustar o PWM (exemplo: velocidade)
void setPWM(int fd, int channel, int on, int off) {
    wiringPiI2CWriteReg8(fd, 0x06 + 4 * channel, on & 0xFF);
    wiringPiI2CWriteReg8(fd, 0x07 + 4 * channel, on >> 8);
    wiringPiI2CWriteReg8(fd, 0x08 + 4 * channel, off & 0xFF);
    wiringPiI2CWriteReg8(fd, 0x09 + 4 * channel, off >> 8);
}

int main() {
    int fd = wiringPiI2CSetup(I2C_ADDR);
    if (fd == -1) {
        std::cerr << "Erro ao inicializar I2C!" << std::endl;
        return -1;
    }

    std::cout << "Configurando driver PCA9685..." << std::endl;
    setupPCA9685(fd);

    std::cout << "Acionando motor..." << std::endl;
    // Define o motor em 50% de velocidade (metade do ciclo de 12 bits = 4096)
    setPWM(fd, MOTOR_CHANNEL, 0, 2048);

    sleep(5); // Mantém o motor ligado por 5 segundos

    std::cout << "Desligando motor..." << std::endl;
    setPWM(fd, MOTOR_CHANNEL, 0, 0);

    return 0;
}

