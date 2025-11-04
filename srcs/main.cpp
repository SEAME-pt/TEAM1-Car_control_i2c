#include "../include/sdl.h"

// define the global joystick instance to properlly clear after sigint
SDL_Joystick*	g_joystick = nullptr;

int main() {

	int steering = MID_ANGLE;		//rotation
	int throttle = 0;				//direction & speed
	SDL_Joystick *joystick = NULL;

	signal(SIGINT, signalHandler);

    try {
		initGpio();
        joystick = initCar();

        // store global for signal handler / cleanup
        g_joystick = joystick;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }

	std::thread	speedSensor(wheelRotationCalculation);
	bool running = true;

	SDL_Event e;
	while (running) {

		float axisSteering = SDL_JoystickGetAxis(joystick, 2) / MAX_AXIS_VALUE;
		float axisThrottle = SDL_JoystickGetAxis(joystick, 1) / MAX_AXIS_VALUE;

		steering = static_cast<int>(mapAxisToAngle(axisSteering, 0, 120, 60));
		throttle = static_cast<int>(mapAxisToAngle(axisThrottle, -100, 100, 0));

		I2c::set_servo_angle(steering);

		if (throttle > 0)
			I2c::motor(0, throttle, 1); // Backward
		else if (throttle < 0)
			I2c::motor(0, -throttle, 0); // Forward
		else
			I2c::stop_motors(); // Stop

		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_JOYBUTTONDOWN) {
				if (e.jbutton.button == START_BUTTON) {
					std::cout << "Button start pressed. Exiting...\n";
					I2c::brake_motor();
					running = false;
					cleanExit();
					break ;
				}
			}
		}
		SDL_Delay(25);
	}
	speedSensor.join();
    std::cout << "The main and speed sensor thread ended bitches!" << std::endl;

    cleanExit();
    return (0);
}

//c++ srcs/main.cpp srcs/init/init_car_i2c.cpp srcs/init/init_gpio.cpp srcs/sensors/speedSensor.cpp srcs/utils/exitCleanups.cpp srcs/utils/math_utils.cpp libs/srcs/I2c.cpp libs/srcs/I2c_PcA9685.cpp libs/srcs/I2c_INA219.cpp -lSDL2
// git submodule update --init --recursive