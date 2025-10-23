#include "../lib/include/I2c.hpp"
#include "../include/I2c_PcA9685.hpp"
#include <unistd.h>
#include <iostream>

int main() {
    std::cout << "Inicializando I2C..." << std::endl;
    I2c::All_init();

    // --- Teste volante ---
    std::cout << "Servo para 90° (centro)" << std::endl;
    I2c::set_servo_angle(90);
    sleep(2);

    std::cout << "Servo para 150° (direita)" << std::endl;
    I2c::set_servo_angle(150);
    sleep(2);

    std::cout << "Servo para 30° (esquerda)" << std::endl;
    I2c::set_servo_angle(30);
    sleep(2);

    // --- Teste motores ---
    std::cout << "Motor ambos para frente 50%" << std::endl;
    I2c::motor(0, 50, true);  // dir = true -> forward
    sleep(3);
    I2c::stop_motors();

    std::cout << "Motor ambos para trás 50%" << std::endl;
    I2c::motor(0, 50, false); // dir = false -> reverse
    sleep(3);
    I2c::stop_motors();

    std::cout << "Teste concluído. Freando motores..." << std::endl;
    I2c::brake_motor();

    I2c::end_motor_use();
    return 0;
}


/*
#include "../lib/include/I2c.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (SDL_NumJoysticks() < 1) {
        std::cout << "No joystick connected!\n";
        return 1;
    }

    SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (joystick == nullptr) {
        std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;

    SDL_Event e;
    while (true) {
        SDL_PollEvent(&e);
        float axis0 = SDL_JoystickGetAxis(joystick, 0) / 32767.0f; // steering
        float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle
        std::cout << "Steering: " << axis0 << " Throttle: " << -axis1 << "\r";
        SDL_Delay(50);
    }

    SDL_JoystickClose(joystick);
    SDL_Quit();
    return 0;
}

*/