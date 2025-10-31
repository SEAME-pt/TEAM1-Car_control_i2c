#include "../include/sdl.h"

// define the global joystick instance to properlly vlear after sigint
SDL_Joystick* g_joystick = nullptr;

//Axis value (range: -32768 to 32767)

void signalHandler(int signum) {

    std::cout << "Interrupt signal (" << signum << ") received.\n";
	cleanExit();
    exit(signum);
}

static double mapAxisToAngle(double axisValue, double angleMin, double angleMax, double angleCenter) {
    if (axisValue < 0) {
        // Map [-1, 0] -> [angleMin, angleCenter]
        return ((angleCenter - angleMin) * (axisValue - (-1)) / (0 - (-1)) + angleMin);
    } else {
        // Map [0, 1] -> [angleCenter, angleMax]
        return ((angleMax - angleCenter) * (axisValue - 0) / (1 - 0) + angleCenter);
    }
}

int main() {

	int i = 0;
	int steering = MID_ANGLE;
	int throttle = 0;
	SDL_Joystick *joystick = NULL;

	signal(SIGINT, signalHandler);

    try {
        joystick = initCar();
        // store global for signal handler / cleanup
        g_joystick = joystick;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }

	SDL_Event e;
	while (true) {

		float axis0 = SDL_JoystickGetAxis(joystick, 2) / 32767.0f; // steering
		float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle

		steering = static_cast<int>(mapAxisToAngle(axis0, 0, 120, 60));
		throttle = static_cast<int>(mapAxisToAngle(axis1, -100, 100, 0));

		I2c::set_servo_angle(steering);

		if (throttle > 0)
			I2c::motor(0, throttle, 1); // Backward
		else if (throttle < 0)
			I2c::motor(0, -throttle, 0); // Forward
		else
			I2c::stop_motors(); // Stop

		while (SDL_PollEvent(&e)) {

			if (e.jbutton.button == START_BUTTON) {
				std::cout << "Button start pressed. Exiting...\n";
				cleanExit();
				return (0);
			}
			SDL_Delay(15);
		}
	}
    std::cout << "The loop ended bitches!" << std::endl;

    cleanExit();
    return (0);
}

//c++ srcs/main.cpp srcs/initCar.cpp libs/srcs/I2c.cpp libs/srcs/I2c_PcA9685.cpp libs/srcs/I2c_INA219.cpp -lSDL2
// git submodule update --init --recursive